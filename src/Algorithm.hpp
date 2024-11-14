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
    std::string                _nameAndExtra;
    std::vector<std::string>   _output_options;
    
    // reset info
    void resetInfo ( void );

    
    // No copy constructor
    Algorithm (const Algorithm &) = delete;
    // No copy assignment constructor
    Algorithm & operator=(const Algorithm & ) = delete;
    
    bool set_id(const std::string & id) { if (id.empty()) return false; _id = id; return true;}
    
    bool setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att);

public:
    
    // Constructor #1
    Algorithm( const std::string & id, const std::string & nameAndExtra, const std::vector<std::string> & output_options) : _id(id), _nameAndExtra(nameAndExtra), _output_options(output_options)
    { }
    
    // Constructor #2
    Algorithm (std::string & single_pb_description, std::string & error_msg);
    
    Algorithm() = delete;
    
    // destructor:
    virtual ~Algorithm ( void ) { }
    
    // SET methods:
    void set_name ( const std::string & name) { _nameAndExtra = name ;}
    bool set_output_options(const std::vector<std::string> & s) { _output_options = s; return true;} // TODO: break into specific output options

        
    // GET methods:
    const std::string &              get_id            ( void ) const { return _id;       }
    const std::string &              get_name    ( void ) const { return _nameAndExtra;}
    const std::vector<std::string>&  get_output_options() const { return _output_options; }
    
    
};

#include "runnerpost_nsend.hpp"

#endif
