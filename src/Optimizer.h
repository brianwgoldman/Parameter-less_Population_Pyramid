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
#include <memory>
using std::shared_ptr;

#define create_optimizer(name) static shared_ptr<Optimizer> create(Configuration& config)\
{\
	return shared_ptr<Optimizer>(new name(config));\
}

class Optimizer
{
public:
	Optimizer() = default;
	virtual ~Optimizer() = default;
	virtual void optimize(Random& rand, Evaluator& evaluator, Configuration& config) = 0;
};


#endif /* OPTIMIZER_H_ */
