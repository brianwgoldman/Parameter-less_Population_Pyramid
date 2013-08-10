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

float float_round(float value, int precision)
{
	return round(value * precision) / precision;
}

BinaryToFloat::BinaryToFloat(size_t _bits, float min, float max, int precision)
{
	bits = _bits;
	size_t length = 1 << bits;
	float gray;
	values.resize(length);
	float span = max - min;
	for(size_t i=0; i < length; i++)
	{
		gray = (i>>1) ^ i;
		values[i] = float_round(gray / (length - 1) * span + min, precision);
	}
}
