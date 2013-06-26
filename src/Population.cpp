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

float Population::get_distance(int x, int y)
{
	if(x > y)
	{
		std::swap(x, y);
	}
	return pairwise_distance[x][y];
}

float Population::get_distance(const vector<int> & c1, const vector<int> & c2)
{
	float distance = 0;
	for(int x: c1)
	{
		for(int y: c2)
		{
			distance += get_distance(x, y);
		}
	}
	return distance / (c1.size() * c2.size());
}

void Population::rebuild_tree(Random& rand)
{
	vector<vector<int> > clusters(length);
	for(size_t i=0; i < clusters.size(); i++)
	{
		clusters[i].push_back(i);
	}
	vector<pair<int, int> > minimums;
	float min_value;
	int choice;

	for(size_t index=0; index < masks.size(); index++)
	{
		min_value=3;
		minimums.clear();
		// for all pairs of clusters
		for(size_t i=0; i < clusters.size()-1; i++)
		{
			for(size_t j=i+1; j < clusters.size(); j++)
			{
				float distance;
				// TODO memory here
				distance = get_distance(clusters[i], clusters[j]);
				if(distance <= min_value)
				{
					if(distance < min_value)
					{
						min_value = distance;
						minimums.clear();
					}
					minimums.push_back(pair<int, int>(i, j));
				}
			}
		}
		// select a minimum at random
		choice = std::uniform_int_distribution<int>(0, minimums.size()-1)(rand);
		size_t first = minimums[choice].first;
		size_t second = minimums[choice].second;
		// Concatenate the clusters
		clusters[second].insert(clusters[second].end(),
				clusters[first].begin(), clusters[first].end());

		// save the combined cluster
		masks[index] = clusters[second];

		// swap the first to the end
		std::swap(clusters[first], clusters[clusters.size()-1]);

		// If the second choice was originally at the end
		if(second == clusters.size()-1)
		{
			second = first;
		}
		std::swap(clusters[second], clusters[clusters.size()-2]);
		clusters.pop_back();
	}
}
