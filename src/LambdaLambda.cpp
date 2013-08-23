/*
 * LambdaLambda.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: goldman
 */

#include "LambdaLambda.h"

void LambdaLambda::optimize(Random& rand, Evaluator& evaluator, Configuration& config)
{
	int length = config.get<int>("length");

	// initialize original individual
	vector<bool> solution = rand_vector(rand, length);
	float fitness = evaluator.evaluate(solution);

	// variables for number of offspring / mutation rate
	float lambda = 1;
	int flips;

	// storage for mutation offspring
	vector<bool> best_offspring;
	float best_offspring_fitness;

	// tool for choosing random mutation locations
	vector<int> options(length);
	std::iota(options.begin(), options.end(), 0);
	vector<std::uniform_int_distribution<>> selectors(length);
	for(int i=0; i < length; i++)
	{
		selectors[i] = std::uniform_int_distribution<>(i, length-1);
	}

	while(fitness < 1)
	{
		std::binomial_distribution<> binom(length, lambda / length);
		std::bernoulli_distribution cross(1 / lambda);
		do
		{
			flips = binom(rand);
		} while(flips == 0);

		// mutation loop
		best_offspring_fitness = -1;
		for(int i=0; i < round(lambda); i++)
		{
			vector<bool> next(solution);
			for(int j=0; j < flips; j++)
			{
				// swaps a random option into location j
				std::swap(options[j], options[selectors[j](rand)]);
				// flip the bit
				next[options[j]] = not next[options[j]];
			}
			float next_fitness = evaluator.evaluate(next);
			if(best_offspring_fitness < next_fitness)
			{
				best_offspring_fitness = next_fitness;
				best_offspring = next;
			}
		}

		// TODO Determine if you should keep best mutation if its better than all crossover
		// crossover loop
		vector<bool> next_parent(best_offspring);
		float next_parent_fitness = best_offspring_fitness;
		float next_fitness;
		for(int i=0; i < round(lambda); i++)
		{
			vector<bool> next(solution);
			for(int j=0; j < length; j++)
			{
				if(cross(rand))
				{
					next[j] = best_offspring[j];
				}
			}

			// TODO Mention that you prevent these evaluations
			if(next == solution)
			{
				next_fitness = fitness;
			}
			else if(next == best_offspring)
			{
				next_fitness = best_offspring_fitness;
			}
			else
			{
				next_fitness = evaluator.evaluate(next);
			}

			// replace if strictly better, or no worse but higher hamming distance
			if(next_parent_fitness < next_fitness or
					(next_parent_fitness == next_fitness and
							hamming_distance(solution, next_parent) <
							hamming_distance(solution, next)))
			{
				next_parent_fitness = next_fitness;
				next_parent = next;
			}
		}
		if(fitness < next_parent_fitness)
		{
			// magic number from paper
			lambda = lambda / 1.5;
			if(lambda < 1)
			{
				lambda = 1;
			}
		}
		else
		{
			// magic number for 1.5 ** 0.25
			lambda *= 1.1067;
		}
		if(fitness <= next_parent_fitness)
		{
			fitness = next_parent_fitness;
			solution = next_parent;
		}

		// TODO Mention that you do this restart mechanism
		if(lambda >= length)
		{
			solution = rand_vector(rand, length);
			fitness = evaluator.evaluate(solution);
			lambda = 1;
		}
	}
}

