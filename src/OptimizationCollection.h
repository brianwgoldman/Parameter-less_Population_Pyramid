/*
 * OptimizationCollection.h
 *
 *  Created on: Jul 18, 2013
 *      Author: goldman
 */

#ifndef OPTIMIZATIONCOLLECTION_H_
#define OPTIMIZATIONCOLLECTION_H_

#include "Optimizer.h"
#include "LTGA.h"
#include "Pyramid.h"
#include "RandomRestartHC.h"
#include <unordered_map>

namespace optimize
{
	using pointer=shared_ptr<Optimizer> (*)(Configuration& config);

	static std::unordered_map<string, pointer> lookup({
			{"LTGA", LTGA::create},
			{"Pyramid", Pyramid::create},
			{"RandomRestartHC", RandomRestartHC::create}
		});
}


#endif /* OPTIMIZATIONCOLLECTION_H_ */
