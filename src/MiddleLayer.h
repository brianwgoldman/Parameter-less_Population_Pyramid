/*
 * MiddleLayer.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef MIDDLELAYER_H_
#define MIDDLELAYER_H_
#include "Evaluation.h"
#include <unordered_map>
#include <stdexcept>

class Middle_Layer: public Evaluator
{
protected:
	shared_ptr<Evaluator> evaluator;

public:
	int counter;
	float best_fitness;
	int best_found;
	std::vector<bool> best_solution;
	std::unordered_map<vector<bool>, float> seen;
	Middle_Layer(shared_ptr<Evaluator> evaler): evaluator(evaler), counter(0), best_fitness(0), best_found(0) {}
	virtual float evaluate(const vector<bool> & solution) override;


};

#endif
