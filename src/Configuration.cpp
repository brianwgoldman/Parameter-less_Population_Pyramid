// Created: Brian Goldman

// The configuration class is just a wrapper around
// loading dictionaries from the command line / files
// and making the contents available using desired types.


#include "Configuration.h"
using namespace std;

// Command line arguments starting with a - are considered
// keys, with the following argument then used as the value.
// Everything else is considered a filename.
void Configuration::parse(int argc, char* argv[]) {
  for (int working=1; working < argc; working++) {
    // key found
    if (argv[working][0] == '-') {
      string key = string(argv[working] + 1);
      working++;
      string value = string(argv[working]);
      data[key] = value;
    } else {
      parse(argv[working]);
    }
  }
}

// Loads a configuration file, assumes the file is
// formatted as space separated key value pairs of strings
void Configuration::parse(char filename[]) {
  ifstream in(filename);
  string key;
  string value;
  if (!in) {
    throw invalid_argument(
        "Configuration file does not exist: " + string(filename));
  }
  while (in >> key) {
    // Ignore lines starting with a #
    if (key[0] == '#') {
      in.ignore(1024, '\n');
    } else if (in >> value) {
      data[key] = value;
    }
  }
}

// Prints the key value pairs in sorted order,
// one pair per line
void Configuration::dump(ostream& out) {
  vector<pair<string, string> > sortable;
  for (const auto& it : data) {
    sortable.push_back(it);
  }
  sort(sortable.begin(), sortable.end());
  for (const auto& it : sortable) {
    out << it.first << " " << it.second << endl;
  }
}

// Template specialized for strings, protects
// against accessing invalid keys
template<>
string Configuration::get(const string key) {
  auto it = data.find(key);
  if (it == data.end()) {
    throw invalid_argument(
        "No value specified for configuration option: " + key);
  }
  return it->second;
}

// Template specialized on integer
template<>
int Configuration::get(const string key) {
  return atoi(get<string>(key).c_str());
}

// Template specialized on float
template<>
float Configuration::get(const string key) {
  return atof(get<string>(key).c_str());
}
