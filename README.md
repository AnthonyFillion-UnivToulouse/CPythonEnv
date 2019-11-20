# CPythonEnv

This is a simple environment to launch C algorithms from python.
The idea is that C is fast but python is handy for post-processing.

## Structure

A python main script (e.g. "dummy.py", "L63.py", "L95.py") generates a configuration file written on disk.
Then it launches a C main executable with the path of the configuration file as argument.
The C main executable reads the configuration file and launches the algorithm specified in it.
Results are written in the configuration file if they are small. Big results are written in csv files.
The python main script reads those results and post-process them.

## Installation

Clone the repository, initialize the iniparser submodule, compile the iniparser sumodule.

## Examples

Examples are "dummy.py", "L63.py" and "L95.py". They can be run with e.g. :

`python L63.py`

This will automatically compile the C code, create and experiment directory, generate a config file, run the C code, compute a trajectory of the Lorenz 63 model and plot the result.

