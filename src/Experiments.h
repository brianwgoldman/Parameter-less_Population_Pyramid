/*
 * Experiments.h
 *
 *  Created on: Jul 20, 2013
 *      Author: goldman
 */

#ifndef EXPERIMENTS_H_
#define EXPERIMENTS_H_

#include "Configuration.h"
#include "OptimizationCollection.h"
#include "Evaluation.h"
#include "Record.h"
#include "Util.h"
#include "MiddleLayer.h"

vector<Record> multirun(Random& rand, Configuration& config, evaluation::pointer problem, optimize::pointer solver);

int bisection(Random& rand, Configuration& config, evaluation::pointer problem, optimize::pointer solver);

#endif /* EXPERIMENTS_H_ */
