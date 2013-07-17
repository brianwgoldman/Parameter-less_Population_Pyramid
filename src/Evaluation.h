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
#include <algorithm>
#include <unordered_map>
#include "Util.h"
#include <math.h>

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
	DeceptiveTrap(Configuration& config, int run_number):
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
	DeceptiveStepTrap(Configuration& config, int run_number):
		trap_size(config.get<int>("trap_size")),
		step_size(config.get<int>("step_size"))
		{
			offset = (trap_size-step_size) % step_size;
		}
	float evaluate(const vector<bool> & solution) override;
};

class NearestNeighborNK: public Evaluator
{
	using trimap=std::unordered_map<size_t,
			std::unordered_map<size_t,
			std::unordered_map<size_t, float>>>;
private:
	vector<vector<float> > table;
	size_t k;
	int length;
	int precision;

	float chunk_fitness(trimap& known, size_t chunk_index, size_t a, size_t b);
	void int_into_bit(size_t src, vector<bool>& dest);
	float solve(vector<bool>& solution, bool maximize);

public:
	float minimum;
	float maximum;
	vector<bool> best;
	vector<bool> worst;
	NearestNeighborNK(Configuration& config, int run_number);
	float evaluate(const vector<bool> & solution) override;

};
#endif /* EVALUATION_H_ */
