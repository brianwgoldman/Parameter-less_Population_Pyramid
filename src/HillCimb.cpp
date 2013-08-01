/*
 * HillCimb.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "HillClimb.h"
#include <algorithm>

using namespace std;


template <>
hill_climb::pointer Configuration::get(const string key)
{
	return hill_climb::lookup[get<string>(key)];
}

void hill_climb::first_improvement(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
{
	// set up data structure for random bit selection
	vector<int> options(solution.size());
	iota(options.begin(), options.end(), 0);
	float new_fitness;
	bool improvement;
	do
	{
		improvement = false;
		std::shuffle(options.begin(), options.end(), rand);
		for(const auto& index: options)
		{
			solution[index] = not solution[index];
			new_fitness = evaluator.evaluate(solution);
			if(fitness < new_fitness)
			{
				fitness = new_fitness;
				improvement = true;
			}
			else
			{
				solution[index] = not solution[index];
			}
		}
	} while(improvement);
}

void hill_climb::steepest_ascent(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
{
	float new_fitness;
	bool improved;
	vector<size_t> bests;
	size_t previous = -1;
	// Loop as long as improvement is made
	do
	{
		improved = false;
		for(size_t working=0; working < solution.size(); working++)
		{
			if(working != previous)
			{
				solution[working] = not solution[working];
				new_fitness = evaluator.evaluate(solution);
				if(fitness <= new_fitness)
				{
					if(fitness < new_fitness)
					{
						fitness = new_fitness;
						improved=true;
						bests.clear();
					}
					bests.push_back(working);
				}
				solution[working] = not solution[working];
			}
		}
		if(improved)
		{
			int index = std::uniform_int_distribution<int>(0, bests.size() - 1)(rand);
			previous = bests[index];
			solution[previous] = not solution[previous];
			bests.clear();
		}
	} while(improved);
}

void hill_climb::no_action(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
{

}

void hill_climb::binary_tournament(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
{
	auto guess = rand_vector(rand, solution.size());
	float guess_fitness = evaluator.evaluate(guess);
	if(fitness < guess_fitness)
	{
		solution = guess;
		fitness = guess_fitness;
	}
}

void hill_climb::once_each(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
{
	vector<int> options(solution.size());
	iota(options.begin(), options.end(), 0);
	float new_fitness;
	std::shuffle(options.begin(), options.end(), rand);
	for(const auto& index: options)
	{
		solution[index] = not solution[index];
		new_fitness = evaluator.evaluate(solution);
		if(fitness < new_fitness)
		{
			fitness = new_fitness;
		}
		else
		{
			solution[index] = not solution[index];
		}
	}
}
