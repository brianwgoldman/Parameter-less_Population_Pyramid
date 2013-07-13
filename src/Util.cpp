/*
 * Util.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: goldman
 */

#include "Util.h"


vector<bool> rand_vector(Random& rand, const size_t length)
{
	std::uniform_int_distribution<int> rbit(0, 1);
	vector<bool> vect(length, 0);
	for(size_t index=0; index < vect.size(); index++)
	{
		vect[index] = rbit(rand);
	}
	return vect;
}

void print(const vector<bool> & vect, std::ostream & out)
{
	for(const auto & bit: vect)
	{
		out << bit;
	}
	out << std::endl;

}

vector<int> indices(const size_t length)
{
	vector<int> vect(length, 0);
	for(size_t i=0; i < vect.size(); i++)
	{
		vect[i] = i;
	}
	return vect;
}



