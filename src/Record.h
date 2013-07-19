/*
 * Record.h
 *
 *  Created on: Jul 19, 2013
 *      Author: goldman
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <vector>
#include <utility>
#include <array>
#include <stdlib.h>
#include "Util.h"

using std::vector;
using std::pair;
using std::array;

enum RecordKey {MES, MAD, FAILURES};

class Record {
private:
	vector<pair<float, int>> levels;
public:
	Record() = default;
	virtual ~Record() = default;
	void add(float fitness, int evals);
	const pair<float, int>& best() const;
	static array<int, 3> summarize(const vector<Record>& records);
};

#endif /* RECORD_H_ */
