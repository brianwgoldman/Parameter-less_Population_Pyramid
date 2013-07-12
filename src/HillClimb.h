/*
 * HillClimb.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef HILLCLIMB_H_
#define HILLCLIMB_H_

#include "Evaluation.h"
#include "Util.h"

using hc_pointer=void (*)(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);

void first_improvement(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);

void steepest_ascent(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);

#endif /* HILLCLIMB_H_ */
