// Brian Goldman

// Implements a wrapper around an evaluation function to
// allow for fitness trajectory tracking

#include "MiddleLayer.h"

// Pass through evaluation which also performs some recording
float Middle_Layer::evaluate(const vector<bool>& solution) {
  // keeps track of how many times "evaluate" is called
  evaluations++;
  float fitness = evaluator->evaluate(solution);
  // keeps track of improvements in best found fitness
  if (best_fitness < fitness) {
    best_fitness = fitness;
    best_solution = solution;
    best_found = evaluations;
    results.add(fitness, evaluations);
    // If configured to do so, will append the current best
    // to the output solution file
    if (not disable_solution_outfile) {
      std::ofstream output(outfile, std::fstream::app);
      // Format is <fitness> <evaluations> <solution>
      output << best_fitness << " " << evaluations << " ";
      print(best_solution, output);
    }
  }
  return fitness;
}
