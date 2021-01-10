'''
Bruno Vieira Ribeiro - bruno64bits@gmail.com
'''

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import subprocess

# Calling the cpp executable with given arguments
size = '20'
T = '0.10'
steps = '1000000'
density = '0.5'
sample = '20'
subprocess.call(["g++", "sample-avg.cpp", "-o", "sample.exe"])      # Compiling
print("Done compiling!")
wait = input("Press Enter to run program.")
subprocess.call(["./sample.exe", size, T, steps, density, sample]) # Executing
print("Program executed!")

# Storing magnetization data in DataFrame and plotting
MagFile = "avg/mag-T0"+T+"_Size-"+size+".csv"

Mag_df = pd.read_csv(MagFile, names = ['T', 'Magnetization'])

plt.plot(Mag_df['T'], Mag_df.Magnetization, '-o', markersize=0.2)
plt.xlabel("Temperature")
plt.ylabel("Magnetization - averaged over last "+sample+" steps")
plt.title("Temperature: "+T+", Size of grid: "+size+ ", Initial neg. density: "+density)
plt.grid()
plt.savefig("avg/mag-T"+T+"_Size-"+size+".png")
plt.close()

# Storing energy data in DataFrame and plotting
EnFile = "avg/energy-T0"+T+"_Size-"+size+".csv"

En_df = pd.read_csv(EnFile, names = ['T', 'Energy'])

plt.plot(En_df['T'], En_df.Energy, '-o', markersize=0.2)
plt.xlabel("Temperature")
plt.ylabel("Total Energy - averaged over last "+sample+" steps")
plt.title("Temperature: "+T+", Size of grid: "+size+ ", Initial neg. density: "+density)
plt.grid()
plt.savefig("avg/energy-T"+T+"_Size-"+size+".png")
plt.close()
