/*
 * LTGA.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: goldman
 */

#include "LTGA.h"

LTGA::LTGA(Random& _rand, Evaluator& _evaluator, Configuration& _config):
	Optimizer(_rand, _evaluator, _config), pop(_config)
{
	pop_size = config.get<int>("pop_size");
	disable_binary_insert = config.get<int>("binary_insert") != 1;
	hc = config.get<hill_climb::pointer>("hill_climber");

	float fitness;
	vector<vector<bool>> solutions;
	for(size_t i=0; i < pop_size; i++)
	{
		auto solution = rand_vector(rand, length);
		fitness = evaluator.evaluate(solution);
		hc(rand, solution, fitness, evaluator);
		solutions.push_back(solution);
		fitnesses[solution] = fitness;
	}
	binary_insert(rand, solutions, pop);
}

void LTGA::binary_insert(Random& rand, vector<vector<bool>> & solutions, Population& next_pop)
{
	std::shuffle(solutions.begin(), solutions.end(), rand);
	for(size_t i = 0; i + 1 < solutions.size(); i+=2)
	{
		if(fitnesses[solutions[i]] < fitnesses[solutions[i+1]])
		{
			next_pop.add(solutions[i+1]);
			next_pop.add(solutions[i], disable_binary_insert);
		}
		else
		{
			next_pop.add(solutions[i]);
			next_pop.add(solutions[i+1], disable_binary_insert);
		}
	}
}

void LTGA::generation()
{
	pop.rebuild_tree(rand);
	float fitness;
	Population next_generation(config);
	vector<vector<bool>> solutions;
	for(auto solution: pop.solutions)
	{
		fitness = fitnesses[solution];
		pop.improve(rand, solution, fitness, evaluator);
		fitnesses[solution] = fitness;
		solutions.push_back(solution);
	}
	binary_insert(rand, solutions, next_generation);
	pop = next_generation;
}

bool LTGA::iterate()
{
	generation();
	decltype(pop_set) new_set(pop.solutions.begin(), pop.solutions.end());
	if(new_set == pop_set)
	{
		// all organisms identical after two generation, so stop
		return false;
	}
	pop_set = new_set;
	return true;
}
