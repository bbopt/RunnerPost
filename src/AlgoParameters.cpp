#include "AlgoParameters.hpp"


/*----------------------------------------------*/
/*                 reset (private)              */
/*----------------------------------------------*/
void AlgoParameters::resetBaseInfo ( void )
{

    _solverName = "";
    _solverVersion = "";
    _algoParamsFileName ="";
    _allAttributesAsASingleString ="All attributes have default value";
    _allAttributes.clear();

}


/*-------------------------------------------------*/
/*  get a string with the solver name and version  */
/*-------------------------------------------------*/
std::string AlgoParameters::getSolverNameAndVersion ( void ) const
{
    std::ostringstream msg;
    msg << getSolverName() << " " << getSolverVersion();
    return msg.str();
}



void AlgoParameters::separateAttributesNameAndValue( void )
{
    // Each attribute Name and Value pair is within []
    // Separate each pair
    size_t pos0=_allAttributesAsASingleString.find("[",0);
    while ( pos0 != std::string::npos )
    {
        size_t pos1 = _allAttributesAsASingleString.find("]",pos0+1);
        if ( pos1 == std::string::npos )
            break;

        std::string str = _allAttributesAsASingleString.substr(pos0+1,pos1-pos0-1);

        // Ignore the keywords that are not algo attributes
        if ( str.find("SOLVER") == std::string::npos && str.find("RUNNER") == std::string::npos && str.find("ALGO_LEGEND") == std::string::npos )
            _allAttributes.push_back( str );

        pos0 = _allAttributesAsASingleString.find("[",pos1+1);
    }
}
