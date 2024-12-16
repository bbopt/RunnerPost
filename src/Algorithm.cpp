#include "Algorithm.hpp"
#include "Utils.hpp"

#include "ArrayOfString.hpp"


//  Static member initialization
const std::string RUNNERPOST::Algorithm::DEFAULT_STATS_FILE_NAME = "history.txt";
const bool RUNNERPOST::Algorithm::DEFAULT_ADD_PBINSTANCE_TO_STATS_FILE = false;

/*----------------------------------------------*/
/*                 reset (private)              */
/*----------------------------------------------*/
void RUNNERPOST::Algorithm::resetInfo ( void )
{
    _id = "";
    _nameAndExtra = "";
    _output_options.clear();

}


RUNNERPOST::Algorithm::Algorithm(std::string s, std::string & error_msg)
{
    if (s.empty())
    {
        error_msg = "Error: Empty output description";
        return;
    }
    
    std::string s0 = s;
    
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
    if ( !set_id(pType_str))
    {
        error_msg = "Error: Cannot read output profile type in \"" + s0 + "\"" ;
    }
    
    s.erase(0,i);
    
    // Output title provided between parenthesis
    size_t i0 = s.find("(",0);
    size_t i1 = s.find(")",i0+1);

    if ( i0 == std::string::npos || i1 == std::string::npos )
    {
        resetInfo();
        error_msg = "Error(2) in output selection file. Output title must be provided between parenthesis in " + s0 ;
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
            resetInfo();
            error_msg = "Error: Cannot read output bracket value in " + s;
            break;
        }
        
        if ( !setSingleAttribute(p) )
        {
            resetInfo();
            error_msg = "Error: Cannot read output bracket value in " + s;
        }
        
    }
    
    set_stats_output_type_list();

    
    // TODO: check inconsistencies. Example: tau provided but default output file name are used. At least give a warning
    
}

bool RUNNERPOST::Algorithm::setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att)
{
    // TODO: break into specific options. BBOT case
    // For now we do not use this info
    
    if (att.second.empty())
    {
        return false;
    }
    
    // Concatenate the key and the values into a single string
    std::string s = att.first + " ";
    for (const auto & v: att.second)
    {
        s += v + " ";
    }
    // Remove trailing space
    s.erase(s.length()-1);
    
    _output_options.push_back(s);
    
    return true;
    
}


std::vector<std::string> RUNNERPOST::Algorithm::get_output_option(const std::string & label) const
{
    std::vector<std::string> sWords;
    
    // We must duplicate output_option to extract words for it
    auto output_options= _output_options;
    
    for (auto & o: output_options)
    {
        if ( o.find(label) !=std::string::npos)
        {
            sWords = RUNNERPOST::extract_words(o);
            for (auto & word: sWords)
            {
                RUNNERPOST::toUpperCase(word);
            }
            break;
        }
    }
    return sWords;
}

void RUNNERPOST::Algorithm::set_stats_output_type_list()
{
    for (const auto & o: _output_options)
    {
        if ( o.find("STATS_FILE_OUTPUT") !=std::string::npos)
        {
            _sotList = RUNNERPOST::stringToStatOutputTypeList(o);
            break;
        }
    }
    
    // Default stats file output depends if all evals are given
    if (_sotList.empty())
    {
        bool display_all_eval = false;
        for (const auto & o: _output_options)
        {
            if ( o.find("DISPLAY_ALL_EVAL") !=std::string::npos)
            {
                RUNNERPOST::ArrayOfString aos(o," ");
                std::string response = aos[1];
                RUNNERPOST::toUpperCase(response);
                if ( response == "YES" || response == "TRUE" || response =="1" )
                {
                    _sotList = RUNNERPOST::stringToStatOutputTypeList("OBJ");
                    display_all_eval = true;
                }
                break;
            }
        }
        if (!display_all_eval)
        {
            _sotList = RUNNERPOST::stringToStatOutputTypeList("CNT_EVAL OBJ");
        }
    }
    
    // Some checks
    if (RUNNERPOST::getNbConstraints(_sotList) > 1)
    {
        std::cerr << "Invalid STATS_FILE_OUTPUT for " << get_name() << ". The definition contains more than one entry for CST." << std::endl;
        _sotList.clear();
    }
    if (RUNNERPOST::getNbOfType(_sotList, StatOutputType::SOL) > 1)
    {
        std::cerr << "Invalid STATS_FILE_OUTPUT for " << get_name() << ". The definition contains more than one entry for SOL." << std::endl;
        _sotList.clear();
    }
    if (RUNNERPOST::getNbOfType(_sotList, StatOutputType::TIME) > 1)
    {
        std::cerr << "Invalid STATS_FILE_OUTPUT for " << get_name() << ". The definition contains more than one entry for TIME." << std::endl;
        _sotList.clear();
    }
    if (RUNNERPOST::getNbOfType(_sotList, StatOutputType::FEAS) > 1)
    {
        std::cerr << "Invalid STATS_FILE_OUTPUT for " << get_name() << ". The definition contains more than one entry for FEAS." << std::endl;
        _sotList.clear();
    }
    
}


//void Algorithm::separateAttributesNameAndValue( void )
//{
//    // Each attribute Name and Value pair is within []
//    // Separate each pair
//    size_t pos0=_allAttributesAsASingleString.find("[",0);
//    while ( pos0 != std::string::npos )
//    {
//        size_t pos1 = _allAttributesAsASingleString.find("]",pos0+1);
//        if ( pos1 == std::string::npos )
//            break;
//
//        std::string str = _allAttributesAsASingleString.substr(pos0+1,pos1-pos0-1);
//
//        // Ignore the keywords that are not algo attributes
//        if ( str.find("SOLVER") == std::string::npos && str.find("RUNNER") == std::string::npos && str.find("ALGO_LEGEND") == std::string::npos )
//            _allAttributes.push_back( str );
//
//        pos0 = _allAttributesAsASingleString.find("[",pos1+1);
//    }
//}
