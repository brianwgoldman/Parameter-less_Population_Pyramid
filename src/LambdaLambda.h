/*
 * LambdaLambda.h
 *
 *  Created on: Aug 22, 2013
 *      Author: goldman
 */

#ifndef LAMBDALAMBDA_H_
#define LAMBDALAMBDA_H_

#include "Optimizer.h"
#include "Util.h"

class LambdaLambda : public Optimizer {
 public:
  LambdaLambda(Random& _rand, Evaluator& _evaluator, Configuration& _config);
  virtual bool iterate() override;create_optimizer(LambdaLambda);

 private:
  vector<bool> solution;
  float fitness;
  float lambda;
  vector<std::uniform_int_distribution<>> selectors;
  vector<size_t> options;
  vector<bool> mutate(const vector<bool>& parent, const int flips);
  vector<bool> crossover(const vector<bool>& p1, const vector<bool>& p2,
                         std::bernoulli_distribution& prob);
};

#endif /* LAMBDALAMBDA_H_ */
