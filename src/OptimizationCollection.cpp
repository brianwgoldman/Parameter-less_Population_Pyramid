/*
 * OptimizationCollection.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: goldman
 */

#include "OptimizationCollection.h"

template<>
optimize::pointer Configuration::get(const string key) {
  return optimize::lookup[get<string>(key)];
}

