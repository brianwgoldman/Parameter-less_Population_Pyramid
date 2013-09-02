/*
 * Experiments.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: goldman
 */

#include "Experiments.h"

Record single_run(Random& rand, Configuration& config,
                  evaluation::pointer problem, optimize::pointer solver,
                  int run) {
  size_t limit = config.get<int>("eval_limit");
  Middle_Layer recorder(config, problem(config, run));
  auto optimizer = solver(rand, recorder, config);

  // Iterate the optimizer until the solution is reached, the maximum number
  // of evaluations is performed, or the optimizer reaches stagnation
  bool improvement_possible = true;
  while (recorder.best_fitness < 1.0 and recorder.evaluations < limit
      and improvement_possible) {
    improvement_possible = optimizer->iterate();
  }
  return recorder.results;
}

vector<Record> multirun(Random& rand, Configuration& config,
                        evaluation::pointer problem, optimize::pointer solver) {
  int runs = config.get<int>("runs");
  int verbosity = config.get<int>("verbosity");
  vector<Record> records;
  for (int run = 0; run < runs; run++) {
    records.push_back(single_run(rand, config, problem, solver, run));
    auto summary = Record::summarize(records);
    if (verbosity > 0) {
      std::cout << "Run: " << run << " Evals: "
                << records[records.size() - 1].best().second << " MES: "
                << summary[MES] << " MAD: " << summary[MAD] << " FAILURES: "
                << summary[FAILURES] << std::endl;
    }
  }
  return records;
}

int bisection(Random& rand, Configuration& config, evaluation::pointer problem,
              optimize::pointer solver) {
  int runs = config.get<int>("runs");
  vector<Record> records;
  int min = 0;
  int max = 1;
  size_t failed_on = 0;
  bool success;
  // Double the maximum size until a successful run is found
  do {
    min = max;
    max *= 2;
    config.set("pop_size", max);
    std::cout << "Pop size: " << max << std::endl;
    success = true;
    for (int i = 0; success and i < runs; i++) {
      int problem_number = ((i + failed_on) % runs);
      std::cout << "\tTrying problem: " << problem_number << std::endl;
      auto results = single_run(rand, config, problem, solver, problem_number);
      if (results.best().first < 1) {
        failed_on = problem_number;
        success = false;
      }
    }
  } while (!success);

  // Bisect between the min (unsuccessful) and max (successful) until they meet
  int guess;
  while (min + 1 < max) {
    guess = (max + min) / 2;
    std::cout << "Pop size: " << guess << std::endl;
    config.set("pop_size", guess);
    success = true;
    for (int i = 0; success and i < runs; i++) {
      int problem_number = ((i + failed_on) % runs);
      std::cout << "\tTrying problem: " << problem_number << std::endl;
      auto results = single_run(rand, config, problem, solver, problem_number);
      if (results.best().first < 1) {
        failed_on = problem_number;
        success = false;
      }
    }
    if (success) {
      max = guess;
    } else {
      min = guess;
    }
  }
  return max;
}
