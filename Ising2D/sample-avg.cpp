/*
Bruno Vieira Ribeiro - bruno64bits@gmail.com
*/

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
// main takes 5 arguments from the command line:
// * argv[1]: size of grid
// * argv[2]: temperature
// * argv[3]: number of Monte Carlo steps
// * argv[4]: density of negative sites on grid
// * argv[5]: number of samples for avg
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

	string avgPath = "avg", figuresPath = "figures", csvPath = "csv";
	int figs = mkdir(figuresPath.c_str(), 0777);
	int dats = mkdir(csvPath.c_str(), 0777);
	int avgs = mkdir(avgPath.c_str(), 0777);

	int size = (int)stod(argv[1]);
	double J = 1.0, H = 0.0;						// Coupling constant and field
	double T = stod(argv[2]);						// Temperature for the initial metropolis run
	int steps = (int)stod(argv[3]);			// Number of Monte Carlo steps
	double density = stod(argv[4]);			// density of -1 in grid

	cout << "*********************************\n";
	cout << "Size of grid: " << size << "x" << size << endl;
	cout << "Initial Temperature: " << T << endl;
	cout << "Monte Carlo steps: " << steps << endl;
	cout << "Density of negative sites: " << density << endl;
	cout << "*********************************\n";

	int avg_sample  = stod(argv[5]);		// Number of samples to take average values from end of run
	double dT = 0.2;				// Step in temperature variation
	double avg_E, avg_m;

	// Output files
	ofstream AvgMagFile, AvgEnFile;
	string magFileName, enFileName;

	magFileName = "avg/mag-T0"+to_string(T).substr(0, to_string(T).find(".") + 2 + 1)+"_Size-"+to_string(size)+".csv";
	enFileName = "avg/energy-T0"+to_string(T).substr(0, to_string(T).find(".") + 2 + 1)+"_Size-"+to_string(size)+".csv";
	// In the lines above we use the substr() function to set precision of 2 decimal places after "."

	AvgMagFile.open(magFileName);
	AvgEnFile.open(enFileName);

	// Loop for various values of temperature
	while(T < 5.0){
		spinGrid g(density, size, J, H);		// Grid of spins +-1
		metropolisAlgorithm algo(T,steps,&g);

		avg_E = 0.0;
		avg_m = 0.0;

		algo.run(avg_sample, &avg_E, &avg_m, steps, true);

		AvgMagFile << T << "," << abs(avg_m) << endl;
		AvgEnFile << T << "," << avg_E << endl;

		cout << "Temperature of current run: " << T << endl;
		T+= dT;
	}

	AvgMagFile.close();
	AvgEnFile.close();


	return 0;
}
