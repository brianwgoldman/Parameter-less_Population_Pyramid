Deprecated version of P3! See https://github.com/brianwgoldman/FastEfficientP3 for the most recent version.
==
This version exists solely to recreate the results of the paper by the same name.


Parameterless Population Pyramid

To compile you will need C++11.  We use gcc version 4.7.2 for our complilation.

Our build system uses Makefiles to build.  You can compile the release version
by changing directory to Release and calling "make".

All of the source code is available in the 'src' directory.

To run an experiment, call the executable with command line arguments for configuration.
This will run the default test configuration:

Release/P3 config/default.cfg

The command line accepts any number of configuration files and configuration flags given
in the form "-key value" where key is the name of the option to change and value is the
value to change it to.  Arguments override in last given order.  For example:

Release/P3 config/default.cfg config/p3.cfg -problem NearestNeighborNK -length 50

This line will use the default configuration, replacing configuration values with those
found in p3.cfg, setting the problem to Nearest Neighbor NK with a genome size of 50.

All of the individual configuration files and output files are contained in raw.tar.
You can duplicate any of the experiments from the paper by running the associated cfg
file from the raw.tar collection.  For instance:

Release/P3 clean/DeceptiveTrap.0105.p3.cfg -verbosity 1

This will run P3 on the Deceptive Trap problem using 105 bits.  The "verbosity" flag reduces
the console spam.  The "final" folder will then contain the newly generated output.

The results are aggregated in complete.csv for use by the plotter.R and stats.R scripts.


Also, see main.cpp for implementation related details.
