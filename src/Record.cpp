// Brian Goldman

// Data storage of over time optimization progress

#include "Record.h"

// Records that a new fitness level was reached after 'evals' number
// of evaluations.  Outputs information to the screen if verbosity
// is high enough
void Record::add(float fitness, int evals) {
  if (verbosity > 1) {
    std::cout << fitness << "\t" << evals << std::endl;
  }
  levels.push_back(pair<float, int>(fitness, evals));
}

// Returns information about the best fitness reached
const pair<float, int>& Record::best() const {
  return levels[levels.size() - 1];
}

// Summarizes a collection of record's final fitness level reached:
// * MES - Median evaluations to success
// * MAD - Median absolute deviation in the evaluations to success
// * FAILURES - Number of times the global optimum was not found
// Returns as an array of three values.
array<int, 3> Record::summarize(const vector<Record>& records, Configuration& config) {
  array<int, 3> result = { { 0, 0, 0 } };
  vector<int> evals;
  float good_enough = config.get<int>("fitness_limit");
  // look at the best found in each run
  for (const auto& record : records) {
    auto best = record.best();
    if (best.first >= good_enough) {
      evals.push_back(best.second);
    } else {
      result[FAILURES]++;
    }
  }

  // none of the runs succeeded
  if (evals.size() == 0) {
    return result;
  }
  // finds the median by filling in failures as twice the maximum found
  int fill_value = *std::max_element(evals.begin(), evals.end());
  evals.resize(records.size(), fill_value * 2);
  result[MES] = median(evals);
  // Calculate the mediate absolute deviation
  vector<int> differences;
  for (const auto& x : evals) {
    differences.push_back(std::abs(result[MES] - x));
  }
  result[MAD] = median(differences);
  return result;
}
