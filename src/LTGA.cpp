/*
 * LTGA.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: goldman
 */

#include "LTGA.h"

void LTGA::initialize(Random & rand, Evaluator & evaluator, hill_climb::pointer hc)
{
	float fitness;
	for(size_t i=0; i < pop_size; i++)
	{
		auto solution = rand_vector(rand, length);
		fitness = evaluator.evaluate(solution);
		hc(rand, solution, fitness, evaluator);
		pop.add(solution);
		fitnesses[solution] = fitness;
	}
}

void LTGA::generation(Random& rand, Evaluator& evaluator)
{
	pop.rebuild_tree(rand);
	pop.rand_smallest_first(rand);
	float fitness;
	Population next_generation(length);
	for(auto solution: pop.solutions)
	{
		fitness = fitnesses[solution];
		pop.improve(rand, solution, fitness, evaluator);
		fitnesses[solution] = fitness;
		next_generation.add(solution);
	}
	pop = next_generation;
}

std::unordered_set<vector<bool>> LTGA::construct_set(float& fitness)
{
	std::unordered_set<vector<bool>> set;
	float new_best;
	for(auto & solution: pop.solutions)
	{
		new_best = fitnesses[solution];
		if(fitness < new_best)
		{
			fitness = new_best;
		}
		set.insert(solution);
	}
	return set;
}

void LTGA::optimize(Random& rand, Evaluator& evaluator, Configuration& config)
{
	float fitness = 0;
	auto hc = config.get<hill_climb::pointer>("hill_climber");
	initialize(rand, evaluator, hc);

	previous_set = construct_set(fitness);
	while(fitness < 1.0)
	{
		generation(rand, evaluator);
		auto new_set = construct_set(fitness);
		if(new_set == previous_set)
		{
			break;
		}
		previous_set = new_set;
	}
}
