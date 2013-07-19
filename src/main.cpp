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
#include "Configuration.h"
#include "Optimizer.h"
#include "OptimizationCollection.h"
#include <memory>
#include "Record.h"

int main(int argc, char * argv[])
{
	Random rand;
	std::random_device rd;

	Configuration config;
	config.parse(argc, argv);

	rand.seed(rd());

	int runs = config.get<int>("runs");
	long int total_evals = 0;
	auto problem = config.get<evaluation::pointer>("problem");
	auto optimizer_method = config.get<optimize::pointer>("optimizer");
	vector<Record> records;
	for(int run=0; run < runs; run++)
	{
		Middle_Layer layer(problem(config, run));
		auto optimizer = optimizer_method(config);
		optimizer->optimize(rand, layer, config);
		cout << layer.seen.size() << ' ' << layer.counter << endl;
		cout << layer.best_fitness << ' ' << layer.best_found << endl;
		records.push_back(layer.results);
		print(layer.best_solution);
		total_evals += layer.best_found;
		cout <<"Running " << total_evals / (run + 1.0) << " " << run+1 << endl;
	}
	cout << "TOTAL " << total_evals << " AVERAGE: " << total_evals / ((float)runs) << endl;
	auto summary = Record::summarize(records);
	cout << summary[MES] << " " << summary[MAD] << " " << summary[FAILURES] << endl;
	return 0;
}
