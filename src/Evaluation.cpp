/*
 * Evaluation.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: goldman
 */

#include "Evaluation.h"
using namespace std;

template<>
evaluation::pointer Configuration::get(const string key) {
  return evaluation::lookup[get<string>(key)];
}

float OneMax::evaluate(const vector<bool> & solution) {
  float sum = 0;
  for (const bool & bit : solution) {
    sum += bit;
  }
  // Convert to percentage of total
  return sum / solution.size();
}

float DeceptiveTrap::evaluate(const vector<bool> & solution) {
  int partial;
  int total = 0;

  for (size_t i = 0; i < solution.size(); i += trap_size) {
    // Find the number of bits set in each trap
    partial = 0;
    for (size_t index = i; index < i + trap_size; index++) {
      partial += solution[index];
    }

    // Make it deceptive
    if (partial < trap_size) {
      partial = trap_size - partial - 1;
    }
    total += partial;
  }

  // Convert to percentage of total
  return float_round(float(total) / solution.size(), precision);
}

float DeceptiveStepTrap::evaluate(const vector<bool> & solution) {
  int partial;
  int total = 0;

  int trap_maximum = (offset + trap_size) / step_size;
  for (size_t i = 0; i < solution.size(); i += trap_size) {
    // Find the number of bits set in each trap
    partial = 0;
    for (size_t index = i; index < i + trap_size; index++) {
      partial += solution[index];
    }

    // Make it deceptive
    if (partial < trap_size) {
      partial = trap_size - partial - 1;
    }

    // Convert it to stepwise
    total += (offset + partial) / step_size;
  }

  // Convert to percentage of total
  float fitness = (float(total) * trap_size) / (solution.size() * trap_maximum);
  return float_round(fitness, precision);
}

NearestNeighborNK::NearestNeighborNK(Configuration& config, int run_number) {
  k = config.get<int>("k");
  length = config.get<int>("length");
  precision = config.get<int>("precision");
  table.resize(length, vector<float>(2 << k, 0));
  int rng_seed = config.get<int>("problem_seed") + run_number;

  // Build up the filename where this problem is stored
  string filename = config.get<string>("problem_folder");
  filename += +"NearestNeighborNK_";
  filename += config.get<string>("length") + "_";
  filename += config.get<string>("k") + "_";
  filename += to_string(rng_seed) + ".txt";

  ifstream in(filename);
  // If this problem has been tried before
  if (in) {
    // Read in information about the global minimum
    in >> minimum;
    worst.resize(length);
    string temp;
    in >> temp;
    for (int i = 0; i < length; i++) {
      worst[i] = temp[i] == '1';
    }

    // Read in information about the global maximum
    in >> maximum;
    best.resize(length);
    in >> temp;
    for (int i = 0; i < length; i++) {
      best[i] = temp[i] == '1';
    }

    // Read in the fitness table
    for (auto& row : table) {
      for (auto& entry : row) {
        in >> entry;
      }
    }
    in.close();
  } else {
    // Generate the new problem
    Random rand(rng_seed);

    // Generate the table
    auto generator = std::uniform_real_distribution<>(0, 1);
    for (auto& row : table) {
      for (auto& entry : row) {
        entry = make_filable(generator(rand));
      }
    }

    // Find its minimum and maximum
    minimum = make_filable(solve(worst, false));
    maximum = make_filable(solve(best, true));

    // Write it out to the file
    ofstream out(filename);
    out << minimum << " ";
    print(worst, out);

    out << maximum << " ";
    print(best, out);

    for (auto& row : table) {
      for (auto& entry : row) {
        out << entry << " ";
      }
      out << endl;
    }
    out.close();
  }
}

float NearestNeighborNK::chunk_fitness(trimap& known, size_t chunk_index,
                                       size_t a, size_t b) {
  // If we know the fitness, return it
  const auto& first = known.find(chunk_index);
  if (first != known.end()) {
    const auto& second = first->second.find(a);
    if (second != first->second.end()) {
      const auto& third = second->second.find(b);
      if (third != second->second.end()) {
        return third->second;
      }
    }
  }

  // Calculate the fitness
  float fitness = 0;
  size_t mask = (2 << k) - 1;

  size_t combined = (a << k) | b;
  combined = (combined << 1) | (a & 1);

  for (size_t g = 0; g < k; g++) {
    size_t value = (combined >> (k - g)) & mask;
    fitness += table[chunk_index * k + g][value];
  }

  // Record the chuck fitness for later
  known[chunk_index][a][b] = fitness;
  return fitness;
}

void NearestNeighborNK::int_into_bit(size_t src, vector<bool>& dest) {
  for (size_t i = 1; i <= k; i++) {
    dest.push_back((src >> (k - i)) & 1);
  }
}

float NearestNeighborNK::solve(vector<bool>& solution, bool maximize) {
  size_t numbers = 1 << k;
  trimap known;
  std::unordered_map<size_t,
      std::unordered_map<size_t, std::unordered_map<size_t, size_t> > > partial;
  float current;
  for (size_t n = length / k - 1; n > 1; n--) {
    std::unordered_map<size_t, std::unordered_map<size_t, float> > utility;
    std::unordered_map<size_t, std::unordered_map<size_t, size_t> > value;
    for (size_t left = 0; left < numbers; left++) {
      for (size_t right = 0; right < numbers; right++) {
        utility[left][right] = -1;
        if (not maximize) {
          utility[left][right] = 2 * length;
        }
        for (size_t middle = 0; middle < numbers; middle++) {
          current = chunk_fitness(known, n - 1, left, middle);
          current += chunk_fitness(known, n, middle, right);
          if ((maximize and utility[left][right] < current)
              or (not maximize and utility[left][right] > current)) {
            utility[left][right] = current;
            value[left][right] = middle;
          }
        }
      }
    }

    // Extract information
    for (size_t left = 0; left < numbers; left++) {
      for (size_t right = 0; right < numbers; right++) {
        known[n - 1][left][right] = utility[left][right];
        partial[n][left][right] = value[left][right];
      }
    }
  }

  // Initialize fitness to be worse than worst possible
  float fitness = -1;
  if (not maximize) {
    fitness = 2 * length;
  }

  // Find the best left and right values
  size_t best_left = 0;
  size_t best_right = 0;
  for (size_t left = 0; left < numbers; left++) {
    for (size_t right = 0; right < numbers; right++) {
      current = chunk_fitness(known, 0, left, right);
      current += chunk_fitness(known, 1, right, left);
      if ((maximize and fitness < current)
          or (not maximize and fitness > current)) {
        fitness = current;
        best_left = left;
        best_right = right;
      }
    }
  }

  // Recreate the optimal string
  solution.clear();
  solution.reserve(length);
  int_into_bit(best_left, solution);
  int_into_bit(best_right, solution);
  size_t last = best_right;
  for (size_t i = 2; i < length / k; i++) {
    last = partial[i][last][best_left];
    int_into_bit(last, solution);
  }

  return fitness;
}

float NearestNeighborNK::evaluate(const vector<bool> & solution) {
  float total = 0;
  for (size_t i = 0; i < solution.size(); i++) {
    // Construct the integer represented by this subset of the solution
    size_t index = 0;
    for (size_t neighbor = i; neighbor <= i + k; neighbor++) {
      index = (index << 1) | solution[neighbor % length];
    }
    total += table[i][index];
  }

  // Convert to percentage of total
  float fitness = (total - minimum) / (maximum - minimum);
  // Ensures the best fitness actually gets 1.0
  return float_round(fitness, precision);
}

float LeadingOnes::evaluate(const vector<bool> & solution) {
  for (size_t i = 0; i < solution.size(); i++) {
    if (not solution[i]) {
      return float_round(float(i) / solution.size(), precision);
    }
  }
  return 1;
}

float HIFF::evaluate(const vector<bool> & solution) {
  // Data structure used to represent values as 0, 1, or -1 (Null)
  int * level = new int[solution.size()];
  int level_length = solution.size();

  // Initialize the level to the current solution
  for (size_t i = 0; i < solution.size(); i++) {
    level[i] = solution[i];
  }
  int power = 1;
  int next_length = level_length >> 1;
  int total = 0;
  int maximum = 0;

  // Keep going while the next level actual has bits in it
  while (next_length > 0) {
    int * next_level = new int[next_length];
    // Construct the next level using the current level
    for (int i = 0; i + 1 < level_length; i += 2) {
      if (level[i] == level[i + 1] and level[i] != -1) {
        // Score points for a correct setting at this level
        total += power;
        next_level[i >> 1] = level[i];
      } else {
        next_level[i >> 1] = -1;
      }
      // Keep track of the maximum possible score
      maximum += power;
    }
    delete[] level;
    level = next_level;
    level_length = next_length;
    next_length = level_length >> 1;
    power <<= 1;
  }
  delete[] level;

  // Convert to percentage of total
  return float(total) / maximum;
}

MAXSAT::MAXSAT(Configuration& config, int run_number) {
  size_t length = config.get<int>("length");
  precision = config.get<int>("precision");
  clauses.resize(
      float_round(config.get<float>("clause_ratio") * length, precision));
  signs.resize(clauses.size());

  int rng_seed = config.get<int>("problem_seed") + run_number;
  Random rand(rng_seed);

  // Create the random target solution
  vector<bool> solution = rand_vector(rand, length);

  // Data structure used to select random variables to include in a clause
  vector<int> options(length);
  std::iota(options.begin(), options.end(), 0);

  // Distributions for selecting 3 variables at random
  std::uniform_int_distribution<> dist[] = {
      std::uniform_int_distribution<>(0, length - 1),
      std::uniform_int_distribution<>(1, length - 1),
      std::uniform_int_distribution<>(2, length - 1)
  };

  auto sign_select = std::uniform_int_distribution<>(0, sign_options.size() - 1);

  for (size_t i = 0; i < clauses.size(); i++) {
    // Choose a random sign setting for this clause
    int select = sign_select(rand);

    // For each term in this clause
    for (int c = 0; c < 3; c++) {
      // Choose a random variable to be a term in the clause
      std::swap(options[c], options[dist[c](rand)]);
      clauses[i][c] = options[c];
      // Set the sign to agree / disagree with the solution based on chosen
      // sign option
      signs[i][c] = sign_options[select][c] == solution[options[c]];
    }
  }
}

float MAXSAT::evaluate(const vector<bool> & solution) {
  int total = 0;
  for (size_t i = 0; i < clauses.size(); i++) {
    for (size_t c = 0; c < 3; c++) {
      if (solution[clauses[i][c]] == signs[i][c]) {
        total++;
        break;
      }
    }
  }

  // Convert to percentage of total
  return float_round(float(total) / clauses.size(), precision);
}

IsingSpinGlass::IsingSpinGlass(Configuration& config, int run_number)
    : length(config.get<int>("length")),
      precision(config.get<int>("precision")) {
  int rng_seed = config.get<int>("problem_seed") + run_number;

  // Build up the filename where this problem is stored
  string filename = config.get<string>("problem_folder");
  filename += +"IsingSpinGlass_";
  filename += config.get<string>("ising_type") + "_";
  filename += config.get<string>("length") + "_";
  filename += to_string(rng_seed) + ".txt";
  ifstream in(filename.c_str());
  if (!in) {
    throw invalid_argument(
        "IsingSpinGlass data file does not exist: " + filename);
  }
  in >> min_energy;
  string solution_string;
  in >> solution_string;
  int number_of_spins;
  in >> number_of_spins;
  spins.resize(number_of_spins);
  for (auto& spin : spins) {
    for (auto& part : spin) {
      in >> part;
    }
  }
  in.close();
  span = number_of_spins - min_energy;

  // Sanity check
  vector<bool> solution(length);
  for (int i = 0; i < length; i++) {
    solution[i] = solution_string[i] == '1';
  }
  if (evaluate(solution) != 1) {
    throw invalid_argument(
        "IsingSpinGlass data file has inconsistent data: " + filename);
  }
}

float IsingSpinGlass::evaluate(const vector<bool>& solution) {
  int energy = 0;
  for (const auto& spin : spins) {
    energy -= (bit_to_sign[solution[spin[0]]] * spin[2]
        * bit_to_sign[solution[spin[1]]]);
  }

  // Convert to percentage of total
  return float_round(1 - (energy - min_energy) / span, precision);
}

Rastrigin::Rastrigin(Configuration& config, int run_number)
    : precision(config.get<int>("precision")),
      converter(
          BinaryToFloat(config.get<int>("bits_per_float"), -5.12, 5.12,
                        precision)) {
  worst = 0;

  // Build a table for all possible function values
  for (const auto& x : converter.possible()) {
    function[x] = 10 + x * x - 10 * cos(2 * PI * x);
    if (worst < function[x]) {
      worst = function[x];
    }
  }
}

float Rastrigin::evaluate(const vector<bool>& solution) {
  auto it = solution.begin();
  float x, total = 0;
  int n = 0;

  // Use the converter to turn bits in the solution into float values
  while (it != solution.end()) {
    x = converter.convert(it);
    total += function[x];
    n++;
  }

  // Convert to percentage of total
  total /= (n * worst);
  return float_round(1 - total, precision);
}
