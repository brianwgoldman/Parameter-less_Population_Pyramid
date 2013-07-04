/*
 * Population.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: goldman
 */

#include "Population.h"
#include <iostream>
using namespace std;

Population::Population(int l)
{
	length = l;
	clusters.resize(2*length - 1);
	for(size_t i = 0; i < length; i++)
	{
		clusters[i].push_back(i);
	}
}

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
	unordered_set<size_t> usable;
	for(size_t i=0; i < length; i++)
	{
		usable.insert(i);
	}
	// shuffle the single variable clusters
	shuffle(clusters.begin(), next(clusters.begin(), length), rand);
	vector<pair<int, int> > minimums;
	float min_value;
	int choice;

	unordered_map<int, unordered_map<int, float> > distances;
	for(size_t i=0; i < length - 1; i++)
	{
		for(size_t j=i + 1; j < length; j++)
		{
			distances[i][j] = get_distance(clusters[i], clusters[j]);
		}
	}

	// rebuild all clusters after the single variable clusters
	for(size_t index=length; index < clusters.size(); index++)
	{
		min_value=3;
		minimums.clear();
		// for all pairs of clusters
		for(auto i=usable.begin(); i != usable.end(); i++)
		{
			for(auto j=next(i); j != usable.end(); j++)
			{
				auto x = *i;
				auto y = *j;
				if(x > y)
				{
					std::swap(x, y);
				}
				float distance = distances[x][y];
				if(distance <= min_value)
				{
					if(distance < min_value)
					{
						min_value = distance;
						minimums.clear();
					}
					minimums.push_back(pair<int, int>(*i, *j));
				}
			}
		}
		// select a minimum at random
		choice = std::uniform_int_distribution<int>(0, minimums.size()-1)(rand);
		size_t first = minimums[choice].first;
		size_t second = minimums[choice].second;

		// create new cluster
		clusters[index] = clusters[first];
		clusters[index].insert(clusters[index].end(),
				clusters[second].begin(), clusters[second].end());
		usable.erase(first);
		usable.erase(second);

		for(auto i=usable.begin(); i != usable.end(); i++)
		{
			auto x = *i;
			float first_distance;
			float second_distance;
			if(x > first)
			{
				first_distance = distances[first][x];
			}
			else
			{
				first_distance = distances[x][first];
				distances[x].erase(first);
			}

			first_distance *= clusters[first].size();
			if(x > second)
			{
				second_distance = distances[second][x];
			}
			else
			{
				second_distance = distances[x][second];
				distances[x].erase(first);
			}

			second_distance *= clusters[second].size();
			distances[x][index] = ((first_distance + second_distance) /
						   (clusters[first].size() + clusters[second].size()));

		}
		distances.erase(first);
		distances.erase(second);
		//*/
		usable.insert(index);
	}
}

bool Population::donate(vector<bool> & solution, float & fitness, vector<bool> & source, const vector<int> & cluster, Evaluator& evaluator)
{
	// swap all of the cluster indices, watching for any change
	bool changed=false;
	for(const auto& index: cluster)
	{
		changed |= (solution[index] != source[index]);
		vector<bool>::swap(solution[index], source[index]);
	}

	if(changed)
	{
		float new_fitness = evaluator.evaluate(solution);
		if(fitness <= new_fitness)
		{
			fitness = new_fitness;
			// copy pattern back into the source, leave solution changed
			for(const auto& index: cluster)
			{
				source[index] = solution[index];
			}
		}
		else
		{
			// revert both solution and source
			for(const auto& index: cluster)
			{
				vector<bool>::swap(solution[index], source[index]);
			}
		}
	}
	return changed;
}

void Population::improve(Random& rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
{
	auto options = indices(solutions.size());
	int unused;
	int index, working = 0;
	bool different;

	// for each cluster of size > 1
	for(size_t i = length; i < clusters.size(); i++)
	{
		unused = options.size()-1;
		different = false;
		// Find a donor which has at least one gene value different
		// from the current solution for this cluster of genes
		while(unused >= 0 and not different)
		{
			// choose a donor
			index = std::uniform_int_distribution<int>(0, unused)(rand);
			working = options[index];
			swap(options[unused], options[index]);
			unused -= 1;

			// attempt the donation
			different = donate(solution, fitness, solutions[working], clusters[i], evaluator);
		}
	}
}
