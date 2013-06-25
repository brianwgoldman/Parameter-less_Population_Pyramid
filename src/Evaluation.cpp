/*
 * Evaluation.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "Evaluation.h"

float Evaluator::evaluate(const vector<bool> & solution)
{
	float sum = 0;
	for(const bool & bit: solution)
	{
		sum += bit;
	}
	return sum / solution.size();
}

float DeceptiveTrap::evaluate(const vector<bool> & solution)
{
	int partial;
	int total=0;
	for(size_t i=0; i < solution.size(); i+=trap_size)
	{
		partial=0;
		for(size_t index=i; index<i+trap_size; index++)
		{
			partial += solution[index];
		}
		if(partial < trap_size)
		{
			partial = trap_size - partial - 1;
		}
		total += partial;
	}
	return float(total) / solution.size();
}

