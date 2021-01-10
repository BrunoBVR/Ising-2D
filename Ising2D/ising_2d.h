#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <math.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>

using namespace std;
const long seed = 137;

///////		Class for construction of 2D grid of spins +-1
class spinGrid{
public:
	// Constructor - takes in the density of negative spins in the grid
	spinGrid(double neg_density, int s, double j, double h){
		size = s;
		H = h;
		J = j;

		// Random number generation
		mt19937 gen(seed);
		uniform_real_distribution<double> dist(0.0, 1.0);

		// Filling grid with ones
		grid.resize(size, vector<short>(size, 1));

		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
					if (dist(gen) < neg_density)
						grid[i][j] = - 1;
	}

	// Function to flip the spin at position (i,j)
	void flipSpin(int i, int j){
		grid[i][j] *= -1;
	}

	// Return the sum of neighboring spins with periodic boundary conditions
	double Neighbors(int i, int j){
		double nu, nd, nl, nr;

		// Handling 'vertical' boundary conditions
		if(i == 0){
			nu = grid[size-1][j];
		}
		else{
			nu = grid[i-1][j];
		}

		nd = grid[(i+1)%size][j];

		// Handling 'horizontal' boundary conditions
		if(j == 0){
			nl = grid[i][size-1];
		}
		else{
			nl = grid[i][j-1];
		}

		nr = grid[i][(j+1)%size];

		return (nu + nd + nl + nr);
	}

	// Calculate total energy of the system
	double totalEnergy(){
		double E = 0;

		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
				E -= J*grid[i][j]*Neighbors(i,j)/2.0 + H*grid[i][j];
				// the '/2.0' factor accounts for pairs being count only once

		return E;
	}

	// Calculate total magnetization
	double mag(){
		double m = 0;

		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
				m += grid[i][j];

		return m;
	}

	// Calculate variation of energy if i,j is flipped
	double varE(int i, int j){
		return 2*J*grid[i][j]*Neighbors(i,j) + 2*H*grid[i][j];
	}

	// Utility to print graph in matrix representation
	void printGrid(){

		for (int i = 0; i < size; ++i){
			for (int j = 0; j < size; ++j)
				cout << grid[i][j] << "\t";
			cout << endl;
		}
	}

	// Utility to output grid to a csv file
	void printCSV(string base){
		ofstream file;
		string filename = base;
		file.open(filename+".csv");

		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j){
				file << grid[i][j];
				if ( (j+1) % size == 0)
					file << endl;
				else
					file << ",";
			}

		file.close();
	}

	// Getters
	int getSize(){
		return size;
	}

	vector< vector<short> > getGrid(){
		return grid;
	}

private:
	vector< vector<short> > grid;
	int size;
	double J, H;
};
///////////// END OF Grid CLASS

// Class for Metropolis Algorithm
class metropolisAlgorithm{
public:
	metropolisAlgorithm(double T, int steps, spinGrid* g): temp(T), steps(steps), grid(g){}

	void run(int avg_sample, double* avg_E, double* avg_m, int frame = 1, bool rem = false){
		int i,j;
		double dE, mag, E = grid->totalEnergy();
		int size = grid->getSize();


		// Random number generation
		mt19937 gen(seed);
		uniform_real_distribution<double> dist(0.0, 1.0);
		uniform_int_distribution<> dist_int(0, size - 1);

		// Output files
		ofstream MagFile, EnFile;
		string magFileName, enFileName;

		magFileName = "csv/mag-T"+to_string(temp).substr(0, to_string(temp).find(".") + 2 + 1)+"_Size-"+to_string(size)+".csv";
		enFileName = "csv/energy-T"+to_string(temp).substr(0, to_string(temp).find(".") + 2 + 1)+"_Size-"+to_string(size)+".csv";
		// In the lines above we use the substr() function to set precision of 2 decimal places after "."

		MagFile.open(magFileName);
		EnFile.open(enFileName);

		// Writting initial conditions
		MagFile << "0," << grid->mag()/(static_cast<double>(size*size)) << endl;
		EnFile << "0," << E/(static_cast<double>(size*size)) << endl;

		for(int count = 0; count < steps; ++count){
			i = dist_int(gen);
			j = dist_int(gen);

			if(count%100 == 0)
				cout << "Monte Carlo step: " << count << endl;

			dE = grid->varE(i,j);

			if(dist(gen) < exp(-dE/temp)){
				grid->flipSpin(i,j);
				E += dE;
			}
			mag = grid->mag();

			// Writing to output files
			if (count%frame == 0){
				MagFile << count+1 << "," << mag/(static_cast<double>(size*size)) << endl;
				EnFile << count+1 << "," << E/(static_cast<double>(size*size)) << endl;
			}

			// Adding values to average values
			if(count >= steps-avg_sample){
				*avg_E += E;
				*avg_m += mag;
			}
		}

		*avg_E /= (avg_sample*(static_cast<double>(size*size)));
		*avg_m /= (avg_sample*(static_cast<double>(size*size)));

		MagFile.close();
		EnFile.close();

		if (rem){
			remove(magFileName.c_str());
			remove(enFileName.c_str());
		}
	}

private:
	double temp;
	int steps;
	spinGrid *grid;
};
/////////// END OF metropolisAlgorithm CLASS
