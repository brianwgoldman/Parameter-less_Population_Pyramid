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
#include "Optimizer.h"

int main(int argc, char * argv[])
{
	Random rand;
	std::random_device rd;

	Configuration config;
	config.parse(argc, argv);

	rand.seed(rd());
	int runs = config.get<int>("runs");
	DeceptiveStepTrap evaluator(config);
	long int total_evals = 0;
	for(int run=0; run < runs; run++)
	{
		Middle_Layer layer(evaluator);
		//Pyramid pyramid(length);
		//TODO MAKE CONFIGURABLE
		Optimizer* optimizer;
		if(config.get<string>("optimizer") == "LTGA")
		{
			optimizer = new LTGA(config);
		}
		else
		{
			optimizer = new Pyramid(config);
		}

		optimizer->optimize(rand, layer, config);
		delete optimizer;
		cout << layer.seen.size() << ' ' << layer.counter << endl;
		cout << layer.best_fitness << ' ' << layer.best_found << endl;
		total_evals += layer.best_found;
	}
	cout << "TOTAL " << total_evals << " AVERAGE: " << total_evals / ((float)runs) << endl;
	return 0;
}
