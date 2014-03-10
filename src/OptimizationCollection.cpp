// Brian Goldman

// Configurable optimization methods

#include "OptimizationCollection.h"

// Template specialization allowing strings to be converted
// directly to the functions which create optimizer objects.
template<>
optimize::pointer Configuration::get(const string key) {
  return optimize::lookup[get<string>(key)];
}

