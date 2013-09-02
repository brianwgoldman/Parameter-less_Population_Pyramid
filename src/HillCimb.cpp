/*
 * HillCimb.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "HillClimb.h"
#include <algorithm>

using namespace std;

template<>
hill_climb::pointer Configuration::get(const string key) {
  return hill_climb::lookup[get<string>(key)];
}

void hill_climb::first_improvement(Random & rand, vector<bool> & solution,
                                   float & fitness, Evaluator& evaluator) {
  // Set up data structure for random bit selection
  vector<int> options(solution.size());
  iota(options.begin(), options.end(), 0);
  float new_fitness;
  bool improvement;
  std::unordered_set<int> tried;

  // Keep looping until there is no single bit flip improvement
  do {
    improvement = false;
    // Test the bits in a random order
    std::shuffle(options.begin(), options.end(), rand);
    for (const auto& index : options) {
      if (tried.count(index) != 0) {
        continue;
      }
      solution[index] = not solution[index];
      new_fitness = evaluator.evaluate(solution);
      if (fitness < new_fitness) {
        // Keep change, update variables
        fitness = new_fitness;
        improvement = true;
        tried.clear();
      } else {
        // Revert the change
        solution[index] = not solution[index];
      }
      tried.insert(index);
    }
  } while (improvement);
}

void hill_climb::steepest_ascent(Random & rand, vector<bool> & solution,
                                 float & fitness, Evaluator& evaluator) {
  float new_fitness;
  bool improved;
  vector<size_t> bests;
  // Stores which bit was flipped to create the current solution
  size_t previous = -1;
  // Keep looping until there is no single bit flip improvement
  do {
    improved = false;
    for (size_t working = 0; working < solution.size(); working++) {
      if (working != previous) {
        solution[working] = not solution[working];
        new_fitness = evaluator.evaluate(solution);
        if (fitness <= new_fitness) {
          if (fitness < new_fitness) {
            fitness = new_fitness;
            improved = true;
            bests.clear();
          }
          // Record the index of each solution that obtained the best fitness
          bests.push_back(working);
        }
        solution[working] = not solution[working];
      }
    }
    if (improved) {
      int index = std::uniform_int_distribution<int>(0, bests.size() - 1)(rand);
      previous = bests[index];
      solution[previous] = not solution[previous];
      bests.clear();
    }
  } while (improved);
}

void hill_climb::no_action(Random & rand, vector<bool> & solution,
                           float & fitness, Evaluator& evaluator) {
}

void hill_climb::binary_tournament(Random & rand, vector<bool> & solution,
                                   float & fitness, Evaluator& evaluator) {
  auto guess = rand_vector(rand, solution.size());
  float guess_fitness = evaluator.evaluate(guess);
  if (fitness < guess_fitness) {
    solution = guess;
    fitness = guess_fitness;
  }
}

void hill_climb::once_each(Random & rand, vector<bool> & solution,
                           float & fitness, Evaluator& evaluator) {
  vector<int> options(solution.size());
  iota(options.begin(), options.end(), 0);
  float new_fitness;
  std::shuffle(options.begin(), options.end(), rand);

  for (const auto& index : options) {
    solution[index] = not solution[index];
    new_fitness = evaluator.evaluate(solution);
    if (fitness < new_fitness) {
      fitness = new_fitness;
    } else {
      solution[index] = not solution[index];
    }
  }
}
