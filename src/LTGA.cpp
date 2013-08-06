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

void LTGA::generation(Random& rand, Evaluator& evaluator)
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
