import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import subprocess

# Calling the cpp executable with given arguments
size = '10'
T = '0.10'
steps = '1000'
density = '0.5'
subprocess.call(["g++", "grid-cl.cpp", "-o", "grid-cl.exe"])      # Compiling
print("Done compiling!")
wait = input("Press Enter to run program.")
subprocess.call(["./grid-cl.exe", size, T, steps, density]) # Executing
print("Program executed!")

# Storing magnetization data in DataFrame and plotting
MagFile = "csv/mag-T"+T+"_Size-"+size+".csv"

Mag_df = pd.read_csv(MagFile, names = ['step', 'Magnetization'])

plt.plot(Mag_df.step, Mag_df.Magnetization, '-o', markersize=0.2)
plt.xlabel("Monte Carlo step")
plt.ylabel("Magnetization")
plt.title("Temperature: "+T+", Size of grid: "+size+ ", Initial neg. density: "+density)
plt.grid()
plt.savefig("figures/mag-T"+T+"_Size-"+size+".png")
plt.close()

# Storing energy data in DataFrame and plotting
EnFile = "csv/energy-T"+T+"_Size-"+size+".csv"

En_df = pd.read_csv(EnFile, names = ['step', 'Energy'])

plt.plot(En_df.step, En_df.Energy, '-o', markersize=0.2)
plt.xlabel("Monte Carlo step")
plt.ylabel("Total Energy")
plt.title("Temperature: "+T+", Size of grid: "+size+ ", Initial neg. density: "+density)
plt.grid()
plt.savefig("figures/energy-T"+T+"_Size-"+size+".png")
plt.close()

# Plotting the grid of spins
initialFile = "csv/initial"+T+"_Size-"+size+".csv"
finalFile = "csv/final"+T+"_Size-"+size+".csv"
grid_i = np.genfromtxt(initialFile, delimiter=',')
grid_f = np.genfromtxt(finalFile, delimiter=',')

plt.imshow(grid_i, cmap='hot', interpolation='nearest')
plt.savefig("figures/initial-T-"+T+"_Size-"+size+".png")

plt.imshow(grid_f, cmap='hot', interpolation='nearest')
plt.savefig("figures/final-T-"+T+"_Size-"+size+".png")
