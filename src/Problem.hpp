#ifndef __PROBLEM__
#define __PROBLEM__

#include "runner_defines.hpp"
#include "Algorithm.hpp"
#include <set>

#include "runnerpost_nsbegin.hpp"

class Problem {

private:

    std::string                        _id="";
    std::string                        _name="";
    int                                _n =-1;  // Pb dimension. Must be set.
    int                                _m =-1;  // Number of outputs (ineq + eq + objs). Must be set.
    int                                _p = 0; // Number of equality constraints. May not be set.
    
    std::vector<std::string>           _pbInstance; // Can be undefined (single instance, no id -> not used in history name)
    size_t                             _nbPbInstances;
    bool                               _pbFromParse = false;  // if true, the pb is deduced from the result files not from the problem definition file.
    
    
    // The max_bb_eval termination criterion is min( _overallMaxBBEvals, (n+1)*_nbSimplexEvals
    static int                         _nbSimplexEvals; // if <=0 ==> other termination criterions will be used
    static int                         _overallMaxBBEvals; // if <= 0 ==> other termination criterions will be used
    
    // TODO
    // std::set<std::string>              _keywords;
    // static std::set<std::string>       _all_keywords;
    
    // display keywords:
    // TODO
    // static void display_keywords ( const std::set<std::string> & keywords );
    
public:
    
    // constructor #1
    Problem (const std::string & id       )
    : _id              ( id     ) 
    {};
    
    // constructor #2:
    Problem ( const std::string & id          ,
             const std::string & name )
    : _id              ( id     ) ,
      _name            ( name   ) 
    {};
    
    // constructor #3:
    Problem ( const std::string & id          ,
             const std::string & name         ,
             int                 n            ,
             int                 m            )
    : _id              ( id     ) ,
      _name            ( name   ) ,
      _n               ( n      ) ,
      _m               ( m      )
    {};
    
    // Constructor #4
    Problem (std::string single_pb_description, std::string & error_msg);
    
    // Constructor #5: From a result file
    Problem(const std::string&  result_file, RUNNERPOST::StatOutputTypeList & sotList, const std::string & pbInst, bool pbFromParse, std::string & error_msg);

    
    // destructor:
    virtual ~Problem ( void ) {}

    // SET methods:
    void set_name ( const std::string & name) { _name = name ;}
    
    bool set_n(const int & n) { if (n<=0) return false; _n = n ; return true; }
    bool set_n(const std::string & s) {return set_n(std::stoi(s));}
    bool set_m(const int & m) { if (m<=0) return false; _m = m ; return true; }
    bool set_m(const std::string & s) {return set_m(std::stoi(s));}
    bool set_p(const int & p) { if (p<0) return false; _p = p ; return true; }
    bool set_p(const std::string & s) {return set_p(std::stoi(s));}
    bool set_pbInstance (const std::vector<std::string> & vs);
    bool set_pbInstance (const std::string & s);
    
    void display ( void ) const;

    
    // GET methods:
    std::string         get_id        ( ) const {return _id;}
    const std::string & get_name    ( void ) const { return _name;           }
    int                 get_n         ( void ) const { return _n;                }
    int                 get_m         ( void ) const { return _m;                }
    int                 get_p         ( void ) const { return _p;                }
    const std::vector<std::string> & get_pbInstance( void) const { return _pbInstance; }
    const size_t get_nbPbInstances( void) const { return _nbPbInstances; }
    
    bool get_pbFromParse( void ) const { return _pbFromParse; }
    

private:
    
    bool set_id(const std::string & id) { if (id.empty()) return false; _id = id; return true;}
    
    bool setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att);
    
};

#include "runnerpost_nsend.hpp"

#endif


