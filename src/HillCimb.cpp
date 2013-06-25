/*
 * HillCimb.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "HillClimb.h"
#include <algorithm>

using namespace std;

void next_best(MTRand & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
{
	// set up data structure for random bit selection
	vector<int> options(solution.size(), 0);
	for(size_t i=0; i < options.size(); i++)
	{
		options[i] = i;
	}

	int unused;
	int index, working = 0;
	float new_fitness;
	bool improved;
	// Loop as long as improvement is made
	do
	{
		unused = options.size()-1;
		improved = false;
		while(unused >= 0 and not improved)
		{
			index = rand.randInt(unused);
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
	} while(improved);
}


