// Brian Goldman

// Entry point for the entire project.  Allows experiments performing
// comparison of each of the techniques across many problems, either as
// single runs, multiple runs, or bisection tuning of population size.

// -------------- Building ---------------------------
// Compilation requires C++11.  We use g++4.7 for our experiments.
// We provide makefiles in the Debug and Release folders.  To achieve best performance,
// use the Release makefile.  Compiles to the executable name P3

// ------------- Command Line Interface -------------------
// This program accepts command line arguments in two types:
// * Any argument starting with a dash "-" will be treated as a configuration key.
//   The argument following that key will be treated as the configuration value.
//   Example: -runs 7 -problem NearestNeighborNK
//   This will set the number of runs to perform to 7, and optimize the Nearest Neighbor NK
//   problem.
// * All other arguments are assumed to be configuration file paths.  Each configuration
//   file should contain one or more white space separated key value pairs.
//   Example: config/default.cfg
//   This will load all of the default parameter values from the config/default.cfg file.
// You can mix and match any number of configuration files and arguments.  If a key appears twice
// the last value given is used.
// Example: -runs 7 -runs 10
// This would set the number of runs to 10.
// See config/default.cfg for a description of valid configuration keys.

// ------------- Running an Experiment -----------------
// Example: Release/P3 config/default.cfg -verbosity 2 config/ltga.cfg
// This will call the release version of the program (optimized for speed), using the default
// configuration options.  It will uses higher than normal terminal output
// (as requested by the higher verbosity value), and it will use the LTGA optimizer configuration

// ------------- Using an External Evaluation Program ------------
// We provide a configuration file "external" which can allow you to interface with an
// external evaluation program.  To evaluate a solution, it is written to the specified
// output file (external_out).  It then calls the external execution script (script_path)
// which is responsible for reading in the solution and writing out its fitness.  The
// floating point fitness is then read in from the file (external_in).

// -------------- Creating New Internal Evaluation Problem ---------
// All evaluation methods are defined in Evaluation.h.  To create a new problem,
// first look at the simple OneMax example.  Each problem needs to inherit from the Evaluator
// class and have at minimum the same functions as OneMax.  Furthermore, to make the problem
// accessible through configuration you need to add it to the lookup map at the end of that file.
#include <iostream>
using namespace std;

#include "Evaluation.h"
#include "HillClimb.h"
#include "Util.h"
#include "MiddleLayer.h"
#include "Population.h"
#include "Configuration.h"
#include "Optimizer.h"
#include "OptimizationCollection.h"
#include "Record.h"
#include "Experiments.h"

int main(int argc, char * argv[]) {
  // Parse the configuration options
  Configuration config;
  config.parse(argc, argv);

  // Set up the random number seed either using the configuration value
  // or the computers built in seed generator.
  Random rand;
  int seed = config.get<int>("seed");
  if (seed == -1) {
    std::random_device rd;
    seed = rd();
    config.set("seed", seed);
  }
  rand.seed(seed);

  // Specify output files
  string dat_file = config.get<string>("dat_file");
  // Writes out the combined configuration for this experiment
  ofstream cfg_out(config.get<string>("cfg_file"));
  config.dump(cfg_out);
  cfg_out.close();

  // choose the problem and the optimization method
  auto problem = config.get<evaluation::pointer>("problem");
  auto optimizer_method = config.get<optimize::pointer>("optimizer");

  if (config.get<string>("experiment") == "bisection") {
    // Performs bisection to tune the population size
    int pop_size = bisection(rand, config, problem, optimizer_method);
    cout << "POP SIZE " << pop_size << endl;
  } else if (config.get<string>("experiment") == "multirun") {
    // Performs a multiple run experiment
    vector<Record> records = multirun(rand, config, problem, optimizer_method);
    auto summary = Record::summarize(records, config);
    ofstream out(dat_file);
    cout << summary[MES] << " " << summary[MAD] << " ";
    cout << summary[FAILURES] << endl;
    out << "# " << summary[MES] << " " << summary[MAD] << " ";
    out << summary[FAILURES] << endl;
    for (const auto& record : records) {
      if (config.get<int>("verbosity") > 1) {
        cout << record.best().first << " " << record.best().second << endl;
      }
      out << record.best().first << " " << record.best().second << endl;
    }
    out.close();
  } else { // single_run
    Record record = single_run(rand, config, problem, optimizer_method, 0);
    ofstream out(dat_file.c_str());
    for (const auto& line : record.levels) {
      out << line.first << " " << line.second << endl;
    }
    out.close();
  }
  return 0;
}
