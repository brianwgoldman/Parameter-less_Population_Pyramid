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
#include <array>
#include <cmath>
#include <algorithm>
using std::vector;
using std::unordered_map;
using std::array;
using std::log;
#include "Util.h"

class Population
{
private:
	unordered_map<int, unordered_map<int, array<int, 4>>> occurrences;
	unordered_map<int, unordered_map<int, float> > pairwise_distance;
	float neg_entropy(const array<int, 4>& counts, const float& total);
	void update_entropy(int i, int j, const array<int, 4>& entry);

	float get_distance(int x, int y);
	float get_distance(const vector<int> & c1, const vector<int> & c2);
	int length;

public:
	Population(int l): length(l), masks(l-2) {}
	virtual ~Population() = default;
	vector<vector<bool> > solutions;
	vector<vector<int> > masks;

	void add(const vector<bool> & solution);
	void rebuild_tree(Random& rand);

};

#endif /* POPULATION_H_ */
