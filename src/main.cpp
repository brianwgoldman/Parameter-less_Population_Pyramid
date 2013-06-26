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
#include "Population.h"
#include <array>

int main()
{
	Random rand;
	Population pop;
	DeceptiveTrap evaluator(5);
	Middle_Layer layer(evaluator);

	for(int i=0; i<20; i++)
	{
		vector<bool> test = rand_vector(rand, 20);
		print(test);

		float fitness = layer.evaluate(test);
		cout << fitness << endl;
		next_best(rand, test, fitness, layer);
		print(test);
		cout << fitness << ' ' << layer.seen.size() << " " << layer.counter << endl;
		pop.add(test);
	}
	// TODO When looking for donation patterns, guess and test in random order
	return 0;
}
