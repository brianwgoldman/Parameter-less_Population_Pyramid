/*
 * Population.h
 *
 *  Created on: Jun 25, 2013
 *      Author: goldman
 */

#ifndef POPULATION_H_
#define POPULATION_H_
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <cmath>
#include <algorithm>
#include <numeric>
using std::vector;
using std::unordered_map;
using std::array;
using std::log;
#include "Util.h"
#include "Evaluation.h"

class Population
{
private:

	unordered_map<int, unordered_map<int, array<int, 4>>> occurrences;
	unordered_map<int, unordered_map<int, float> > pairwise_distance;
	float neg_entropy(const array<int, 4>& counts, const float& total);
	void update_entropy(int i, int j, const array<int, 4>& entry);

	float get_distance(int x, int y);
	float get_distance(const vector<int> & c1, const vector<int> & c2);
	size_t length;

	bool donate(vector<bool> & solution, float & fitness, vector<bool> & source, const vector<int> & cluster, Evaluator& evaluator);
	static bool minimize(const vector<vector<float>> & distances, const vector<size_t>& usable, const size_t& first, size_t & second);

	using tree_build=void (*)(Random& rand, const vector<vector<float>> & distances, vector<size_t>& usable, size_t& first, size_t & second);
	tree_build builder;
	static void new_way(Random& rand, const vector<vector<float>> & distances, vector<size_t>& usable, size_t& first, size_t & second);
	static void old_way(Random& rand, const vector<vector<float>> & distances, vector<size_t>& usable, size_t& first, size_t & second);
	std::unordered_map<string, tree_build> builder_lookup = {
			{"new_way", new_way},
			{"old_way", old_way}
	};


	using cluster_ordering_method=void (*)(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
	cluster_ordering_method ordering;

	static void no_action(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
	static void smallest_first(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
	static void random(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
	static void least_linked_first(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
	std::unordered_map<string, cluster_ordering_method> ordering_lookup = {
			{"no_action", no_action},
			{"smallest_first", smallest_first},
			{"random", random},
			{"least_linked_first", least_linked_first},
	};

	bool no_singles;
	void never_use_singletons();
public:
	Population(Configuration& config);
	virtual ~Population() = default;
	vector<vector<bool> > solutions;
	vector<vector<int> > clusters;
	vector<int> cluster_ordering;

	void add(const vector<bool> & solution);
	void improve(Random& rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);
	void rebuild_tree(Random& rand);


};

#endif /* POPULATION_H_ */
