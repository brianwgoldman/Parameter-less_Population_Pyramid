/*
 * Util.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <ostream>
using std::vector;
#include "MersenneTwister.h"

vector<bool> rand_vector(MTRand& rand, const int length)
{
	vector<bool> vect(length, 0);
	for(size_t index=0; index < vect.size(); index++)
	{
		vect[index] = rand.randInt(1);
	}
	return vect;
}

void print(vector<bool> & vect, ostream & out=std::cout)
{
	for(const auto & bit: vect)
	{
		out << bit;
	}
	out << std::endl;

}
#endif /* UTIL_H_ */
