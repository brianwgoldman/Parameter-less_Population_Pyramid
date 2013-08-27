/*
 * MiddleLayer.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "MiddleLayer.h"

Middle_Layer::Middle_Layer(Configuration& config, shared_ptr<Evaluator> evaler):
   evaluator(evaler), evaluations(0),
   best_fitness(std::numeric_limits<float>::min()),
   best_found(0), results(config) { }

float Middle_Layer::evaluate(const vector<bool>& solution)
{
	evaluations++;
	float fitness = evaluator->evaluate(solution);
	if(best_fitness < fitness)
	{
		best_fitness = fitness;
		best_solution = solution;
		best_found = evaluations;
		results.add(fitness, evaluations);
	}
	return fitness;
}
