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
	Evaluator& evaluator;

public:
	int counter;
	std::unordered_map<vector<bool>, float> seen;
	Middle_Layer(Evaluator& evaler): evaluator(evaler), counter(0) {}
	virtual float evaluate(const vector<bool> & solution) override;



};

#endif
