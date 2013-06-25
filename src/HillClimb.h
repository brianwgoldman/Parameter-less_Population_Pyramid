/*
 * HillClimb.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef HILLCLIMB_H_
#define HILLCLIMB_H_

#include "Evaluation.h"
#include "MersenneTwister.h"

void next_best(MTRand & rand, vector<bool> & solution, float & fitnes, Evaluator& evaluator);


#endif /* HILLCLIMB_H_ */
