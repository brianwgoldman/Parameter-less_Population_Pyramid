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

#include "Util.h"
#include "Evaluation.h"

using std::vector;
using std::unordered_map;
using std::array;
using std::log;

class Population {
 public:
  Population(Configuration& config);
  virtual ~Population() = default;
  vector<vector<bool> > solutions;

  void add(const vector<bool> & solution, bool use_in_tree = true);
  void improve(Random& rand, vector<bool> & solution, float & fitness,
               Evaluator& evaluator);
  void rebuild_tree(Random& rand);

 private:
  vector<vector<int> > clusters;
  vector<int> cluster_ordering;

  unordered_map<int, unordered_map<int, array<int, 4>>> occurrences;
  unordered_map<int, unordered_map<int, float> > pairwise_distance;

  template <size_t T>
  float neg_entropy(const array<int, T>& counts, const float& total);
  void update_entropy(int i, int j, const array<int, 4>& entry);

  float get_distance(int x, int y);
  float get_distance(const vector<int> & c1, const vector<int> & c2);
  size_t length;
  bool stop_after_one;

  bool donate(vector<bool> & solution, float & fitness, vector<bool> & source,
      const vector<int> & cluster, Evaluator& evaluator);

  using cluster_ordering_method=void (*)(Random& rand,
      const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  cluster_ordering_method ordering;

  static void no_action(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  static void smallest_first(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  static void random(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  static void least_linked_first(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  std::unordered_map<string, cluster_ordering_method> ordering_lookup = {
    { "no_action", no_action},
    { "smallest_first", smallest_first},
    { "random", random},
    { "least_linked_first", least_linked_first},
  };

  bool no_singles;
  int precision;
  bool keep_zeros;
};

template<size_t T>
float Population::neg_entropy(const array<int, T>& counts, const float& total) {
  float sum = 0;
  float p;
  for (const auto& value : counts) {
    if (value) {
      p = value / total;
      sum += (p * log(p));
    }
  }
  return sum;
}

#endif /* POPULATION_H_ */
