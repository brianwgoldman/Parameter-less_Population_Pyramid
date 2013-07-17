/*
 * Evaluation.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "Evaluation.h"

float Evaluator::evaluate(const vector<bool> & solution)
{
	float sum = 0;
	for(const bool & bit: solution)
	{
		sum += bit;
	}
	return sum / solution.size();
}

float DeceptiveTrap::evaluate(const vector<bool> & solution)
{
	int partial;
	int total=0;

	for(size_t i=0; i < solution.size(); i+=trap_size)
	{
		partial=0;
		for(size_t index=i; index<i+trap_size; index++)
		{
			partial += solution[index];
		}
		if(partial < trap_size)
		{
			partial = trap_size - partial - 1;
		}
		total += partial;
	}
	return float(total) / solution.size();
}

float DeceptiveStepTrap::evaluate(const vector<bool> & solution)
{
	int partial;
	int total=0;

	int trap_maximum = (offset + trap_size) / step_size;
	for(size_t i=0; i < solution.size(); i+=trap_size)
	{
		partial=0;
		for(size_t index=i; index<i+trap_size; index++)
		{
			partial += solution[index];
		}
		if(partial < trap_size)
		{
			partial = trap_size - partial - 1;
		}
		total += (offset + partial) / step_size;
	}
	return (float(total) * trap_size) / (solution.size() * trap_maximum);
}

NearestNeighborNK::NearestNeighborNK(Configuration& config, int run_number)
{
	k = config.get<int>("k");
	length = config.get<int>("length");
	table.resize(length, vector<float>(2 << k, 0));

	Random rand(config.get<int>("problem_seed") + run_number);
	auto generator = std::uniform_real_distribution<>(0, 1);
	for(auto& row: table)
	{
		for(auto& entry: row)
		{
			entry = generator(rand);
		}
	}
	minimum = solve(worst, false);
	maximum = solve(best, true);
	precision = config.get<int>("precision");
}

float NearestNeighborNK::chunk_fitness(trimap& known, size_t chunk_index, size_t a, size_t b)
{
	// If we have an answer, return it
	const auto& first = known.find(chunk_index);
	if(first != known.end())
	{
		const auto& second = first->second.find(a);
		if(second != first->second.end())
		{
			const auto& third = second->second.find(b);
			if(third != second->second.end())
			{
				return third->second;
			}
		}
	}

	// calculate the answer
	float fitness=0;
	size_t mask = (2 << k) - 1;

	size_t combined = (a << k) | b;
	combined = (combined << 1) | (a & 1);

	for(size_t g=0; g < k; g++)
	{
		size_t value = (combined >> (k-g)) & mask;
		fitness += table[chunk_index * k + g][value];
	}
	known[chunk_index][a][b] = fitness;
	return fitness;
}

void NearestNeighborNK::int_into_bit(size_t src, vector<bool>& dest)
{
	for(size_t i=1; i <= k; i++)
	{
		dest.push_back((src >> (k-i)) & 1);
	}
}

float NearestNeighborNK::solve(vector<bool>& solution, bool maximize)
{
	size_t numbers = 1 << k;
	trimap known;
	std::unordered_map<size_t,
	std::unordered_map<size_t,
	std::unordered_map<size_t, size_t> > > partial;
	float current;
	for(size_t n=length / k - 1; n > 1; n--)
	{
		std::unordered_map<size_t,
		std::unordered_map<size_t, float> > utility;
		std::unordered_map<size_t,
		std::unordered_map<size_t, size_t> > value;
		for(size_t left=0; left < numbers; left++)
		{
			for(size_t right=0; right < numbers; right++)
			{
				utility[left][right] = -1;
				if(not maximize)
				{
					utility[left][right] = 2 * length;
				}
				for(size_t middle=0; middle < numbers; middle++)
				{
					current = chunk_fitness(known, n-1, left, middle);
					current += chunk_fitness(known, n, middle, right);
					if((maximize and utility[left][right] < current) or
					   (not maximize and utility[left][right] > current))
					{
						utility[left][right] = current;
						value[left][right] = middle;
					}
				}
			}
		}

		// extract information
		for(size_t left=0; left < numbers; left++)
		{
			for(size_t right=0; right < numbers; right++)
			{
				known[n-1][left][right] = utility[left][right];
				partial[n][left][right] = value[left][right];
			}
		}
	}

	// Top level decision
	float fitness=-1;
	if(not maximize)
	{
		fitness = 2 * length;
	}
	size_t best_left=0;
	size_t best_right=0;
	for(size_t left=0; left < numbers; left++)
	{
		for(size_t right=0; right < numbers; right++)
		{
			current = chunk_fitness(known, 0, left, right);
			current += chunk_fitness(known, 1, right, left);
			if((maximize and fitness < current) or
				(not maximize and fitness > current))
			{
				fitness = current;
				best_left = left;
				best_right = right;
			}
		}
	}

	// Recreate the optimal string
	solution.clear();
	solution.reserve(length);
	int_into_bit(best_left, solution);
	int_into_bit(best_right, solution);
	size_t last = best_right;
	for(size_t i=2; i < length / k; i++)
	{
		last = partial[i][last][best_left];
		int_into_bit(last, solution);
	}

	return fitness;
}

float NearestNeighborNK::evaluate(const vector<bool> & solution)
{
	float total = 0;
	for(size_t i=0; i < solution.size(); i++)
	{
		size_t index=0;
		for(size_t neighbor=i; neighbor <= i + k; neighbor++)
		{
			index = (index<<1) | solution[neighbor%length];
		}
		total += table[i][index];
	}
	float fitness = (total-minimum) / (maximum - minimum);
	fitness = round(fitness * precision) / precision;
	if(solution == best)
	{
		if(fitness < 1)
		{
			std::cout<<"YEP "<<fitness << std::endl;
		}
	}
	return fitness;
}
