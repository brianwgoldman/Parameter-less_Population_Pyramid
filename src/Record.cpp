/*
 * Record.cpp
 *
 *  Created on: Jul 19, 2013
 *      Author: goldman
 */

#include "Record.h"

void Record::add(float fitness, int evals) {
  if (verbosity > 1) {
    std::cout << fitness << "\t" << evals << std::endl;
  }
  levels.push_back(pair<float, int>(fitness, evals));
}

const pair<float, int>& Record::best() const {
  return levels[levels.size() - 1];
}

array<int, 3> Record::summarize(const vector<Record>& records, Configuration& config) {
  array<int, 3> result = { { 0, 0, 0 } };
  vector<int> evals;
  float good_enough = config.get<int>("fitness_limit");
  for (const auto& record : records) {
    auto best = record.best();
    if (best.first >= good_enough) {
      evals.push_back(best.second);
    } else {
      result[FAILURES]++;
    }
  }

  if (evals.size() == 0) {
    return result;
  }
  int fill_value = *std::max_element(evals.begin(), evals.end());
  evals.resize(records.size(), fill_value * 2);
  result[MES] = median(evals);
  vector<int> differences;
  for (const auto& x : evals) {
    differences.push_back(std::abs(result[MES] - x));
  }
  result[MAD] = median(differences);
  return result;
}
