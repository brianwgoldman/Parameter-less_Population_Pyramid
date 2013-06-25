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


vector<bool> rand_vector(Random& rand, const int length);

void print(vector<bool> & vect, std::ostream & out=std::cout);

vector<int> indices(const int length);

#endif /* UTIL_H_ */
