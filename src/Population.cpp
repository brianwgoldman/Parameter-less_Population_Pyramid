/*
 * Population.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: goldman
 */

#include "Population.h"
#include <iostream>
using namespace std;

void Population::add(const vector<bool> & solution)
{
	solutions.push_back(solution);
	for(size_t i=0;i<solution.size()-1; i++)
	{
		for(size_t j=i+1; j < solution.size(); j++)
		{
			auto& entry = occurrences[i][j];
			entry[(solution[j] << 1) + solution[i]]++;
			update_entropy(i, j, entry);
		}
	}
}

float Population::neg_entropy(const array<int, 4>& counts, const float& total)
{
	float sum = 0;
	float p;
	for (const auto& value: counts)
	{
		if(value)
		{
			p = value / total;
			sum += (p * log(p));
		}
	}
	return sum;
}

void Population::update_entropy(int i, int j, const array<int, 4>& entry)
{
	array<int, 4> bits;
	bits[0] = entry[0] + entry[2]; // i zero
	bits[1] = entry[1] + entry[3]; // i one
	bits[2] = entry[0] + entry[1]; // j zero
	bits[3] = entry[2] + entry[3]; // j one
	float total = bits[0] + bits[1];
	float separate = neg_entropy(bits, total);
	float together = neg_entropy(entry, total);
	float ratio = 1;
	if (together)
	{
		ratio = (separate / together);
	}
	pairwise_distance[i][j] = 2 - ratio;
}
