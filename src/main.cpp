//============================================================================
// Name        : P3.cpp
// Author      : Brian W. Goldman
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "Evaluation.h"
#include "HillClimb.h"
#include "Util.h"
#include "MiddleLayer.h"
#include "Population.h"
#include "Configuration.h"
#include "Optimizer.h"
#include "OptimizationCollection.h"
#include "Record.h"
#include "Experiments.h"

int main(int argc, char * argv[]) {
  Configuration config;
  config.parse(argc, argv);

  Random rand;
  int seed = config.get<int>("seed");
  if (seed == -1) {
    std::random_device rd;
    seed = rd();
    config.set("seed", seed);
  }
  rand.seed(seed);

  string dat_file = config.get<string>("dat_file");
  ofstream cfg_out(config.get<string>("cfg_file"));
  config.dump(cfg_out);
  cfg_out.close();

  auto problem = config.get<evaluation::pointer>("problem");
  auto optimizer_method = config.get<optimize::pointer>("optimizer");

  if (config.get<string>("experiment") == "bisection") {
    int pop_size = bisection(rand, config, problem, optimizer_method);
    cout << "POP SIZE " << pop_size << endl;
  } else if (config.get<string>("experiment") == "multirun") {
    vector<Record> records = multirun(rand, config, problem, optimizer_method);
    auto summary = Record::summarize(records, config);
    ofstream out(dat_file.c_str());
    cout << summary[MES] << " " << summary[MAD] << " ";
    cout << summary[FAILURES] << endl;
    out << "# " << summary[MES] << " " << summary[MAD] << " ";
    out << summary[FAILURES] << endl;
    for (const auto& record : records) {
      if (config.get<int>("verbosity") > 1) {
        cout << record.best().first << " " << record.best().second << endl;
      }
      out << record.best().first << " " << record.best().second << endl;
    }
    out.close();
  } else { // single_run
    Record record = single_run(rand, config, problem, optimizer_method, 0);
    ofstream out(dat_file.c_str());
    for (const auto& line : record.levels) {
      out << line.first << " " << line.second << endl;
    }
    out.close();
  }
  return 0;
}
