#ifndef __RUNNER__
#define __RUNNER__

#include "Result.hpp"
#include "Problem.hpp"
#include "AlgoParameters.hpp"

#include <list>

/*--------------------------------------*/
/*           the Runner class           */
/*--------------------------------------*/
class Runner
{
    
private:

    std::vector<Problem *>         _selected_pbs;        // list of selected problems
    std::vector<AlgoParameters *>  _selected_algos;       // list of algo from test configs
    std::vector<std::string>       _selected_algo_legends; // list of legends for the selected
    
    size_t                         _n_pb;                // number of selected pbs
    size_t                         _n_algo;              // number of test configs
    size_t                         _n_seed_run;
    Result                     *** _results;      // results
    std::string                **  _test_id;      // the test names
    
    bool                           _use_avg_fx_first_feas;  // if true, use the average fx value of all the first feasible points instead of the max (default)
    bool                           _use_evals_for_dataprofiles; // if true, the data profiles use (n+1)*nbevals as x axis
    bool                           _use_h_for_profiles;  // if true, set the options for data/performance profiles on h
    
    double                         _feasibilityThreshold;  // if > 0 => changes feasibility detection for data/performance profiles
    
    
    bool                           _use_hypervolume_for_profiles;  // if true, set the options for data/performance profiles on hypervolume
//    std::vector<std::vector<NOMAD_BASE::Point>> _combinedParetoAllAlgos; // combined paretos for all runs for a given problem for all algos and seeds
//    std::vector<NOMAD_BASE::Point>              _refParetoIdealPtAllAlgos, _refParetoNadirPtAllAlgos ;   // Pareto reference point: The max for each objective (feasible points only) for a given problem for all algos and seeds.
    
    std::list<int>                 _algoRunSeeds; //The seeds for algo runs are stored as a list of int

    
    
    // Clear memory
    void clear_memory ( );
    
    // add a problem to the list of selected problems:
    void add_selected_problem ( Problem & pb );

    // add a test config to the list of test configs:
    void add_to_selected_algos ( const AlgoParameters & ap );

    // find a problem in the list of all problems:
    Problem * find_problem ( const std::string & problem_id ) const;
    
    // display a list of problems:
    void display_pbs ( const std::vector<Problem *> & pbs ) const;
    
    // display instance name:
    void display_instance_name (size_t i_pb, size_t i_algo, size_t seed = INF_SIZE_T) const;
    
    // set a result:
    void set_result ( const std::string             & test_id ,
                     Result                           result[],
                     Problem                        & pb      ,
                     const AlgoParameters           & ap        ) ;
    
//    // set a result:
//    void set_hypervolume_result ( ) ;
    
    // check if fx is at alpha % relatively close to fxe:
    static bool is_within ( const double & fx    ,
                           const double & fxe   ,
                           const double & alpha   );
    
    // compute relative error between fx and fxe:
    static double compute_alpha ( const double & fx  ,
                                        const double & fxe   );
    
    // get the results:
    bool get_results ( const std::string      & test_id ,
                      const Problem           & pb      ,
                      const AlgoParameters    & ap      ,
                      Result                    result[]);

//    // Write partial pareto.
//    // Partial: for all seeds run of a single algo
//    void writePartialCombinedPareto ( const std::string      & test_id ,
//                                     const Problem           & pb      ,
//                                     const AlgoParameters    & ap      ,
//                                     std::vector<NOMAD_BASE::Point> & partialCombinedPareto ) const ;
//
//    // Update combined pareto with a single point
//    bool addToCombinedPareto(const NOMAD_BASE::ArrayOfDouble & pt,
//                             const size_t & pbIndex );
//
//    // Update combined pareto with pareto points
//    bool addToCombinedPareto(const std::vector<NOMAD_BASE::Point> & paretoPts,
//                             const size_t & pbIndex );
    
//    // construct a list of sub-directories:
//    static bool construct_list_of_subdirs ( std::list<std::string> & list_of_dirs ,
//                                           const std::string      & directory      );
    
    // functions to access directory and file names:
    static std::string get_test_dir ( const std::string & test_id ,
                                     const Problem     & pb      )
    {
        return pb.get_tests_dir() + test_id +  "/";
    }
    
//    // functions to access directory and file names:
//    static std::string get_param_file_name ( const std::string & test_id ,
//                                            const Problem     & pb      ,
//                                            int                 seed    )
//    {
//        std::ostringstream os;
//        os << Runner::get_test_dir ( test_id , pb ) << PARAM_FILE ;
//
//        std::string paramFileName = os.str();
//
//        add_seed_to_file_name ( seed , paramFileName );
//
//        return paramFileName;
//
//    }
    
    static std::string get_stats_file_name ( const std::string & test_id ,
                                            const Problem      & pb      ,
                                            int                  seed    ,
                                            bool                 seed_sensitive,
                                            bool                 full_path )
    {
        std::ostringstream os;
        os << ( (full_path) ?  Runner::get_test_dir ( test_id , pb ):"") << STATS_FILE ;
        
        std::string statsFileName = os.str();
        
        if ( seed_sensitive )
            add_seed_to_file_name ( seed , statsFileName );
        
        return statsFileName;
    }
    
    static void add_seed_to_file_name ( int seed, std::string & file_name ) ;
    
    // access to the date:
    std::string get_date ( void ) const;
    
    ArrayOfDouble         get_fx0s() const;
    ArrayOfDouble         get_best_fx() const;
    ArrayOfDouble get_mean_algo_times(size_t i_bbe) const;
    ArrayOfDouble get_relative_algo_times(size_t i_bbe) const;
//    NOMAD_BASE::ArrayOfDouble get_mean_algo_bbes() const;
    size_t get_bbe_max() const;
    size_t get_bbe_max(size_t i_algo) const;
    size_t get_bbe_max(size_t i_pb, size_t i_algo) const;
    int get_dimPbMin() const;
    
    
public:
    
    // constructor:
    Runner ( );
    
    // destructor:
    virtual ~Runner ( void );
    
    // run:
    bool run ( std::string & error_msg );
    
    // display all problems:
    void display_all_problems ( void ) const;
    
    // display special options:
    void display_special_options ( void ) const;
    
    // display results:
    void display_algo_diff     ( void ) const;

    // Compute outputs for graphs 
    // tau >= 0
    bool output_perf_profile_plain(const double& tau, const std::string& pp_file_name) const;
    bool output_data_profile_plain(const double& tau, const std::string& dp_file_name) const;
     // Time in function of bbe (for parallel testing)
    bool output_time_profile_plain(const std::string& time_profile_file_name) const;
    // Data profile in function of time
    bool output_time_data_profile_plain(const double& tau, const std::string& tdp_file_name) const;
    
    void output_problems_unsolved(const double& tau, const double& nbSimplexEval) const;
    
    // Access to selected algo legends (used for data/perf profiles plot legends)
    const std::vector<std::string> & get_selected_algo_legends ( void ) const { return _selected_algo_legends ;}
    
    // Select the seeds for Nomad solvers
    // ----------------------------------
//    void clearAlgoRunSeeds ( void ) { _algoRunSeeds.clear() ; }
//    void appendAlgoRunSeed(int seed){ _algoRunSeeds.push_back(seed);}
    
    // select data/performance profile options:
    // ---------------------------------------
    void set_use_avg_fx_first_feas() { _use_avg_fx_first_feas = true; }
    
    
    // select data/performance profile using infeasibility h
    // --> adjust options:
    // 1 - stats contains all evaluated points
    // 2 - output value for each constraint
    // 3 - calculate infeasibility h
    // 4 - data profile using tau=0, 1E-7,1E-5,1E-3
    // -------------------------------------------------------------
//    void set_use_h_for_profiles () { _use_h_for_profiles = true;}
//
//    void set_use_hypervolume_for_profiles () { _use_hypervolume_for_profiles = true;}
//
//    void set_use_evals_for_dataprofiles () { _use_evals_for_dataprofiles = true; }
//    bool get_use_evals_for_dataprofiles () { return _use_evals_for_dataprofiles; }
//
//    // get flag for data/performance profile options:
//    bool get_use_h_for_profiles () const { return _use_h_for_profiles ;}
//    bool get_use_hypervolume_for_profiles () const { return _use_hypervolume_for_profiles ;}
    
    // Set feasibility detection threshold. Used when Result read function is called
    // ----------------
//    void setFeasibilityThreshold( const double & threshold) { _feasibilityThreshold = threshold; }
    
    // select problems:
    // ----------------
//    void select_all_problems         ( bool batch_eval=false );
//    bool select_problem_by_name      ( std::string name , bool batch_eval=false );
//    bool select_problems_by_keyword  ( std::string kw , bool batch_eval=false );
//    bool select_problems_by_size     ( int n_min , int n_max , bool batch_eval=false );
    
//    bool refine_problems_by_keyword  ( std::string kw   );
    
//    bool exclude_problem_by_name     ( std::string name );
//    bool exclude_problems_by_keyword ( std::string kw   );
//    bool exclude_problems_with_infeasible_x0 ();
//    bool exclude_problems_by_size    ( int n_min , int n_max );
//
    void clear_selected_problems     ( void );
    int  display_selected_problems   ( void ) const;
//
//    void clean_all_tests         ( void ) const;
//
//    void clean_selected_tests    ( void ) ;
    
    
    // test param files :
    // -----------------
//    bool read_algo_params_file    ( const std::string & algo_param_file_name ,
//                                   std::string       & error_msg          );
//
    void clear_selected_algos     ( void );
    void display_selected_algos   ( void ) const;
    
};

#endif
