// Brian Goldman

// There are many ways to perform local search, and this
// file contains the implementation for a lot of configuration options.
// In all published experiments (so far) "first_improvement" was used,
// but other methods were informally tested.

#ifndef HILLCLIMB_H_
#define HILLCLIMB_H_

#include <unordered_map>
#include <unordered_set>
#include "Evaluation.h"
#include "Configuration.h"
#include "Util.h"

namespace hill_climb {
// Creates a type name for pointers to hill climbing functions
using pointer=void (*)(Random & rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);

// Each time an improvement is found, it is immediately kept, with search progressing
// from the modified solution
void first_improvement(Random & rand, vector<bool> & solution, float & fitness,
                       Evaluator& evaluator);

// Before accepting any improvement, all possible 1 bit neighbors are tested.
// The 1 bit change that results in the highest fitness is then chosen, with search
// progressing from that modified solution
void steepest_ascent(Random & rand, vector<bool> & solution, float & fitness,
                     Evaluator& evaluator);

// Stub function used to turn off local search.  Always returns immediately
// without modifying the solution.
void no_action(Random & rand, vector<bool> & solution, float & fitness,
               Evaluator& evaluator);

// Generates a single random solution.  If that solution is better than the passed
// in vector, the passed in vector is overwritten.
void binary_tournament(Random & rand, vector<bool> & solution, float & fitness,
                       Evaluator& evaluator);

// Similar to "first_improvement" except that each bit is flipped at most one
// time.  Improvements are kept as they are found, but a gene is never revisited
void once_each(Random & rand, vector<bool> & solution, float & fitness,
               Evaluator& evaluator);

// Mapping used to convert configuration string names to actual function pointers
static std::unordered_map<string, pointer> lookup( {
  { "first_improvement", first_improvement },
  { "steepest_ascent", steepest_ascent },
  { "no_action", no_action },
  { "binary_tournament", binary_tournament },
  { "once_each", once_each }
});
}

#endif /* HILLCLIMB_H_ */
