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
#include "Configuration.h"
#include <unordered_map>

namespace hill_climb
{
	using pointer=void (*)(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);

	void first_improvement(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);


	void steepest_ascent(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);

	static std::unordered_map<string, pointer> lookup({
		{"first_improvement", first_improvement},
		{"steepest_ascent", steepest_ascent}
	});
}
#endif /* HILLCLIMB_H_ */
