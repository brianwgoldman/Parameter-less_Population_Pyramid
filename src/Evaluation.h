/*
 * Evaluation.h
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#ifndef EVALUATION_H_
#define EVALUATION_H_
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <math.h>
#include <fstream>
#include <memory>
#include <array>
#include "Configuration.h"
#include "Util.h"

#define PI 3.14159265

using std::vector;
using std::size_t;
using std::shared_ptr;

#define create_evaluator(name) static shared_ptr<Evaluator> create(Configuration& config, int run_number)\
{\
	return shared_ptr<Evaluator>(new name(config, run_number));\
}

class Evaluator {
 public:
  Evaluator() = default;
  virtual ~Evaluator() = default;
  float virtual evaluate(const vector<bool> & solution) = 0;
};

class OneMax : public Evaluator {
 public:
  OneMax(Configuration& config, int run_number) {
  }
  float evaluate(const vector<bool> & solution) override;create_evaluator(OneMax);
};

class DeceptiveTrap : public Evaluator {
 public:
  DeceptiveTrap(Configuration& config, int run_number)
      : trap_size(config.get<int>("trap_size")),
        precision(config.get<int>("precision")) {
  }
  float evaluate(const vector<bool> & solution) override;create_evaluator(DeceptiveTrap);

 private:
  int trap_size;
  int precision;
};

class DeceptiveStepTrap : public Evaluator {
 public:
  DeceptiveStepTrap(Configuration& config, int run_number)
      : trap_size(config.get<int>("trap_size")),
        step_size(config.get<int>("step_size")),
        precision(config.get<int>("precision")) {
    offset = (trap_size - step_size) % step_size;
  }
  create_evaluator(DeceptiveStepTrap);
  float evaluate(const vector<bool> & solution) override;

 private:
  int trap_size;
  int step_size;
  int offset;
  int precision;

};

class NearestNeighborNK : public Evaluator {
  using trimap=std::unordered_map<size_t,
  std::unordered_map<size_t,
  std::unordered_map<size_t, float>>>;

 public:
  float minimum;
  float maximum;
  vector<bool> best;
  vector<bool> worst;
  NearestNeighborNK(Configuration& config, int run_number);
  float evaluate(const vector<bool> & solution) override;create_evaluator(NearestNeighborNK);

 private:
  vector<vector<float> > table;
  size_t k;
  int length;
  int precision;

  float chunk_fitness(trimap& known, size_t chunk_index, size_t a, size_t b);
  void int_into_bit(size_t src, vector<bool>& dest);
  float solve(vector<bool>& solution, bool maximize);
};

class LeadingOnes : public Evaluator {
 public:
  LeadingOnes(Configuration& config, int run_number)
      : precision(config.get<int>("precision")) {
  }
  float evaluate(const vector<bool> & solution) override;create_evaluator(LeadingOnes);

 private:
  int precision;
};

class HIFF : public Evaluator {
 public:
  HIFF(Configuration& config, int run_number)
      : precision(config.get<int>("precision")) {
  }
  float evaluate(const vector<bool> & solution) override;create_evaluator(HIFF);

 private:
  int precision;
};

class MAXSAT : public Evaluator {
 public:
  MAXSAT(Configuration& config, int run_number);
  float evaluate(const vector<bool> & solution) override;create_evaluator(MAXSAT);
 private:
   int precision;
   vector<std::array<int, 3>> clauses;
   vector<std::array<bool, 3>> signs;
   vector<std::array<int, 3>> sign_options = { { { 0, 0, 1 } }, { { 0, 1, 0 } },
       { { 1, 0, 0 } }, { { 1, 0, 0 } }, { { 0, 1, 1 } }, { { 1, 1, 1 } }, };
};

class IsingSpinGlass : public Evaluator {
 public:
  IsingSpinGlass(Configuration& config, int run_number);
  float evaluate(const vector<bool> & solution) override;create_evaluator(IsingSpinGlass);

 private:
  int length;
  int precision;
  int min_energy;
  float span;
  std::array<int, 2> bit_to_sign = { { -1, 1 } };
  vector<std::array<int, 3>> spins;
};

class Rastrigin : public Evaluator {
 public:
  Rastrigin(Configuration& config, int run_number);
  float evaluate(const vector<bool> & solution) override;create_evaluator(Rastrigin);

 private:
  int precision;
  BinaryToFloat converter;
  unordered_map<float, float> function;
  float worst;
};

class External : public Evaluator {
 public:
  External(Configuration& config, int run_number)
     : script_file(config.get<string>("script_path")),
       out_file(config.get<string>("external_out")),
       in_file(config.get<string>("external_in")){
  }
  float evaluate(const vector<bool> & solution) override;
  create_evaluator(External);
 private:
  string script_file;
  string out_file;
  string in_file;
};

namespace evaluation {
using pointer=shared_ptr<Evaluator> (*)(Configuration &, int);
static std::unordered_map<string, pointer> lookup( {
    { "OneMax", OneMax::create },
    { "DeceptiveTrap", DeceptiveTrap::create },
    { "DeceptiveStepTrap", DeceptiveStepTrap::create },
    { "NearestNeighborNK", NearestNeighborNK::create },
    { "LeadingOnes", LeadingOnes::create },
    { "HIFF", HIFF::create },
    { "MAXSAT", MAXSAT::create },
    { "IsingSpinGlass", IsingSpinGlass::create },
    { "Rastrigin", Rastrigin::create },
    { "External", External::create },
});
}

#endif /* EVALUATION_H_ */
