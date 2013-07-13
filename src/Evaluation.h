/*
 * Evaluation.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef EVALUATION_H_
#define EVALUATION_H_
#include <vector>

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
	DeceptiveTrap(int ts): trap_size(ts) {}
	float evaluate(const vector<bool> & solution) override;
};

class DeceptiveStepTrap: public Evaluator
{
private:
	int trap_size;
	int step_size;
	int offset;

public:
	DeceptiveStepTrap(int ts, int ss):
		trap_size(ts), step_size(ss), offset((ts-ss) % ss) {}
	float evaluate(const vector<bool> & solution) override;
};
#endif /* EVALUATION_H_ */
