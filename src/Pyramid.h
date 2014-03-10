// Brian Goldman

// Implemention of the Parameter-less Populaiton Pyramid
// Full description given in our publication:
// "Parameter-less Population Pyramid"
// by B. W. Goldman and W. F. Punch

#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "Population.h"
#include "Evaluation.h"
#include "Util.h"
#include "HillClimb.h"
#include "Configuration.h"
#include "Optimizer.h"

// Implements the Optimizer interface
class Pyramid : public Optimizer {
 public:
  Pyramid(Random& _rand, Evaluator& _evaluator, Configuration& _config)
      : Optimizer(_rand, _evaluator, _config),
        only_add_improvements(_config.get<int>("only_add_improvements")),
        hill_climber(_config.get<hill_climb::pointer>("hill_climber")) {
  }
  // Iteratively improves the solution using the pyramid of populations
  // leverages the Population class extensively
  void climb(vector<bool> & solution, float & fitness);
  // Peforms one complete iteration of
  // * random generation
  // * hill climbing
  // * crossover with each level of the pyramid (climb function)
  bool iterate() override;
  create_optimizer(Pyramid);

 private:
  // Handles adding a solution to the population.
  // Returns true if it was unique and therefore added.
  bool add_unique(const vector<bool> & solution, size_t level);

  // the pyramid of populations
  vector<Population> pops;
  // keeps track of the set of solutions in the pyramid
  std::unordered_set<vector<bool>> seen;
  // configuration options
  bool only_add_improvements;
  hill_climb::pointer hill_climber;

};

#endif /* PYRAMID_H_ */
