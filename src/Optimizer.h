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

#define create_optimizer(name) static shared_ptr<Optimizer> create(Random& rand, Evaluator& evaluator, Configuration& config)\
{\
	return shared_ptr<Optimizer>(new name(rand, evaluator, config));\
}

class Optimizer
{
protected:
	Random& rand;
	Evaluator& evaluator;
	Configuration& config;
	size_t length;
public:
	Optimizer(Random& _rand, Evaluator& _evaluator, Configuration& _config):
		rand(_rand), evaluator(_evaluator), config(_config), length(_config.get<int>("length")) { }
	virtual ~Optimizer() = default;
	virtual bool iterate() = 0;
};


#endif /* OPTIMIZER_H_ */
