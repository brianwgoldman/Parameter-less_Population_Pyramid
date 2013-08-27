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
	size_t pop_size;
	bool disable_binary_insert;
	Population pop;
	std::unordered_map<vector<bool>, float> fitnesses;
	std::unordered_set<vector<bool>> pop_set;
	void binary_insert(Random& rand, vector<vector<bool>> & solutions, Population& next_pop);
	void generation();
	hill_climb::pointer hc;

public:
	LTGA(Random& _rand, Evaluator& _evaluator, Configuration& _config);
	bool iterate() override;
	create_optimizer(LTGA);
};

#endif /* LTGA_H_ */
