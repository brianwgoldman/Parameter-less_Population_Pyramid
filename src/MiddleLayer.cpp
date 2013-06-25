/*
 * MiddleLayer.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "MiddleLayer.h"

float Middle_Layer::evaluate(const vector<bool>& solution)
{
	try
	{
		return seen.at(solution);
	}
	catch (const std::out_of_range& oor)
	{
		float fitness = evaluator.evaluate(solution);
		seen[solution] = fitness;
		counter ++;
		return fitness;
	}
}
