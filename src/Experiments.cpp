/*
 * Experiments.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: goldman
 */

#include "Experiments.h"

vector<Record> multirun(Random& rand, Configuration& config, evaluation::pointer problem, optimize::pointer solver)
{
	int runs = config.get<int>("runs");
	vector<Record> records;
	for(int run=0; run < runs; run++)
	{
		Middle_Layer layer(problem(config, run));
		auto optimizer = solver(config);
		optimizer->optimize(rand, layer, config);
		records.push_back(layer.results);
		auto summary = Record::summarize(records);
		std::cout << "Run: " << run
				<< " MES: " << summary[MES]
			    << " MAD: " << summary[MAD]
			    << " FAILURES: " << summary[FAILURES] << std::endl;
	}
	return records;
}
