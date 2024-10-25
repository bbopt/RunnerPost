#include "Utils.hpp"

#include <cctype>

/// \brief Toupper utility
void RUNNERPOST::toUpperCase(std::string& str)
{
    for_each(str.begin(), str.end(), [](char& in){ in = std::toupper(in); });
}

std::pair<std::string,std::string> RUNNERPOST::extract_from_bracket(std::string &s)
{
    std::pair<std::string,std::string> out={"",""};

    size_t pos0 = s.find("[",0);
    size_t pos1 = s.find("=",pos0);
    size_t pos2 = s.find("]",pos1);
    if (pos0 == std::string::npos || pos2 == std::string::npos || pos1 == std::string::npos)
    {
        return out;
    }
    out.first = s.substr(pos0+1,pos1-pos0-1);
    RUNNERPOST::toUpperCase(out.first);
    out.second = s.substr(pos1+1,pos2-pos1-1);
    RUNNERPOST::toUpperCase(out.second);
    
    // Extract (remove) the bracket info from string
    s.erase(pos0,pos2-pos0+1);
    return out;
}

