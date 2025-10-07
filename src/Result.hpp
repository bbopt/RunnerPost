#ifndef __RESULT__
#define __RESULT__

#include "runner_defines.hpp"

#include "Utils.hpp"

#include "runnerpost_nsbegin.hpp"

// Comparison type for Multi-Objective
enum class MOCompareType
{
    EQUAL,              ///< Both points are feasible or infeasible, and their
                        ///< objective values and h (where h is the squared sum
                        ///< of violations of all constraints) are equal to
                        ///< approximation tolerance rounding.
    INDIFFERENT,        ///< Both point are non dominated relatively to each other.
    DOMINATED,          ///< The first point is dominated by the other.
    DOMINATING         ///< The first point dominates the other.
};

// Class to store the results from a stats file in a compact form.
// Keep only improving solution for f or h.
class Result
{
    
private:
        
    std::vector<size_t>             _bbe;
    size_t                          _totalBbe;
    std::vector<double>             _time;
    double                          _totalTime;
    std::vector<double>             _obj;
    
    // These are empty when a solution (feas) exists.
    std::vector<size_t>             _bbeForH;
    std::vector<double>             _timeForH;
    std::vector<double>             _infH; // Values >=0
    
    
    std::vector<std::vector<double>>   _mobj;
    std::vector<size_t>                _nb_dominating_ref_obj;
    
   
    std::string                _last_x;
    
    bool                       _has_feas_sol; // Tag to indicate feasibility of result. Set in compute_solution.
    
    // Option to compute f (hypervolume) and h
    // bool                       _use_hypervolume_for_obj;
    bool                       _use_std_h;
    
    size_t                     _nb_obj;  // Used for multi objective
    
    // A solution is feasible
    size_t                     _sol_bbe;
    double                     _sol_fx; // Not used. For now, let's keep it and update its value (compute_solution).
    size_t                     _nb_pareto_points;
    ArrayOfDouble              _sol_xe; // Not used. For now, let's keep it and update its value (compute_solution).
    
    // clear solution (feas):
    void clear_solution ( void );
    
    // clear best infeas:
    void clear_best_infeas ( void );


    // Helpers to update pareto when a single objs point is provided, return success true if at least one point and false otherwise
    //
    bool update_pareto_single ( const std::vector<double> & point,
                                std::vector<std::vector<double>> & combinedPareto,
                                bool sort) const;
    
public:
    
    // constructor:
    Result ( bool use_hypervolume_for_obj =false ):
        _totalBbe(0),
        _totalTime(0),
        // _use_hypervolume_for_obj(use_hypervolume_for_obj),
        _use_std_h(true) // For now use_std_h is always true
    {
        clear_solution();
    }
    
    // destructor:
    virtual ~Result ( void ) {}
    
    // reset stored bbe, obj, mobjs and set option to use for h computation
    void reset (  ); // Reset can change the default computation of obj set in constructor. For now _use_std_h is always true
    
    // compute solution:
    bool compute_solution ( int                   n     ,
                            size_t                bbe );
    bool compute_best_infeasible ( int            n ,
                                   size_t         bbe);
    
    // compute hypervolume for multi objective:
    bool compute_hypervolume_solution ( int                   n     ,
                                    size_t                   bbe ,
                                    const std::vector<std::vector<double>> & combinedPareto,
                                    const std::vector<double>              & refParetoIdealPt,
                                    const std::vector<double>              & refParetoNadirPt);

    // Compute scaled hypervolume of a pareto front (multi obj pb)
    static double compute_hv (const std::vector<std::vector<double>> & pareto,
                                          const std::vector<double>              & refParetoIdealPt,
                                          const std::vector<double>              & refParetoNadirPt,
                                          size_t & nb_dominating_ref_obj) ;
    
    // Compute a single f from the objectives fs. The single f values have monotonic decrease.
    // The value is computed as the hypervolume between the pareto of the result and a ref combined pareto
    bool compute_hypervolume_for_obj ( const size_t bbe         ,
                                      const std::vector<std::vector<double>> & combinedPareto,
                                      const std::vector<double>              & refParetoIdealPt,
                                      const std::vector<double>              & refParetoNadirPt) ;
    
    bool update_pareto ( size_t bbeMax,
                         std::vector<std::vector<double>> & pareto ) const;
    

    static MOCompareType compMultiObjForDominate(const std::vector<double> & point1,
                                                 const std::vector<double> & point2) ;
    
    // read results:
    bool read ( std::ifstream & in , size_t max_bbe , const StatOutputTypeList & sotList, const double & feasibilityThreshold );
    
    // GET methods:
    size_t                     get_last_bbe   ( void    ) const;
    size_t                     get_last_time   ( void    ) const;
    size_t                     getTotalBbe() const { return _totalBbe; }
    bool                       has_feas_solution   ( bool isForH    ) const { return (isForH)? (_infH.size()> 0 && _infH.back()<INF):_has_feas_sol;    }
    double                     get_first_feas_fx() const ;
    
    size_t                     get_sol_bbe    ( void    ) const { return _sol_bbe;    }
    
//    // CHT TEMP FOR SOME EXPERIMENTAL DATA MANIPULATION -- DO NOT KEEP
//     void TMPtransform() ;
    
    void writeToStatsFile(size_t i_pb, size_t i_algo, size_t i_pb_inst, size_t pb_size) const;
    
    
    // Getter for the improving objs and the corresponding bbe.
    const std::vector<size_t> & get_bbes ( bool forH ) const { return (forH)? _bbeForH:_bbe; }
    const std::vector<double> & get_sols ( bool forH ) const { return (forH)? _infH:_obj; }
    
    // Getter for multi-objs
    size_t get_nbNbObjs( void ) const { if (_mobj.empty()) return 0 ; return _mobj[0].size(); }
    
    size_t                     get_nb_pareto_points   ( void    ) const { return _nb_pareto_points; }
    size_t                     get_nb_dominating_ref_obj   ( void    ) const { return _nb_dominating_ref_obj.back(); }
    
    double         get_sol        ( const size_t & bbe, bool forH ) const;
    double         get_time       (const size_t &bbe, bool forH) const;
    double         get_sol_by_time(const double& time, bool forH) const;
    double         getTotalTime() const { return _totalTime; }
    
    // display:
    void display ( ) const;
    
    // comparison operator:
    bool operator == ( const Result & r ) const
    {
        return _bbe == r._bbe && _obj == r._obj;
    }
    
    // comparison operator:
    bool operator != ( const Result & r ) const
    {
        return ! operator==(r);
    }
    
    
};

#include "runnerpost_nsend.hpp"

#endif
