#include "Output.hpp"
#include "Utils.hpp"

#include <cctype>

RUNNERPOST::Output::Output (std::string s, std::string & error_msg)
{
    if (s.empty())
    {
        error_msg = "Error: Empty output description";
        return;
    }

    std::string sO = s;
    
    // Remove trailing comments
    size_t i = s.find("#");
    if ( i != std::string::npos)
    {
        s.erase(i,s.length());
    }
    
    // Get pb id as the first word on the line
    i = s.find_first_not_of(" ");
    if (i > 0)
    {
        s.erase(i) ;  // Remove initial white spaces
    }
    i = s.find(" ");
    std::string pType_str = s.substr(0,i);
    if ( !setProfileType(pType_str))
    {
        error_msg = "Error: Cannot read output profile type in \"" + sO + "\"" ;
    }
    
    s.erase(0,i);
    
    // Output title provided between parenthesis
    size_t i0 = s.find("(",0);
    size_t i1 = s.find(")",i0+1);

    if ( i0 == std::string::npos || i1 == std::string::npos )
    {
        error_msg = "Error(2) in output selection file. Output title must be provided between parenthesis in " + sO ;
        return ;
    }
    setTitle(s.substr(i0+1,i1-i0-1));
    s.erase(0,i1+1);
    
        
    size_t pos = 0;
    // Parse for Key + Value(s) given between brackets [Key Val1]
    while ((pos = s.find("[")) != std::string::npos)
    {
        
        // Strip any character (should only be empty spaces) before a [
        pos = s.find_first_not_of(" ");
        s.erase(0, pos);
        
        const auto& p = RUNNERPOST::extract_from_bracket(s);
        
        if ( p.first.empty() || p.second.empty())
        {
            error_msg = "Error: Cannot read output bracket value in " + s;
            break;
        }
        
        if ( !setSingleAttribute(p) )
        {
            error_msg = "Error: Cannot read output bracket value in " + p.first + " " + p.second[0];
            break;
        }
        
        // Concatenate the key and the values into a single string
        std::string tmp = p.first + " ";
        for (const auto & v: p.second)
        {
            tmp += v + " ";
        }
        _profile_type_options.push_back(tmp);


    }
    
    // TODO: check inconsistencies. Example: tau provided but default output file name are used. At least give a warning
    if ((_pType == DATA_PROFILE || _pType == PERFORMANCE_PROFILE) && _plotType != UNDEFINED_PLOT_TYPE)
    {
        error_msg = "Error: Plot type cannot be defined for DATA_PROFILE or PERFORMANCE_PROFILE, only for CONVERGENCE_PROFILE.";
    }
    if (_pType == CONVERGENCE_PROFILE && _tau != -1)
    {
        error_msg = "Error: Cannot set tau for CONVERGENCE_PROFILE.";
    }
    
}


RUNNERPOST::Output::Profile_Type RUNNERPOST::Output::stringToProfileType(const std::string & sConst)
{
    Profile_Type pType = Output::Profile_Type::UNDEFINED_PROFILE;
    
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
    else if (s == "CONVERGENCE_PROFILE")
    {
        pType = Profile_Type::CONVERGENCE_PROFILE;
    }
    else if (s == "ACCURACY_PROFILE")
    {
        pType = Profile_Type::ACCURACY_PROFILE;
    }
    
    return pType;
}

RUNNERPOST::Output::Fx_First_Feas_Method RUNNERPOST::Output::stringToFFFeasMeth(const std::string & sConst)
{
    Fx_First_Feas_Method fffm = Output::Fx_First_Feas_Method::UNDEFINED_FFFM;
    
    std::string s = sConst;
    RUNNERPOST::toUpperCase(s);
    
    if (s == "MIN")
    {
        fffm = Fx_First_Feas_Method::min;
    }
    else if (s == "MAX")
    {
        fffm = Fx_First_Feas_Method::max;
    }
    else if (s == "AVG" || s == "AVERAGE")
    {
        fffm = Fx_First_Feas_Method::avg;
    }
    
    return fffm;
}


std::string RUNNERPOST::Output::fFeasMethToString(const Fx_First_Feas_Method & fffm)
{
    std::string fffmStr;
    switch (fffm)
    {
        case Fx_First_Feas_Method::min:
            fffmStr = "MIN";
            break;
        case Fx_First_Feas_Method::max:
            fffmStr = "MAX";
            break;
        case Fx_First_Feas_Method::avg:
            fffmStr = "AVG";
            break;
        case Fx_First_Feas_Method::UNDEFINED_FFFM:
            fffmStr = "UNDEFINED";
            break;
        default:
            break;
    }
    return fffmStr;

    
}

RUNNERPOST::Output::Plot_Type RUNNERPOST::Output::stringToPlotType(const std::string & sConst)
{
    Plot_Type pType = Output::Plot_Type::UNDEFINED_PLOT_TYPE;
    
    std::string s = sConst;
    RUNNERPOST::toUpperCase(s);
    
    if (s == "ONLYFFEASIBLE")
    {
        pType = Plot_Type::OnlyFFeasible;
    }
    else if (s == "ONLYF")
    {
        pType = Plot_Type::OnlyF;
    }
    else if (s == "ONLYHINFEASIBLE")
    {
        pType = Plot_Type::OnlyHInfeasible;
    }
    else if (s == "COMBOHINFANDFFEAS")
    {
        pType = Plot_Type::ComboHInfAndFFeas;
    }
        
    return pType;
}

std::string RUNNERPOST::Output::plotTypeToString(const Output::Plot_Type & pType)
{
    std::string pTypeStr;
    switch (pType)
    {
        case Plot_Type::OnlyFFeasible:
            pTypeStr = "OnlyFFeasible";
            break;
        case Plot_Type::OnlyF:
            pTypeStr = "OnlyF";
            break;
        case Plot_Type::OnlyHInfeasible:
            pTypeStr = "OnlyHInfeasible";
            break;
        case Plot_Type::ComboHInfAndFFeas:
            pTypeStr = "ComboHInfAndFFeas";
            break;
        default:
            break;
    }
    return pTypeStr;
}


bool RUNNERPOST::Output::setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att)
{
    if (att.second.size() > 3)
    {
        return false;
    }
    
    if (att.first.find("X_SEL") != std::string::npos)
    {
        return setXSelect(att.second[0]);
    }
    else if (att.first.find("Y_SEL") != std::string::npos)
    {
        return setYSelect(att.second[0]);
    }
    else if (att.first.find("TAU") != std::string::npos )
    {
        return setTau(att.second[0]);
    }
    else if (att.first.find("X_MAX") != std::string::npos )
    {
        if (toUpperCase(att.second[0]) =="MAX" || toUpperCase(att.second[0]) == "INF")
        {
            return setXMax(RUNNERPOST::P_INF_INT);
        }
        else
        {
            try
            {
                int xmax = std::stoi(att.second[0]);
                return setXMax(xmax);
            }
            catch (const std::invalid_argument& ia)
            {
                std::cerr << "Invalid argument for x_max: " << att.second[0] << " --> " << ia.what() << '\n';
                return false;
            }
        }
    }
    else if (att.first.find("PLOT_SELECTION") != std::string::npos )
    {
        return setPlotSelection(att.second);
    }
    else if (att.first.find("PLOT_TYPE") != std::string::npos )
    {
        return setPlotType(att.second[0]);
    }
    else if (att.first.find("OUTPUT_PLAIN") != std::string::npos)
    {
        return setPlainFileName(att.second[0]);
    }
    else if (att.first.find("OUTPUT_LATEX") != std::string::npos)
    {
        return setLatexFileName(att.second[0]);
    }
    else if (att.first.find("FX_FIRST_FEAS") != std::string::npos)
    {
        return setFFFeasMeth(att.second[0]);
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
        case Profile_Type::CONVERGENCE_PROFILE:
            pTypeStr = "CONVERGENCE_PROFILE";
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
    std::string sUpper = s;
    _xSel = RUNNERPOST::Output::X_Select::UNDEFINED_X;

    RUNNERPOST::toUpperCase(sUpper);
    if (sUpper == "EVAL")
    {
        _xSel = RUNNERPOST::Output::X_Select::EVAL;
    }
    else if (sUpper == "NP1EVAL")
    {
        _xSel = RUNNERPOST::Output::X_Select::NP1EVAL;
    }
    else if (sUpper == "TIME")
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
    _ySel = RUNNERPOST::Output::Y_Select::UNDEFINED_Y;  

    std::string sUpper = s;
    RUNNERPOST::toUpperCase(sUpper);

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


bool RUNNERPOST::Output::setPlotSelection(const std::vector<std::string> & s)
{
    _plotSelection.clear();
    
    if (s.empty())
    {
        std::cerr << "Error: PLOT_SELECTION requires at least one argument." << std::endl;
        return false;
    }
    
    if (s.size() > 3)
    {
        std::cerr << "Error: PLOT_SELECTION requires at most three arguments." << std::endl;
        return false;
    }
    
    
    for (auto v: s)
    {
        RUNNERPOST::toUpperCase(v);
        _plotSelection.push_back(v);
    }
    return true;
}

bool RUNNERPOST::Output::plotIsSelected(const std::string & algoId, const std::string & pbId, size_t i_pbInstance) const
{
    if (_plotSelection.empty())
    {
        return true;
    }
    
    std::string s = algoId;
    RUNNERPOST::toUpperCase(s);
    bool algoIsSelected = (_plotSelection[0] == "ALL" || _plotSelection[0] == "*" || _plotSelection[0] == s );

    if (_plotSelection.size() == 1 )
    {
        return algoIsSelected;
    }
    
    s = pbId;
    RUNNERPOST::toUpperCase(s);
    bool pbIsSelected = (_plotSelection[1] == "ALL" || _plotSelection[1] == "*" || _plotSelection[1] == s );
    if (_plotSelection.size() == 2 )
    {
        return algoIsSelected && pbIsSelected;
    }
    
    bool pbInstanceIsSelected = (_plotSelection[2] == "ALL" || _plotSelection[2] == "*" || std::stoi(_plotSelection[2]) == i_pbInstance );
    return algoIsSelected && pbIsSelected && pbInstanceIsSelected;
    
}

