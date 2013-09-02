/*
 * Pyramid.h
 *
 *  Created on: Jun 26, 2013
 *      Author: goldman
 */

#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "Population.h"
#include "Evaluation.h"
#include "Util.h"
#include "HillClimb.h"
#include "Configuration.h"
#include "Optimizer.h"

class Pyramid: public Optimizer
{
private:
	vector<Population> pops;

	bool add_unique(const vector<bool> & solution, size_t level);
	bool only_add_improvements;
	hill_climb::pointer hill_climber;
public:
	std::unordered_set<vector<bool>> seen;
	Pyramid(Random& _rand, Evaluator& _evaluator, Configuration& _config):
		Optimizer(_rand, _evaluator, _config),
		only_add_improvements(_config.get<int>("only_add_improvements")),
		hill_climber(_config.get<hill_climb::pointer>("hill_climber")) { }
	void climb(vector<bool> & solution, float & fitness);
	bool iterate() override;
	create_optimizer(Pyramid);
};

#endif /* PYRAMID_H_ */
