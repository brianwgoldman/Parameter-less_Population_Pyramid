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
#include "Configuration.h"

int main(int argc, char * argv[])
{
	Random rand;
	std::random_device rd;

	Configuration config;
	config.parse(argc, argv);

	rand.seed(rd());
	int length = config.get<int>("length");
	auto hc = first_improvement;
	if (config.get<string>("hill_climber") == "steepest_ascent")
	{
		hc = steepest_ascent;
	}
	Pyramid pyramid(length);
	DeceptiveTrap evaluator(5);
	Middle_Layer layer(evaluator);

	float fitness = 0;
	while(fitness < 1.0)
	{
		cout << "-------- Start --------" << endl;
		vector<bool> solution = rand_vector(rand, length);
		fitness = layer.evaluate(solution);
		hc(rand, solution, fitness, layer);
		print(solution);
		pyramid.climb(rand, solution, fitness, evaluator);
		cout << "Climbed fit " << fitness << endl;
		print(solution);
	}

	cout << layer.seen.size() << ' ' << layer.counter
			<< ' ' << pyramid.seen.size() << endl;
	return 0;
}
