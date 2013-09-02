/*
 * RandomRestartHC.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: goldman
 */

#include "RandomRestartHC.h"

bool RandomRestartHC::iterate() {
  auto solution = rand_vector(rand, length);
  float fitness = evaluator.evaluate(solution);
  hill_climber(rand, solution, fitness, evaluator);

  // Always possible
  return true;
}
