/*
 * Configuraiton.h
 *
 *  Created on: Jul 4, 2013
 *      Author: goldman
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <unordered_map>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

using std::unordered_map;
using std::string;

class Configuration
{
private:
	unordered_map<string, string> data;
public:

	Configuration() = default;
	virtual ~Configuration() = default;

	void parse(int argc, char * argv[]);
	void parse(char filename[]);

	template <class T>
	T get(const string key);

	template <class T>
	void set(const string key, const T value)
	{
		data[key] = std::to_string(value);
	}
};


#endif /* CONFIGURAITON_H_ */
