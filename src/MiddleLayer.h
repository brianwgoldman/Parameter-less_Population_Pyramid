/*
 * MiddleLayer.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef MIDDLELAYER_H_
#define MIDDLELAYER_H_
#include "Evaluation.h"
#include <stdexcept>
#include <numeric>
#include "Record.h"

class Middle_Layer : public Evaluator {
 public:
  size_t evaluations;
  float best_fitness;
  int best_found;
  std::vector<bool> best_solution;
  Record results;
  Middle_Layer(Configuration& config, shared_ptr<Evaluator> evaler)
      : evaluations(0),
        best_fitness(std::numeric_limits<float>::min()),
        best_found(0),
        results(config),
        evaluator(evaler) {
  }
  virtual float evaluate(const vector<bool> & solution) override;

 protected:
  shared_ptr<Evaluator> evaluator;
};

#endif
