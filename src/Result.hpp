#ifndef __RESULT__
#define __RESULT__

#include "runner_defines.hpp"

#include "Utils.hpp"

#include "runnerpost_nsbegin.hpp"


// Class to store the results from a stats file in a compact form.
// Keep only improving solution for f or h (only if interested in the infeasibility).
class Result
{
    
private:
        
    std::vector<size_t>             _bbe;
    size_t                          _totalBbe;
    std::vector<double>             _time;
    double                          _totalTime;
    std::vector<double>             _obj;
    
//    std::vector<NOMAD_BASE::EvalPoint> _mobj;
//    std::vector<size_t>                _nb_dominating_ref_obj;
    
   
    std::string                _last_x;
    
    bool                       _has_sol;
    bool                       _is_infeas;
    
    bool                       _use_hypervolume_for_obj;
    bool                       _use_h_for_obj;
    bool                       _use_std_h;
    
    size_t                     _nb_obj;  // Used for multi objective
    
    size_t                     _sol_bbe;
    double                     _sol_fx;
    double                     _sol_fxe;
//    size_t                     _nb_pareto_points;
//    ArrayOfDouble              _sol_xe;
    
    // clear solution:
    void clear_solution ( void );

//
//    // Helper to update pareto when a single objs point is provided, return success true if at least one point and false otherwise
//    bool update_pareto_single ( const NOMAD_BASE::EvalPoint & evalPoint,
//                                std::vector<NOMAD_BASE::Point> & combinedPareto ) const;
    
public:
    
    // constructor:
    Result ( bool use_hypervolume_for_obj =false, bool use_h_for_obj = false  ):
        _totalBbe(0),
        _totalTime(0),
        _use_hypervolume_for_obj(use_hypervolume_for_obj),
        _use_h_for_obj(use_h_for_obj),
        _use_std_h(true) // For now use_std_h is always true
    {
        if (_use_h_for_obj && _use_hypervolume_for_obj)
        {
            std::cout << "Result constructor: Cannot use both h AND hypervolume for obj" <<std::endl;
        }
        clear_solution();
    }
    
    // destructor:
    virtual ~Result ( void ) {}
    
    // reset stored bbe, obj, mobjs and set option to use for obj computation
    void reset ( bool use_hypervolume_for_obj , bool use_h_for_obj ); // Reset can change the default computation of obj set in constructor. For now _use_std_h is always true
    
    // compute solution:
    bool compute_solution ( int                   n     ,
                            size_t                   bbe );
    
//    // compute hypervolume for multi objective:
//    bool compute_hypervolume_solution ( int                   n     ,
//                                    size_t                   bbe ,
//                                    const std::vector<NOMAD_BASE::Point> & combinedPareto,
//                                    const NOMAD_BASE::Point              & refParetoIdealPt,
//                                    const NOMAD_BASE::Point              & refParetoNadirPt);

//    // Compute scaled hypervolume of a pareto front (multi obj pb)
//    static NOMAD_BASE::Double compute_hv (const std::vector<NOMAD_BASE::Point> & pareto,
//                                          const NOMAD_BASE::Point              & refParetoIdealPt,
//                                          const NOMAD_BASE::Point              & refParetoNadirPt,
//                                          size_t & nb_dominating_ref_obj) ;
    
//    // Compute a single f from the objectives fs. The single f values have monotonic decrease.
//    // The value is computed as the hypervolume between the pareto of the result and a ref combined pareto
//    bool compute_hypervolume_for_obj ( const size_t bbe         ,
//                                      const std::vector<NOMAD_BASE::Point> & combinedPareto,
//                                      const NOMAD_BASE::Point              & refParetoIdealPt,
//                                      const NOMAD_BASE::Point              & refParetoNadirPt) ;
    
//    bool update_pareto ( size_t bbe,
//                         std::vector<NOMAD_BASE::Point> & pareto ) const;
    
    // read results:
    bool read ( std::ifstream & in , size_t max_bbe , const StatOutputTypeList & sotList, const double & feasibilityThreshold );
    
    // GET methods:
    size_t                     get_last_bbe   ( void    ) const;
    size_t                     getTotalBbe() const { return _totalBbe; }
    bool                       has_solution   ( void    ) const { return _has_sol;    }
    bool                       is_infeas      ( void    ) const { return _is_infeas;  }
    size_t                     get_sol_bbe    ( void    ) const { return _sol_bbe;    }
    const double & get_sol_fx     ( void    ) const { return _sol_fx;     }
    const double & get_first_fx   ( void    ) const { return _obj[0];     }
    
    const double & get_sol_fxe    ( void    ) const { return _sol_fxe;    }
//    size_t                     get_nb_pareto_points   ( void    ) const { return _nb_pareto_points; }
//    size_t                     get_nb_dominating_ref_obj   ( void    ) const { return _nb_dominating_ref_obj.back(); }
//    const ArrayOfDouble  & get_sol_xe     ( void    ) const { return _sol_xe;     }
    double         get_sol        ( size_t bbe ) const;
    double         get_time       (const size_t bbe = INF_SIZE_T) const;
    double         get_sol_by_time(size_t time) const;
    double         getTotalTime() const { return _totalTime; }
    
    // display:
    void display ( ) const;
    
    // comparison operator:
    bool operator == ( const Result & r )
    {
        return _bbe == r._bbe && _obj == r._obj;
    }
};

#include "runnerpost_nsend.hpp"

#endif
