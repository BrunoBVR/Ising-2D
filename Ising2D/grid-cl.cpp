#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <math.h>
#include <string>
#include <sys/stat.h>
// Defines classes: spinGrid() and metropolisAlgorithm()
#include "ising_2d.h"

using namespace std;
//////////// Start of main function
// main takes 4 arguments from the command line:
// * argv[1]: size of grid
// * argv[2]: temperature
// * argv[3]: number of Monte Carlo steps
// * argv[4]: density of negative sites on grid
////////////
int main(int argc, char *argv[]){
	// Check for input in the command line
	if (argc <= 1){
		if (argv[0])
			cout << "Using: " << argv[0] << " with no arguments.\n";
		else
			cout << "Using: <program name> with no arguments.\n";
		return 1;
	}
	else if(argc < 5){
		if (argv[0])
			cout << "Using: " << argv[0] << " with missing arguments.\n";
		else
			cout << "Using: <program name> with missing arguments.\n";
			return 1;
	}

	string figuresPath = "figures", csvPath = "csv";
	int figs = mkdir(figuresPath.c_str(), 0777);
	int dats = mkdir(csvPath.c_str(), 0777);

	int size = (int)stod(argv[1]);
	double J = 1.0, H = 0.0;						// Coupling constant and field
	double T = stod(argv[2]);						// Temperature for the metropolis run
	int steps = (int)stod(argv[3]);			// Number of Monte Carlo steps
	double density = stod(argv[4]);			// density of -1 in grid
	spinGrid g(density, size, J, H);		// Grid of spins +-1
	metropolisAlgorithm algo(T,steps,&g);

	cout << "*********************************\n";
	cout << "Size of grid: " << size << "x" << size << endl;
	cout << "Temperature: " << T << endl;
	cout << "Monte Carlo steps: " << steps << endl;
	cout << "Density of negative sites: " << density << endl;
	cout << "*********************************\n";

	int avg_sample = 50;		// Number of samples to take average values from end of run
	double avg_E = 0.0, avg_m = 0.0;

	cout << avg_E << avg_m << endl;
	g.printCSV("csv/initial"+to_string(T).substr(0, to_string(T).find(".") + 2 + 1)+"_Size-"+to_string(size));
	algo.run(avg_sample, &avg_E, &avg_m);
	g.printCSV("csv/final"+to_string(T).substr(0, to_string(T).find(".") + 2 + 1)+"_Size-"+to_string(size));
	cout << avg_E << avg_m << endl;

	return 0;
}
