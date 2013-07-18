/*
 * Util.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <iostream>
#include <random>

using std::vector;
using Random=std::mt19937;

vector<bool> rand_vector(Random& rand, const size_t length);

void print(const vector<bool> & vect, std::ostream & out=std::cout);

vector<int> indices(const size_t length);

#endif /* UTIL_H_ */
