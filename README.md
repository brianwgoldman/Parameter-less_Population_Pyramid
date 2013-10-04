P3
==

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

Release/P3 config/default.cfg config/alt1.cfg -problem NearestNeighborNK -length 50

This line will use the default configuration, replacing configuration values with those
found in alt1.cfg, setting the problem to Nearest Neighbor NK with a genome size of 50.


