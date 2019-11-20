"""
Master python file
Constructs config files, run main C/C++ code, read data, plot data
"""

import sys
import configparser
import os

#### Construct config file ##########################################################
CONFIG = configparser.ConfigParser() # Contain configurations for the C
CONFIG["files"] = { # Files parameters, only use lowercases
    "dir" : sys.argv[0][:-3], # Name of the experiment directory
    "cfgfile": "config.cfg", # Name of the config file
    "cfile" : "C", # Name of the file containing C/C++ code
    "cmain" : "main", # Name of the executable
}
CONFIG["inputs"] = { # Inputs parameters
    "algo": "PlusQuaranteDeux", # Name of the algo to be run
    "n" : "1", # An inupt parameter
}
CONFIG["outputs"] = { # Outputs parameters
    "r" : "-1", # C algo writes result here
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

#### Run C/C++ code #################################################################
os.chdir(FILES["cfile"])
if (len(sys.argv)>1 and sys.argv[1]=="make") or\
   (not os.path.exists(FILES["cmain"])):        # Compiling C if it is not
    print("compiling C")                        # or if "make" is given in arg
    os.system("make clean")
    os.system("make all")
flag = os.system( # Running main C code with config file path in parameter
    "./"+FILES["cmain"]+
    " ../"+FILES["dir"]+
    "/"+FILES["cfgfile"])
if flag != 0:
    print("Error while executing C")
os.chdir("../")
#####################################################################################

#### Read data ######################################################################
CONFIG.read(FILES["dir"]+"/"+FILES["cfgfile"])
print("r= "+CONFIG["outputs"]["r"])
#####################################################################################
