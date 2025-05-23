#ifndef __RUNNER_DEFINES__
#define __RUNNER_DEFINES__


#include <string>
#include <iostream>
#include <sstream>
#include <limits.h> // For INT_MAX
#include <limits>   // For numeric_limits
#include <math.h> // For pow, fabs, round, etc
#include <cstdlib>
#include <memory>   // For shared_ptr, unique_ptr
#include <vector>
#include <algorithm>

#include "runnerpost_nsbegin.hpp"

// Directory separator
#ifdef WINDOWS
const char        DIR_SEP = '\\';           ///< Directory separator
#else
const char        DIR_SEP = '/';            ///< Directory separator
#endif

const double INF = std::numeric_limits<double>::max(); ///< Infinity
const double M_INF = std::numeric_limits<double>::lowest(); ///< -Infinity
const double NaN = std::numeric_limits<double>::quiet_NaN(); ///< Quiet Not-A-Number
const int P_INF_INT = std::numeric_limits<int>::max(); ///< plus infinity for int
const int M_INF_INT = std::numeric_limits<int>::lowest(); ///< minus infinity for int
const size_t INF_SIZE_T = std::numeric_limits<size_t>::max();///< The infinity for \c size_t

typedef std::vector<double> ArrayOfDouble;


const std::string STATS_FILE_RUN_DIR_BASE = ".";

// If true we have a directory structure AlgoId/PbId
// If false we have PbId/AlgoId
const bool DIR_ORDER_IS_ALGO_PB = true;


// Some constants for the latex plots
// For latex we use one symbol and one color per algo.
// We must have the same number of symbols and colors.
 const std::vector<std::string> SYMBOLS = {"diamond","square","o","^","v","+"};
 const std::vector<std::string> COLORS = {"red","green","blue","cyan","magenta","black"};
 

#include "runnerpost_nsend.hpp"

#endif
