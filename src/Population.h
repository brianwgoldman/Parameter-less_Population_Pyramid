// Brian Goldman

// This is the work-horse of our implementation, abstracting
// out large, complicated pieces of P3 and LTGA.  The Population
// stores a collection of solutions and maintains the pairwise entropy
// tables for those solutions.  It also provides the tools to construct
// linkage trees and to perform population level crossover

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

// Stores all information related to a population
class Population {
 public:
  Population(Configuration& config);
  virtual ~Population() = default;
  // Storage for all of the solutions in the population
  vector<vector<bool> > solutions;

  // Adds a solution to the population.  Optional argument to prevent
  // the solution from being used in entropy calculations
  void add(const vector<bool> & solution, bool use_in_tree = true);
  // Improves the passed in solution using crossover, as guided by the
  // entropy table and solutions in the population.  "fitness" should
  // start as the fitness of "solution", and both will be set to the
  // improved values after application.
  void improve(Random& rand, vector<bool> & solution, float & fitness,
               Evaluator& evaluator);
  // Access the entropy table to construct a linkage tree
  void rebuild_tree(Random& rand);

 private:
  // Tools used for cluster creation
  vector<vector<int> > clusters;
  vector<int> cluster_ordering;

  // Keeps track of how often each possible pairing of bit values occurs in the population
  // for all pairs of genes
  unordered_map<int, unordered_map<int, array<int, 4>>> occurrences;
  // Tracks the entropy distance between pairs of genes
  unordered_map<int, unordered_map<int, float> > pairwise_distance;

  // Tool used to calculate the negative entropy of a list of occurrences
  template <size_t T>
  float neg_entropy(const array<int, T>& counts, const float& total);

  // Given a list of occurrences, updates the pairwise_distance for the
  // related genes
  void update_entropy(int i, int j, const array<int, 4>& entry);

  // tool to access pairwise distance for two genes
  float get_distance(int x, int y);

  // Donates the genes specified by "cluster" from "source" into "solution".
  // If the solution was modified by this action, it is evaluated.  If the new
  // fitness is no worse the modification is kept and the fitness value is updated.
  // Returns true if an evaluation was performed.
  bool donate(vector<bool> & solution, float & fitness, vector<bool> & source,
      const vector<int> & cluster, Evaluator& evaluator);

  // Cluster Ordering Methods - These specify how to take the clusters created by linking and convert
  // them into an order for application with crossover.

  // Tool used to allow configuration of cluster ordering
  using cluster_ordering_method=void (*)(Random& rand,
      const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  cluster_ordering_method ordering;

  // Leave the clusters in the order they were created
  static void no_action(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  // Sort clusters by size, smallest first.  Randomize order of equal sized clusters
  static void smallest_first(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  // Shuffle the order of clusters
  static void random(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);
  // Reverse the order of clusters such that the cluster with the tightest linkage appears last.
  static void least_linked_first(Random& rand, const vector<vector<int>>& clusters, vector<int>& cluster_ordering);

  // Lookup tool used to convert a string name to a ordering function.
  std::unordered_map<string, cluster_ordering_method> ordering_lookup = {
    { "no_action", no_action},
    { "smallest_first", smallest_first},
    { "random", random},
    { "least_linked_first", least_linked_first},
  };

  // General configuration options
  // Number of genes in the genome
  size_t length;
  // Even if the crossover did not result in an evaluation, stop trying more donors.
  bool stop_after_one;
  // Automatically throw out all crossover clusters of size 1
  bool no_singles;
  // Rounding precision
  int precision;
  // Even if two clusters have zero distance, keep them both as crossover clusters.
  bool keep_zeros;
};

// Returns the negative of the entropy given the list of counts and a total number,
// where total = sum(counts)
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
