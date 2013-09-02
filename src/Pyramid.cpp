/*
 * Pyramid.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: goldman
 */

#include "Pyramid.h"

void Pyramid::climb(vector<bool> & solution, float & fitness)
{
	add_unique(solution, 0);
	for(size_t level=0; level < pops.size(); level++)
	{
		float prev = fitness;
		//use population clusters and population solutions to make new solution
		pops[level].improve(rand, solution, fitness, evaluator);
		if(not only_add_improvements or prev < fitness)
		{
			add_unique(solution, level+1);
		}
	}
}

bool Pyramid::add_unique(const vector<bool> & solution, size_t level)
{
	if(seen.find(solution) == seen.end())
	{
		if(pops.size() == level)
		{
			pops.push_back(Population(config));
		}
		pops[level].add(solution);
		pops[level].rebuild_tree(rand);
		seen.insert(solution);
		return true;
	}
	return false;
}

bool Pyramid::iterate()
{
	vector<bool> solution = rand_vector(rand, length);
	float fitness = evaluator.evaluate(solution);
	hill_climber(rand, solution, fitness, evaluator);
	climb(solution, fitness);
	// P3 never "converges"
	return true;
}
