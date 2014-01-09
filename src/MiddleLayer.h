/*
 * MiddleLayer.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef MIDDLELAYER_H_
#define MIDDLELAYER_H_
#include "Evaluation.h"
#include "Util.h"
#include <stdexcept>
#include <numeric>
#include <fstream>
#include "Record.h"

class Middle_Layer : public Evaluator {
 public:
  size_t evaluations;
  float best_fitness;
  int best_found;
  std::vector<bool> best_solution;
  Record results;
  string outfile;
  Middle_Layer(Configuration& config, shared_ptr<Evaluator> evaler)
      : evaluations(0),
        best_fitness(std::numeric_limits<int>::min()),
        best_found(0),
        results(config),
        outfile(config.get<string>("solution_file")),
        evaluator(evaler) {
  }
  virtual float evaluate(const vector<bool> & solution) override;

 protected:
  shared_ptr<Evaluator> evaluator;
};

#endif
