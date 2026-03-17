#include "Problem.hpp"
#include "Utils.hpp"

#include <cctype>

#include <fstream>

#include <filesystem>


// Default values. Can be set.
// TODO: this is available from Algo
int RUNNERPOST::Problem::_nbSimplexEvals = 150;
int RUNNERPOST::Problem::_overallMaxBBEvals = 20000;

// TODO: accept the syntax * () to use all directory names as pb ids. This should work only when output has no constraint, a single objective. Can be default format: SOL OBJ for all evals or EVAL SOL OBJ. We need SOL to determine the dimenstion from the outputs.

RUNNERPOST::Problem::Problem(std::string  s, std::string & error_msg)
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
    
    // Remove commented lines
    i = s.find("//");
    if ( i != std::string::npos)
    {
        s.erase(i,s.length());
    }
    
    // Case where the line is empty
    if (s.empty())
    {
        error_msg = "Found an empty line in problem selection file";
        return;
    }

    // Get pb id as the first word on the line
    i = s.find_first_not_of(" ");
    if (i > 0)
    {
        s.erase(i) ;  // Remove initial white spaces
    }
    i = s.find(" ");
    std::string pType_str = s.substr(0,i);
    if ( !set_id(pType_str))
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
    set_name(s.substr(i0+1,i1-i0-1));
    s.erase(0,i1+1);
    
        
    size_t pos = 0;
    // Parse for Key + Value(s) given between brackets [Key Val1]
    while ((pos = s.find("[")) != std::string::npos)
    {
        
        // Strip any character (should only be empty spaces) before a [
        pos = s.find_first_not_of(" ");
        s.erase(0, pos);
        
        auto p = RUNNERPOST::extract_from_bracket(s);
        
        if ( p.first.empty() || p.second.empty())
        {
            error_msg = "Error: Cannot read output bracket value in " + s;
            break;
        }
        
        if ( !setSingleAttribute(p) )
        {
            error_msg = "Error: Cannot read output bracket value for " + p.first;
        }
        
    }
    
    if (_pbInstance.empty())
    {
        _nbPbInstances = 1;
    }
    else
    {
        _nbPbInstances = _pbInstance.size();
    }
    
    // TODO: check inconsistencies. Example: tau provided but default output file name are used. At least give a warning
    
}

RUNNERPOST::Problem::Problem(const std::string&  result_file, RUNNERPOST::StatOutputTypeList & sotList, const std::string & pbInst, bool pbFromParse, std::string & error_msg)
{
    bool hasConst = (std::count(sotList.begin(),sotList.end(),StatOutputType::CST) > 0);
    bool hasSol = (std::count(sotList.begin(),sotList.end(),StatOutputType::SOL) > 0);
    size_t nbObj = std::count(sotList.begin(),sotList.end(),StatOutputType::OBJ);
    
    if (hasConst && hasSol)
    {
        error_msg = "Error. Cannot create pb. Constraints and solution are in " + result_file;
        return;
    }
    
    
    std::ifstream in ( result_file, std::ios::in );
    if ( in.fail() )
    {
        in.close();
        error_msg = "Error. Cannot read pb result file " + result_file;
        return;
    }
    
    std::string line;
    getline (in , line);
    
    if (line.empty())
    {
        error_msg = "Error. Cannot read pb file. First line is empty. " + result_file;
        in.close();
        return;
    }
    
    // Let's try to read the pb dimension provided in the first line of the result file
    size_t dimPb = 0;
    if (!hasSol)
    {
        auto first_line_words = RUNNERPOST::extract_words(line, true /*to upper case*/);
        if (first_line_words[0] !="DIM" && first_line_words[1] != "=")
        {
            error_msg = "Error. Cannot read pb from result file " + result_file + ". First line does not start with DIM = xx, where xx is the number of variables.";
            in.close();
            return;
        }
        dimPb = std::stoi(first_line_words[2]);
        if (first_line_words.size() == 6 && first_line_words[3] =="M" && first_line_words[4] == "=")
        {
            _m = std::stoi(first_line_words[5]);
        }
        
        
    }
    else
    {
        
        //Count the number of words in the line
        size_t nbWords = RUNNERPOST::extract_words(line).size();
        
        // Deduce the dimension of pb
        dimPb = nbWords - 1; // Single objective, no constraints. The rest of the words can be the solution.
        if (std::count(sotList.begin(),sotList.end(),StatOutputType::CNT_EVAL) > 0)
        {
            dimPb = int(nbWords) - 2; // Single objective, no constraints and eval counter. The rest of the words can be the solution.
        }
    }
    if (dimPb < 1)
    {
        error_msg = "Error. Cannot read pb result file. Cannot deduce the number of variables in " + result_file;
        in.close();
        return;
    }
    
    _n = int(dimPb);
    // Number of outputs is not provided. Let's guess it.
    if (_m < 0)
    {
        _m = int(nbObj);
    }
    
    _pbFromParse = pbFromParse;
    
    // Extract the id of the pb from the path in pb_dirs
    std::string pbId = RUNNERPOST::split(result_file, RUNNERPOST::DIR_SEP)[1];
    set_id(pbId);
    set_name("Pb"+pbId);
    
    set_pbInstance(pbInst);
    _nbPbInstances = _pbInstance.size();
    
    in.close();
}
    



bool RUNNERPOST::Problem::setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att)
{
    if (att.second.empty())
    {
        return false;
    }
    
    if (att.first == "N")
    {
        if (att.second.size() != 1)
        {
            return false;
        }
        return set_n(att.second[0]);
    }
    else if (att.first =="M")
    {
        if (att.second.size() != 1)
        {
            return false;
        }
        return set_m(att.second[0]);
    }
    else if (att.first =="P")
    {
        if (att.second.size() != 1)
        {
            return false;
        }
        return set_p(att.second[0]);
    }
    else if (att.first == "PB_INSTANCE")
    {
        return set_pbInstance(att.second);
    }
    else
    {
        return false;
    }
    
}

bool RUNNERPOST::Problem::set_pbInstance(const std::vector<std::string> &vs)
{
    _pbInstance.clear();
    
    // Remove white spaces
    std::vector<std::string> tmpVS=vs;
    
    for (auto &s: tmpVS)
    {
        auto tmpS = RUNNERPOST::extract_words(s);
        _pbInstance.insert(_pbInstance.end(),tmpS.begin(), tmpS.end());
    }
    return true;
}

bool RUNNERPOST::Problem::set_pbInstance(const std::string &s)
{
    _pbInstance.clear();
    
    // Remove white spaces
    std::string tmpS=s;
    
    auto tmp2S = RUNNERPOST::extract_words(tmpS);
    
    if (tmp2S.size() > 0)
    {
        _pbInstance.insert(_pbInstance.end(),tmp2S.begin(), tmp2S.end());
        return true;
    }
    return false;
}


/*----------------------------------------------*/
/*                    display                   */
/*----------------------------------------------*/
void RUNNERPOST::Problem::display ( void ) const
{
    std::cout << _id << ": "
    << "(" << _name << ") [N " << _n
    << "] [M " << _m << "] [ P " << _p << "]";
    
    if (_pbInstance.size() > 0)
    {
        std::cout << " [ PB_INSTANCE ";
        for (const auto &s: _pbInstance)
        {
            std::cout << s << " " ;
        }
        std::cout << " ]";
    }
    
}

