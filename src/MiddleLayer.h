/*
 * MiddleLayer.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef MIDDLELAYER_H_
#define MIDDLELAYER_H_
#include "Evaluation.h"
#include <stdexcept>
#include "Record.h"

class Middle_Layer: public Evaluator
{
protected:
	shared_ptr<Evaluator> evaluator;

public:
	int counter;
	float best_fitness;
	int best_found;
	std::vector<bool> best_solution;
	Middle_Layer(Configuration& config, shared_ptr<Evaluator> evaler);
	virtual float evaluate(const vector<bool> & solution) override;
	Record results;

};

#endif
