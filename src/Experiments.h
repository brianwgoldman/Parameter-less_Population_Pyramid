// Brian Goldman

// There are a couple of different kind of experiment performed with
// this software, each producing different types of data.  These
// are the drivers of optimization.

#ifndef EXPERIMENTS_H_
#define EXPERIMENTS_H_

#include "Configuration.h"
#include "OptimizationCollection.h"
#include "Evaluation.h"
#include "Record.h"
#include "Util.h"
#include "MiddleLayer.h"

// Performs a single optimization run using the specified solver to optimizing
// the passed in problem.  Returns a record of how many evaluations were required
// to reach each new fitness level.
Record single_run(Random& rand, Configuration& config,
                  evaluation::pointer problem, optimize::pointer solver,
                  int run);

// Performs multiple runs of optimization, repeatedly calling single_run with
// different run numbers.  Returns the collection of records produced.
// Will periodically print to the screen depending on the "verbosity" level
vector<Record> multirun(Random& rand, Configuration& config,
                        evaluation::pointer problem, optimize::pointer solver);

// Tool used to determine the minimum population size capable of performing
// the desired number of runs and having all runs find the global optimum.
int bisection(Random& rand, Configuration& config, evaluation::pointer problem,
              optimize::pointer solver);

#endif /* EXPERIMENTS_H_ */
