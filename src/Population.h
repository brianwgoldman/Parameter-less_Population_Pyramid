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
using std::vector;
using std::unordered_map;
using std::array;
using std::log;

class Population
{
private:
	unordered_map<int, unordered_map<int, array<int, 4>>> occurrences;
	unordered_map<int, unordered_map<int, float> > pairwise_distance;
	float neg_entropy(const array<int, 4>& counts, const float& total);
	void update_entropy(int i, int j, const array<int, 4>& entry);

public:
	Population() = default;
	virtual ~Population() = default;
	vector<vector<bool> > solutions;

	void add(const vector<bool> & solution);
};

#endif /* POPULATION_H_ */
