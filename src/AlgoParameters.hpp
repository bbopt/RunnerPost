#ifndef __ALGO__
#define __ALGO__

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "runner_defines.hpp"

class AlgoParameters {

private:
    
    // Base info
    std::string                _algoParamsFileName;
    std::string                _solverName;
    std::string                _solverVersion;
    size_t                        _index;
    std::string                _allAttributesAsASingleString;
    std::vector<std::string>   _allAttributes;
    
    // reset base info
    void resetBaseInfo ( void );

    // separate all attributes name and value into a vector of string (printable for param file)
    void separateAttributesNameAndValue();
    
    // No copy constructor
    AlgoParameters (const AlgoParameters &) = delete;
    // No copy assignment constructor
    AlgoParameters & operator=(const AlgoParameters & ) = delete;

public:
    
    // The only constructor allowed
    AlgoParameters( const std::string & solverName , const std::string & solverVersion, const std::string & algoParamsFileName , size_t index ) : _algoParamsFileName(algoParamsFileName),_solverName(solverName),_solverVersion(solverVersion), _index(index) { }

    // destructor:
    virtual ~AlgoParameters ( void ) { }
        
    // GET methods:
    size_t                  getIndex          ( void ) const { return _index;       }
    const std::string & getSolverName    ( void ) const { return _solverName;}
    const std::string & getSolverVersion ( void ) const { return _solverVersion; }
    const std::string & getAlgoParamsFileName( void ) const { return _algoParamsFileName ; }
    std::string getSolverNameAndVersion ( void ) const;
    
    const std::vector<std::string> & getAttributesNameAndValue ( void ) const { return _allAttributes ; }
    const std::string& getAttributesNameAndValueAsString() const { return _allAttributesAsASingleString; }
    
};

#endif
