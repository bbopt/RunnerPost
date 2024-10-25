#ifndef __ALGO__
#define __ALGO__

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "runner_defines.hpp"

#include "runnerpost_nsbegin.hpp"

class Algorithm {

private:
    
    // Base info
    std::string                _id;
    std::string                _nameAndVersion;
    size_t                     _index;
    std::string                _options;
    std::vector<int>           _run_seeds;
    size_t                     _n_seed;
    
    // reset info
    void resetInfo ( void );

    
    // No copy constructor
    Algorithm (const Algorithm &) = delete;
    // No copy assignment constructor
    Algorithm & operator=(const Algorithm & ) = delete;

public:
    
    // The only constructor allowed
    Algorithm( size_t index, const std::string & id, const std::string & nameAndVersion, const std::string & options) : _index(index), _id(id), _nameAndVersion(nameAndVersion), _options(options), _run_seeds({0} /* TODO */ ), _n_seed(1 /*todo*/) { }
    Algorithm() = delete;
    
    // destructor:
    virtual ~Algorithm ( void ) { }
        
    // GET methods:
    size_t                   get_index          ( void ) const { return _index;       }
    const std::string &      get_id            ( void ) const { return _id;       }
    const std::string &      get_name    ( void ) const { return _nameAndVersion;}
    const std::string&       get_options() const { return _options; }
    const std::vector<int> & get_run_seeds( void ) const { return _run_seeds; }
    const size_t get_n_seeds( void ) const { return _n_seed ; }
    
};

#include "runnerpost_nsend.hpp"

#endif
