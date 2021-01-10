# C++ program for the 2D-Ising model with periodic boundary conditions

All content inside `Ising2D` folder.

## Classes definition - located in `ising_2d.h`

### * **spinGrid** class:

This class is defined for the construction of a square grid of spins (+-1).
It has four private members:
- `grid`: a vector of vectors of size **size x size**. The actual container for all spins. Each site grid[i][j] contains a short integer.
- `size`: **int** value. the **size** of the grid (actually, the grid has **size x size** sites).
- `J`: **double** value. The coupling constant for the Ising Hamiltonian.
- `H`: **double** value. the external field.

#### Constructor:
Fills the grid randomly given a density of negative sites.
Sets values for `size`, `J` and `H`.
Arguments are (in order):
- `neg_density`: **double** value. The density of negative spins.
- `s`: **int** value of `size`.
- `j`: **double** value of `J`.
- `h`: **double** value of `H`.

#### void method flipSpin(int i, int j):
Changes sign of spin at site [i][j].

#### double method Neighbors(int i, int j):
Returns the sum of neighboring spins with periodic boundary conditions.

#### double method totalEnergy():
Returns the total energy of the system.

#### double method mag():
Returns total magnetization of the system.

#### double method varE(int i, int j):
Returns variation of energy IF site [i][j] is *flipped*.

#### void method printGrid():
Utility to print graph in matrix representation on terminal. (NOT RECOMMENDED FOR LARGE `size`).

#### void method printCSV(string base):
Utility to print the grid sites in a **csv** file named base.csv.

#### int method getSize():
Getter for the `size` member.

#### vector<vector<short>> method getGrid():
Getter for the `grid` member.

### * **metropolisAlgorithm** class:

This class defines the  Metropolis algorithm for solving the 2D Ising model. It has three private members:

- `temp`: a **double** value for the temperature of the run.
- `steps`: **int** value for the number of Monte Carlo steps of the run.
- `grid`: a pointer to a **spinGrid** object with the spins of the system.

#### Constructor:
Initializes all members with an initializer list (*order: temp, steps, grid*).

#### void method run(int avg_sample, double* avg_E, double* avg_m, int frame = 1, bool rem = false):

Does a complete Metropolis run with `steps` Monte Carlo steps using the initial `grid` as starting point. The `grid` is updated on `main` as the method uses a pointer to a spinGrid object.
Values for energy and magnetization per spin are written to .csv files inside a folder named **csv** (MUST EXIST OR BE CREATED IN main).

##### Arguments:

- `avg_sample`: number of samples to be taken from the end of the run to calculate average "equilibrium" values.
- `avg_E` and `avg_m`: pointers to the cumulative values of energy and magnetization per spin, respectively, of the last `avg_sample` steps of the run.
- `frame`: interval to write values to the csv files. Data is written every `frame` step of the run. Default value = 1.
- `rem`: boolean value. If **true**, csv files are deleted after run. If **false**, csv files are kept. Default value is **false**.

## Example usage: grid-cl.cpp

NOTE: MUST BE SAVED IN SAME ADDRESS OF ising_2d.h.

`main` code for executing a complete Metroplois run of the Ising 2D model.

`main` takes 4 arguments from the command line:
 * argv[1]: size of grid
 * argv[2]: temperature
 * argv[3]: number of Monte Carlo steps
 * argv[4]: density of negative sites on grid

If no argument (or not enough arguments) is provided, main exits with error message and returns value 1.

Example compiling with g++:

**g++ grid-cl.cpp -o grid-cl.exe**

Example running the executable:

**./grid-cl.exe 20 2.0 10000 0.5**

This will run the algorithm with a 20 x 20 grid of spins, temperature 2.0, 10000 Monte Carlo steps and initial density of negative spins equal to 0.5.

A folder **csv** is created and data for magnetization and energy are saved there.

### **Using python to compile, run and plot results**: plotting.py
Command line arguments are defined inside the script and a `figures` folder is filled with resulting plots (some examples are inside this repo).

Besides energy and magnetization, a visualization of the initial and final grid is saved with names as `initial-T-1.00_Size-50.png` and `final-T-1.00_Size-50.png` where numerical entries refer to parameters of the run.

## Example usage: sample-avg.cpp

NOTE: MUST BE SAVED IN SAME ADDRESS OF ising_2d.h.

`main` code for executing a complete Metroplois run of the Ising 2D model for various values of temperature. For each temperature run, the average value of energy and magnetization are saved in a .csv file inside a **avg** folder created by the program.

`main` takes 5 arguments from the command line:
 * argv[1]: size of grid
 * argv[2]: initial temperature
 * argv[3]: number of Monte Carlo steps
 * argv[4]: density of negative sites on grid
 * argv[5]: number of samples for avg files

 The size of the temperature step for each run can be changed in the `dT` variable inside `main`.

If no argument (or not enough arguments) is provided, main exits with error message and returns value 1.

Example compiling with g++:

**g++ sample-avg.cpp -o sample.exe**

Example running the executable:

**./sample.exe 10 0.1 1000 0.5 20**

This will run the algorithm with a 10 x 10 grid of spins, initial temperature of 0.1, 1000 Monte Carlo steps, initial density of negative spins equal to 0.5 and 20 samples to calculate average in the end of run.

A folder **avg** is created and data for magnetization and energy are saved there.

### **Using python to compile, run and check for phase transitions**: avg-plotting.py
Command line arguments are defined inside the script and an `avg` folder is filled with resulting plots (some examples are inside this repo).

For both python scripts, sufficient requirements are:
- matplotlib.pyplot
- pandas
- numpy
- subprocess
