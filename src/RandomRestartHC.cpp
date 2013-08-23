/*
 * RandomRestartHC.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: goldman
 */

#include "RandomRestartHC.h"

void RandomRestartHC::optimize(Random& rand, Evaluator& evaluator, Configuration& config)
{
	float fitness = 0;
	auto hill_climber = config.get<hill_climb::pointer>("hill_climber");
	int length = config.get<int>("length");
	int restarts_without_improvement = 0;
	while(fitness < 1.0 and restarts_without_improvement < length * length)
	{
		auto solution = rand_vector(rand, length);
		float current = evaluator.evaluate(solution);
		hill_climber(rand, solution, current, evaluator);
		if(fitness < current)
		{
			fitness = current;
			restarts_without_improvement = 0;
		}
		else
		{
			restarts_without_improvement++;
		}
	}
}
