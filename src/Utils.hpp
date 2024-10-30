#ifndef __UTILS__
#define __UTILS__

#include "runner_defines.hpp"

#include "runnerpost_platform.hpp"
#include "runnerpost_nsbegin.hpp"
    
std::pair<std::string,std::vector<std::string>> extract_from_bracket(std::string &s, const char * keyValSep =" ");

std::vector<std::string> extract_words(std::string &s);

void toUpperCase(std::string& str);


#include "runnerpost_nsend.hpp"

#endif


