// Brian Goldman

// Collection of utilities not better suited for placement
// in other files

#include "Util.h"

// Creates a new vector containing "length" number of random bits
vector<bool> rand_vector(Random& rand, const size_t length) {
  std::uniform_int_distribution<int> rbit(0, 1);
  vector<bool> vect(length, 0);
  for (size_t index = 0; index < vect.size(); index++) {
    vect[index] = rbit(rand);
  }
  return vect;
}

// Outputs the vector to the stream as 0 and 1 characters
void print(const vector<bool> & vect, std::ostream & out) {
  for (const auto & bit : vect) {
    out << bit;
  }
  out << std::endl;
}

// Counts how many bits are different between "a" and "b".
size_t hamming_distance(const vector<bool> & a, const vector<bool> & b) {
  size_t difference = 0;
  for (size_t i = 0; i < a.size(); i++) {
    difference += a[i] != b[i];
  }
  return difference;
}

// Rounds the number to the desired precision
float float_round(float value, int precision) {
  return round(value * precision) / precision;
}

// Constructs the conversion tool for gray coded bits to floats
// on a given range of numbers
BinaryToFloat::BinaryToFloat(size_t _bits, float min, float max,
                             int precision) {
  bits = _bits;
  size_t length = 1 << bits;
  size_t gray;
  // all possible gray coded values
  values.resize(length);
  float span = max - min;
  for (size_t i = 0; i < length; i++) {
    // magical conversion of an integer to a gray code
    gray = (i >> 1) ^ i;
    values[gray] = i / static_cast<float>(length) * span + min;
  }
}
