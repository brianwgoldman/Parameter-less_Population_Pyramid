/*
 * RandomRestartHC.h
 *
 *  Created on: Jul 18, 2013
 *      Author: goldman
 */

#ifndef RANDOMRESTARTHC_H_
#define RANDOMRESTARTHC_H_

#include "Optimizer.h"
#include "Util.h"
#include "HillClimb.h"

class RandomRestartHC: public Optimizer {
private:
	hill_climb::pointer hill_climber;
public:
	RandomRestartHC(Random& _rand, Evaluator& _evaluator, Configuration& _config):
		Optimizer(_rand, _evaluator, _config),
		hill_climber(_config.get<hill_climb::pointer>("hill_climber")) { }
	virtual bool iterate() override;
	create_optimizer(RandomRestartHC);
};

#endif /* RANDOMRESTARTHC_H_ */
