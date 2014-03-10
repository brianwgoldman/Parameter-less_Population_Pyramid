// Brian Goldman

// A record stores the result information for a single optimization run,
// recording how many evaluations are required to reach each new fitness level

#ifndef RECORD_H_
#define RECORD_H_

#include <vector>
#include <utility>
#include <array>
#include <stdlib.h>
#include "Util.h"
#include "Configuration.h"

using std::vector;
using std::pair;
using std::array;

// Labels used for decoding the "summary" return statistics
enum RecordKey {
  MES,
  MAD,
  FAILURES
};

// Stores optimization history information
class Record {
 public:
  Record(Configuration& config)
      : verbosity(config.get<int>("verbosity")) {
  }
  virtual ~Record() = default;
  // Tracks increases in fitness.  If verbosity is set high enough, outputs
  // information to the screen.
  void add(float fitness, int evals);
  // The best fitness achieved in this optimization run
  const pair<float, int>& best() const;
  // Summarizes the end of run statistics for mutliple records
  static array<int, 3> summarize(const vector<Record>& records, Configuration& config);
  // access to the stored values:
  const vector<pair<float, int>>& progression() {
    return levels;
  }

 private:
  // Raw stored data
  vector<pair<float, int>> levels;

  int verbosity;

};

#endif /* RECORD_H_ */
