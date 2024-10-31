#include "Utils.hpp"

#include <cctype>

/// \brief Toupper utility
void RUNNERPOST::toUpperCase(std::string& str)
{
    for_each(str.begin(), str.end(), [](char& in){ in = std::toupper(in); });
}

std::pair<std::string,std::vector<std::string>> RUNNERPOST::extract_from_bracket(std::string &s, const char  *keyValSep)
{
    
    // First word is key, the rest is the value(s) (can contain multiple words and may need cleaning)
    
    std::pair<std::string,std::vector<std::string>> out ; 

    size_t pos0 = s.find("[",0);
    
    // Values must be between []
    if (s.find("]") < pos0)
    {
        return out;
    }
    
    // Remove space before key
    if (s.compare(pos0+1,1," ") == 0)
    {
        size_t pos = s.find_first_not_of(" ",pos0);
        s.erase(pos0+1,pos-pos0+1);
        pos0 = s.find("[",0); // Should be 0!
    }
    
    if (pos0 != 0)
    {
        std::cerr << "Error in extract from bracket" << std::endl;
        return out;
    }

    size_t pos1 = s.find(keyValSep,pos0);
    size_t pos2 = s.find("]",pos1);
    if (pos2 == std::string::npos || pos1 == std::string::npos)
    {
        return out;
    }
    out.first = s.substr(pos0+1,pos1-pos0-1);
    RUNNERPOST::toUpperCase(out.first);
    std::string sTmp = s.substr(pos1+1,pos2-pos1-1);
    std::vector<std::string> sWords = RUNNERPOST::extract_words(sTmp);
    out.second = sWords;
    
    // Extract (remove) the bracket content from string
    s.erase(0,pos2+1);
    return out;
}


std::vector<std::string> RUNNERPOST::extract_words(std::string &s)
{
    std::vector<std::string> words;
    
    // Remove leading spaces
    size_t pos = s.find_first_not_of(" ");
    if (pos != std::string::npos)
    {
        s.erase(0,pos);
    }
    
    // Remove trailing spaces
    pos = s.find_last_not_of(" ");
    if ( pos+1 != s.length() )
    {
        s.erase(pos+1,s.length()-pos +1);
    }
    
    
    std::string w ;
    while (!s.empty())
    {
        
        size_t pos = s.find(" ");
        if (pos == std::string::npos)
        {
           // Single word
            w = s;
            s.clear();
        }
        else
        {
            w = s.substr(0,pos);
            s.erase(0,pos+1); // Remove word
        }
        
        words.push_back(w);
    }
    return words;
}
