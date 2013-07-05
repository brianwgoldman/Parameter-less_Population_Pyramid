/*
 * Configuration.cpp
 *
 *  Created on: Jul 4, 2013
 *      Author: goldman
 */

#include "Configuration.h"
using namespace std;

void Configuration::parse(int argc, char* argv[])
{
	int working = 1;
	while(working < argc)
	{
		if(argv[working][0] == '-')
		{
			string key = string(argv[working] + 1);
			working++;
			string value = string(argv[working]);
			data[key] = value;
		}
		else
		{
			parse(argv[working]);
		}
		working++;
	}
}

void Configuration::parse(char filename[])
{
	ifstream in(filename);
	string key;
	string value;
	while(in)
	{
		in >> key;
		if(key.length() == 0)
		{
			break;
		}
		else if(key[0] == '#')
		{
			in.ignore(1024, '\n');
		}
		else if (in)
		{
			in >> value;
			data[key] = value;
		}
	}
}

template <>
int Configuration::get(const string key)
{
	return atoi(data[key].c_str());
}

template <>
float Configuration::get(const string key)
{
	return atof(data[key].c_str());
}

template <>
string Configuration::get(const string key)
{
	return data[key];
}
