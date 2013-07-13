/*
 * LTGA.h
 *
 *  Created on: Jul 12, 2013
 *      Author: goldman
 */

#ifndef LTGA_H_
#define LTGA_H_

#include "Population.h"
#include "Evaluation.h"
#include "Util.h"
#include "HillClimb.h"

class LTGA
{
private:
	size_t length;
	size_t pop_size;
	Population pop;
	std::unordered_map<vector<bool>, float> fitnesses;
	std::unordered_set<vector<bool>> construct_set(float& fitness);

public:
	std::unordered_set<vector<bool>> previous_set;
	LTGA(size_t l, size_t p): length(l), pop_size(p), pop(l) {}
	virtual ~LTGA() = default;

	void initialize(Random& rand, Evaluator& evaluator, hc_pointer hc);
	void generation(Random& rand, Evaluator& evaluator);
	void optimize(Random& rand, Evaluator& evaluator, hc_pointer hc);

};

#endif /* LTGA_H_ */
