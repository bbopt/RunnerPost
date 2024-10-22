#ifndef __PROBLEM__
#define __PROBLEM__

#include "runner_defines.hpp"

#include <set>

class Problem {

private:
    
    int                                _index;
    std::string                        _id;
    std::string                        _pb_dir;
    std::string                        _xe_file_name;
    int                                _n;
    int                                _m;

//    std::string                        _bb_exe;
//    bool                               _batch_eval;
//    NOMAD_BASE::BBInputTypeList        _bbit;
//    NOMAD_BASE::BBOutputTypeList       _bbot;
//    std::string                        _bbotS;
//    bool                               _has_constraints;
//    bool                               _has_trend_matrix;
//    bool                               _has_integers;
//    bool                               _has_binaries;
//    NOMAD_BASE::Point                  _lb;
//    NOMAD_BASE::Point                  _ub;
//    NOMAD_BASE::Point                  _x0;
//    NOMAD_BASE::Point                  _xe;
//    NOMAD_BASE::Double                 _fxe;  // best known solution
//    bool                               _x0_feas_info;
//    bool                               _x0_is_feas;
    
    // The max_bb_eval termination criterion is min( _overallMaxBBEvals, (n+1)*_nbSimplexEvals
    static int                         _nbSimplexEvals; // if <=0 ==> other termination criterions will be used
    static int                         _overallMaxBBEvals; // if <= 0 ==> other termination criterions will be used
    
    std::set<std::string>              _keywords;
    static std::set<std::string>       _all_keywords;
    
    // display keywords:
    static void display_keywords ( const std::set<std::string> & keywords );
    
public:
    
    // constructor #1
    Problem (const std::string & pb_dir       );
    
    // constructor #2:
    Problem ( const std::string & id          ,
             const std::string & pb_dir       ,
             const std::string & xe_file_name ,
             int                 n            ,
             int                 m            );
    

    
    // destructor:
    virtual ~Problem ( void ) {}

    void set_pb_id ( std::string id) { _id = id ;}
    void set_n ( int n ) { _n = n; }
    void set_m ( int m ) { _m = m; }
    void set_xe_file_name( std::string f){ _xe_file_name = f ;}
    
    
    void display ( void ) const;
    
    void display_keywords ( ) const
    {
        Problem::display_keywords ( _keywords );
    }
    
    static void display_all_keywords ( )
    {
        Problem::display_keywords ( Problem::_all_keywords );
    }
    
    // GET methods:
    std::string         get_id        ( bool latex = false ) const;
    int                 get_index     ( void ) const { return _index;            }
    const std::string & get_pb_dir    ( void ) const { return _pb_dir;           }
    std::string         get_tests_dir ( void ) const { return _pb_dir+TESTS_DIR; }
    int                 get_n         ( void ) const { return _n;                }
    int                 get_m         ( void ) const { return _m;                }
    
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

    // SET methods:
    void set_index ( int i ) { _index = i; }
    
    static void setOverallMaxBBEvals ( int overallMaxBBEvals ) { _overallMaxBBEvals = overallMaxBBEvals; }
    static void setNbSimplexEvals ( int nbSimplexEvals )  { _nbSimplexEvals = nbSimplexEvals; }
    
    static int getNbSimplexEvals ( void ) { return _nbSimplexEvals; }

    
    
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

#endif


