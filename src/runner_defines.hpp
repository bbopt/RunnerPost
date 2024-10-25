#ifndef __RUNNER_DEFINES__
#define __RUNNER_DEFINES__


#include <string>
#include <iostream>
#include <sstream>
#include <limits.h> // For INT_MAX
#include <limits>   // For numeric_limits
#include <cstdlib>
#include <memory>   // For shared_ptr, unique_ptr


// All directories are relative to the run directory
//const std::string PROBLEMS_DIR         = "./problems/";

// The test directory for a problem is relative to the problem 
//const std::string TESTS_DIR            = "tests/";

// Directory separator
#ifdef WINDOWS
const char        DIR_SEP = '\\';           ///< Directory separator
#else
const char        DIR_SEP = '/';            ///< Directory separator
#endif

#include "runnerpost_nsbegin.hpp"

const double INF = std::numeric_limits<double>::max(); ///< Infinity
const double M_INF = std::numeric_limits<double>::lowest(); ///< -Infinity
const double NaN = std::numeric_limits<double>::quiet_NaN(); ///< Quiet Not-A-Number
const int P_INF_INT = std::numeric_limits<int>::max(); ///< plus infinity for int
const int M_INF_INT = std::numeric_limits<int>::lowest(); ///< minus infinity for int
const size_t INF_SIZE_T = std::numeric_limits<size_t>::max();///< The infinity for \c size_t

typedef std::vector<double> ArrayOfDouble;


// TODO pass these options in the algo configuration file
const std::string STATS_FILE = "history.txt";
// If true we add seed in stats file name. For example history.txt -> history.0.txt
const bool ADD_SEED_TO_STATS_FILE = true;

const std::string STATS_FILE_RUN_DIR_BASE = "Run";

// If true we have a directory structure AlgoId/PbId
// If false we have PbId/AlgoId
const bool DIR_ORDER_IS_ALGO_PB = true;

#include "runnerpost_nsend.hpp"

#endif
