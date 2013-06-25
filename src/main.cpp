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
#include <random>

int main()
{
	Random rand;
	vector<bool> test = rand_vector(rand, 2000);
	print(test);

	DeceptiveTrap evaluator(5);
	Middle_Layer layer(evaluator);
	float fitness = layer.evaluate(test);
	cout << fitness << endl;
	next_best(rand, test, fitness, layer);
	print(test);
	cout << fitness << ' ' << layer.seen.size() << " " << layer.counter << endl;

	return 0;
}
