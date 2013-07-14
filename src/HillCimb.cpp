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
	auto options = indices(solution.size());

	int unused = options.size()-1;
	int index, working = 0;
	float new_fitness;
	bool improved;
	// Loop as long as improvement is made
	do
	{

		improved = false;
		while(unused >= 0 and not improved)
		{
			index = std::uniform_int_distribution<int>(0, unused)(rand);
			working = options[index];
			swap(options[unused], options[index]);
			unused -= 1;
			solution[working] = not solution[working];
			new_fitness = evaluator.evaluate(solution);
			if(fitness < new_fitness)
			{
				fitness = new_fitness;
				improved=true;
			}
			else
			{
				solution[working] = not solution[working];
			}
		}
		// The index used to make this improvement isn't valid next time
		swap(options[unused+1], options[options.size()-1]);
		unused = options.size()-2;
	} while(improved);
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
