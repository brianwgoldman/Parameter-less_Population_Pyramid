/*
 * MiddleLayer.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "MiddleLayer.h"

float Middle_Layer::evaluate(const vector<bool>& solution) {
  evaluations++;
  float fitness = evaluator->evaluate(solution);
  if (best_fitness < fitness) {
    best_fitness = fitness;
    best_solution = solution;
    best_found = evaluations;
    results.add(fitness, evaluations);
    std::ofstream output(outfile, std::fstream::app);
    output << best_fitness << " " << evaluations << " ";
    print(best_solution, output);
  }
  return fitness;
}
