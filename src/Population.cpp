/*
 * Population.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: goldman
 */

#include "Population.h"
#include <iostream>
using namespace std;

Population::Population(Configuration& config)
{
	length = config.get<int>("length");
	clusters.resize(2*length - 1);
	cluster_ordering.resize(clusters.size());
	for(size_t i = 0; i < length; i++)
	{
		clusters[i].push_back(i);
	}
	for(size_t i = 0; i < cluster_ordering.size(); i++)
	{
		cluster_ordering[i] = i;
	}
	builder = builder_lookup[config.get<string>("tree_builder")];
	ordering = ordering_lookup[config.get<string>("cluster_ordering")];
	no_singles = config.get<int>("no_singles");
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

bool Population::minimize(const vector<vector<float>> & distances, const vector<size_t>& usable, const size_t& first, size_t & second)
{
	float min_value = distances[first][second];
	bool change = false;
	for(const auto i: usable)
	{
		if(distances[first][i] < min_value and i != first)
		{
			change = true;
			min_value = distances[first][i];
			second = i;
		}
	}
	return change;
}

void Population::new_way(Random& rand, const vector<vector<float>> & distances, vector<size_t>& usable, size_t& first, size_t & second)
{
	std::shuffle(usable.begin(), usable.end(), rand);
	first = *usable.begin();
	second = *next(usable.begin());
	while(minimize(distances, usable, first, second))
	{
		swap(first, second);
		std::shuffle(usable.begin(), usable.end(), rand);
	}
}

void Population::old_way(Random& rand, const vector<vector<float>> & distances, vector<size_t>& usable, size_t& first, size_t & second)
{
	float min_value=3;
	vector<pair<int, int> > minimums;
	// for all pairs of clusters
	for(auto i=usable.begin(); i != usable.end(); i++)
	{
		for(auto j=next(i); j != usable.end(); j++)
		{
			auto x = *i;
			auto y = *j;
			float distance = distances[x][y];
			if(distance <= min_value)
			{
				if(distance < min_value)
				{
					min_value = distance;
					minimums.clear();
				}
				minimums.push_back(pair<int, int>(x, y));
			}
		}
	}
	// select a minimum at random
	int choice = std::uniform_int_distribution<int>(0, minimums.size()-1)(rand);
	first = minimums[choice].first;
	second = minimums[choice].second;
}

void Population::rebuild_tree(Random& rand)
{
	vector<size_t> usable(length);
	std::iota(usable.begin(), usable.end(), 0);
	// shuffle the single variable clusters
	shuffle(clusters.begin(), clusters.begin() + length, rand);

	vector<vector<float> > distances(clusters.size(), vector<float>(clusters.size(), -1));
	for(size_t i=0; i < length - 1; i++)
	{
		for(size_t j=i + 1; j < length; j++)
		{
			distances[i][j] = get_distance(clusters[i][0], clusters[j][0]);
			distances[j][i] = distances[i][j];
		}
	}
	size_t first, second;
	// rebuild all clusters after the single variable clusters
	for(size_t index=length; index < clusters.size(); index++)
	{
		builder(rand, distances, usable, first, second);
		// create new cluster
		clusters[index] = clusters[first];
		clusters[index].insert(clusters[index].end(),
				clusters[second].begin(), clusters[second].end());
		int i = 0;
		int end = usable.size() - 1;
		while(i <= end)
		{
			auto x = usable[i];
			if(x == first or x == second)
			{
				swap(usable[i], usable[end]);
				end--;
				continue;
			}
			float first_distance = distances[first][x];
			first_distance *= clusters[first].size();
			float second_distance = distances[second][x];
			second_distance *= clusters[second].size();
			distances[x][index] = ((first_distance + second_distance) /
						   (clusters[first].size() + clusters[second].size()));
			distances[index][x] = distances[x][index];
			i++;
		}
		usable.pop_back();
		usable.back() = index;
	}
	cluster_ordering.resize(clusters.size());
	std::iota(cluster_ordering.begin(), cluster_ordering.end(), 0);
	if(no_singles)
	{
		never_use_singletons();
	}
	ordering(rand, clusters, cluster_ordering);
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
		// NOTE: My previous work used strict improvement
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
	for(auto& cluster_index: cluster_ordering)
	{
		auto cluster = clusters[cluster_index];
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
			different = donate(solution, fitness, solutions[working], cluster, evaluator);
		}
	}
}

void Population::never_use_singletons()
{
	cluster_ordering.resize(length - 1);
	for(size_t i=0; i < cluster_ordering.size(); i++)
	{
		cluster_ordering[i] = i + length;
	}
}

void Population::smallest_first(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering)
{
	// NOTE: My previous work did not shuffle here
	std::shuffle(cluster_ordering.begin(), cluster_ordering.end(), rand);
	std::stable_sort(cluster_ordering.begin(), cluster_ordering.end(),
			[clusters](int x, int y) { return clusters[x].size() < clusters[y].size();});
}

void Population::least_linked_first(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering)
{
	std::reverse(cluster_ordering.begin(), cluster_ordering.end());
}


void Population::no_action(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering)
{

}

void Population::random(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering)
{
	std::shuffle(cluster_ordering.begin(), cluster_ordering.end(), rand);
}
