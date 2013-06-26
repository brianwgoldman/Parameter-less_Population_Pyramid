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

int main(int argc, char * argv[])
{
	Random rand;
	std::random_device rd;
	rand.seed(rd());
	int length = 300;
	if(argc > 1)
	{
		length = atoi(argv[1]);
	}
	Population pop(length);
	DeceptiveTrap evaluator(5);
	Middle_Layer layer(evaluator);

	for(int i=0; i<200; i++)
	{
		vector<bool> test = rand_vector(rand, length);
		float fitness = layer.evaluate(test);
		next_best(rand, test, fitness, layer);
		print(test);
		pop.add(test);
		pop.rebuild_tree(rand);
	}
	// pop.rebuild_tree(rand);
	for(auto& mask: pop.masks)
	{
		if(mask.size() == 5)
		{
			for(auto& value: mask)
			{
				cout << ' ' << value;
			}
			cout << endl;
		}
	}
	cout << layer.seen.size() << ' ' << layer.counter << endl;
	// TODO When looking for donation patterns, guess and test in random order
	return 0;
}
