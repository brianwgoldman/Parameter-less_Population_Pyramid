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
#include <fstream>
#include <memory>

using std::vector;
using std::size_t;
using std::shared_ptr;

#define create_evaluator(name) static shared_ptr<Evaluator> create(Configuration& config, int run_number)\
{\
	return shared_ptr<Evaluator>(new name(config, run_number));\
}

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
	create_evaluator(DeceptiveTrap);
};

class DeceptiveStepTrap: public Evaluator
{
private:
	int trap_size;
	int step_size;
	int offset;
	int precision;

public:
	DeceptiveStepTrap(Configuration& config, int run_number):
		trap_size(config.get<int>("trap_size")),
		step_size(config.get<int>("step_size")),
		precision(config.get<int>("precision"))
		{
			offset = (trap_size-step_size) % step_size;
		}
	create_evaluator(DeceptiveStepTrap);
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
	create_evaluator(NearestNeighborNK);
};

class LeadingOnes: public Evaluator
{
public:
	LeadingOnes(Configuration& config, int run_number) {}
	float evaluate(const vector<bool> & solution) override;
	create_evaluator(LeadingOnes);
};


namespace evaluation
{
	using pointer=shared_ptr<Evaluator> (*)(Configuration &, int);
	static std::unordered_map<string, pointer> lookup({
		{"DeceptiveTrap", DeceptiveTrap::create},
		{"DeceptiveStepTrap", DeceptiveStepTrap::create},
		{"NearestNeighborNK", NearestNeighborNK::create},
		{"LeadingOnes", LeadingOnes::create},
	});

}

#endif /* EVALUATION_H_ */
