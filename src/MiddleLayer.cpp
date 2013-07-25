/*
 * MiddleLayer.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "MiddleLayer.h"

Middle_Layer::Middle_Layer(Configuration& config, shared_ptr<Evaluator> evaler):
   evaluator(evaler), counter(0), best_fitness(0), best_found(0), results(config)
{

}

float Middle_Layer::evaluate(const vector<bool>& solution)
{
	counter ++;
	const auto& it = seen.find(solution);
	if(it != seen.end())
	{
		return it->second;
	}
	else
	{
		float fitness = evaluator->evaluate(solution);
		seen[solution] = fitness;
		if(best_fitness < fitness)
		{
			best_fitness = fitness;
			best_solution = solution;
			best_found = seen.size();
			results.add(fitness, seen.size());
		}
		return fitness;
	}
}
