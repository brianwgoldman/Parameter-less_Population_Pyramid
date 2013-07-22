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
#include <algorithm>
#include <sstream>

using std::vector;
using Random=std::mt19937;

vector<bool> rand_vector(Random& rand, const size_t length);

void print(const vector<bool> & vect, std::ostream & out=std::cout);

vector<int> indices(const size_t length);

template <class T>
T median(vector<T> data)
{
	auto middle = data.size() >> 1;
	std::nth_element(data.begin(), data.begin() + middle, data.end());
	auto result = data[middle];
	// even length lists
	if(!(data.size() & 1))
	{
		std::nth_element(data.begin(), data.begin() + middle-1, data.end());
		result = (result + data[middle-1]) / 2;
	}
	return result;
}

template <class T>
T make_filable(const T entry)
{
	std::stringstream ss;
	ss << entry;
	T new_value;
	ss >> new_value;
	return new_value;
}

#endif /* UTIL_H_ */
