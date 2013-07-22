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
#include "Experiments.h"

int main(int argc, char * argv[])
{
	Random rand;
	Configuration config;
	config.parse(argc, argv);
	int seed = config.get<int>("seed");
	if(seed == -1)
	{
		std::random_device rd;
		seed = rd();
		config.set("seed", seed);
	}
	rand.seed(seed);
	auto problem = config.get<evaluation::pointer>("problem");
	auto optimizer_method = config.get<optimize::pointer>("optimizer");
	if(config.get<string>("experiment") == "bisection")
	{
		int pop_size = bisection(rand, config, problem, optimizer_method);
		cout <<"POP SIZE " << pop_size << endl;
	}
	else
	{
		vector<Record> records = multirun(rand, config, problem, optimizer_method);
		auto summary = Record::summarize(records);
		cout << summary[MES] << " " << summary[MAD] << " " << summary[FAILURES] << endl;
	}
	return 0;
}
