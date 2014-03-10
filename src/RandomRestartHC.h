// Brian Goldman

// Creates a complete optimization method out of
// a local search operator.  Whenever a local optima
// is reached, search simply restarts from a random solution.

#ifndef RANDOMRESTARTHC_H_
#define RANDOMRESTARTHC_H_

#include "Optimizer.h"
#include "Util.h"
#include "HillClimb.h"

// Implements the optimizer interface
class RandomRestartHC : public Optimizer {
 public:
  RandomRestartHC(Random& _rand, Evaluator& _evaluator, Configuration& _config)
      : Optimizer(_rand, _evaluator, _config),
        hill_climber(_config.get<hill_climb::pointer>("hill_climber")) {
  }
  // Performs a single local search from random solution to local optima
  virtual bool iterate() override;
  create_optimizer(RandomRestartHC);

 private:
  hill_climb::pointer hill_climber;
};

#endif /* RANDOMRESTARTHC_H_ */
