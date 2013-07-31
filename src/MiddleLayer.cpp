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
	float fitness = evaluator->evaluate(solution);
	if(best_fitness < fitness)
	{
		best_fitness = fitness;
		best_solution = solution;
		best_found = counter;
		results.add(fitness, counter);
	}
	return fitness;
}
