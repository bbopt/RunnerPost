#ifndef __ALGO__
#define __ALGO__

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "runner_defines.hpp"
#include "Utils.hpp"

#include "runnerpost_nsbegin.hpp"

class Algorithm {
public:

    static const std::string DEFAULT_STATS_FILE_NAME;

    // If true we add pb instance in stats file name. For example history.txt -> history.0.txt
    static const bool DEFAULT_ADD_PBINSTANCE_TO_STATS_FILE;

private:
    
    // Base info
    std::string                _id;
    std::string                _nameAndExtra;
    std::vector<std::string>   _output_options;  // TODO make it an ArrayOfString or better make it a structure with all functions
    
    StatOutputTypeList         _sotList;  ///< Stat output type list extracted from output options.
    
    
    bool set_id(const std::string & id) { if (id.empty()) return false; _id = id; return true;}
    
    bool setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att);
    
    void set_stats_output_type_list();
    
public:
    
    // Constructor #1
    Algorithm( const std::string & id, const std::string & nameAndExtra, const std::vector<std::string> & output_options) : _id(id), _nameAndExtra(nameAndExtra), _output_options(output_options)
    {
        set_stats_output_type_list(); // possibly extracted from output options if available
    }
    
    // Constructor #2
    Algorithm (std::string single_pb_description, std::string & error_msg);
    
    // No default constructor
    Algorithm() = delete;

    // No copy constructor
    Algorithm(const Algorithm&) = delete;

    // No copy assignment operator
    Algorithm& operator=(const Algorithm&) = delete;
    
    // destructor:
    virtual ~Algorithm ( void ) { }
    
    // reset info
    void resetInfo ( void );

    // SET methods:
    void set_name ( const std::string & name) { _nameAndExtra = name ;}

    bool set_output_options(const std::vector<std::string> & s) { _output_options = s; set_stats_output_type_list();  return true;}

        
    // GET methods:
    const std::string &              get_id            ( void ) const { return _id;       }
    const std::string &              get_name    ( void ) const { return _nameAndExtra;}
    const std::vector<std::string>&  get_output_options() const { return _output_options; }
    std::vector<std::string>         get_output_option(const std::string & label) const;
    
    const StatOutputTypeList &       get_stats_output_type_list() const { return _sotList;}
    
    bool hasConstraint() const { return (std::count(_sotList.begin(),_sotList.end(),StatOutputType::CST) > 0);}
    bool hasMultiObjective() const { return (std::count(_sotList.begin(),_sotList.end(),StatOutputType::OBJ) > 1);}
    size_t getNbConstraints() const { return std::count(_sotList.begin(),_sotList.end(),StatOutputType::CST);}
    size_t getNbObjectives() const { return std::count(_sotList.begin(),_sotList.end(),StatOutputType::OBJ);}
};

#include "runnerpost_nsend.hpp"

#endif
