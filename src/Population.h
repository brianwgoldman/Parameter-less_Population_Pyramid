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

public:
	Population(int l);
	virtual ~Population() = default;
	vector<vector<bool> > solutions;
	vector<vector<int> > clusters;
	vector<int> cluster_ordering;

	void add(const vector<bool> & solution);
	void improve(Random& rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);
	void rebuild_tree(Random& rand);

	void never_use_singletons();
	void rand_smallest_first(Random& rand);

};

#endif /* POPULATION_H_ */
