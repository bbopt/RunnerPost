#include "Algorithm.hpp"
#include "Utils.hpp"


/*----------------------------------------------*/
/*                 reset (private)              */
/*----------------------------------------------*/
void RUNNERPOST::Algorithm::resetInfo ( void )
{
    _id = "";
    _nameAndExtra = "";
    _output_options.clear();

}


RUNNERPOST::Algorithm::Algorithm(std::string & s, std::string & error_msg)
{
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
            error_msg = "Error: Cannot read output bracket value in " + s;
        }
        
    }

    
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
    
    _output_options.push_back(s);
    
    return true;
    
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
