/*
 * HillCimb.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "HillClimb.h"
#include <algorithm>

using namespace std;

void next_best(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
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


