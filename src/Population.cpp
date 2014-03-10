// Brian Goldman

// Implements the population structure, which includes pairwise entropy
// recording, linkage detection, and crossover

#include "Population.h"
#include <iostream>
using namespace std;

Population::Population(Configuration& config) {
  length = config.get<int>("length");
  // initializes clusters to have the maximium possible number of spaces
  clusters.resize(2 * length - 1);
  cluster_ordering.resize(clusters.size());
  // first "length" clusters just contain a single gene
  for (size_t i = 0; i < length; i++) {
    clusters[i].push_back(i);
  }
  // initially the ordering is just to use the clusters in numeric order
  for (size_t i = 0; i < cluster_ordering.size(); i++) {
    cluster_ordering[i] = i;
  }

  // sets up the ordering function
  ordering = ordering_lookup[config.get<string>("cluster_ordering")];
  no_singles = config.get<int>("no_singles");
  stop_after_one = config.get<int>("donate_until_different") != 1;
  precision = config.get<int>("precision");
  keep_zeros = config.get<int>("keep_zeros");
}

// Puts the solution into the population, updates the entropy table as requested
void Population::add(const vector<bool> & solution, bool use_in_tree) {
  solutions.push_back(solution);
  if (not use_in_tree) {
    return;
  }

  // loop through all pairs of genes and update the pairwise counts / entropy
  for (size_t i = 0; i < solution.size() - 1; i++) {
    for (size_t j = i + 1; j < solution.size(); j++) {
      auto& entry = occurrences[i][j];
      // Updates the entry of the 4 long array based on the two bits
      entry[(solution[j] << 1) + solution[i]]++;
      update_entropy(i, j, entry);
    }
  }
}

// Records the actual pairwise distance based of the two genes based on the counts
void Population::update_entropy(int i, int j, const array<int, 4>& entry) {
  array<int, 4> bits;
  // extracts the occurrences of the individual bits
  bits[0] = entry[0] + entry[2];  // i zero
  bits[1] = entry[1] + entry[3];  // i one
  bits[2] = entry[0] + entry[1];  // j zero
  bits[3] = entry[2] + entry[3];  // j one
  float total = bits[0] + bits[1];
  // entropy of the two bits on their own
  float separate = neg_entropy(bits, total);
  // entropy of the two bits as a single unit
  float together = neg_entropy(entry, total);

  // If together there is 0 entropy, the distance is zero
  float ratio = 0;
  if (together) {
    ratio = 2 - (separate / together);
  }
  pairwise_distance[i][j] = ratio;
}

// Wrapper to ensure that x < y when accessing the pairwise_distance table
float Population::get_distance(int x, int y) {
  if (x > y) {
    std::swap(x, y);
  }
  return pairwise_distance[x][y];
}

// Uses the entropy table to construct the clusters to use for crossover.
// Developed from the O(N^2) method described in:
// "Optimal implementations of UPGMA and other common clustering algorithms"
// by I. Gronau and S. Moran
void Population::rebuild_tree(Random& rand) {
  // usable keeps track of which clusters can still be merged
  vector<size_t> usable(length);
  // initialize it to just the clusters of size 1
  std::iota(usable.begin(), usable.end(), 0);
  // useful keeps track of clusters that should be used by crossover
  vector<bool> useful(clusters.size(), true);
  // Shuffle the single variable clusters
  shuffle(clusters.begin(), clusters.begin() + length, rand);

  // keeps track of the distances between clusters
  vector<vector<float> > distances(clusters.size(),
                                   vector<float>(clusters.size(), -1));

  // Find the initial distances between the clusters
  for (size_t i = 0; i < length - 1; i++) {
    for (size_t j = i + 1; j < length; j++) {
      distances[i][j] = get_distance(clusters[i][0], clusters[j][0]);
      // make it symmetric
      distances[j][i] = distances[i][j];
    }
  }

  size_t first, second;
  size_t final, best_index;
  // Each iteration we add some amount to the path, and remove the last
  // two elements.  This keeps track of how much of usable is in the path.
  size_t end_of_path = 0;

  // rebuild all clusters after the single variable clusters
  for (size_t index = length; index < clusters.size(); index++) {
    // Shuffle everything not yet in the path
    std::shuffle(usable.begin() + end_of_path, usable.end(), rand);

    // if nothing in the path, just add a random usable node
    if (end_of_path == 0) {
      end_of_path++;
    }

    while (end_of_path < usable.size()) {
      // last node in the path
      final = usable[end_of_path - 1];

      // best_index stores the location of the best thing in usable
      best_index = end_of_path;
      float min_dist = distances[final][usable[best_index]];
      // check all options which might be closer to "final" than "usable[best_index]"
      for (size_t option = end_of_path + 1; option < usable.size(); option++) {
        if (distances[final][usable[option]] < min_dist) {
          min_dist = distances[final][usable[option]];
          best_index = option;
        }
      }

      // If the current last two in the path are minimally distant
      if (end_of_path > 1
          and min_dist >= distances[final][usable[end_of_path - 2]]) {
        break;
      }

      // move the best to the end of the path
      swap(usable[end_of_path], usable[best_index]);
      end_of_path++;
    }

    // Last two elements in the path are the clusters to join
    first = usable[end_of_path - 2];
    second = usable[end_of_path - 1];

    // If the distance between the joining clusters is zero, only keep them
    // if configured to do so
    if (float_round(distances[first][second], precision) == 0) {
      useful[first] = keep_zeros;
      useful[second] = keep_zeros;
    }

    // Remove things from the path
    end_of_path -= 2;

    // create new cluster
    clusters[index] = clusters[first];
    // merge the two clusters
    clusters[index].insert(clusters[index].end(), clusters[second].begin(),
                           clusters[second].end());

    // Calculate distances from all clusters to the newly created cluster
    int i = 0;
    int end = usable.size() - 1;
    while (i <= end) {
      auto x = usable[i];
      // Removes 'first' and 'second' from usable
      if (x == first or x == second) {
        swap(usable[i], usable[end]);
        end--;
        continue;
      }
      // Use the previous distances to calculate the joined distance
      float first_distance = distances[first][x];
      first_distance *= clusters[first].size();
      float second_distance = distances[second][x];
      second_distance *= clusters[second].size();
      distances[x][index] = ((first_distance + second_distance)
          / (clusters[first].size() + clusters[second].size()));
      // make it symmetric
      distances[index][x] = distances[x][index];
      i++;
    }
    // Shorten usable by 1, insert the new cluster
    usable.pop_back();
    usable.back() = index;
  }

  // Now that we know what clusters exist, determine their ordering
  cluster_ordering.resize(clusters.size());
  std::iota(cluster_ordering.begin(), cluster_ordering.end(), 0);
  if (no_singles) {
    // marks all clusters of size 1 as not useful
    for (size_t i = 0; i < length; i++) {
      useful[i] = false;
    }
  }

  // The last cluster contains all variables and is always useless
  useful.back() = false;

  // remove not useful clusters
  size_t kept = 0;
  for (size_t i = 0; i < cluster_ordering.size(); i++) {
    if (useful[cluster_ordering[i]]) {
      swap(cluster_ordering[i], cluster_ordering[kept]);
      kept++;
    }
  }
  cluster_ordering.resize(kept);

  // Applies the desired ordering to the clusters
  ordering(rand, clusters, cluster_ordering);
}

// Moves gene values for "cluster" genes from "source" into "solution".  Performs
// an evaluation if a change was actually made to "solution".  Reverts that
// change if the new fitness was worse than before the change.  Returns
// true if an evaluation was performed.
bool Population::donate(vector<bool> & solution, float & fitness,
                        vector<bool> & source, const vector<int> & cluster,
                        Evaluator& evaluator) {
  // swap all of the cluster indices, watching for any change
  bool changed = false;
  for (const auto& index : cluster) {
    changed |= (solution[index] != source[index]);
    // uses the "source" to store the original value of "solution"
    vector<bool>::swap(solution[index], source[index]);
  }

  if (changed) {
    float new_fitness = evaluator.evaluate(solution);
    // NOTE: My previous work used strict improvement
    if (fitness <= new_fitness) {
      // improvement made, keep change to solution
      fitness = new_fitness;
      // copy pattern back into the source, leave solution changed
      for (const auto& index : cluster) {
        source[index] = solution[index];
      }
    } else {
      // revert both solution and source
      for (const auto& index : cluster) {
        vector<bool>::swap(solution[index], source[index]);
      }
    }
  }
  return changed;
}

// Performs an entire crossover event, applying all discovered clusters, in an
// attempt to use the population to improve "solution".
void Population::improve(Random& rand, vector<bool> & solution, float & fitness,
                         Evaluator& evaluator) {
  // Data structure used to select random population donors
  vector<int> options(solutions.size());
  iota(options.begin(), options.end(), 0);
  int unused;
  int index, working = 0;
  bool different;

  // iterates over the clusters in the specified order
  for (auto& cluster_index : cluster_ordering) {
    auto cluster = clusters[cluster_index];
    // Mark all individuals in the population as unused
    unused = options.size() - 1;
    different = false;

    // Find a donor which has at least one gene value different
    // from the current solution for this cluster of genes
    while (unused >= 0 and not different) {
      // Choose a donor
      index = std::uniform_int_distribution<int>(0, unused)(rand);
      working = options[index];
      // make certain that donor cannot be chosen again
      swap(options[unused], options[index]);
      unused -= 1;

      // Attempt the donation
      different = donate(solution, fitness, solutions[working], cluster,
                         evaluator);
      // Break loop if configured to stop_after_one or donate returned true
      different |= stop_after_one;
    }
  }
}

// Order the clusters based on size, smallest first, ties broken randomly
void Population::smallest_first(Random& rand,
                                const vector<vector<int>>& clusters,
                                vector<int>& cluster_ordering) {
  // NOTE: My previous work did not shuffle here
  std::shuffle(cluster_ordering.begin(), cluster_ordering.end(), rand);
  std::stable_sort(
      cluster_ordering.begin(),
      cluster_ordering.end(),
      [clusters](int x, int y) {return clusters[x].size() < clusters[y].size();});
}

// Order the clusters such that the last to be linked is the first to appear
void Population::least_linked_first(Random& rand,
                                    const vector<vector<int>>& clusters,
                                    vector<int>& cluster_ordering) {
  std::reverse(cluster_ordering.begin(), cluster_ordering.end());
}

// Leave the clusters as the were created.  AKA most linked first
void Population::no_action(Random& rand, const vector<vector<int>>& clusters,
                           vector<int>& cluster_ordering) {
}

// Put the clusters in a completely random order
void Population::random(Random& rand, const vector<vector<int>>& clusters,
                        vector<int>& cluster_ordering) {
  std::shuffle(cluster_ordering.begin(), cluster_ordering.end(), rand);
}
