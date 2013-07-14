/*
 * LTGA.h
 *
 *  Created on: Jul 12, 2013
 *      Author: goldman
 */

#ifndef LTGA_H_
#define LTGA_H_

#include "Population.h"
#include "Evaluation.h"
#include "Util.h"
#include "HillClimb.h"
#include "Configuration.h"
#include "Optimizer.h"

class LTGA: public Optimizer
{
private:
	size_t length;
	size_t pop_size;
	Population pop;
	std::unordered_map<vector<bool>, float> fitnesses;
	std::unordered_set<vector<bool>> construct_set(float& fitness);
	void initialize(Random& rand, Evaluator& evaluator, hill_climb::pointer hc);
	void generation(Random& rand, Evaluator& evaluator);

public:
	std::unordered_set<vector<bool>> previous_set;
	LTGA(Configuration& config):
		length(config.get<int>("length")),
		pop_size(config.get<int>("pop_size")),
		pop(config.get<int>("length")) {}
	void optimize(Random& rand, Evaluator& evaluator, Configuration& config) override;
};

#endif /* LTGA_H_ */
