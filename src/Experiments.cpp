/*
 * Experiments.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: goldman
 */

#include "Experiments.h"

Record single_run(Random& rand, Configuration& config, evaluation::pointer problem, optimize::pointer solver, int run)
{
	Middle_Layer layer(config, problem(config, run));
	auto optimizer = solver(config);
	optimizer->optimize(rand, layer, config);
	return layer.results;
}

vector<Record> multirun(Random& rand, Configuration& config, evaluation::pointer problem, optimize::pointer solver)
{
	int runs = config.get<int>("runs");
	vector<Record> records;
	for(int run=0; run < runs; run++)
	{
		records.push_back(single_run(rand, config, problem, solver, run));
		auto summary = Record::summarize(records);
		std::cout << "Run: " << run
				<< " Evals: " << records[records.size()-1].best().second
				<< " MES: " << summary[MES]
			    << " MAD: " << summary[MAD]
			    << " FAILURES: " << summary[FAILURES] << std::endl;
	}
	return records;
}

int bisection(Random& rand, Configuration& config, evaluation::pointer problem, optimize::pointer solver)
{
	int runs = config.get<int>("runs");
	vector<Record> records;
	int min = 0;
	int max = 1;
	size_t failed_on = 0;
	bool success;
	do
	{
		min = max;
		max *= 2;
		config.set("pop_size", max);
		std::cout << "Pop size: " << max << std::endl;
		success = true;
		for(int i=0; success and i < runs; i++)
		{
			int problem_number = ((i + failed_on) % runs);
			std::cout <<"\tTrying problem: " << problem_number << std:: endl;
			Middle_Layer layer(config, problem(config, problem_number));
			solver(config)->optimize(rand, layer, config);
			if(layer.results.best().first < 1)
			{
				failed_on = problem_number;
				success = false;
			}
		}
	} while(!success);

	// max is doable, min is not
	int guess;
	while(min + 1 < max)
	{
		guess = (max + min) / 2;
		std::cout << "Pop size: " << guess << std::endl;
		config.set("pop_size", guess);
		success = true;
		for(int i=0; success and i < runs; i++)
		{
			int problem_number = ((i + failed_on) % runs);
			std::cout <<"\tTrying problem: " << problem_number << std:: endl;
			Middle_Layer layer(config, problem(config, problem_number));
			solver(config)->optimize(rand, layer, config);
			if(layer.results.best().first < 1)
			{
				failed_on = problem_number;
				success = false;
			}
		}
		if(success)
		{
			max = guess;
		}
		else
		{
			min = guess;
		}
	}
	return max;
}
