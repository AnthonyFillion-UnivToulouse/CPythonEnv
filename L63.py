"""
Master python file
Constructs config files, run main C/C++ code, read data, plot data
"""

import sys
import configparser
import os
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

#### Construct config file ##########################################################
CONFIG = configparser.ConfigParser() # Contain configurations for the C
CONFIG["files"] = {  # Files parameters, only use lowercases
    "dir" : sys.argv[0][:-3], # Name of the experiment directory
    "cfgfile": "config.cfg",  # Name of the config file
    "cfile" : "C", # Name of the file containing C/C++ code
    "cmain" : "main",  # Name of the executable
}
CONFIG["inputs"] = {  # Inputs parameters
    "algo": "lorenz63",  # Name of the algo to be run
    "n" : "3", # State dimension
    "ndt" : "1", # Number of time step integration during one propagation
    "dt" : "0.01", # time step
    "sigma" : "10.0", # a parameter Cf wikipedia
    "rho" : "28.0", # idem
    "beta" : str(8.0/3.0), # idem
    "nc": "4000", # total number of propagation
    "x0": "x0.csv", # initial state file path in CSV format
}
CONFIG["outputs"] = {
    "xts" : "xts.csv", # trajectory file path in CSV format
}
#####################################################################################

# Creating experiment directory if does not exist
FILES = CONFIG["files"]
if not os.path.exists(FILES["dir"]):
    print("creating experiment dir")
    os.makedirs(FILES["dir"])
# Creating config file
with open(FILES["dir"]+"/"
          +FILES["cfgfile"], 'w+') as configfile:
    CONFIG.write(configfile)
# Creating random x0 if it does not exist
if not os.path.exists(FILES["dir"]+"/"+CONFIG["inputs"]["x0"]):
    print("creating random x0")
    x0 = np.empty((1,3))
    x0[0,:] = 3*np.ones(3)+np.random.randn(3)
    np.savetxt(FILES["dir"]+"/"+CONFIG["inputs"]["x0"], x0, delimiter=",")

#### Run C/C++ code #################################################################
os.chdir(FILES["cfile"])
if (len(sys.argv)>1 and sys.argv[1]=="make") or\
   (not os.path.exists(FILES["cmain"])): # Compiling C if it is not
    print("compiling main.c")            # or if "make" is given in arg
    os.system("make clean")
    os.system("make all")
N = os.system( # Running main C code with config file path in parameter
    "./"+FILES["cmain"]+
    " ../"+FILES["dir"]+
    "/"+FILES["cfgfile"])
if N != 0:
    print("Error while executing C")
os.chdir("../")
#####################################################################################

#### Read and plot data #############################################################
CONFIG.read(FILES["dir"]+"/"+FILES["cfgfile"])
ofile = CONFIG["files"]["dir"]+"/"+CONFIG["outputs"]["xts"]
xts = np.genfromtxt(ofile, delimiter=',')
fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot(xts[:,0], xts[:,1], xts[:,2])
plt.savefig(FILES["dir"]+"/"+FILES["dir"]+".pdf")
plt.show()
#####################################################################################
