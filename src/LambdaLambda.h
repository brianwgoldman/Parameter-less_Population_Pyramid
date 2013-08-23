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

class LambdaLambda: public Optimizer {
public:
	LambdaLambda(Configuration& config) {};
	virtual void optimize(Random& rand, Evaluator& evaluator, Configuration& config) override;
	create_optimizer(LambdaLambda);
};

#endif /* LAMBDALAMBDA_H_ */
