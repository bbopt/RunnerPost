#include "Output.hpp"
#include "Utils.hpp"

#include <cctype>


RUNNERPOST::Output::Output (std::string & s, std::string & error_msg)
{
    
    // Get pb id as the first word on the line
    size_t i = s.find_first_not_of(" ");
    if (i > 0)
    {
        s.erase(i) ;  // Remove initial white spaces
    }
    i = s.find(" ");
    std::string pType_str = s.substr(0,i);
    if ( !setProfileType(pType_str))
    {
        error_msg = "Error: Cannot read output profile type in \"" + s + "\"" ;
    }

    s.erase(0,i);
    size_t pos = 0;
    // Parse for n and m given as bracket values
    while ((pos = s.find("[")) != std::string::npos)
    {
        
        // Strip empty spaces before a [
        pos = s.find_first_not_of("[");
        if (pos == std::string::npos)
        {
            break;
        }
        s.erase(0, pos+1);
        
        auto p = RUNNERPOST::extract_from_bracket(s);
        
        if ( p.first.empty() || p.second.empty())
        {
            error_msg = "Error: Cannot read output bracket value (1)" ;
        }
        
        if ( !setSingleAttribute(p) )
        {
            error_msg = "Error: Cannot read output bracket value (2)" ;
        }
        
    }
    
}


RUNNERPOST::Output::Profile_Type RUNNERPOST::Output::stringToProfileType(const std::string & sConst)
{
    Profile_Type pType = Output::Profile_Type::UNDEFINED;
    
    std::string s = sConst;
    RUNNERPOST::toUpperCase(s);
    
    if (s == "DATA_PROFILE")
    {
        pType = Profile_Type::DATA_PROFILE;
    }
    else if (s == "PERFORMANCE_PROFILE")
    {
        pType = Profile_Type::PERFORMANCE_PROFILE;
    }
    else if (s == "HISTORY")
    {
        pType = Profile_Type::HISTORY;
    }
    else if (s == "ACCURACY_PROFILE")
    {
        pType = Profile_Type::ACCURACY_PROFILE;
    }
    
    return pType;
}

bool RUNNERPOST::Output::setSingleAttribute(const std::pair<std::string,std::string> & att)
{
    if (att.first.find("X_SEL") != std::string::npos)
    {
        return setXSelect(att.second);
    }
    else if (att.first.find("Y_SEL") != std::string::npos)
    {
        return setYSelect(att.second);
    }
    else if (att.first.find("TAU") != std::string::npos )
    {
        return setTau(att.second);
    }
    else if (att.first.find("X_MAX") != std::string::npos )
    {
        return setXMax(att.second);
    }
    else if (att.first.find("OUT") != std::string::npos)
    {
        return setFileName(att.second);
    }
    else
    {
        return false;
    }
    
}

std::string RUNNERPOST::Output::profileTypeToString(const Output::Profile_Type & pType)
{
    std::string pTypeStr;
    switch (pType)
    {
        case Profile_Type::DATA_PROFILE:
            pTypeStr = "DATA_PROFILE";
            break;
        case Profile_Type::PERFORMANCE_PROFILE:
            pTypeStr = "PERFORMANCE_PROFILE";
            break;
        case Profile_Type::HISTORY:
            pTypeStr = "HISTORY";
            break;
        case Profile_Type::ACCURACY_PROFILE:
            pTypeStr = "ACCURACY_PROFILE";
            break;
        default:
            break;
    }
    return pTypeStr;
}

bool RUNNERPOST::Output::setXSelect(const std::string &s)
{
    if (s == "EVAL")
    {
        _xSel = RUNNERPOST::Output::X_Select::EVAL;
    }
    else if (s == "NP1EVAL")
    {
        _xSel = RUNNERPOST::Output::X_Select::NP1EVAL;
    }
    else if (s == "TIME")
    {
        _xSel = RUNNERPOST::Output::X_Select::TIME;
    }
    else
    {
        return false;
    }
    return true;

}

bool RUNNERPOST::Output::setYSelect(const std::string &s)
{
    if (s.find("OBJ") != std::string::npos)
    {
        _ySel = RUNNERPOST::Output::Y_Select::OBJ;
    }
    else if (s.find("INF") != std::string::npos)
    {
        _ySel = RUNNERPOST::Output::Y_Select::INFEAS;
    }
    else
    {
        return false;
    }
    return true;
}


/*----------------------------------------------*/
/*                    display                   */
/*----------------------------------------------*/
void RUNNERPOST::Output::display ( void ) const
{
    std::cout << "Output selection display --- TODO " << std::endl;
    
//    std::cout << _type << ": "
//    << "(tau=" << _tau << ") [output=" << _output << "]";;
//    << "] [bnds=" << has_bounds()
//    << "] [cstr=" << _has_constraints
//    << "] [trend=" << _has_trend_matrix
//    << "] [int=" << _has_integers
//    << "] [bin=" << _has_binaries << "] ["
//    << ( is_batch() ? "batch" : "lib" )
//    << "] [f*=" << _fxe << "]";
}

