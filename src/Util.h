// Brian Goldman

// This is a collection of utilities used throughout programming.
// Basically just a bunch of misfits.

#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>

using std::vector;

// Defines that the random number genrator in use is the Mersenne Twister
using Random=std::mt19937;

// Generate a random bit vector of the requested length
vector<bool> rand_vector(Random& rand, const size_t length);

// Print out a vector to the stream.
void print(const vector<bool> & vect, std::ostream & out = std::cout);

// Calculate the hamming distance between two bit vectors
size_t hamming_distance(const vector<bool> & a, const vector<bool> & b);

// Round to a desired precision
float float_round(float value, int precision);

// Calculate the median.
template<class T>
T median(vector<T> data) {
  // finds the middle
  auto middle = data.size() >> 1;
  // More efficient than sorting, finds the middle element in O(N) time
  std::nth_element(data.begin(), data.begin() + middle, data.end());
  auto result = data[middle];
  // even length lists
  if (!(data.size() & 1)) {
    // Finds the middle -1 element
    std::nth_element(data.begin(), data.begin() + middle - 1, data.end());
    result = (result + data[middle - 1]) / 2;
  }
  return result;
}

// used to simulate writing and reading a value to a file.
// Helps ensure that after saving and reloading the value no
// further rounding is performed.
template<class T>
T make_filable(const T entry) {
  std::stringstream ss;
  ss << entry;
  T new_value;
  ss >> new_value;
  return new_value;
}

// Tool used to convert a binary vector to a floating
// point number using gray coding
class BinaryToFloat {
 public:
  BinaryToFloat(size_t _bits, float min, float max, int precision);

  // reads and advances the iterator to get the bit values
  // required for conversion
  template<typename ForwardIterator>
  float convert(ForwardIterator& it) {
    size_t index = 0;
    // build up the integer index represented by these bits
    for (size_t i = 0; i < bits; i++) {
      index <<= 1;
      index |= *it;
      it++;
    }
    return values[index];
  }
  const vector<float>& possible() {
    return values;
  }

 private:
  // number of bits per float
  size_t bits;
  // table used to convert integers to floats
  vector<float> values;

};

#endif /* UTIL_H_ */
