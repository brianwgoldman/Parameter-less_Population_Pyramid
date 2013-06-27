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

int main(int argc, char * argv[])
{
	Random rand;
	std::random_device rd;
	rand.seed(rd());
	int length = 20;
	if(argc > 1)
	{
		length = atoi(argv[1]);
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
		next_best(rand, solution, fitness, layer);
		print(solution);
		// return 0;
		pyramid.climb(rand, solution, fitness, evaluator);
		cout << "Climbed fit " << fitness << endl;
		print(solution);
	}

	cout << layer.seen.size() << ' ' << layer.counter
			<< ' ' << pyramid.seen.size() << endl;
	return 0;
}
