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
#include <memory>

int main(int argc, char * argv[])
{
	Random rand;
	std::random_device rd;

	Configuration config;
	config.parse(argc, argv);

	rand.seed(rd());

	int runs = config.get<int>("runs");
	long int total_evals = 0;
	for(int run=0; run < runs; run++)
	{
		NearestNeighborNK evaluator(config, run);
		Middle_Layer layer(evaluator);
		shared_ptr<Optimizer> optimizer;
		if(config.get<string>("optimizer") == "LTGA")
		{
			optimizer = shared_ptr<Optimizer>(new LTGA(config));
		}
		else
		{
			optimizer = shared_ptr<Optimizer>(new Pyramid(config));
		}

		optimizer->optimize(rand, layer, config);
		cout << layer.seen.size() << ' ' << layer.counter << endl;
		cout << layer.best_fitness << ' ' << layer.best_found << endl;
		total_evals += layer.best_found;
		cout <<"Running " << total_evals / (run + 1.0) << " " << run+1 << endl;
	}
	cout << "TOTAL " << total_evals << " AVERAGE: " << total_evals / ((float)runs) << endl;
	return 0;
}
