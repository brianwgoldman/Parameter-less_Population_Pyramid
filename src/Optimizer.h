/*
 * Optimizer.h
 *
 *  Created on: Jul 14, 2013
 *      Author: goldman
 */

#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_

#include "Util.h"
#include "Evaluation.h"
#include "Configuration.h"

class Optimizer
{
public:
	Optimizer() = default;
	virtual ~Optimizer() = default;
	virtual void optimize(Random& rand, Evaluator& evaluator, Configuration& config) = 0;
};


#endif /* OPTIMIZER_H_ */
