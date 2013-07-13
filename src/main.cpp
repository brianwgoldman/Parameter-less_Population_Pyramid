//============================================================================
// Name        : P3.cpp
// Author      : Brian W. Goldman
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "Evaluation.h"
#include "HillClimb.h"
#include "Util.h"
#include "MiddleLayer.h"
#include "Population.h"
#include "Pyramid.h"
#include "LTGA.h"
#include "Configuration.h"

int main(int argc, char * argv[])
{
	Random rand;
	std::random_device rd;

	Configuration config;
	config.parse(argc, argv);

	rand.seed(rd());
	int length = config.get<int>("length");
	int k_size = config.get<int>("k");
	int runs = config.get<int>("runs");
	int pop_size = config.get<int>("pop_size");
	hc_pointer hc = first_improvement;

	if (config.get<string>("hill_climber") == "steepest_ascent")
	{
		hc = steepest_ascent;
	}
	DeceptiveStepTrap evaluator(k_size, 2);
	long int total_evals = 0;
	for(int run=0; run < runs; run++)
	{
		Middle_Layer layer(evaluator);
		//Pyramid pyramid(length);
		//TODO MAKE CONFIGURABLE
		LTGA pyramid(length, pop_size);

		pyramid.optimize(rand, layer, hc);

		cout << layer.seen.size() << ' ' << layer.counter << endl;
		cout << layer.best_fitness << ' ' << layer.best_found << endl;
		total_evals += layer.best_found;
	}
	cout << "TOTAL " << total_evals << " AVERAGE: " << total_evals / ((float)runs) << endl;
	return 0;
}
