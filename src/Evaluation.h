/*
 * Evaluation.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef EVALUATION_H_
#define EVALUATION_H_
#include <vector>
#include "Configuration.h"
using std::vector;
using std::size_t;

class Evaluator
{
public:
	Evaluator() = default;
	virtual ~Evaluator() = default;
	float virtual evaluate(const vector<bool> & solution);
};

class DeceptiveTrap: public Evaluator
{
private:
	int trap_size;
public:
	DeceptiveTrap(Configuration& config):
		trap_size(config.get<int>("trap_size")) {}
	float evaluate(const vector<bool> & solution) override;
};

class DeceptiveStepTrap: public Evaluator
{
private:
	int trap_size;
	int step_size;
	int offset;

public:
	DeceptiveStepTrap(Configuration& config):
		trap_size(config.get<int>("trap_size")),
		step_size(config.get<int>("step_size"))
		{
			offset = (trap_size-step_size) % step_size;
		}
	float evaluate(const vector<bool> & solution) override;
};
#endif /* EVALUATION_H_ */
