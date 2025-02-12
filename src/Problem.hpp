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
    int                                _m =-1;  // Number of output. Must be set.
    
    std::vector<std::string>           _pbInstance; // Can be undefined (single instance, no id -> not used in history name)
    size_t                             _nbPbInstances;
    
    
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
    Problem(const std::string&  result_file, RUNNERPOST::StatOutputTypeList & sotList, const std::string & pbInst, std::string & error_msg);

    
    // destructor:
    virtual ~Problem ( void ) {}

    // SET methods:
    void set_name ( const std::string & name) { _name = name ;}
    
    bool set_n(const int & n) { if (n<=0) return false; _n = n ; return true; }
    bool set_n(const std::string & s) {return set_n(std::stoi(s));}
    bool set_m(const int & m) { if (m<=0) return false; _m = m ; return true; }
    bool set_m(const std::string & s) {return set_m(std::stoi(s));}
    bool set_pbInstance (const std::vector<std::string> & vs);
    bool set_pbInstance (const std::string & s);
    
    
    // void set_xe_file_name( std::string f){ _xe_file_name = f ;}
    
    
    void display ( void ) const;
    
//    void display_keywords ( ) const
//    {
//        Problem::display_keywords ( _keywords );
//    }
//    
//    static void display_all_keywords ( )
//    {
//        Problem::display_keywords ( Problem::_all_keywords );
//    }
    
    // GET methods:
    std::string         get_id        ( ) const;
    const std::string & get_name    ( void ) const { return _name;           }
    // std::string         get_tests_dir ( void ) const { return _pb_dir+TESTS_DIR; }
    int                 get_n         ( void ) const { return _n;                }
    int                 get_m         ( void ) const { return _m;                }
    const std::vector<std::string> & get_pbInstance( void) const { return _pbInstance; }
    const size_t get_nbPbInstances( void) const { return _nbPbInstances; }
    
//    const NOMAD_BASE::Point  & get_x0          ( void ) const { return _x0;               }
//    bool hasX0FeasInfo( void ) const { return _x0_feas_info ; }
//    bool x0Feas( void ) const { return _x0_is_feas ; }
//    const NOMAD_BASE::Point  & get_xe          ( void ) const { return _xe;               }
//    const NOMAD_BASE::Double & get_fxe         ( void ) const { return _fxe;              }
//    const NOMAD_BASE::Point  & get_lb          ( void ) const { return _lb;               }
//    const NOMAD_BASE::Point  & get_ub          ( void ) const { return _ub;               }
//
//    const NOMAD_BASE::BBInputTypeList & get_bbit ( void ) const { return _bbit; }
//
//    const NOMAD_BASE::BBOutputTypeList & get_bbot ( void ) const { return _bbot; }
//
//    const std::string & getBbotAsString ( void ) const { return _bbotS; }
//
//    bool has_bounds      ( void ) const { return _lb.isComplete() || _ub.isComplete(); }
//
//    bool has_constraints ( void ) const { return _has_constraints;   }
//
//    size_t getNbObj ( void ) const { return NOMAD_BASE::getNbObj(_bbot); }
//
//    bool has_keyword ( const std::string & kw );
//
//    bool is_batch ( void ) const { return _batch_eval; }
    
    int getMaxBBEvals() const;
    
    static void setOverallMaxBBEvals ( int overallMaxBBEvals ) { _overallMaxBBEvals = overallMaxBBEvals; }
    static void setNbSimplexEvals ( int nbSimplexEvals )  { _nbSimplexEvals = nbSimplexEvals; }
    
    static int getNbSimplexEvals ( void ) { return _nbSimplexEvals; }

private:
    
    bool set_id(const std::string & id) { if (id.empty()) return false; _id = id; return true;}
    
    bool setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att);
    
protected:
//    // Set bb executable name (batch mode if _batch_eval)
//    void set_bb_exe ( const std::string & s );
//
//    bool set_bbit ( int i , NOMAD_BASE::BBInputType t );
//
//    bool set_bbot ( int i , NOMAD_BASE::BBOutputType t );
//
//    bool set_trend_matrix ( int i , const NOMAD_BASE::Point & T);
//
//    void set_x0 ( const NOMAD_BASE::Point & x ) { _x0 = x; }
//
//    void set_x0_feas( bool feas ){ _x0_feas_info = true; _x0_is_feas = feas ; }
//
//    void read_x0 ( const std::string & input_file_name , NOMAD_BASE::Point & x0 );
//
//    void set_bounds ( const NOMAD_BASE::Point & lb , const NOMAD_BASE::Point & ub );
//
//    void add_keyword ( std::string s );
};

#include "runnerpost_nsend.hpp"

#endif


