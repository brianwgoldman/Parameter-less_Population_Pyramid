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
	if(!in)
	{
		throw invalid_argument("Configuration file does not exist: " + string(filename));
	}
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

void Configuration::dump(ostream& out)
{
	vector<pair<string, string> > sortable;
	for(const auto& it: data)
	{
		sortable.push_back(it);
	}
	sort(sortable.begin(), sortable.end());
	for(const auto& it: sortable)
	{
		out << it.first << " " << it.second << endl;
	}
}

template <>
string Configuration::get(const string key)
{
	auto it = data.find(key);
	if(it == data.end())
	{
		throw invalid_argument("No value specified for configuration option: " + key);
	}
	return it->second;
}

template <>
int Configuration::get(const string key)
{
	return atoi(get<string>(key).c_str());
}

template <>
float Configuration::get(const string key)
{
	return atof(get<string>(key).c_str());
}
