#include "Utils.hpp"
#include "ArrayOfString.hpp"

#include <cctype>

/// \brief Toupper utility
void RUNNERPOST::toUpperCase(std::string& str)
{
    for_each(str.begin(), str.end(), [](char& in){ in = std::toupper(in); });
}

void RUNNERPOST::toUpperCase(std::vector<std::string>& vstr)
{
    for_each(vstr.begin(), vstr.end(), [](std::string & in){ toUpperCase(in); });
}

std::string RUNNERPOST::toUpperCase(const std::string& str)
{
    std::string s = str;
    for_each(s.begin(), s.end(), [](char& in){ in = std::toupper(in); });
    return s;
}

std::string RUNNERPOST::removeChar(const std::string& str, const char c)
{
    std::string s = str;
    for_each(s.begin(), s.end(), [c](char& in){ if (in==c) in=' '; });
    return s;
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


std::vector<std::string> RUNNERPOST::extract_words(std::string &s, bool toUpperCase)
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
            s.erase(0,pos+1); // Remove selected word
            // Remove extra spaces
            size_t pos2 = s.find_first_not_of(" ");
            if (pos2 != std::string::npos)
            {
                s.erase(0,pos2);
            }
        }
        if (toUpperCase)
        {
            RUNNERPOST::toUpperCase(w);
        }
        words.push_back(w);
    }
    return words;
}



// Convert a string (ex "OBJ", "CST"...)
// to a StatOutputType.
RUNNERPOST::StatOutputType::StatOutputType(const std::string &sConst)
{
    _type = RUNNERPOST::StatOutputType::Type::UNDEFINED;
    std::string s = sConst;
    RUNNERPOST::toUpperCase(s);
    
    if (s == "OBJ" || s == "OBJECTIVE")
    {
        _type = RUNNERPOST::StatOutputType::Type::OBJ;
    }
    else if (s == "CST" || s == "CON" || s == "CONS" || s == "CONST" || s == "CONSTRAINT")
    {
        _type = RUNNERPOST::StatOutputType::Type::CST;
    }
    else if (s == "FEAS" || s == "FEA" || s == "FEASIBLE")
    {
        _type = RUNNERPOST::StatOutputType::Type::FEAS;
    }
    else if (s == "TIME" )
    {
        _type = RUNNERPOST::StatOutputType::Type::TIME;
    }
    else if (s == "CNT_EVAL" || s == "COUNT" || s =="NB_EVAL" || s== "EVAL")
    {
        _type = RUNNERPOST::StatOutputType::Type::CNT_EVAL;
    }
    else if (s == "SOL" || s == "SOLUTION" || s == "X" || s == "XSOL")
    {
        _type = RUNNERPOST::StatOutputType::Type::SOL;
    }
    else if (s == "EXTRA_O")
    {
        _type = RUNNERPOST::StatOutputType::Type::EXTRA_O;
    }
    else if (s == "UNDEFINED")
    {
        std::cerr << "Unrecognized string for RUNNERPOST::StatOutputType: "  << s << std::endl;
        _type = RUNNERPOST::StatOutputType::Type::UNDEFINED;
    }
    else
    {
        std::cerr << "Unrecognized string for RUNNERPOST::StatOutputType: "  << s << std::endl;
        // TODO throw RUNNERPOST::Exception(__FILE__, __LINE__, "Unrecognized string for RUNNERPOST::StatOutputType: " + s);
    }

}

// Convert a string containing multiple StatOutputTypes (ex "OBJ CST")
// to a RUNNERPOST::StatOutputTypeList.
RUNNERPOST::StatOutputTypeList RUNNERPOST::stringToStatOutputTypeList(const std::string &s)
{
    RUNNERPOST::StatOutputTypeList list;
    RUNNERPOST::ArrayOfString aos(s," ");
    if (aos[0] == "STATS_FILE_OUTPUT")
    {
        aos.erase(0);
    }
    for (size_t i = 0; i < aos.size(); i++)
    {
        list.push_back(RUNNERPOST::StatOutputType(aos[i]));
    }
    return list;
}


// Convert a RUNNERPOST::StatOutputTypeList into a string.
std::string RUNNERPOST::StatOutputTypeListToString( const StatOutputTypeList & sotList )
{
    std::ostringstream oss;
    for ( auto sot : sotList )
    {
        oss << sot;
        oss << " ";
    }
    return oss.str();
}


// Count the number of constraints
size_t RUNNERPOST::getNbConstraints(const StatOutputTypeList& sotList)
{
    size_t nbConstraints = 0;
    for (size_t i = 0; i < sotList.size(); i++)
    {
        if (sotList[i].isConstraint())
        {
            nbConstraints++;
        }
    }

    return nbConstraints;
}




// Count the number of objectives
size_t RUNNERPOST::getNbObj(const StatOutputTypeList& sotList)
{
    size_t nbObj = 0;
    for (size_t i = 0; i < sotList.size(); i++)
    {
        if (sotList[i].RUNNERPOST::StatOutputType::isObjective())
        {
            nbObj++;
        }
    }

    return nbObj;
}

// Count the number of objectives
size_t RUNNERPOST::getNbOfType(const StatOutputTypeList& sotList, const StatOutputType::Type & type)
{
    size_t nbOfType = 0;
    for (size_t i = 0; i < sotList.size(); i++)
    {
        if (sotList[i].RUNNERPOST::StatOutputType::isOfType(type))
        {
            nbOfType++;
        }
    }

    return nbOfType;
}


std::istream& RUNNERPOST::operator>>(std::istream& is, RUNNERPOST::StatOutputTypeList &sotList)
{
    std::string s;

    while (is >> s)
    {
        sotList.push_back(RUNNERPOST::StatOutputType(s));
    }

    return is;
}


std::string RUNNERPOST::StatOutputType::display() const
{
    std::string s = "UNDEFINED";
    switch (_type)
    {
        case StatOutputType::Type::OBJ:
            s = "OBJ";
            break;
        case StatOutputType::Type::CST:
            s = "CST";
            break;
        case StatOutputType::Type::TIME:
            s = "TIME";
            break;
        case StatOutputType::Type::CNT_EVAL:
            s = "CNT_EVAL";
            break;
        case StatOutputType::Type::FEAS:
            s = "FEAS";
            break;
        case StatOutputType::Type::SOL:
            s = "SOL";
            break;
        case StatOutputType::Type::EXTRA_O:
            s = "EXTRA_O";
            break;
        case StatOutputType::Type::UNDEFINED:
        default:
            break;
    }
    
    return s;
}


// Function to split a string based on a delimiter
// From GitHub copilot
std::vector<std::string> RUNNERPOST::split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        token = str.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
        end = str.find(delimiter, start);
    }

    token = str.substr(start);
    tokens.push_back(token);

    return tokens;
}
