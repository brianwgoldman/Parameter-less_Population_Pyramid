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
public:
	RandomRestartHC(Configuration& config) {};
	virtual void optimize(Random& rand, Evaluator& evaluator, Configuration& config) override;
	create_optimizer(RandomRestartHC);
};

#endif /* RANDOMRESTARTHC_H_ */
