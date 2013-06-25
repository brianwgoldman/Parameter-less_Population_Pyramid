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

void next_best(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);


#endif /* HILLCLIMB_H_ */
