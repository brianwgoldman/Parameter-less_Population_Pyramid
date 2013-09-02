/*
 * LambdaLambda.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: goldman
 */

#include "LambdaLambda.h"

LambdaLambda::LambdaLambda(Random& _rand, Evaluator& _evaluator, Configuration& _config):
		Optimizer(_rand, _evaluator, _config), selectors(length), options(length)
{
	solution = rand_vector(rand, length);
	fitness = evaluator.evaluate(solution);
	lambda = 1;

	// tool for choosing random mutation locations
	for(size_t i=0; i < length; i++)
	{
		selectors[i] = std::uniform_int_distribution<>(i, length-1);
	}
	std::iota(options.begin(), options.end(), 0);
}

vector<bool> LambdaLambda::mutate(const vector<bool>& parent, const int flips)
{
	vector<bool> mutant(solution);
	for(int j=0; j < flips; j++)
	{
		// swaps a random option into location j
		std::swap(options[j], options[selectors[j](rand)]);
		// flip the bit
		mutant[options[j]] = not mutant[options[j]];
	}
	return mutant;
}

vector<bool> LambdaLambda::crossover(const vector<bool>& p1, const vector<bool>& p2, std::bernoulli_distribution& prob)
{
	// copy over all of p1
	vector<bool> offspring(p1);
	for(size_t j=0; j < length; j++)
	{
		// take p2 at set probability
		if(prob(rand))
		{
			offspring[j] = p2[j];
		}
	}
	return offspring;
}

bool LambdaLambda::iterate()
{
	// variables for number of offspring / mutation rate
	int flips;

	// distributions needed at the current lambda
	std::binomial_distribution<> binom(length, lambda / length);
	std::bernoulli_distribution cross(1 / lambda);
	do
	{
		flips = binom(rand);
	} while(flips == 0);

	// mutation loop
	vector<bool> best_offspring = mutate(solution, flips);
	float best_offspring_fitness = evaluator.evaluate(best_offspring);
	for(int i=1; i < round(lambda); i++)
	{
		vector<bool> next = mutate(solution, flips);
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
		vector<bool> next = crossover(solution, best_offspring, cross);

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

		// TODO Mention hamming distance, not in original paper
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
	else // TODO Mention not the "Mod" version
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
	return true;
}

