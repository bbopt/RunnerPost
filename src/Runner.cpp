#include "Runner.hpp"

#include <ctime>
#include <fstream>

#include <filesystem>

/*----------------------------------*/
/*            constructor           */
/*----------------------------------*/
RUNNERPOST::Runner::Runner ( ) :
_results    ( NULL ) ,
_test_id    ( NULL ) ,
// _use_avg_fx_first_feas( false ) ,
_use_evals_for_dataprofiles ( false ) ,
_use_hypervolume_for_profiles ( false ),
_feasibilityThreshold (0.0)
{

}

/*----------------------------------*/
/*             destructor           */
/*----------------------------------*/
RUNNERPOST::Runner::~Runner ( void )
{

    clear_memory();

    size_t n = _selected_algos.size();
    for ( size_t k = 0 ; k < n ; ++k )
        delete _selected_algos[k];
    _selected_algos.clear();
    
    n = _selected_pbs.size();
    for ( size_t k = 0 ; k < n ; ++k )
        delete _selected_pbs[k];
    _selected_pbs.clear();
    
    n = _selected_outputs.size();
    for ( size_t k = 0 ; k < n ; ++k )
        delete _selected_outputs[k];
    _selected_outputs.clear();
    
    

}

/*-------------------------------------*/
/*        clear memory (private)       */
/*-------------------------------------*/
void RUNNERPOST::Runner::clear_memory ( )
{

    size_t i_pb, i_algo;

    for ( i_pb = 0 ; i_pb < _selected_pbs.size() ; ++i_pb )
    {
        if ( _results && _results[i_pb] )
        {
            for ( i_algo = 0 ; i_algo < _selected_algos.size() ; ++i_algo )
                if ( _results[i_pb][i_algo] )
                    delete [] _results[i_pb][i_algo];
            delete [] _results [i_pb];
        }
        if ( _test_id && _test_id[i_pb] )
            delete [] _test_id[i_pb];
    }
    if ( _results )
        delete [] _results;
    if ( _test_id )
        delete [] _test_id;

    _results = NULL;
    _test_id = NULL;
}


///*-----------------------------------*/
///*         select all problems       */
///*-----------------------------------*/
//void Runner::select_all_problems ( bool batch_eval )
//{
//    clear_selected_problems();
//    size_t n = _all_pbs.size();
//    for ( size_t k = 0 ; k < n ; ++k )
//    {
//        if ( batch_eval )
//        {
//            std::string bb_exe_name = _all_pbs[k]->get_bb_exe();
//            if ( ! bb_exe_name.empty() )
//                _all_pbs[k]->set_batch_eval(true);
//        }
//        add_selected_problem ( *_all_pbs[k] );
//    }
//}
//
///*------------------------------------*/
///*  select one problem with its name  */
///*------------------------------------*/
//bool Runner::select_problem_by_name ( std::string name , bool batch_eval )
//{
//    NOMAD_BASE::toupper ( name );
//    size_t n = _all_pbs.size();
//    for ( size_t k = 0 ; k < n ; ++k )
//    {
//        std::string id = _all_pbs[k]->get_id();
//        NOMAD_BASE::toupper(id);
//        if ( id == name )
//        {
//            if ( batch_eval )
//            {
//                if ( ! _all_pbs[k]->get_bb_exe().empty() )
//                {
//                    _all_pbs[k]->set_batch_eval(true);
//                }
//                else
//                {
//                    return false;
//                }
//            }
//            add_selected_problem ( *_all_pbs[k] );
//            return true;
//        }
//    }
//    return false;
//}

///*-----------------------------------*/
///*      exclude problem by name      */
///*-----------------------------------*/
//bool Runner::exclude_problem_by_name ( std::string name )
//{
//
//    NOMAD_BASE::toupper ( name );
//
//    std::list<Problem *> tmp;
//    size_t np = _selected_pbs.size();
//
//    if ( np == 0 )
//        std::cerr << "Warning: Problems must be selected first before proceeding to exclusion. Let us try anyway" << std::endl;
//
//    for ( size_t k = 0 ; k < np ; ++k )
//    {
//        std::string id = _selected_pbs[k]->get_id();
//        NOMAD_BASE::toupper(id);
//        if ( id.compare(name) !=0 )
//            tmp.push_back ( _selected_pbs[k] );
//    }
//
//    clear_selected_problems();
//
//    std::list<Problem *>::const_iterator it , end = tmp.end();
//
//    for ( it = tmp.begin() ; it != end ; ++it )
//        add_selected_problem ( **it );
//
//    return !tmp.empty();
//}

///*----------------------------------*/
///*     select problems by size      */
///*----------------------------------*/
//bool Runner::select_problems_by_size ( int n_min , int n_max , bool bb_eval )
//{
//
//    int    n;
//    bool   chk = false;
//    size_t np  = _all_pbs.size();
//
//    for ( size_t k = 0 ; k < np ; ++k )
//    {
//        n = _all_pbs[k]->get_n();
//        if ( n >= n_min && n <= n_max )
//        {
//            if ( bb_eval )
//            {
//                if ( ! _all_pbs[k]->get_bb_exe().empty() )
//                    _all_pbs[k]->set_batch_eval(true);
//                else
//                    return false;
//            }
//            add_selected_problem ( *_all_pbs[k] );
//            chk = true;
//        }
//    }
//
//    return chk;
//}
//
///*-----------------------------------*/
///*     exclude problems by size      */
///*-----------------------------------*/
//bool Runner::exclude_problems_by_size ( int n_min , int n_max )
//{
//
//    std::list<Problem *> tmp;
//
//    int    n;
//    size_t np = _selected_pbs.size();
//
//    if ( np == 0 )
//        std::cerr << "Warning: Problems must be selected first before proceeding to exclusion. Let us try anyway" << std::endl;
//
//    for ( size_t k = 0 ; k < np ; ++k )
//    {
//        n = _selected_pbs[k]->get_n();
//        if ( n < n_min || n > n_max )
//            tmp.push_back ( _selected_pbs[k] );
//    }
//
//    clear_selected_problems();
//
//    std::list<Problem *>::const_iterator it , end = tmp.end();
//
//    for ( it = tmp.begin() ; it != end ; ++it )
//        add_selected_problem ( **it );
//
//    return !tmp.empty();
//}
//
///*----------------------------------*/
///*  select problems from a keyword  */
///*----------------------------------*/
//bool Runner::select_problems_by_keyword ( std::string kw , bool batch_eval )
//{
//    NOMAD_BASE::toupper ( kw );
//    bool chk = false;
//    size_t n = _all_pbs.size();
//    for ( size_t k = 0 ; k < n ; ++k )
//        if ( _all_pbs[k]->has_keyword ( kw ) )
//        {
//            if ( batch_eval )
//            {
//                if ( ! _all_pbs[k]->get_bb_exe().empty() )
//                    _all_pbs[k]->set_batch_eval(true);
//                else
//                    return false;
//            }
//            add_selected_problem ( *_all_pbs[k] );
//            chk = true;
//        }
//    return chk;
//}
//
///*----------------------------------*/
///*  refine problems from a keyword  */
///*----------------------------------*/
//bool Runner::refine_problems_by_keyword ( std::string kw )
//{
//
//    NOMAD_BASE::toupper ( kw );
//
//    std::list<Problem *> tmp;
//
//    size_t n = _selected_pbs.size();
//    for ( size_t k = 0 ; k < n ; ++k )
//        if ( _selected_pbs[k]->has_keyword ( kw ) )
//            tmp.push_back ( _selected_pbs[k] );
//
//    clear_selected_problems();
//
//    std::list<Problem *>::const_iterator it , end = tmp.end();
//
//    for ( it = tmp.begin() ; it != end ; ++it )
//        add_selected_problem ( **it );
//
//    return !tmp.empty();
//}
//
///*-----------------------------------*/
///*  exclude problems from a keyword  */
///*-----------------------------------*/
//bool Runner::exclude_problems_by_keyword ( std::string kw )
//{
//
//    NOMAD_BASE::toupper ( kw );
//
//    std::list<Problem *> tmp;
//
//    size_t np = _selected_pbs.size();
//
//    if ( np == 0 )
//        std::cerr << "Warning: Problems must be selected first before proceeding to exclusion. Let us try anyway" << std::endl;
//
//    for ( size_t k = 0 ; k < np ; ++k )
//        if ( !_selected_pbs[k]->has_keyword ( kw ) )
//            tmp.push_back ( _selected_pbs[k] );
//
//    clear_selected_problems();
//
//    std::list<Problem *>::const_iterator it , end = tmp.end();
//
//    for ( it = tmp.begin() ; it != end ; ++it )
//        add_selected_problem ( **it );
//
//    return !tmp.empty();
//}
//
///*-----------------------------------------------*/
///*  exclude problems with infeasible initial x0  */
///*------------------------------------------------*/
//bool Runner::exclude_problems_with_infeasible_x0 ( )
//{
//
//    std::list<Problem *> tmp;
//
//    size_t np = _selected_pbs.size();
//
//    if ( np == 0 )
//        std::cerr << "Warning: Problems must be selected first before proceeding to exclusion. Let us try anyway" << std::endl;
//
//    for ( size_t k = 0 ; k < np ; ++k )
//    {
//        // For batch we can provide if X0 is feasible
//        if (_selected_pbs[k]->hasX0FeasInfo())
//        {
//            // We must rely on provided info
//            if (_selected_pbs[k]->x0Feas() )
//            {
//                tmp.push_back ( _selected_pbs[k] );
//            }
//        }
//        else
//        {
//            NOMAD_BASE::EvalPoint x0(_selected_pbs[k]->get_x0());
//            bool countEval;
//            _selected_pbs[k]->eval_x(x0, NOMAD_BASE::Double(NOMAD_BASE::INF), countEval);
//            if (x0.isFeasible(NOMAD_BASE::defaultFHComputeType))
//            {
//                tmp.push_back ( _selected_pbs[k] );
//            }
//        }
//    }
//    clear_selected_problems();
//
//    std::list<Problem *>::const_iterator it , end = tmp.end();
//
//    for ( it = tmp.begin() ; it != end ; ++it )
//        add_selected_problem ( **it );
//
//    return !tmp.empty();
//}
//
//
///*--------------------------------------------------*/
///*  add a problem to the list of selected problems  */
///*  (private)                                       */
///*--------------------------------------------------*/
//void Runner::add_selected_problem ( Problem & pb )
//{
//    if ( pb.get_index() < 0 )
//    {
//        pb.set_index ( static_cast<int>(_n_pb++) );
//        _selected_pbs.push_back ( &pb );
//    }
//}
//
///*---------------------------------------*/
///*  clear the list of selected problems  */
///*---------------------------------------*/
//void Runner::clear_selected_problems ( void )
//{
//    size_t n = _selected_pbs.size();
//    for ( size_t k = 0 ; k < n ; ++k )
//        _selected_pbs[k]->set_index ( -1 );
//    _selected_pbs.clear();
//    _n_pb = 0;
//}

/*------------------------------------------*/
/*  clear the list of selected algo params  */
/*------------------------------------------*/
void RUNNERPOST::Runner::clear_selected_algos ( void )
{
    const size_t n = _selected_algos.size();
    for ( size_t k = 0 ; k < n ; ++k )
        delete _selected_algos[k];

    _selected_algos.clear();

}

/*---------------------------------------------------*/
/*   Post processing of optimization runs on pbs     */
/*---------------------------------------------------*/
bool RUNNERPOST::Runner::run_post_processing ( std::string & error_msg )
{
    
    error_msg.clear();
    
    size_t       i_pb;
    size_t       i_algo;
    
    const size_t n_algo = static_cast<int>(_selected_algos.size());
    if ( n_algo == 0 )
    {
        error_msg = "no test configurations";
        return false;
    }
    
    const size_t n_pb = static_cast<int>(_selected_pbs.size());
    if ( n_pb == 0 ) {
        error_msg = "no selected problem";
        return false;
    }
    
    size_t nn = n_pb * n_algo;
    
    std::ostringstream msg;
    msg << "algos results on pb";
    if ( nn > 1 )
    {
        msg << "s (" << nn << ")";
    }
    
//    if ( _use_h_for_profiles )
//    {
//        msg << " [ h(x)=sum_j ( max(c_j(x),0)^2) -- > replaces f ]" ;
//    }
//    
//    if ( _use_hypervolume_for_profiles )
//    {
//        msg << " [ Pareto hypervolume is used for f ]" ;
//    }
    
    msg << ": \n";
    std::cout << msg.str();
    
    std::list<size_t> run_list;
    
    _results = new Result     ** [n_pb];
    _test_id = new std::string * [n_pb];
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        _results [i_pb] = nullptr;
        _test_id [i_pb] = nullptr;
    }
    
    // loop on the problems:
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        
        _results [i_pb] = new Result    * [n_algo];
        _test_id [i_pb] = new std::string [n_algo];
        
        auto n_pb_inst = _selected_pbs[i_pb]->get_nbPbInstances();
        
        // loop on the algorithm parameters:
        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            
            // result:
            _results [i_pb][i_algo] = new Result [ n_pb_inst ];
            
            for (size_t i_pb_inst = 0 ; i_pb_inst < n_pb_inst ; i_pb_inst++)
            {
                _results[i_pb][i_algo][i_pb_inst].reset( );
            }

            if ( !RUNNERPOST::Runner::get_results ( _test_id              [i_pb][i_algo]  ,
                                                   *_selected_pbs         [i_pb]          ,
                                                   *_selected_algos       [i_algo]        ,
                                                   _results               [i_pb][i_algo]   ) )
            {
                display_instance_name ( *_selected_pbs[i_pb] , *_selected_algos[i_algo] );
                
                // TODO find a way to display which instance is missing
                std::cout << ": cannot get result" << std::endl << std::endl;
                error_msg = "cannot get result";
                return false;
            }
            
//            // CHT TEMP for data manipulation (PAPER) --- DANGEROUS ---- DO NOT KEEP-
//             if (i_algo == 0)
            {
//                for (size_t i_pb_inst = 0 ; i_pb_inst < n_pb_inst ; i_pb_inst++)
//                {
//                    _results[i_pb][i_algo][i_pb_inst].TMPtransform();
//                }
//                for (size_t i_pb_inst = 0 ; i_pb_inst < n_pb_inst ; i_pb_inst++)
//                {
//                    _results[i_pb][i_algo][i_pb_inst].writeToStatsFile(i_pb,i_algo,i_pb_inst,_selected_pbs         [i_pb]->get_n() );
//                }
                
            }
            
            
            // set the result:
            set_result ( _test_id              [i_pb][i_algo] ,
                        _results               [i_pb][i_algo] ,
                        *_selected_pbs         [i_pb  ]       ,
                        *_selected_algos       [i_algo]         );
        }
    }
    
    
    // 4- Set the combined pareto results
    bool multiObj = (_selected_algos[0]->getNbObjectives() > 1);
    for (size_t i_algo = 1 ; i_algo < n_algo ; ++i_algo )
    {
        if ( multiObj != (_selected_algos[i_algo]->getNbObjectives() > 1))
        {
            std::cerr << "Inconsistent number of objectives in algo definitiion." << std::endl;
            return false;
        }
    }
    if ( multiObj )
    {
        std::cout << "Use hyper volume for profiling." << std::endl;
        _use_hypervolume_for_profiles = true;
        set_hypervolume_result();
    }
    
    std::cout << std::endl;
    
    return true;
}

/*---------------------------------------------------*/
/*   Post processing of optimization runs on pbs     */
/*---------------------------------------------------*/
bool RUNNERPOST::Runner::generate_outputs(std::string &error_msg)
{
    
    error_msg.clear();
    
    bool success = true;
    
    size_t n_output = static_cast<int>(_selected_outputs.size());
    if ( n_output == 0 )
    {
        error_msg = "no output configurations";
        return false;
    }
    
    std::ostringstream msg;
    msg << "output";
    if ( n_output > 1 )
    {
        msg << "s (" << n_output << ")";
    }
    
    if ( _use_h_for_profiles )
    {
        msg << " [ h(x)=sum_j ( max(c_j(x),0)^2) -- > replaces f ]" ;
    }
    
    if ( _use_hypervolume_for_profiles )
    {
        msg << " [ Pareto hypervolume is used for f ]" ;
    }
    
    msg << ": \n";
    std::cout << msg.str();

    // loop on outputs:
    for ( const auto & out: _selected_outputs )
    {
        const auto& pt = out->get_profile_type();
        
        if (!success)
        {
            error_msg = "Error in generating output.";
            return false;
        }
        
        if (Output::Profile_Type::DATA_PROFILE == pt)
        {
            const auto& xSel = out->get_x_select();
            if (Output::X_Select::TIME == xSel)
            {
                success = output_time_data_profile_plain(*out);
            }
            else
            {
                success = output_data_profile_plain(*out);
            }
            if (success)
            {
                success = output_profile_pgfplots(*out);
            }
        }
        else if (Output::Profile_Type::ACCURACY_PROFILE == pt)
        {
            success = output_accuracy_profile_plain(*out);
            if (success)
            {
                success = output_profile_pgfplots(*out);
            }
        }
        else if (Output::Profile_Type::PERFORMANCE_PROFILE == pt)
        {
            success = output_perf_profile_plain(*out);
            if (success)
            {
                success = output_profile_pgfplots(*out);
            }
        }
        else if (Output::Profile_Type::CONVERGENCE_PROFILE == pt)
        {
            success = output_convergence_profile_plain(*out);
            if (success)
            {
                success = output_profile_pgfplots(*out);
            }
        }
        else
        {
            error_msg = "Profile type not handled.";
            return false;
        }
        
    }
    std::cout << std::endl;
    
    return true;
}


///*-----------------------------------------------------*/
///*  check if fx is at alpha % relatively close to fxe  */
///*  (static, private)                                  */
///*-----------------------------------------------------*/
//bool RUNNERPOST::Runner::is_within ( const double & fx    ,
//                        const double & fxe   ,
//                        const double & alpha   )
//{
//    if ( fxe != 0.0 )
//        return ( std::fabs( (fx - fxe) / fxe ) <= alpha / 100.0 );
//
//    return ( std::fabs(fx) <= alpha / 100.0 );
//}
//
///*---------------------------------------------*/
///*  compute relative error between fx and fxe  */
///*  (static, private)                          */
///*---------------------------------------------*/
//double RUNNERPOST::Runner::compute_alpha ( const double & fx  ,
//                                          const double & fxe   )
//{
//    if ( fxe != 0.0 )
//        return std::fabs(100.0 * ( (fx - fxe) / fxe ));
//    return std::fabs(fx) * 100.0;
//}

/*---------------------------------------*/
/*      display algorithm differences    */
/*---------------------------------------*/
void RUNNERPOST::Runner::display_algo_diff ( void ) const
{

    const size_t n_algo = _selected_algos.size();
    const size_t n_pb = _selected_pbs.size();
    if ( n_algo == 1 )
    {
        std::cout << "algorithm differences: not displayed for one algorithm"
        << std::endl << std::endl;
        return;
    }

    std::cout <<  "diplay algorithm differences ....." << std::endl;

    size_t i, j, k, cnt;

    for ( i = 0 ; i < n_algo-1 ; ++i )
        for ( j = i+1 ; j < n_algo ; ++j )
        {
            std::cout << "\t [algo #";
            std::cout << ", algo #";
            std::cout << "] ";
            cnt = 0;
            for ( k = 0 ; k < n_pb ; ++k )
                if ( _results[k][i] && _results[k][j] &&
                    *_results[k][i] == *_results[k][j] )
                    ++cnt;
            if ( cnt == n_pb )
                std::cout << "identical results";
            else if ( cnt == 0 )
                std::cout << "differences for all problems";
            else {
                std::ostringstream msg;
                msg << "differences on " << n_pb - cnt << " problem";
                if ( n_pb - cnt > 1 )
                    msg << "s";
                msg << " out of " << n_pb;
                std::cout << msg.str() ;
                for ( k = 0 ; k < n_pb ; ++k )
                    if ( _results[k][i] && _results[k][j] &&
                        *_results[k][i] != *_results[k][j] )
                    {
                        std::cout << "pb #";
                        // TODO std::cout.display_int_w ( k , _n_pb );
                        std::cout << " ["
                        << _selected_pbs[k]->get_id() << "]"
                        << std::endl;
                    }
                // std::cout.close_block();
            }
            std::cout << std::endl;
        }
    std::cout << std::endl;
}

/*---------------------------------------*/
/*       display performance profiles    */
/*---------------------------------------*/
bool RUNNERPOST::Runner::output_perf_profile_plain ( const Output & out ) const
{
    
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    if ( out.get_tau() < 0 || n_pb == 0 || n_algo == 0 )
    {
        std::cerr << "Error: cannot compute performance profile for tau < 0 or n_pb ==0 or n_algo == 0" << std::endl;
        return false;
    }

    std::ofstream fout ( out.get_plain_file_name() );
    fout << std::setprecision(10);
    if ( fout.fail() )
    {
        std::cerr << "Warning: cannot create performance profile (MW) output file "
        << out.get_plain_file_name() << std::endl;
        return false;
    }

    std::cout << "\t writing of " << out.get_plain_file_name() << " ..." << std::flush;

    size_t i_pb, i_algo, i_pb_instance;

    // check that best solution and all results are available:
    std::list<size_t> miss_list;
    std::list<size_t> infeas_list;
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {

                if ( !_results[i_pb][i_algo][i_pb_instance].has_solution()  )
                {
                    miss_list.push_back ( i_pb   );
                    miss_list.push_back ( i_algo );
                    miss_list.push_back ( i_pb_instance );

                    // An infeasible run has no solution -> special flag in miss_list is set
                    if ( _results[i_pb][i_algo][i_pb_instance].is_infeas() )
                        miss_list.push_back( 1 );
                    else
                        miss_list.push_back( 0 );
                }
            }
        }
    }

    if ( !miss_list.empty() )
    {
        std::cout << "... the following results are missing" << std::endl;
        std::list<size_t>::const_iterator it , end = miss_list.end();
        bool need_for_fix = false;
        for ( it = miss_list.begin() ; it != end ; ++it )
        {
            i_pb = *it;
            ++it;
            i_algo = *it;
            display_instance_name ( *_selected_pbs[i_pb] , *_selected_algos[i_algo]);
            ++it;
            i_pb_instance = *it;
            std::cout << " seed run#" << i_pb_instance ;
            ++it;
            if ( *it==1 )
                std::cout << " --> no feasible point found " <<std::endl;
            else
            {
                std::cout << " --> run failed, fix it to get data profile " <<std::endl;
                need_for_fix = true;
            }
        }
        std::cout << std::endl;

        if ( need_for_fix )
        {
            fout << "fail(2)" << std::endl;
            fout.close();
            return false;
        }

        fout.close();

    }

    // Get fx0s for all problems
    const ArrayOfDouble& fx0s = get_fx0s(out.get_FFFeasMeth());
    
    // Failsafe for Fx0. A single run without valid x0 and fx0s is empty
    if ( fx0s.empty())
    {
        std::cerr << "Error: Undefined fx0. Failed to obtain data profile" << std::endl;
        fout.close();
        return false;
    }

    // get the best solution for each problem:
    const ArrayOfDouble& fxe = get_best_fx();


    // compute tpsMin (Moré and Wild  2009, eq. 2.1)
    // -------------------------
    std::vector<size_t> tpsMin;
    size_t tpsMinTmp;
    std::vector<size_t> algoHasSolvedFirst(n_pb,0);
    const auto & tau = out.get_tau();
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        tpsMinTmp = RUNNERPOST::INF_SIZE_T;
        if ( fx0s[i_pb] < INF )
        {
            for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
            {
                for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
                {
                    if ( _results[i_pb][i_algo][i_pb_instance].has_solution() )
                    {
                        
                        // Get the improving objs and the corresponding bbes
                        const auto & objs = _results[i_pb][i_algo][i_pb_instance].get_objs();
                        const auto & bbes = _results[i_pb][i_algo][i_pb_instance].get_bbes();
                        
                        for ( size_t i = 0 ; i < objs.size() ; i++ )
                        {
                            if (objs[i] <= fxe[i_pb] + tau *(fx0s[i_pb]-fxe[i_pb]) )
                            {
                                if (bbes[i]<tpsMinTmp)
                                {
                                    tpsMinTmp=bbes[i];
                                    algoHasSolvedFirst[i_pb] = i_algo;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
        tpsMin.push_back(tpsMinTmp);
    }
    
//    // TMP for testing. Display the number of problems solved first by each algorithm
//    std::cout << "Number of problems solved first by each algorithm: " << std::endl;
//    for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
//    {
//        // Get the number of problems solved first by each algorithm
//        size_t nb_pb_solved_first = std::count(algoHasSolvedFirst.begin(), algoHasSolvedFirst.end(), i_algo);
//        std::cout << "Algorithm " << i_algo << " solved " << double(nb_pb_solved_first)/n_pb << " % problems first." << std::endl;
//    }
    
    
    if (tpsMin.size() != n_pb)
    {
        std::cerr << "Error: tpsMin not computed for all problems" << std::endl;
        fout.close();
        return false;
    }
    
    
    // Compute list of alphas
    // -------------------------
    std::vector<double> alphas;
    for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
    {
        for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb)
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                if ( _results[i_pb][i_algo][i_pb_instance].has_solution() )
                {
                    // Get the improving objs and the corresponding bbes
                    const auto & objs = _results[i_pb][i_algo][i_pb_instance].get_objs();
                    const auto & bbes = _results[i_pb][i_algo][i_pb_instance].get_bbes();
                    
                    for ( size_t i = 0 ; i < objs.size() ; i++ )
                    {
                        if (objs[i] <= fxe[i_pb] + tau *(fx0s[i_pb]-fxe[i_pb]) )
                        {
                            alphas.push_back(double(bbes[i])/tpsMin[i_pb]);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    /// Sort alphas in ascending order
    /// -------------------------
    std::sort(alphas.begin(), alphas.end());
    
    
    // Remove the duplicates
    /// -------------------------
    auto last = std::unique(alphas.begin(), alphas.end());

    // Erase the remaining elements after the unique elements
    alphas.erase(last, alphas.end());
    
    
    size_t cnt;
    for (const auto& alpha: alphas)
    {
        fout << alpha;
        for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
        {
            cnt = 0;
            int cnt_pb_instance = 0;
            for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
            {
                auto n_pb_instance = _selected_pbs[i_pb]->get_nbPbInstances();
                cnt_pb_instance += n_pb_instance;
                for ( i_pb_instance = 0 ; i_pb_instance < n_pb_instance; ++i_pb_instance )
                    if ( _results[i_pb][i_algo][i_pb_instance].has_solution() )
                    {
                        // Get the improving objs and the corresponding bbes
                        const auto & objs = _results[i_pb][i_algo][i_pb_instance].get_objs();
                        const auto & bbes = _results[i_pb][i_algo][i_pb_instance].get_bbes();
                        
                        for ( size_t i = 0 ; i < objs.size() ; i++ )
                        {
                            if (objs[i] <= fxe[i_pb] + tau *(fx0s[i_pb]-fxe[i_pb]) )
                            {
                                if (double(bbes[i])/tpsMin[i_pb]<=alpha)
                                    cnt++;
                                break;
                            }
                        }
                    }
            }
            fout << " ";
            fout << 1.0*cnt/cnt_pb_instance;
        }
        fout << std::endl;
    }

    fout.close();

    std::cout << "... done" << std::endl;
    return true;
}

/*---------------------------------------*/
/*       display convergence profiles    */
/*---------------------------------------*/
bool RUNNERPOST::Runner::output_convergence_profile_plain ( const Output & out ) const
{
    
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    if ( n_pb == 0 || n_algo == 0 )
    {
        std::cerr << "Error: cannot compute convergence profile for n_pb ==0 or n_algo == 0" << std::endl;
        return false;
    }
    if ( out.get_x_select() == Output::X_Select::NP1EVAL)
    {
        std::cerr << "Error: cannot compute convergence profile for X_SEL NP1EVAL, N may vary for each problem. Use EVAL instead." << std::endl;
        return false;
    }

    // F, H or both?
    bool plotF = ( out.get_plot_type() == Output::Plot_Type::OnlyFFeasible || out.get_plot_type() == Output::Plot_Type::ComboHInfAndFFeas );
    bool plotH = ( out.get_plot_type() == Output::Plot_Type::OnlyHInfeasible || out.get_plot_type() == Output::Plot_Type::ComboHInfAndFFeas );
    bool plotFAndH = ( out.get_plot_type() == Output::Plot_Type::ComboHInfAndFFeas );
    
    if (!plotF && !plotH && !plotFAndH)
    {
        std::cerr << "Error: cannot compute convergence profile. Plot type not handled." << std::endl;
        return false;
    }

    bool iterPlotFlag = true;
    
    size_t i_pb, i_algo, i_pb_instance;

    // check that best solution and all results are available:
    std::list<size_t> miss_list;

    // For ComboHInfAndFFeas (plotFAndH=true), we need to loop twice
    while (iterPlotFlag)
    {
        miss_list.clear();
        
        
        if (!plotFAndH)
        {
            // Just one loop. Next while condition will exit
            iterPlotFlag =false;
        }
        else
        {
            // Manage the two loops with a switch
            
            // First loop: enter with plotH and plotF are true. But just do plot for F
            if (plotF && plotH)
            {
                plotH = false;
            }
            else
            {
                // Second loop: Just do plot for H. Next while condition will exit
                plotF = false;
                plotH = true;
                iterPlotFlag = false;
            }
        }
        
        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
            {
                for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
                {
                    if (! out.plotIsSelected(_selected_algos[i_algo]->get_id(), _selected_pbs[i_pb]->get_id(),i_pb_instance))
                        continue;
                    
                    double fxBest = INF;
                    if ( !_results[i_pb][i_algo][i_pb_instance].has_solution()  )
                    {
                        miss_list.push_back ( i_pb   );
                        miss_list.push_back ( i_algo );
                        miss_list.push_back ( i_pb_instance );
                        
                        // An infeasible run has no solution -> special flag in miss_list is set
                        if ( _results[i_pb][i_algo][i_pb_instance].is_infeas() )
                            miss_list.push_back( 1 );
                        else
                            miss_list.push_back( 0 );
                    }
                    
                    if ( plotF || plotH )
                    {
                        
                        std::string plainFileName = out.get_plain_file_name() + "."+_selected_algos[i_algo]->get_id()+"."+_selected_pbs[i_pb]->get_id()+".Inst"+std::to_string(i_pb_instance);
                        
                        if (plotFAndH && plotF)
                        {
                            plainFileName += ".F";
                        }
                        else if (plotFAndH && plotH)
                        {
                            plainFileName += ".H";
                        }
                        
                        std::ofstream fout (plainFileName);
                        fout << std::setprecision(10);
                        if ( fout.fail() )
                        {
                            std::cerr << "Warning: cannot create convergence profile output file "
                            << plainFileName << std::endl;
                            return false;
                        }
                        
                        std::cout << "\t writing of " << plainFileName << " ..." << std::flush;
                        
                        size_t max_bbe = out.get_x_max();
                        if ( max_bbe == RUNNERPOST::P_INF_INT )
                        {
                            max_bbe = _results[i_pb][i_algo][i_pb_instance].get_last_bbe();
                        }
                        
                        
                        bool xSelectIsTime = (out.get_x_select() == RUNNERPOST::Output::TIME);
                        
                        for ( size_t bbe = 1 ; bbe < max_bbe+1 ; ++bbe )
                        {
                            double fx =INF;
                            if (plotF)
                            {
                                
                                fx = _results[i_pb][i_algo][i_pb_instance].get_sol(bbe);
                            }
                            else if (plotH)
                            {
                                fx = _results[i_pb][i_algo][i_pb_instance].get_best_infeas(bbe);
                            }
                            else
                            {
                                std::cerr << "Error: plotH and plotF cannot be both true or false." << std::endl;
                                return false;
                            }
                            
                            // Write bbe and value of fx or hx
                            if (fx < fxBest )
                            {
                                fxBest = fx;
                                fout << ((xSelectIsTime)? _results[i_pb][i_algo][i_pb_instance].get_time(bbe):bbe) << " " << fxBest << std::endl;
                            }
                        }
                        fout.close();
                        std::cout << "... done" << std::endl;
                    }
                    
                }
            }
        }
    }

    if ( !miss_list.empty() )
    {
        std::cout << "... the following results are missing" << std::endl;
        std::list<size_t>::const_iterator it , end = miss_list.end();
        bool need_for_fix = false;
        for ( it = miss_list.begin() ; it != end ; ++it )
        {
            i_pb = *it;
            ++it;
            i_algo = *it;
            display_instance_name ( *_selected_pbs[i_pb] , *_selected_algos[i_algo]);
            ++it;
            i_pb_instance = *it;
            std::cout << " seed run#" << i_pb_instance ;
            ++it;
            if ( *it==1 )
                std::cout << " --> no feasible point found " <<std::endl;
            else
            {
                std::cout << " --> run failed, fix it to get data profile " <<std::endl;
                need_for_fix = true;
            }
        }
        std::cout << std::endl;
    }
    return true;
}


/*-------------------------------------------------*/
/*              data profiles .........            */
/* Use Moré and Wild SIAM JOPT 2009 eq 2.9         */
/*-------------------------------------------------*/
bool RUNNERPOST::Runner::output_data_profile_plain ( const Output & out) const
{
    const size_t n_algo = _selected_algos.size();
    const size_t n_pb = _selected_pbs.size();
    
    if ( out.get_tau() <= 0 || n_pb == 0 || n_algo == 0 )
    {
        std::cerr << "Error: cannot compute data profile for tau <= 0 or n_pb ==0 or n_algo == 0. Make sure to provide a valid tau value in the output_definition file." << std::endl;
        return false;
    }

    if ( out.get_plain_file_name().empty() )
    {
        std::cerr << "Error: output_plain must be specified in the output_definition file." << std::endl;
        return false;
        
    }
    
    std::ofstream fout ( out.get_plain_file_name() );
    fout << std::setprecision(10);
    if ( fout.fail() ) {
        std::cerr << "Error: cannot create data profile output file "
        << out.get_plain_file_name() << std::endl;
        return false;
    }
    
    if (out.get_x_select() == Output::X_Select::TIME)
    {
        std::cerr << "Error: Option TIME not available for eval data profiles. Use time data profiles" << std::endl;
        fout.close();
        return false;
    }

    std::cout << "\t writing of " << out.get_plain_file_name() << " ..." << std::flush;

    size_t i_pb, i_algo, i_pb_instance;

    // check that best solution and all results are available:
    std::list<size_t> miss_list;
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                if ( !_results[i_pb][i_algo][i_pb_instance].has_solution()  )
                {
                    miss_list.push_back ( i_pb   );
                    miss_list.push_back ( i_algo );
                    miss_list.push_back ( i_pb_instance );

                    // An infeasible run has no solution -> special flag in miss_list is set
                    if ( _results[i_pb][i_algo][i_pb_instance].is_infeas() )
                        miss_list.push_back( 1 );
                    else
                        miss_list.push_back( 0 );


                }
            }
        }
    }

    if ( !miss_list.empty() )
    {
        std::cout << "... the following results are missing" << std::endl ;
        std::list<size_t>::const_iterator it , end = miss_list.end();
        bool need_for_fix = false;
        for ( it = miss_list.begin() ; it != end ; ++it )
        {
            i_pb = *it;
            ++it;
            i_algo = *it;
            display_instance_name (*_selected_pbs[i_pb] , *_selected_algos[i_algo] );
            ++it;
            i_pb_instance = *it;
            std::cout << " pb run instance#" << i_pb_instance +1 ;
            ++it;
            if ( *it==1 )
                std::cout << " --> no feasible point found " <<std::endl;
            else
            {
                std::cerr <<std::endl << "Failed run: fix it to get data profile " <<std::endl;
                need_for_fix = true;
                break;
            }
        }

        if ( need_for_fix )
        {
            fout << "failed to obtain data profile" << std::endl;
            fout.close();
            return false;
        }
    }

    // Get fx0s for all problems
    const auto& fx0s = get_fx0s(out.get_FFFeasMeth());
    
    // Failsafe for Fx0. A single run without valid x0 and fx0s is empty
    if ( fx0s.empty())
    {
        std::cerr << "Error: Undefined fx0: failed to obtain data profile" << std::endl;
        fout.close();
        return false;
    }
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        if ( fx0s[i_pb]==INF )
        {
            std::cout << "pb #" << i_pb+1 << " ---> fx0=Inf --> un-resolved"<<std::endl;
        }
    }

    // get the best solution for each problem:
    const auto& fxe = get_best_fx();

    // compute the data profile:
    // -------------------------
    int max_alpha = out.get_x_max();

    // Update the range for x axis according to all problems considered:
    // - take the max of all problems last bbe OR
    // - take the max of all problems last bbe divided by n+1
    bool useNp1Evals = (out.get_x_select() == Output::X_Select::NP1EVAL);
    if ( max_alpha == RUNNERPOST::P_INF_INT )
    {
        max_alpha = 0;
        for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
        {
            size_t dimPb= ( useNp1Evals ) ?  _selected_pbs[i_pb]->get_n() : 0;
            for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
            {
                for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
                {
                    max_alpha = std::max(max_alpha , int(std::ceil(_results[i_pb][i_algo][i_pb_instance].get_last_bbe()/(dimPb+1.0))));
                }
            }
        }
    }
    
    size_t cnt, cnt_pb_instance;
    std::stringstream ftmp_alpha,ftmp_cnt,ftmp_cnt_prev;
    ftmp_cnt << std::setprecision(10);
    ftmp_cnt_prev << std::setprecision(10);
    for (int alpha = 0 ; alpha <= max_alpha ; ++alpha )
    {
        ftmp_alpha << alpha << " ";
        
        for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
        {
            cnt = 0;
            cnt_pb_instance = 0;
            for (i_pb = 0 ; i_pb < n_pb ; ++i_pb)
            {
                auto n_pb_instance = _selected_pbs[i_pb]->get_nbPbInstances();
                cnt_pb_instance += n_pb_instance;
                
                // Use evals instead of (n+1)*evals
                size_t dimPb= ( useNp1Evals ) ?  _selected_pbs[i_pb]->get_n() : 0;
                if ( fx0s[i_pb] < INF && fxe[i_pb] < INF )
                {
                    for ( i_pb_instance = 0 ; i_pb_instance < n_pb_instance ; ++i_pb_instance )
                    {
                        if ( fx0s[i_pb]-_results[i_pb][i_algo] [i_pb_instance].get_sol(alpha*(dimPb+1)) >= (1.0-out.get_tau())*(fx0s[i_pb]-fxe[i_pb]) )
                        {
                            ++cnt;
                        }
                    }
                }
            }
            ftmp_cnt << 1.0*cnt/cnt_pb_instance << " " ;
        }
        // This is used to avoid writing lines with no changes in the counts
        if (ftmp_cnt_prev.str() != ftmp_cnt.str())
        {
            fout << ftmp_alpha.str() << ftmp_cnt.str() << std::endl;
            ftmp_cnt_prev.str(ftmp_cnt.str());
        }
        // Clear fmtp
        ftmp_alpha.str(std::string());
        ftmp_cnt.str(std::string());
    }
    fout.close();

    std::cout << "... done" << std::endl;

    return true;
}

bool RUNNERPOST::Runner::output_time_profile_plain(const Output& out) const
{
    const std::string profileName = "time profile";
    
    const size_t n_algo = _selected_algos.size();
    const size_t n_pb = _selected_pbs.size();
    
    if (n_pb == 0 || n_algo == 0)
    {
        std::cerr << "Error: cannot compute " << profileName << " for n_pb == 0 or n_algo == 0" << std::endl;
        return false;
    }
    // check that best solution and all results are available:
    std::list<size_t> miss_list;
    for (size_t i_pb = 0; i_pb < n_pb; ++i_pb)
    {
        for (size_t i_algo = 0; i_algo < n_algo; ++i_algo)
        {
            for (size_t i_pb_instance = 0; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances(); ++i_pb_instance)
            {
                if (!_results[i_pb][i_algo][i_pb_instance].has_solution())
                {
                    miss_list.push_back (i_pb  );
                    miss_list.push_back (i_algo);
                    miss_list.push_back (i_pb_instance);
                    // An infeasible run has no solution -> special flag in miss_list is set
                    if (_results[i_pb][i_algo][i_pb_instance].is_infeas())
                    {
                        miss_list.push_back(1);
                    }
                    else
                    {
                        miss_list.push_back(0);
                    }
                }
            }
        }
    }
    if ( !miss_list.empty() )
    {
        std::cout << "... the following results are missing" << std::endl ;
        std::list<size_t>::const_iterator it;
        bool need_for_fix = false;
        for (it = miss_list.begin(); it != miss_list.end(); ++it)
        {
            size_t i_pb = *it;
            ++it;
            size_t i_algo = *it;
            display_instance_name (*_selected_pbs[i_pb], *_selected_algos[i_algo]);
            ++it;
            size_t i_pb_instance = *it;
            std::cout << " pb run instance #" << i_pb_instance ;
            ++it;
            if (*it == 1)
            {
                std::cout << " --> no feasible point found " <<std::endl;
            }
            else
            {
                std::cerr << "Failed run: fix it to get " << profileName << std::endl;
                need_for_fix = true;
                break;
            }
        }
        if (need_for_fix)
        {
            std::cerr << "failed to obtain " << profileName << std::endl;
            return false;
        }
    }
    // Write one file for each algo. It makes it easier to plot.
    for (size_t i_algo = 0; i_algo < n_algo; ++i_algo)
    {
        std::string algoFileName = out.get_plain_file_name();
        size_t lastPointIndex = out.get_plain_file_name().rfind(".");
        algoFileName.insert(lastPointIndex, std::to_string(i_algo));
        std::ofstream fout(algoFileName.c_str());
        fout << std::setprecision(10);
        if (fout.fail())
        {
            std::cerr << "Error: cannot create " << profileName << " output file " << algoFileName << std::endl;
            return false;
        }
        std::cout << "writing of " << algoFileName << " ..." << std::flush;
        // compute the time profile
        // ------------------------
        for (size_t i_pb = 0; i_pb < n_pb; ++i_pb)
        {
            auto n_pb_instance = _selected_pbs[i_pb]->get_nbPbInstances();
            
            for (size_t i_pb_instance = 0 ; i_pb_instance < n_pb_instance ; ++i_pb_instance)
            {
                auto bbe = _results[i_pb][i_algo][i_pb_instance].getTotalBbe();
                auto time = _results[i_pb][i_algo][i_pb_instance].getTotalTime();
                if (time < INF)
                {
                        fout << bbe << " " << time << std::endl;
                }
            }
        }
        fout.close();
        std::cout << "... done" << std::endl << std::endl;
    }
    return true;
}
bool RUNNERPOST::Runner::output_time_data_profile_plain ( const Output & out  ) const
{
    const size_t n_algo = _selected_algos.size();
    const size_t n_pb = _selected_pbs.size();
    
    if ( out.get_tau() < 0 || n_pb == 0 || n_algo == 0 )
    {
        std::cerr << "Error: cannot compute time data profile for tau < 0 or n_pb ==0 or n_algo == 0" << std::endl;
        return false;
    }
    std::ofstream fout ( out.get_plain_file_name() );
    fout << std::setprecision(10);
    if ( fout.fail() ) {
        std::cerr << "Error: cannot create time data profile output file "
        << out.get_plain_file_name() << std::endl;
        return false;
    }
    std::cout << "\t writing of " << out.get_plain_file_name() << " ..." << std::flush;
    size_t i_pb, i_algo, i_pb_instance;
    // check that best solution and all results are available:
    std::list<size_t> miss_list;
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                if ( !_results[i_pb][i_algo][i_pb_instance].has_solution()  )
                {
                    miss_list.push_back ( i_pb   );
                    miss_list.push_back ( i_algo );
                    miss_list.push_back ( i_pb_instance );
                    // An infeasible run has no solution -> special flag in miss_list is set
                    if ( _results[i_pb][i_algo][i_pb_instance].is_infeas() )
                        miss_list.push_back( 1 );
                    else
                        miss_list.push_back( 0 );
                }
            }
        }
    }
    if ( !miss_list.empty() )
    {
        std::cout << "... the following results are missing" << std::endl ;
        std::list<size_t>::const_iterator it , end = miss_list.end();
        bool need_for_fix = false;
        for ( it = miss_list.begin() ; it != end ; ++it )
        {
            i_pb = *it;
            ++it;
            i_algo = *it;
            display_instance_name ( *_selected_pbs[i_pb] , *_selected_algos[i_algo] );
            ++it;
            i_pb_instance = *it;
            std::cout << " pb run instance #" << i_pb_instance ;
            ++it;
            if ( *it==1 )
                std::cout << " --> no feasible point found " <<std::endl;
            else
            {
                std::cerr <<std::endl << "Failed run: fix it to get time data profile " <<std::endl;
                need_for_fix = true;
                break;
            }
        }
        if ( need_for_fix )
        {
            fout << "failed to obtain time data profile" << std::endl;
            fout.close();
            return false;
        }
    }
    // Get fx0s for all problems
    ArrayOfDouble fx0s = get_fx0s(out.get_FFFeasMeth());
    
    // Failsafe for Fx0. A single run without valid x0 and fx0s is empty
    if ( fx0s.empty())
    {
        std::cerr << "Error: Undefined fx0. Failed to obtain data profile" << std::endl;
        fout.close();
        return false;
    }
    
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        if ( fx0s[i_pb] == INF )
        {
            std::cout << "pb #" << i_pb+1 << " ---> fx0=Inf --> un-resolved"<<std::endl;
        }
    }
    // get the best solution for each problem:
    ArrayOfDouble fxe = get_best_fx();

    // compute the time data profile:
    // -------------------------
    // Compute max time taken by all problems
    int max_beta = 0;
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        size_t dimPb = (_use_evals_for_dataprofiles) ? 0 : _selected_pbs[i_pb]->get_n();
        for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                int beta = std::round(_results[i_pb][i_algo][i_pb_instance].get_time()) / double(dimPb);
                if (beta > max_beta)
                {
                    max_beta = beta;
                }
            }
        }
    }
    size_t cnt, cnt_pb_instance;
    for (int beta = 0 ; beta <= max_beta ; ++beta )
    {
        fout << beta << " ";
        for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
        {
            cnt = cnt_pb_instance = 0;
            for (i_pb = 0 ; i_pb < n_pb ; ++i_pb)
            {
                auto n_pb_instance = _selected_pbs[i_pb]->get_nbPbInstances();
                cnt_pb_instance += n_pb_instance;
                // Use evals instead of (n+1)*evals
                size_t dimPb= ( _use_evals_for_dataprofiles ) ? 0 : _selected_pbs[i_pb]->get_n();
                if ( fx0s[i_pb] < INF && fxe[i_pb] < INF )
                {
                    for ( i_pb_instance = 0 ; i_pb_instance < n_pb_instance ; ++i_pb_instance )
                    {
                        if (fx0s[i_pb] - _results[i_pb][i_algo][i_pb_instance].get_sol_by_time(beta*(dimPb+1)) >= (1-out.get_tau()) * (fx0s[i_pb]-fxe[i_pb]))
                        {
                            ++cnt;
                        }
                    }
                }
            }
            fout << (1.0 * cnt ) / (n_pb*cnt_pb_instance) << " " ;
        }
        fout << std::endl;
    }
    fout.close();
    std::cout << "... done" << std::endl << std::endl;
    return true;
}


/*-------------------------------------------------*/
/*              accuracy profiles .........        */
/* Use Audet, Hare and Tribes, Def. 3.1           */
/*-------------------------------------------------*/
bool RUNNERPOST::Runner::output_accuracy_profile_plain ( const Output & out) const
{
    const size_t n_algo = _selected_algos.size();
    const size_t n_pb = _selected_pbs.size();
    
    if ( n_pb == 0 || n_algo == 0 )
    {
        std::cerr << "Error: cannot compute data profile for n_pb ==0 or n_algo == 0. Make sure to select pbs and algos in definition files." << std::endl;
        return false;
    }

    if ( out.get_plain_file_name().empty() )
    {
        std::cerr << "Error: output_plain must be specified in the output_definition file." << std::endl;
        return false;
        
    }
    
    std::ofstream fout ( out.get_plain_file_name() );
    fout << std::setprecision(10);
    if ( fout.fail() ) {
        std::cerr << "Error: cannot create data profile output file "
        << out.get_plain_file_name() << std::endl;
        return false;
    }
    
    if (out.get_x_select() != Output::X_Select::EVAL)
    {
        std::cerr << "Error: Option X_SELECT EVAL must be selected in output_definition file." << std::endl;
        fout.close();
        return false;
    }

    std::cout << "\t writing of " << out.get_plain_file_name() << " ..." << std::flush;

    size_t i_pb, i_algo, i_pb_instance;

    // check that best solution and all results are available:
    std::list<size_t> miss_list;
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                if ( !_results[i_pb][i_algo][i_pb_instance].has_solution()  )
                {
                    miss_list.push_back ( i_pb   );
                    miss_list.push_back ( i_algo );
                    miss_list.push_back ( i_pb_instance );

                    // An infeasible run has no solution -> special flag in miss_list is set
                    if ( _results[i_pb][i_algo][i_pb_instance].is_infeas() )
                        miss_list.push_back( 1 );
                    else
                        miss_list.push_back( 0 );


                }
            }
        }
    }

    if ( !miss_list.empty() )
    {
        std::cout << "... the following results are missing" << std::endl ;
        std::list<size_t>::const_iterator it , end = miss_list.end();
        bool need_for_fix = false;
        for ( it = miss_list.begin() ; it != end ; ++it )
        {
            i_pb = *it;
            ++it;
            i_algo = *it;
            display_instance_name (*_selected_pbs[i_pb] , *_selected_algos[i_algo] );
            ++it;
            i_pb_instance = *it;
            std::cout << " pb run instance#" << i_pb_instance +1 ;
            ++it;
            if ( *it==1 )
                std::cout << " --> no feasible point found " <<std::endl;
            else
            {
                std::cerr <<std::endl << "Failed run: fix it to get accuracy profile " <<std::endl;
                need_for_fix = true;
                break;
            }
        }

        if ( need_for_fix )
        {
            fout << "failed to obtain accuracy profile" << std::endl;
            fout.close();
            return false;
        }
    }

    // Get fx0s for all problems
    auto fx_first_feas_method = out.get_FFFeasMeth();
    const auto& fx0s = get_fx0s(fx_first_feas_method);
    
    // Failsafe for Fx0. A single run without valid x0 and fx0s is empty
    if ( fx0s.empty())
    {
        std::cerr << "Error: Undefined fx0: failed to obtain accuracy profile" << std::endl;
        fout.close();
        return false;
    }
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        if ( fx0s[i_pb]==INF )
        {
            std::cout << "pb #" << i_pb+1 << " ---> fx0=Inf --> un-resolved"<<std::endl;
        }
    }

    // get the best solution for each problem:
    const auto& fxe = get_best_fx();

    // Compute list of ds = - log10(1-(fx(nmax)-fx0)/(fx* - fx0))
    // -------------------------
    std::vector<double> ds;
    for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
    {
        for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb)
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                if ( _results[i_pb][i_algo][i_pb_instance].has_solution() )
                {
                    // Get the improving objs and the corresponding bbes
                    const auto & objs = _results[i_pb][i_algo][i_pb_instance].get_objs();
                    if (objs.empty())
                    {
                        continue;
                    }
                    if (objs.back() == fxe[i_pb])
                    {
                        ds.push_back(INF);
                    }
                    else
                    {
                        // Relative accuracy greater than 16 are not considered
                        double accuracy = -log10(1.0-(objs.back()-fx0s[i_pb])/(fxe[i_pb]-fx0s[i_pb]));
                        if (accuracy < 16)
                        {
                            ds.push_back(accuracy);
                        }
                        else
                        {
                            ds.push_back(INF);
                        }
                    }
                }
            }
        }
    }
    
    /// Sort accuraciess in ascending order
    /// -------------------------
    std::sort(ds.begin(), ds.end());
    
    // Remove the duplicates
    /// -------------------------
    auto last = std::unique(ds.begin(), ds.end());
    // Erase the remaining elements after the unique elements
    ds.erase(last, ds.end());
    
    // Replace the last element which should be INF by a large value: the value before the last + 10%
    ds[ds.size()-1] = ds[ds.size()-2]*1.10;
    
    // Count the number of problems with accuracy >= d
    size_t cnt;
    for (const auto& d: ds)
    {
        fout << d;
        for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
        {
            cnt = 0;
            int cnt_pb_instance = 0;
            for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
            {
                auto n_pb_instance = _selected_pbs[i_pb]->get_nbPbInstances();
                cnt_pb_instance += n_pb_instance;
                for ( i_pb_instance = 0 ; i_pb_instance < n_pb_instance; ++i_pb_instance )
                    if ( _results[i_pb][i_algo][i_pb_instance].has_solution() )
                    {
                        // Get the improving objs and the corresponding bbes
                        const auto & objs = _results[i_pb][i_algo][i_pb_instance].get_objs();
                        if (objs.empty())
                        {
                            continue;
                        }
                        if (objs.back() == fxe[i_pb] || -log10(1.0-(objs.back()-fx0s[i_pb])/(fxe[i_pb]-fx0s[i_pb])) >= d)
                        {
                                cnt++;
                        }
                    }
            }
            fout << " ";
            fout << 1.0*cnt/cnt_pb_instance;
        }
        fout << std::endl;
    }

    fout.close();

    std::cout << "... done" << std::endl;

    return true;
}

//void RUNNERPOST::Runner::output_problems_unsolved ( const double & tau , const double & nbSimplexEval ) const
//{
//    const size_t n_algo = _selected_algos.size();
//    const size_t n_pb = _selected_pbs.size();
//    
//    if ( tau < 0 || n_pb == 0 || n_algo == 0 )
//    {
//        std::cerr << "Error: cannot compute data profile for tau < 0 or n_pb ==0 or n_algo == 0" << std::endl;
//        return ;
//    }
//    
//    std::cout << "Detecting unsolved problems at precision tau=" << tau << " for " << ((nbSimplexEval<0) ? "max": std::to_string(nbSimplexEval)) << " nb simplex evals" << std::endl;
//    
//    size_t i_pb, i_algo, i_pb_instance;
//    
//    // check that best solution and all results are available:
//    std::list<size_t> miss_list;
//    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
//    {
//        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
//        {
//            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances(); ++i_pb_instance )
//            {
//                if ( !_results[i_pb][i_algo][i_pb_instance].has_solution()  )
//                {
//                    miss_list.push_back ( i_pb   );
//                    miss_list.push_back ( i_algo );
//                    miss_list.push_back ( i_pb_instance );
//                    
//                    // An infeasible run has no solution -> special flag in miss_list is set
//                    if ( _results[i_pb][i_algo][i_pb_instance].is_infeas() )
//                        miss_list.push_back( 1 );
//                    else
//                        miss_list.push_back( 0 );
//                }
//            }
//        }
//    }
//    
//    // Get fx0s for all problems
//    ArrayOfDouble fx0s = get_fx0s();
//    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
//    {
//        if ( fx0s[i_pb]==INF )
//        {
//            std::cout << "pb #" << i_pb+1 << " ---> fx0=Inf --> un-resolved"<<std::endl;
//        }
//    }
//    
//    // get the best solution for each problem:
//    ArrayOfDouble fxe = get_best_fx();
//    
//    // Search for unsolved problems:
//    // -------------------------
//    int alpha = (nbSimplexEval < 0 ) ?  Problem::getNbSimplexEvals(): std::round(nbSimplexEval) ;
//    std::vector<size_t> nbUnsolved(n_algo,0);
//    for (i_pb = 0 ; i_pb < n_pb ; ++i_pb)
//    {
//        std::vector<size_t> nbUnsolvedByPb(n_algo,0);
//        if ( fx0s[i_pb] < INF && fxe[i_pb] < INF )
//        {
//            size_t dimPb= _selected_pbs[i_pb]->get_n();
//            size_t n_pb_instance = _selected_pbs[i_pb]->get_nbPbInstances();
//            for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
//            {
//                for ( i_pb_instance = 0 ; i_pb_instance < n_pb_instance ; ++i_pb_instance )
//                {
//                    if ( fx0s[i_pb]-_results[i_pb][i_algo] [i_pb_instance].get_sol(alpha*(dimPb+1)) < (1-tau)*(fx0s[i_pb]-fxe[i_pb]) )
//                    {
//                        display_instance_name ( *_selected_pbs[i_pb] , *_selected_algos[i_algo] );
//                        if ( n_pb_instance > 1)
//                        {
//                            std::cout << " pb run instance #" << i_pb_instance << std::endl;
//                        }
//                        nbUnsolved[i_algo]++;
//                        nbUnsolvedByPb[i_algo]++;
//                    }
//                }
//            }
//            for (size_t i_algo=0; i_algo < n_algo ; i_algo++  )
//            {
//                if (nbUnsolvedByPb[i_algo] > 0)
//                {
//                    std::cout << "     ->  Algo #" << i_algo+1 << " -> " << nbUnsolvedByPb[i_algo] << " unsolved instances for pb #" << i_pb+1 << std::endl;
//                }
//            }
//        }
//    }
//    for (size_t i_algo=0; i_algo < n_algo ; i_algo++  )
//    {
//        std::cout << "  Algo #" << i_algo+1 << " -> " << nbUnsolved[i_algo] << " overall unsolved instances" << std::endl;
//    }
//    std::cout << "... done" << std::endl << std::endl;
//}


/*-------------------------------------------------------*/
/* get the value of f at x0 for all problems (private)   */
/*-------------------------------------------------------*/
/* If x0 is infeasible, get the value of fs at the first */
/* feasible point for all problems : max (default) or min*/
/* or average value of first feasible eval of all        */
/* algo/instances                                        */
/*-------------------------------------------------------*/
RUNNERPOST::ArrayOfDouble RUNNERPOST::Runner::get_fx0s(const RUNNERPOST::Output::Fx_First_Feas_Method & fx_first_feas) const
{
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    ArrayOfDouble fx0s(n_pb, INF);
    double fx0;

    // Get fx0 for all problems
    for (size_t i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        fx0s[i_pb] = _results[i_pb][0][0].get_sol(1);
        for (size_t i_algo = 1 ; i_algo < n_algo ; ++i_algo )
        {
            for (size_t i_pb_instance=0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance)
            {
                fx0 = _results[i_pb][i_algo][i_pb_instance].get_sol(1);
                if ( (fx0s[i_pb] == INF && fx0 != INF) || (fx0s[i_pb] != INF && fx0 == INF) )
                {
                    std::cout << "... inconsistent starting points (feas/infeas) between instance runs for problem " << _selected_pbs[i_pb]->get_id() << " and algo " << _selected_algos[i_algo]->get_id() << " fx0=" << fx0 << " fx0s[ipb]=" << fx0s[i_pb] << std::endl;
                    fx0s.clear();
                    return fx0s;
                }
                else if ( fx0s[i_pb] != INF && fx0 != INF && std::fabs(fx0 - fx0s[i_pb]) > 1e-10 )
                {
                    std::cout << "... inconsistent starting points between instance runs for problem " << _selected_pbs[i_pb]->get_id() << " and algo " << _selected_algos[i_algo]->get_id() << " fx0=" << fx0 << " fx0s[ipb]=" << fx0s[i_pb] << std::endl;
                    fx0s.clear();
                    return fx0s;
                }
            }
        }

        // fx0 for problems with constraints may not be available (case infeasible initial point -- > INF)
        // fx0--> average, max, min of first feasible point obj
        if ( fx0s[i_pb]==INF )
        {
            double first_fx;
            size_t nb_first_fx = 0;
            fx0s[i_pb]=0.0;
            
            size_t nbConstraints=0;
            bool checkConsistency = false;
            
            for (size_t  i_algo = 0 ; i_algo < n_algo ; ++i_algo )
            {
                nbConstraints =  _selected_algos[i_algo]->getNbConstraints();
                if (i_algo > 0)
                {
                    size_t nbConstraintsP =  _selected_algos[i_algo-1]->getNbConstraints();
                    if ( nbConstraints != nbConstraintsP)
                    {
                        std::cout << "Error: inconsistent contraint definition between algos " << std::endl;
                        fx0s.clear();
                        return fx0s;
                    }
                }
                
                // Special case where fx0[i_pb] is INF but no constraint is defined.
                // We probably have hidden constraints and several X0 points are evaluated
                // and some failed. Let's use the first valid evaluation for fx0.
                // Unlike when constraint are present we need to verify consistency
                // between the algos. Set a flag for that.
                checkConsistency = (nbConstraints == 0);
                
                for (size_t i_pb_instance=0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance)
                {
                    if ( !_results[i_pb][i_algo][i_pb_instance].is_infeas() )
                    {
                        first_fx = _results[i_pb][i_algo][i_pb_instance].get_first_fx();
                        
                        if (checkConsistency)
                        {
                            if (nb_first_fx == 0 && i_algo == 0)
                                fx0s[i_pb] = first_fx;
                            else
                            {
                                if (std::fabs(fx0s[i_pb]-first_fx) > 1.E-16)
                                {
                                    std::cout << "Error: inconsistency between algos for the first fx. For problem " << _selected_pbs[i_pb]->get_id() << " and algo " << _selected_algos[i_algo]->get_id() << " fx0=" << first_fx << " fx0s[ipb]=" << fx0s[i_pb] << std::endl;
                                    fx0s.clear();
                                    return fx0s;
                                }
                            }
                        }
                        else
                        {
                            if ( fx_first_feas == Output::Fx_First_Feas_Method::max )
                            {
                                if ( nb_first_fx == 0 )
                                    fx0s[i_pb] = first_fx;
                                else
                                    fx0s[i_pb] = std::max( first_fx , fx0s[i_pb] );
                                
                            }
                            else if ( fx_first_feas == Output::Fx_First_Feas_Method::min )
                            {
                                if ( nb_first_fx == 0 )
                                    fx0s[i_pb] = first_fx;
                                else
                                    fx0s[i_pb] = std::min( first_fx , fx0s[i_pb] );
                            }
                            else if ( fx_first_feas == Output::Fx_First_Feas_Method::avg)
                                fx0s[i_pb] += first_fx;
                            else
                            {
                                std::cout << "... unknown strategy to obtain the first feas fx value: " << Output::fFeasMethToString(fx_first_feas) << std::endl;
                                fx0s.clear();
                                return fx0s;
                            }
                        }
                        nb_first_fx++;
                    }
                }
            }
            
            if (!checkConsistency)
            {
                if ( nb_first_fx > 0 )
                {
                    if ( fx_first_feas == Output::Fx_First_Feas_Method::avg )
                        fx0s[i_pb]/=nb_first_fx;
                }
                else
                    fx0s[i_pb]=INF;
                
                std::cout << "pb #"<< i_pb+1 << " has infeasible starting point ---> " << Output::fFeasMethToString(fx_first_feas) << " method for first feasible point objective functions is used: fx0="<<fx0s[i_pb] << std::endl;
            }
            else
            {
                std::cout << "pb #"<< i_pb+1 << " has not constraint but no first point in stats ---> Let's use the first valid fx value: " << fx0s[i_pb] << std::endl;
            }
        }

    }
    return fx0s;

}


/*-------------------------------------------------------*/
/* get the best solution for all problems (private)      */
/*-------------------------------------------------------*/
RUNNERPOST::ArrayOfDouble RUNNERPOST::Runner::get_best_fx() const
{
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    ArrayOfDouble fxe(n_pb, INF);
    double fxe_tmp;
    size_t nbDomRefObj; // not used here
    for (size_t i_pb = 0; i_pb < n_pb ; ++i_pb)
    {
        if ( _use_hypervolume_for_profiles )
        {
            // Get the best hypervolume using the combined paretos of algos
            fxe[i_pb] = - Result::compute_hv( _combinedParetoAllAlgos[i_pb],
                                            _refParetoIdealPtAllAlgos[i_pb],
                                            _refParetoNadirPtAllAlgos[i_pb],
                                            nbDomRefObj );
        }
        else
        {
            for (size_t i_algo = 0 ; i_algo < n_algo ; ++i_algo )
            {
                for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
                {
                    // size_t max_bb_evals=_selected_pbs[i_pb]->getMaxBBEvals();
                    // For now we consider all evaluations available
                    fxe_tmp = _results[i_pb][i_algo][i_pb_instance].get_sol ( INF_SIZE_T );
                    
                    if ( fxe_tmp < INF &&
                        ( fxe[i_pb] == INF || fxe_tmp < fxe[i_pb] ) )
                        fxe[i_pb] = fxe_tmp;
                }
            }
        }
    }
    return fxe;
}


/*------------------------------------------------------*/
/* get the mean value of elapsed time over all problems */
/*------------------------------------------------------*/
RUNNERPOST::ArrayOfDouble RUNNERPOST::Runner::get_mean_algo_times(size_t bbe) const
{
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    ArrayOfDouble times(n_algo, INF);
    for (size_t i_algo = 0 ; i_algo < n_algo; ++i_algo)
    {
        // total time for all problems, on all seeds, for this algo.
        double totalAlgoTime = 0;
        size_t totalNbPbAndSeeds = 0;
        for (size_t i_pb = 0; i_pb < n_pb; ++i_pb)
        {
            if (get_bbe_max(i_pb, i_algo) < bbe)
            {
                // time for this problem and algo should not be counted for this bbe.
                continue;
            }
            for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances(); ++i_pb_instance)
            {
                totalAlgoTime += _results[i_pb][i_algo][i_pb_instance].get_time(bbe);
                totalNbPbAndSeeds++;
            }
        }
        if (totalNbPbAndSeeds > 0)
        {
            times[i_algo] = totalAlgoTime / totalNbPbAndSeeds;
        }
        else
        {
            std::cerr << "Warning: Found 0 problems for bbe = " << bbe << " algo #" << i_algo << std::endl;
            times[i_algo] = 0;
        }
    }
    return times;
}


/*---------------------------------------------------------------------------*/
/* get the time values, relative to first algo, mean value over all problems */
/*---------------------------------------------------------------------------*/
RUNNERPOST::ArrayOfDouble RUNNERPOST::Runner::get_relative_algo_times(size_t bbe) const
{
    
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    ArrayOfDouble relTimes(n_algo, 0);
    relTimes[0] = 100;
    if (get_bbe_max(0) < bbe)
    {
        std::cerr << "Warning: Algo #0 has less than " << bbe << " evaluations" << std::endl;
        return relTimes;
    }
    for (size_t i_algo = 1 ; i_algo < n_algo; ++i_algo)
    {
        // total time for all problems, on all seeds, for this algo.
        double totalAlgoTime = 0;
        size_t totalNbPbAndSeeds = 0;
        double meanTime0 = 0;
        for (size_t i_pb = 0; i_pb < n_pb; ++i_pb)
        {
            if (get_bbe_max(i_pb, i_algo) < bbe)
            {
                // time for this problem and algo should not be counted for this bbe.
                continue;
            }
            for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances(); ++i_pb_instance)
            {
                totalAlgoTime += _results[i_pb][i_algo][i_pb_instance].get_time(bbe);
                meanTime0     += _results[i_pb][0][i_pb_instance].get_time(bbe);
                totalNbPbAndSeeds++;
            }
        }
        if (totalNbPbAndSeeds > 0)
        {
            relTimes[i_algo] = totalAlgoTime / meanTime0 * 100;
        }
        else
        {
            std::cerr << "Warning: Found 0 problems for bbe = " << bbe << " algo #" << i_algo << std::endl;
            relTimes[i_algo] = 0;
        }
    }
    return relTimes;
}


/*----------------------------------------------------------------------*/
/* get the overall maximum of iteration for all problems (private)      */
/*----------------------------------------------------------------------*/
size_t RUNNERPOST::Runner::get_bbe_max() const
{
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    size_t tmp = 0;
    size_t bbe_max = 0;
    for (size_t i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        for (size_t i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                tmp = _results[i_pb][i_algo][i_pb_instance].get_last_bbe();
                if ( tmp > bbe_max )
                {
                    bbe_max = tmp;
                }
            }
        }
    }
    return bbe_max;
}


/*-----------------------------------------------------*/
/* get the maximum of iteration for a specific algorithm */
/*-----------------------------------------------------*/
size_t RUNNERPOST::Runner::get_bbe_max(size_t i_algo) const
{
    
    const size_t n_pb = _selected_pbs.size();
    
    size_t tmp = 0;
    size_t bbe_max = 0;
    for (size_t i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance)
        {
            tmp = _results[i_pb][i_algo][i_pb_instance].get_sol_bbe();
            if (tmp > bbe_max)
            {
                bbe_max = tmp;
            }
        }
    }

    return bbe_max;
}


/*-----------------------------------------------------*/
/* get the maximum of iteration for a specific problem and specific algorithm */
/*-----------------------------------------------------*/
size_t RUNNERPOST::Runner::get_bbe_max(size_t i_pb, size_t i_algo) const
{

    size_t tmp = 0;
    size_t bbe_max = 0;
    for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance)
    {
        tmp = _results[i_pb][i_algo][i_pb_instance].get_sol_bbe();
        if (tmp > bbe_max)
        {
            bbe_max = tmp;
        }
    }

    return bbe_max;
}


/*----------------------------------------------------------------------*/
/* get the minimum problems dimension                                   */
/*----------------------------------------------------------------------*/
int RUNNERPOST::Runner::get_dimPbMin() const
{

    int tmp , dimMin = _selected_pbs[0]->get_n() ;;
    for (size_t i_pb = 1 ; i_pb < _selected_pbs.size() ; ++i_pb )
    {
        tmp = _selected_pbs[i_pb]->get_n() ;
        if ( tmp < dimMin )
            dimMin = tmp;
    }
    return dimMin;
}


/*---------------------------------------*/
/*        display selected problems      */
/*---------------------------------------*/
void RUNNERPOST::Runner::display_selected_problems ( void ) const
{
    std::cout << std::endl;

    if ( _selected_pbs.size() == 0 )
        std::cout << "no problem has been selected" << std::endl;
    else
    {
        std::ostringstream msg;
        const size_t n_pb = _selected_pbs.size();
        msg << "selected problem";
        if ( n_pb > 1 )
            msg << "s (" << n_pb << ")";
        std::cout << msg.str() << std::endl ;
        display_pbs ( _selected_pbs );
    }
    std::cout << std::endl;
}

///*---------------------------------------*/
///*           display all problems        */
///*---------------------------------------*/
//void Runner::display_all_problems ( void ) const
//{
//    size_t n = _all_pbs.size();
//
//    if ( n == 0 )
//        std::cout << "there is no problem to solve";
//    else {
//        std::ostringstream msg;
//        msg << "all problem";
//        if ( n > 1 )
//            msg << "s (" << n << ")";
//        std::cout << msg.str() ;
//        display_pbs ( _all_pbs );
//        std::cout << std::endl;
//
//    }
//    std::cout << std::endl;
//}

/*---------------------------------------*/
/*       display a list of problems      */
/*       (private)                       */
/*---------------------------------------*/
void RUNNERPOST::Runner::display_pbs ( const std::vector<Problem *> & pbs ) const
{

    for ( const auto &pb: pbs )
    {
        std::cout << "\t " ;
        pb->display ( );
        std::cout << std::endl;
    }
}

///*---------------------------------------*/
///*           display all problems        */
///*---------------------------------------*/
//void RUNNERPOST::Runner::display_special_options ( void ) const
//{
//
//    if ( _use_h_for_profiles || _use_avg_fx_first_feas || _use_evals_for_dataprofiles )
//    {
//        std::cout <<std::endl << "Special options for runner:" <<std::endl;
//        if ( _use_h_for_profiles )
//        {
//            std::cout  << "\t Use infeasibility h(x)=sum_j ( max(c_j(x),0)^2) for data and performance profiles" <<std::endl;
//
//        }
//        if ( _use_avg_fx_first_feas )
//        {
//            std::cout << "\t Use average fx of all first feasible points on all instance of a problem (default uses max) as a reference for convergence test." <<std::endl;
//        }
//        if ( _use_evals_for_dataprofiles )
//        {
//            std::cout  << "\t Use number of evals instead of nbevals/(n+1)=number of simplex grad for x axis on data profiles" <<std::endl;
//
//        }
//    }
//
//}


/*------------------------------------------------*/
/*          display instance name (private)       */
/*------------------------------------------------*/
void RUNNERPOST::Runner::display_instance_name (const Problem & pb, const  Algorithm & ac, size_t i_pb_instance) const
{
    std::cout << "\t (" << pb.get_id() << ", " << ac.get_id() << ")";
    if (i_pb_instance < INF_SIZE_T && pb.get_nbPbInstances()> 1)
    {
        std::cout << ", pb run instance " << pb.get_pbInstance()[i_pb_instance];
    }
}


/*------------------------------------*/
/*        display test configs        */
/*------------------------------------*/
void RUNNERPOST::Runner::display_selected_algos ( void ) const
{

    // TODO:
    // - check for seed consistency between algos -> just a warning if number is not the same.
    // - manage add seed to stats file. If there is no seed.
    
    // TODO: For unconstrained pb, if f(x_first_line) differs for each algo, we cannot take f0=f(x_first_line). The display a warning and use f0=max(f(x_first_line)_algos.
    
    // TODO: Implement instance-base, cross-instance, litterature choices for the selection of f*.
    
    // TODO: flag cases with f0 = f* (no algo improved upon f0)
    
    // TODO: For constrained pb. For F0, we can use min(f_feas)_algos or max(f_feas)_algos
    
    size_t n = _selected_algos.size();

    std::cout << std::endl;

    if ( n == 0 )
        std::cout << "there is no algo configuration" << std::endl;
    else
    {

        std::ostringstream msg;
        msg << "selected algo";
        if ( n > 1 )
            msg << "s (" << n << ")";
        msg << ":\n";

        std::cout << msg.str();
        for ( size_t k = 0 ; k < n ; ++k )
        {
            std::cout << "\t " << _selected_algos[k]->get_id();
            std::cout << ": (" << _selected_algos[k]->get_name() << ") ";
            const auto & options = _selected_algos[k]->get_output_options();
            if (options.size() > 0)
            {
                std::cout << " [ ";
                size_t count = 1;
                
                for (const auto & o: options )
                {
                    std::cout << o << "]";
                    if (count < options.size())
                    {
                        std::cout << " [ ";
                    }
                    count++;
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}



/*---------------------------------------*/
/*  construct a list of files            */
/*  (static, private)                    */
/*---------------------------------------*/
bool RUNNERPOST::Runner::construct_list_of_files ( std::list<std::string> & list_of_files ,
                                                    const std::string      & directory      ) const
{

    if ( ! list_of_files.empty() )
        list_of_files.clear();
    
    try {
        
        // loop over all files in the directory
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_regular_file() && ! entry.path().filename().string().starts_with("."))
            {
                list_of_files.push_back(entry.path().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "Filesystem error: " << err.what() << std::endl;
        return false;
    }

    return true;
}


/*---------------------------------------*/
/*  construct a list of sub-directories  */
/*  (static, private)                    */
/*---------------------------------------*/
bool RUNNERPOST::Runner::construct_list_of_subdirs ( std::list<std::string> & list_of_dirs ,
                                                    const std::string      & directory      ) const
{

    if ( ! list_of_dirs.empty() )
        list_of_dirs.clear();
    
    try {
        
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            if (entry.is_directory())
            {
                list_of_dirs.push_back(entry.path().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "Filesystem error: " << err.what() << std::endl;
        return false;
    }

    return true;
}

/*------------------------------------------------*/
/*   Set a result obtained by an algo on a pb     */
/*   for all instances                            */
/*   Call compute_solution and displays brief     */
/*   solution info.                               */
/*   Single objective only                        */
/*------------------------------------------------*/
void RUNNERPOST::Runner::set_result (const std::string        & test_id /*not used*/ ,
                         Result                     result[],
                         const Problem            & pb      ,
                         const  Algorithm         & ac        )
{
    // Set single objective result
    
    if ( ac.getNbObjectives() > 1 )
        return;
    
    // For now we consider all evaluations available
    // size_t  bbe = pb.getMaxBBEvals();
    size_t  bbe = INF_SIZE_T;
    
    // bbe corresponds to the desired max number of bb evaluations,
    //  and not necessarily to the last entry in the stats file, while
    //  xe and fxe  correspond to the best solution (last entry in the
    //  stats file).
    for (size_t i_pb_instance=0 ; i_pb_instance < pb.get_nbPbInstances() ; ++i_pb_instance )
    {
        
        display_instance_name ( pb , ac , i_pb_instance );
        std::cout << ": found in " << Runner::get_test_dir ( ac , pb ) << ": ";
        
        if ( result[i_pb_instance].compute_solution ( pb.get_n() ,
                                                     bbe        ))
        {
            auto time = result[i_pb_instance].get_time(result[i_pb_instance].get_sol_bbe());
            std::string time_str = "";
            if (time > 0)
            {
                time_str = " time=" + std::to_string(time);
            }
            std::cout << "bbe="   << result[i_pb_instance].get_sol_bbe ()
            << time_str
            << " f="    << result[i_pb_instance].get_sol_fx  ()
            << " fx0=" << result[i_pb_instance].get_sol(1)
            << " ffx=" << result[i_pb_instance].get_first_fx();
            std::cout << std::endl;
        }
        else if ( result[i_pb_instance].compute_best_infeasible( pb.get_n() ,
                                                                 bbe        ))
        {
            auto time = result[i_pb_instance].get_time(result[i_pb_instance].get_sol_bbe());
            std::string time_str = "";
            if (time > 0)
            {
                time_str = " time=" + std::to_string(time);
            }
            std::cout << "bbe="   << result[i_pb_instance].get_sol_bbe ()
            << time_str
            << " h="    << result[i_pb_instance].get_sol_fx  ()
            << " hx0=" << result[i_pb_instance].get_sol(1);
            std::cout << std::endl;
        }
        else
        {
            std::cout << "no solution (feas. or infeas)" << std::endl;
        }
    }
}

/*------------------------------------------------*/
/*              set a result (private)            */
/*------------------------------------------------*/
void RUNNERPOST::Runner::set_hypervolume_result ()
{
    const size_t n_algo = static_cast<int>(_selected_algos.size());
    const size_t n_pb = static_cast<int>(_selected_pbs.size());
    
    if (!_use_hypervolume_for_profiles)
    {
        std::cerr << "Error: set_hypervolume_result is called but flag _use_hypervolume_for_profiles is not set" << std::endl;
        return;
        
    }
    
    int xmax = _selected_outputs[0]->get_x_max();
    for (size_t i_o = 1; i_o < _selected_outputs.size() ; i_o++ )
    {
        if (xmax != _selected_outputs[0]->get_x_max())
        {
            std::cerr << "Error: set_hypervolume_result. Cannot handle the case with multiple selected outputs havin different xmax." << std::endl;
            return;
        }
    }
    if (xmax < RUNNERPOST::P_INF_INT)
    {
        std::cerr << "Error: set_hypervolume_result. Cannot handle the case with xmax < INF." << std::endl;
        return;
    }
    int bbeMax = RUNNERPOST::P_INF_INT;

    
    // Initialize reference combined pareto (empty) for all pbs
    _refParetoIdealPtAllAlgos = std::vector<std::vector<double>>(n_pb);
    _refParetoNadirPtAllAlgos = std::vector<std::vector<double>>(n_pb);
    _combinedParetoAllAlgos = std::vector<std::vector<std::vector<double>>>(n_pb);

    // loop on the problems:
    for ( size_t i_pb = 0 ; i_pb <  n_pb; ++i_pb )
    {
        const auto & pb = *_selected_pbs[i_pb];
        
        int n = pb.get_n();
        auto n_pb_inst = pb.get_nbPbInstances();
        
        if (pb.get_m()< 2)
        {
            std::cout << "/t Pb #" << pb.get_id() << " is not multi-objective. Cannot compute hypervolume." << std::endl;
            return;
        }
        
        // loop on the algorithm:
        // For a problem combine all pareto front obtained and the ideal and nadir reference
        // This is use to compute hypervolume
        for ( size_t i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            
            // This is partial because it is for all seeds of a single algo
            std::vector<std::vector<double>> partialCombinedPareto;
            for (size_t i_pb_inst = 0 ; i_pb_inst < n_pb_inst ; i_pb_inst++)
            {
                
                std::cout << "\t pb #" << i_pb + 1 << " algo #" << i_algo+1 << ". Partial pareto of seeds runs: " <<std::endl;
                _results[i_pb][i_algo][i_pb_inst].update_pareto (  bbeMax         ,
                                                                 partialCombinedPareto);
                std::cout << "\t \t - Instance " << i_pb_inst+1 << ", " << partialCombinedPareto.size() << " pts" << std::endl;
            }
            
            
            std::cout << std::endl;
            
            // Clock
            time_t t0,t1;
            time(&t0);
            
            // Update combined pareto with partial
            addToCombinedPareto(partialCombinedPareto, i_pb);
            
            time(&t1);
            
            std::cout << "\t pb #" << i_pb + 1 << " algo 1->" <<  i_algo +1 <<  ". Combined pareto of seeds runs: " << _combinedParetoAllAlgos[i_pb].size() << " pts. Comput. time: " << difftime(t1,t0) << " s" << std::endl;
            
            std::cout << std::endl;
            
        }
        
        auto nb_obj = _results[i_pb][0][0].get_nbNbObjs();
        // Set the reference ideal and nadir pts for pb.
        if (_combinedParetoAllAlgos[i_pb].size() > 0)
        {
            _refParetoIdealPtAllAlgos[i_pb] = std::vector<double>(nb_obj, RUNNERPOST::INF);
            _refParetoNadirPtAllAlgos[i_pb] = std::vector<double>(nb_obj, RUNNERPOST::M_INF);
            for (const auto & p : _combinedParetoAllAlgos[i_pb])
            {
                for (size_t j=0; j < nb_obj ; j++)
                {
                    _refParetoIdealPtAllAlgos[i_pb][j] = std::min( p[j], _refParetoIdealPtAllAlgos[i_pb][j]);
                    _refParetoNadirPtAllAlgos[i_pb][j] = std::max( p[j], _refParetoNadirPtAllAlgos[i_pb][j]);
                }
            }
        }
        else
        {
            std::cout << "/t Cannot set results (ref pts) for hypervolumes of pb " << _selected_pbs[i_pb]->get_id() << ". Reference hypervolume is not defined." << std::endl;
        }
        
        // Compute hypervolume solution for all pb instances and all algos
        for ( size_t i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            const auto & ac = *_selected_algos[i_algo];
            for (size_t i_pb_inst=0 ; i_pb_inst < n_pb_inst ; ++i_pb_inst )
            {
                Result * result = &_results[i_pb][i_algo][i_pb_inst];
                display_instance_name (pb , ac , i_pb_inst );
                std::cout << ": found in " << Runner::get_test_dir ( ac , pb ) << ": ";
                // Clock
                time_t t0,t1;
                time(&t0);
                if (result->compute_hypervolume_solution(n, bbeMax, _combinedParetoAllAlgos[i_pb], _refParetoIdealPtAllAlgos[i_pb], _refParetoNadirPtAllAlgos[i_pb]) )
                {
                    time(&t1);
                    std::cout << "bbe="   << result->get_sol_bbe ()
                    << " optim time=" << result->get_time(result->get_sol_bbe())
                    << " nb_pareto_points="    << result->get_nb_pareto_points  ()
                    << " pareto_dominating_ref_obj="    << result->get_nb_dominating_ref_obj()
                    << " HV_f="    << result->get_sol_fx  ()
                    << " HV_fx0=" << result->get_sol(1)
                    << " compute time=" << difftime(t1,t0) << " s" << std::endl;
                }
                else
                {
                    std::cout << "no solution" << std::endl;
                }
            }
        }
    }
}

///*-------------------------------------------------------*/
///*               read and add algo parameter file        */
///*                   (read id file 1/4)                  */
///*-------------------------------------------------------*/
//bool Runner::read_algo_params_file ( const std::string  & algo_params_file_name ,
//                                    std::string        & error_msg        )
//{
//
//    error_msg.clear();
//
//    std::ifstream fin ( algo_params_file_name.c_str() );
//
//    if ( fin.fail() )
//    {
//        fin.close();
//        error_msg = "cannot read file " + algo_params_file_name;
//        return false;
//    }
//
//    std::string s , runner_version;
//
//    while ( s != "RUNNER" && !fin.eof() )
//    {
//        fin >> s;
//        NOMAD_BASE::toupper(s);
//
//        if ( fin.fail() )
//        {
//            fin.close();
//            error_msg = "error(1) in file " + algo_params_file_name;
//            return false;
//        }
//    }
//
//    fin >> runner_version;
//    if ( fin.fail() )
//    {
//        fin.close();
//        error_msg = "error(2) in file " + algo_params_file_name;
//        return false;
//    }
//
//    if ( runner_version != RUNNER_VERSION )
//    {
//        fin.close();
//        error_msg = "error(3) in file " + algo_params_file_name + " (runner version incompatible with this runner)";
//        return false;
//    }
//
//    std::string file_solver_name , file_solver_version;
//
//    while ( s != "SOLVER" && !fin.eof() )
//    {
//
//        fin >> s;
//        NOMAD_BASE::toupper(s);
//
//        if ( fin.fail() )
//        {
//            fin.close();
//            error_msg = "error(4) in file " + algo_params_file_name;
//            return false;
//        }
//    }
//    if ( fin.eof() )
//    {
//        fin.close();
//        error_msg = "error(5) in file " + algo_params_file_name + ": SOLVER keyword absent";
//        return false;
//    }
//    fin >> file_solver_name >> file_solver_version; // Read line "# SOLVER SOLVER_NAME SOLVER_VERSION"
//
//    if ( fin.fail() )
//    {
//        fin.close();
//        error_msg = "error(6) in file " + algo_params_file_name + ": no solver name/version after keyword SOLVER";
//        return false;
//    }
//    NOMAD_BASE::toupper(file_solver_name);
//
//    std::string algo_legend;
//    while ( s != "ALGO_LEGEND" && !fin.eof() )
//    {
//
//        fin >> s;
//        NOMAD_BASE::toupper(s);
//
//        if ( fin.fail() )
//        {
//            fin.close();
//            error_msg = "error(5) in file " + algo_params_file_name + ": ALGO_LEGEND keyword absent";
//            return false;
//        }
//    }
//    if ( fin.eof() )
//    {
//        fin.close();
//        error_msg = "error(5) in file " + algo_params_file_name + ": ALGO_LEGEND keyword absent";
//        return false;
//    }
//    getline ( fin , algo_legend );
//
//    if ( fin.fail() )
//    {
//        fin.close();
//        error_msg = "error(6) in file " + algo_params_file_name + ": no legend after keyword ALGO_LEGEND";
//        return false;
//    }
//
//    fin.close();
//
//    // create the algorithm parameters from the id file:
//    if ( file_solver_name.compare("NOMAD")==0 )
//    {
//    }
//#ifdef USE_FMS_INTERFACE_1_0
//    else if ( file_solver_name.compare("FMS_INTERFACE")==0 )
//    {
//        if ( _use_h_for_profiles )
//        {
//            error_msg = "error(8) FMS solver does not support data profiles on h";
//            return false;
//        }
//        if ( file_solver_version.compare("1.0")==0 )
//        {
//            size_t n_seed_run= _algoRunSeeds.size();
//            if ( n_seed_run > 1 )
//            {
//                error_msg = "error(8) FMS solver does not support multiple seeds ";
//                return false;
//            }
//            ap = new Algo( ) ;
//        }
//    }
//#endif
//#ifdef USE_GRIDNM_INTERFACE_1_0
//    else if ( file_solver_name.compare("GRIDNM_INTERFACE")==0 )
//    {
//        if ( _use_h_for_profiles )
//        {
//            error_msg = "error(8) GRIDNM solver does not support data profiles on h";
//            return false;
//        }
//        if ( file_solver_version.compare("1.0")==0 )
//        {
//            size_t n_seed_run= _algoRunSeeds.size() ;
//            if ( n_seed_run > 1 )
//            {
//                error_msg = "error(8) GridNM solver does not support multiple seeds ";
//                return false;
//            }
//            ap = new GridNM_Interface_1_0::Parameters( ) ;
//        }
//    }
//#endif
//    else
//    {
//        fin.close();
//        error_msg = "error(8) Solver " +file_solver_name +" not supported ";
//        return false;
//    }
//
//
//
//    _selected_algos.push_back ( new AlgoParameters ( file_solver_name , file_solver_version , algo_params_file_name  , _n_algo ) );
//    _selected_algo_legends.push_back( algo_legend );
//
//    _n_algo++ ;
//
//    return true;
//}


/*-------------------------------------------------------*/
/*               read and add algo config file           */
/*-------------------------------------------------------*/
bool RUNNERPOST::Runner::read_algo_selection_file ( const std::string  & algo_selection_file_name ,
                                    std::string        & error_msg        )
{

    error_msg.clear();

    std::ifstream in ( algo_selection_file_name.c_str(), std::ios::in );
    if ( in.fail() )
    {
        in.close();
        error_msg = "Error(0). Cannot read file " + algo_selection_file_name;
        return false;
    }
    
    while(!in.eof())
    {
        std::string line;
        getline (in , line);
        
        if (line.empty())
        {
            continue;
        }
        
        _selected_algos.push_back(new Algorithm(line, error_msg));
        if (!error_msg.empty())
        {
            in.close();
            return false;
        }
    }

    in.close();
    
    if (_selected_algos.empty())
    {
        error_msg = "Error(1) in file " + algo_selection_file_name + ". Cannot read a single algo config. First line in file must contain an algo config." ;
        return false;
    }

    return true;
}

bool RUNNERPOST::Runner::read_algo_selection ( const std::string  & algo_selection_formatted ,
                                                std::string        & error_msg        )
{

    error_msg.clear();

    if ( algo_selection_formatted.empty() )
    {
        error_msg = "Error(0). Cannot read formatted string. It is empty. " ;
        return false;
    }

    // Read the algo selection from formatted string
    std::stringstream in ( algo_selection_formatted.c_str(), std::ios::in );

    while(!in.eof())
    {
        std::string line;
        getline (in , line);
        
        if (line.empty())
        {
            continue;
        }
        
        _selected_algos.push_back(new Algorithm(line, error_msg));
        if (!error_msg.empty())
        {
            return false;
        }
    }
    
    if (_selected_algos.empty())
    {
        error_msg = "Error(1) in file " + algo_selection_formatted + ". Cannot read a single algo config. First line in string must contain an algo config." ;
        return false;
    }

    return true;
}



/*-----------------------------------------------------------*/
/*          read and add problems from selection file           */
/*-----------------------------------------------------------*/
bool RUNNERPOST::Runner::read_problem_selection_file ( const std::string  & pb_selection_file_name ,
                                                      std::string        & error_msg        )
{

    error_msg.clear();

    std::ifstream in ( pb_selection_file_name.c_str(), std::ios::in );
    if ( in.fail() )
    {
        in.close();
        error_msg = "Error(0). Cannot read file " + pb_selection_file_name;
        return false;
    }
    while(!in.eof())
    {
        std::string line;
        getline (in , line);
        
        if (line.empty())
        {
            continue;
        }
        
        // Manage the case where the problem are defined by a directory
        // The problem selection file contains only the symbol *.
        // The problems are defined in the directories for the algos.
        // Here the * is detected and the pb selection is delayed after
        if (line.find("*") == 0)
        {
            in.close();
            std::cerr << "Select pbs from the content of the algos directories.";
            
            return read_problem_selection_from_algo_dir(error_msg);
        }
        
        _selected_pbs.push_back(new Problem(line, error_msg));
        
        if (!error_msg.empty())
        {
            in.close();
            return false;
        }
    }
    in.close();

    
    if (_selected_pbs.empty())
    {
        in.close();
        error_msg = "Error(1) in file " + pb_selection_file_name + ". Cannot read a single pb config. First line in file must contain an algo config." ;
        return false;
    }
    

    return true;
}

bool RUNNERPOST::Runner::read_problem_selection_from_algo_dir ( std::string        & error_msg        )
{
    error_msg.clear();
    
    // Read the problems from the algo directories
    // Check that some conditions are met
    std::vector<std::string> algo_dirs;
    for (const auto & algo: _selected_algos)
    {
        auto sotList = algo->get_stats_output_type_list();
        
        // TODO: check if the case with FEAS, SOL and OBJ can be handled.
        // TODO: what about multi objectives
        
        bool hasConst = (std::count(sotList.begin(),sotList.end(),StatOutputType::CST) > 0);
        bool constIsUniqueAndLast = ((sotList.back() == StatOutputType::CST) && std::count(sotList.begin(),sotList.end(),StatOutputType::CST) == 1);
        
        // bool hasSol = (std::count(sotList.begin(),sotList.end(),StatOutputType::SOL) > 0); // Let's try to use some info (like DIM = xx or SOL) in stats file to figure out the pb dimension
//        size_t nbObj = std::count(sotList.begin(),sotList.end(),StatOutputType::OBJ);
//
//        if (nbObj > 1)
//        {
//            error_msg = "Error. Algo " + algo->get_id() + " has more than one objective in the output. Cannot select problems from algo directories.";
//            return false;
//        }
        
        if (hasConst && !constIsUniqueAndLast)
        {
            error_msg = "Error. Algo " + algo->get_id() + " has constraints in the output. Cannot deduce problems definition from algo directories. Provide explicit problem definition ";
            return false;
        }
        
        algo_dirs.push_back( algo->get_id());
    }
    
    // Use the first algo dir to read the problems directories
    std::list<std::string> pb_dirs;
    // Construct the pb directories from the first algo dir
    if (!construct_list_of_subdirs(pb_dirs, algo_dirs[0]))
    {
        error_msg = "Error. Cannot read pb directories from algo dir " + algo_dirs[0];
        return false;
    }
    
    // Case no pb dir found.
    // Let's try to read the pb files directly from the algo dir
    // Get the list of all result files in a pb dir
    bool pbFromParse = false;
    if (pb_dirs.empty())
    {
        pbFromParse = true;
        pb_dirs.push_back(algo_dirs[0]);
    }
    
    auto sotList = _selected_algos[0]->get_stats_output_type_list();
    
    // We suppose the instance name is "0"
    const std::string defPbInst = "0";
    
    // Use a reference list of the selected problems created with the first algo.
    // After that, make sure to compare what is obtained with the remaining algos.
    
    std::vector<Problem>         ref_selected_pbs;
    
    // Read the first line of each pb dir to get the pb id, the number of instances, the dimension (n), the number of objectives and constraints (m)
    for (const auto & pb: pb_dirs)
    {
        // Get the list of all result files in a pb dir
        std::list<std::string> res_files;
        if (!construct_list_of_files(res_files, pb))
        {
            error_msg = "Error. Cannot read pb result files from pb dir " + pb;
            return false;
        }

        if (res_files.size() > 1 && pb_dirs.size() > 1)
        {
            error_msg = "Error. More than one instances cannot be managed to read pb result files from several pb dirs " + pb;
            return false;
        }
          
        for (const auto & res_file: res_files)
        {
            // Create a problem definition using the available information from the result file
            Problem pbDef(res_file, sotList, defPbInst, pbFromParse, error_msg);
            
            if (!error_msg.empty())
            {
                return false;
            }
                        
            // Check if this is consistent with the algo stats file name
            // We suppose the instance name is 0
            auto statFileName = get_stats_file_name(*_selected_algos[0], pbDef /*Not used*/, defPbInst);
            if (!pbFromParse && res_files.front().find(statFileName) == std::string::npos)
            {
                error_msg = "Error. The stats file name " + statFileName + " does not match the pb result file name " + res_files.front();
                return false;
            }
            
            // Let's add the pb definition to the reference
            ref_selected_pbs.push_back(pbDef);
        }
    }
        
    // Check the consistency of pb dirs with the other algo dirs
    for (size_t i =1 ; i < algo_dirs.size() ; i++)
    {
        std::list<std::string> pb_dirs;
        if (!construct_list_of_subdirs(pb_dirs, algo_dirs[i] ))
        {
            error_msg = "Error. Cannot read pb directories from algo dir " + algo_dirs[i];
            return false;
        }
        
        // Case no pb dir found.
        // Let's try to read the pb files directly from the algo dir
        // Get the list of all result files in a pb dir
        bool pbFromParse = false;
        if (pb_dirs.empty())
        {
            pbFromParse = true;
            pb_dirs.push_back(algo_dirs[i]);
        }
        
        // Read the first line of each pb dir to get the pb id, the number of instances, the dimension (n), the number of objectives and constraints (m)
        for (const auto & pb: pb_dirs)
        {
            // Get the list of all result files in a pb dir
            std::list<std::string> res_files;
            if (!construct_list_of_files(res_files, pb))
            {
                error_msg = "Error. Cannot read pb result files from pb dir " + pb;
                return false;
            }
            
            if (res_files.size() > 1 && pb_dirs.size() > 1)
            {
                error_msg = "Error. More than one instances cannot be managed to read pb result files from several pb dirs " + pb;
                return false;
            }
            
            auto algoSotList = _selected_algos[i]->get_stats_output_type_list();
            
            // TODO check the same conditions as for algo 0
            
            for (const auto & res_file: res_files)
            {
                // Create a pb from a problem result file
                Problem algoPbDef(res_file, algoSotList, defPbInst, pbFromParse, error_msg);
                
                // Compare algoPbDef with the reference pb having the same id
                bool pbMatch = false;
                for (const auto & refPb: ref_selected_pbs)
                {
                    if (refPb.get_id() == algoPbDef.get_id())
                    {
                        pbMatch = (refPb.get_n() == algoPbDef.get_n()); // Just need to compare the pb dimension
                        break;
                    }
                }
                if (!pbMatch)
                {
                    error_msg = "Error. The pb " + algoPbDef.get_id() + " from algo dir " + algo_dirs[i] + " does not match the pb from the first algo dir " + algo_dirs[0];
                    return false;
                }
            }
        }
    }

    // Read the algo selection from formatted string
    if (ref_selected_pbs.empty())
    {
        error_msg = "Error. Cannot read a single problem config from optimization results." ;
        return false;
    }
    
    // Copy the reference selected pbs to the selected pbs
    for (const auto & pb: ref_selected_pbs)
    {
        _selected_pbs.push_back(new Problem(pb));
    }
    
    return true;

}


bool RUNNERPOST::Runner::addToCombinedPareto(const std::vector<std::vector<double>> & paretoPoints, const size_t & pbIndex )
{
    bool updated_pareto = false;

    if (_combinedParetoAllAlgos[pbIndex].size() == 0)
    {
        _combinedParetoAllAlgos[pbIndex] = paretoPoints;
        updated_pareto = true;
    }
    else
    {
        for (const auto & pt: paretoPoints)
        {
            updated_pareto = addToCombinedPareto(pt, pbIndex) || updated_pareto ;
        }
    }
    return updated_pareto;
}

bool RUNNERPOST::Runner::addToCombinedPareto(const std::vector<double> & pt, const size_t & pbIndex )
{

    bool updated_pareto = false;

    bool insert = true;
    std::vector<std::vector<double>>::iterator itPf = _combinedParetoAllAlgos[pbIndex].begin();
    while (itPf != _combinedParetoAllAlgos[pbIndex].end())
    {
        // Create a fake eval point for using compMO
        auto moCompFlag = RUNNERPOST::Result::compMultiObjForDominate(pt,*itPf);
        if (moCompFlag == RUNNERPOST::MOCompareType::DOMINATED || moCompFlag == RUNNERPOST::MOCompareType::EQUAL)
        {
            return false;
        }
        if (moCompFlag == RUNNERPOST::MOCompareType::DOMINATING)
        {
            itPf = _combinedParetoAllAlgos[pbIndex].erase(itPf);
            updated_pareto = true;
            continue;
        }
        itPf++;
    }
    if (insert)
    {
        _combinedParetoAllAlgos[pbIndex].push_back(pt);
        updated_pareto = true;
    }
    return updated_pareto;

}

bool RUNNERPOST::Runner::read_output_selection_file( const std::string  & output_selection_file_name ,
                                                    std::string        & error_msg )
{
    std::ifstream in(output_selection_file_name , std::ios::in);
    if ( in.fail() )
    {
        in.close();
        std::cerr << "\n Error reading output_selection file " << output_selection_file_name << std::endl;
        return false;
    }
    
    while(!in.eof())
    {
        std::string line;
        getline (in , line);
        
        if (line.empty())
        {
            continue;
        }
        
        _selected_outputs.push_back(new Output(line, error_msg));
        if (!error_msg.empty())
        {
            in.close();
            return false;
        }

    }

    in.close();
    return true;
}

// Used by Python interface only
// Similar to read_output_selection_file
bool RUNNERPOST::Runner::read_output_selection ( const std::string  & output_selection_formatted ,
                                                std::string        & error_msg        )
{
    error_msg.clear();

    if ( output_selection_formatted.empty() )
    {
        error_msg = "Error(0). Cannot read formatted string. It is empty. " ;
        return false;
    }

    // Read the algo selection from formatted string
    std::stringstream in ( output_selection_formatted.c_str(), std::ios::in );

    while(!in.eof())
    {
        std::string line;
        getline (in , line);
        
        if (line.empty())
        {
            continue;
        }
        
        _selected_outputs.push_back(new Output(line, error_msg));
        if (!error_msg.empty())
        {
            return false;
        }
    }
    
    if (_selected_outputs.empty())
    {
        error_msg = "Error(1) in file " + output_selection_formatted + ". Cannot read a single output config. First line in string must contain a valid output config." ;
        return false;
    }

    return true;

}

void RUNNERPOST::Runner::display_selected_outputs   ( void ) const
{
    
    // TODO check that output files do not have the same name.
    
    size_t n = _selected_outputs.size();

    std::cout << std::endl;

    if ( n == 0 )
        std::cout << "there is no output configuration" << std::endl;
    else
    {

        std::ostringstream msg;
        msg << "selected outputs";
        if ( n > 1 )
            msg << "s (" << n << ")";
        msg << ":\n";

        std::cout << msg.str();
        for ( size_t k = 0 ; k < n ; ++k )
        {
            std::cout << "\t " << RUNNERPOST::Output::profileTypeToString(_selected_outputs[k]->get_profile_type());
            const auto & options = _selected_outputs[k]->get_profile_type_options();
            if (options.size() > 0)
            {
                std::cout << " [ ";
                size_t count = 1;
                
                for (const auto & o: options )
                {
                    std::cout << o << "]";
                    if (count < options.size())
                    {
                        std::cout << " [ ";
                    }
                    count++;
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    
}



/*-------------------------------------*/
/*  get the results (static, private)  */
/*    (read id file 3/4)               */
/*-------------------------------------*/
bool RUNNERPOST::Runner::get_results(const std::string    & test_id /*not used*/ ,
                                     const Problem        & pb      ,
                                     const Algorithm      & ac      ,
                                     Result                 result[])
{

    std::vector<std::string> pbIs = pb.get_pbInstance();
    if (pbIs.size() == 0)
    {
        // Default pb instance is 0.
        // If flag add pb instance to stats file name is set to yes, this id is automatically added to filename.
        pbIs.push_back("0");
    }
    
    size_t i_pb_instance = 0;
    for ( const auto & pbInstance : pbIs )
    {
        std::ifstream fin;
        
        std::string stats_file_name = Runner::get_test_dir(ac, pb) + Runner::get_stats_file_name(ac, pb, pbInstance);
        

        // check the results (stats file):
        fin.open ( stats_file_name.c_str() );

        if ( fin.fail() )
        {
            fin.close();
            result[i_pb_instance].reset();
            return false;
        }
        
        // Prepare the stats file complete format from the pb info and the algo stat output type
        StatOutputTypeList statsFileFormat = composeStatsFileFormat(ac.get_stats_output_type_list(), pb.get_n(), pb.get_m());
        
        // Read the stats file into results
        if ( !result[i_pb_instance].read ( fin , INF_SIZE_T /*for now we consider all evaluations */ , statsFileFormat, _feasibilityThreshold )  )
        {
            fin.close();
            result[i_pb_instance].reset();
            return false;
        }
        fin.close();

        size_t res_bbe = result[i_pb_instance].get_last_bbe();

        if ( res_bbe <= 0 )
        {
            result[i_pb_instance].reset();
            return false;
        }
        i_pb_instance++;
    }
    
    return true;
}

///*----------------------------------------------*/
///*  find a problem in the list of all problems  */
///*  (private)                                   */
///*----------------------------------------------*/
//Problem * Runner::find_problem ( const std::string & problem_id ) const
//{
//    size_t n = _all_pbs.size();
//    for ( size_t k = 0 ; k < n ; ++k )
//        if ( _all_pbs[k]->get_id() == problem_id )
//            return _all_pbs[k];
//    return NULL;
//}



//// Partial: for all seeds of a single algo (provided indirectly by test_id)
//void Runner::writePartialCombinedPareto(const std::string      & test_id ,
//                                        const Problem           & pb     ,
//                                        const AlgoParameters    & ap     ,
//                                        std::vector<NOMAD_BASE::Point> & partialCombinedPareto) const
//{
//    // No partial combined pareto. Get the data from the results.
//    // This section can be run in parallel before going to getResults.
//    if (partialCombinedPareto.empty())
//    {
//        Result result;
//
//        std::ifstream fin;
//        size_t i_seed = 0;
//        for ( const auto & seed : _algoRunSeeds )
//        {
//            i_seed++;
//            std::string statsFileName = Runner::get_stats_file_name(test_id, pb, seed, true, true /*full path */);
//            fin.open(statsFileName.c_str());
//            if (fin.fail())
//            {
//                std::cout<<"Cannot open stats file" << statsFileName << " for pb " << pb.get_id() << std::endl;
//                break;
//            }
//            result.reset(_use_hypervolume_for_profiles, _use_h_for_profiles);
//            result.read ( fin , pb.getMaxBBEvals ( ) , pb.get_m() , pb.get_bbot() , pb.get_n() , _feasibilityThreshold );
//            fin.close();
//            std::vector<NOMAD_BASE::Point> temp;
//            result.update_pareto (  pb.getMaxBBEvals ( ),
//                                    partialCombinedPareto    );
//            std::cout << "\t pb #"  << pb.get_index() +1 << " algo #" << ap.getIndex()+1 << ". Partial combined pareto for seeds 1->" << i_seed << ": " << partialCombinedPareto.size() << " pts" << std::endl;
//        }
//    }
//
//    std::string combinedParetoFile = pb.get_tests_dir() + test_id + "/combinedPareto.txt";
//    std::cout << "\t pb #"  << pb.get_index() +1 << " algo #" << ap.getIndex()+1 << ". Write combined pareto output: " << combinedParetoFile << std::endl;
//    std::ofstream fout( combinedParetoFile.c_str(), std::ofstream::trunc );
//    for (const auto & pt: partialCombinedPareto )
//    {
//        fout << pt.displayNoPar() << std::endl;
//    }
//    fout.close();
//}


/*-----------------------------------------*/
/*        access to the date (private)     */
/*        (CPU name is also added)         */
/*-----------------------------------------*/
std::string RUNNERPOST::Runner::get_date ( void ) const
{

    std::ostringstream tmp_file;


    tmp_file << "tmpfordate.txt" ;

    std::string s ,
    cmd = "date \"+%Y-%m-%d, %H:%M:%S\" > " + tmp_file.str() + " 2> /dev/null";

    if (0 != system ( cmd.c_str() ))
    {
        std::cerr << "Error in get_date" << std::endl;
        return "";
    }

    std::ifstream in ( (tmp_file.str()).c_str() );

    getline ( in , s );

    in.close();

    remove ( (tmp_file.str()).c_str() );

    if ( in.fail() )
        return "";


    return s;
}



void RUNNERPOST::Runner::add_pbinstance_to_file_name ( const std::string       &  pbInstance,
                                                      std::string        &  file_name   )
{
    int n_pn = static_cast<int>(file_name.size());

    if ( n_pn == 0 )
        return;

    int         k   = static_cast<int>(file_name.find_last_of("."));
    std::string ext = "";
    std::string fic = file_name;

    if ( k >= 0 && k < n_pn )
    {
        fic  = file_name.substr ( 0 , k      );
        ext  = file_name.substr ( k , n_pn-k );
        n_pn = k;
    }


    file_name = fic + "." + pbInstance + ext;
}

bool RUNNERPOST::Runner::output_dataperf_profile_pgfplots(const Output & out ) const
{
    RUNNERPOST::Output::Profile_Type profile_type = out.get_profile_type();
    
    if (profile_type == RUNNERPOST::Output::Profile_Type::CONVERGENCE_PROFILE)
    {
        std::cerr << "\n Error: Use output_convergence_profile_pgfploats." << std::endl;
        return false;
    }
    
    // Get the plain file name
    std::string plain_file_name = out.get_plain_file_name();

    std::ifstream infile(plain_file_name);
    if (! infile.good() )
    {
        std::cerr << "\n Error in output_selection: file " << plain_file_name << " does not exist for latex profile " << std::endl;
        infile.close();
        return false;
    }
    
    // Modify plain file name to include the key "step"
    std::string plain_file_name_step = plain_file_name + ".step";
    
    // Get the latex file name
    std::string latex_file_name = out.get_latex_file_name();
    
    std::cout<< "\t Writing of " << latex_file_name << " and " << plain_file_name_step << " ......";
    
    // Create data for plotting data profile with step
    std::ofstream outfile_step(plain_file_name_step);
    
    // Need a flag to reverse the step orientation: forward or backward
    bool forwardAdd = (out.get_profile_type()!=RUNNERPOST::Output::Profile_Type::ACCURACY_PROFILE);
    // For backward adding, the first value added is at zero (accuracy profiles for instance)

    // The loop for reading plain_file_name and writing to plain_file_name_step
    std::string line, prevLine, prevLineToken, lineFirstToken, nextLine, lineToken;
    while (std::getline(infile, line))
    {
        if (line.empty())
        {
            std::cerr << "Cannot read line from " << plain_file_name << ". Empty lines are present." << std::endl;
            return false;
        }
        // Add forward step
        if (forwardAdd)
        {
            if (!prevLine.empty())
            {
                // Read the first token of the line
                std::istringstream iss(line);
                
                iss >> lineFirstToken;
                
                // Write the first token of the previous line to the new line
                outfile_step << lineFirstToken << " ";
                
                // Write all tokens of prevLine, except the first one
                std::istringstream issPrev(prevLine);
                issPrev >> prevLineToken;
                while (std::getline(issPrev, prevLineToken, ' '))
                {
                    if (prevLineToken.empty())
                    {
                        continue;
                    }
                    outfile_step << prevLineToken << " ";
                }
                outfile_step << std::endl;
            }
            prevLine = line;
            outfile_step << line << std::endl;
        }
        // Add backward step
        else
        {
            if (!lineFirstToken.empty())
            {
                outfile_step << lineFirstToken << " ";

            }
            else
            {
                outfile_step << "0 ";
            }
            // Read the line token by token
            std::istringstream iss(line);

            iss >> lineFirstToken;

            // Write all tokens of line, except the first one (already done)
            while (std::getline(iss, lineToken, ' '))
            {
                if (lineToken.empty())
                {
                    continue;
                }
                outfile_step << lineToken << " ";
            }
            outfile_step << std::endl;
            outfile_step << line << std::endl;
        }
    }
    outfile_step.close();
    infile.close();

    // Graph title
    std::string profileTitle = out.get_title();


    // Output tex file from previous strings
    std::ofstream out_tex ( latex_file_name , std::ofstream::out | std::ofstream::trunc );
    out_tex << "\\documentclass{standalone}" <<std::endl;
    out_tex << "\\usepackage{pgfplots} " <<std::endl;
    out_tex << "\\pgfplotsset{width=10cm,compat=1.16} " <<std::endl;
    out_tex << "\\begin{document}" <<std::endl;
    out_tex << "\\begin{tikzpicture} "<<std::endl;
    out_tex << "\\begin{axis}[ " << std::endl;

    out_tex << "       title = {" << profileTitle << "}," << std::endl;

    if (RUNNERPOST::Output::Profile_Type::DATA_PROFILE == profile_type)
    {
        out_tex << "       xmin=-10, ymin = -0.05, ymax= 1.05," << std::endl;
        if (RUNNERPOST::Output::X_Select::EVAL == out.get_x_select())
        {
            out_tex << "       xlabel = {Number of evaluations}, " <<std::endl;
        }
        else if (RUNNERPOST::Output::X_Select::NP1EVAL == out.get_x_select())
        {
            out_tex << "       xlabel = {Groups of ($n_p+1$) evaluations $k$}," <<std::endl;
        }
        else if (RUNNERPOST::Output::X_Select::TIME == out.get_x_select())
        {
            out_tex << "       xlabel = Time," <<std::endl;
        }
        else
        {
            std::cerr << "\n Error:  x_select type is not available for latex profile " << std::endl;
            out_tex.close();
            return false;
        }
        
        if (RUNNERPOST::Output::Y_Select::OBJ == out.get_y_select())
        {
            out_tex << "       ylabel = {Portion of {$\\tau$}-solved instances $d_a(k)$}," <<std::endl;
        }
        else if (RUNNERPOST::Output::Y_Select::INFEAS == out.get_y_select())
        {
            out_tex << "       ylabel = {Portion of feasible instances}," <<std::endl;
        }
        else
        {
            std::cerr << "\n Error:  y_select type is not available for latex profile " << std::endl;
            out_tex.close();
            return false;
        }
        
    }
    else if (RUNNERPOST::Output::Profile_Type::PERFORMANCE_PROFILE == profile_type)
    {
        out_tex << "       xmin=1, ymin = -0.05, ymax= 1.05," << std::endl;
        out_tex << "       xlabel = {Ratio of function evaluations $\\alpha$}," <<std::endl;
        
        if (RUNNERPOST::Output::Y_Select::OBJ == out.get_y_select())
        {
            out_tex << "       ylabel = {Portion of {$\\tau$}-solved instances $\\rho_a(\\alpha)$}," <<std::endl;
        }
        else if (RUNNERPOST::Output::Y_Select::INFEAS == out.get_y_select())
        {
            out_tex << "       ylabel = {Portion of feasible instances}," <<std::endl;
        }
        else
        {
            std::cerr << "\n Error:  y_select type is not available for latex profile " << std::endl;
            out_tex.close();
            return false;
        }
    }
    else if (RUNNERPOST::Output::Profile_Type::CONVERGENCE_PROFILE == profile_type)
    {
        out_tex << "       xmin=1," << std::endl;
        out_tex << "       xlabel = {Number of function evaluations}," <<std::endl;
        
        if (RUNNERPOST::Output::Plot_Type::OnlyF == out.get_plot_type())
        {
            out_tex << "       ylabel = {Best objective function value}," <<std::endl;
        }
        if (RUNNERPOST::Output::Plot_Type::OnlyFFeasible == out.get_plot_type())
        {
            out_tex << "       ylabel = {Best objective function value (feasible)}," <<std::endl;
        }
        else if (RUNNERPOST::Output::Plot_Type::OnlyHInfeasible == out.get_plot_type())
        {
            out_tex << "       ylabel = {Infeasibility function value}," <<std::endl;
        }
        else
        {
            std::cerr << "\n Error: History file plot type is not available for latex profile " << std::endl;
            out_tex.close();
            return false;
        }
    }
    else if (RUNNERPOST::Output::Profile_Type::ACCURACY_PROFILE == profile_type)
    {
        out_tex << "       xmin=0," << std::endl;
        out_tex << "       xlabel = {Relative accuracy $d$}," <<std::endl;
        out_tex << "       ylabel = {Portion of instances solved $r_a(d)$}," <<std::endl;
    }


    out_tex << " legend style={ " << std::endl;
    out_tex << "    font=\\small, " <<std::endl;
    out_tex << "    cells={anchor=southeast}, " << std::endl;
    out_tex << "    at={(1,-0.2)}, " <<std::endl;
    out_tex << "   legend cell align=left, } ]" <<std::endl;
    
    //Concatenate algo info to create legends:
    std::vector<std::string> legends;
    
    if (_selected_algos.empty())
    {
        std::cerr << "\n Error:  no algos selected." << std::endl;
        out_tex.close();
        return false;
    }
    
    for (const auto & algo: _selected_algos)
    {
        legends.push_back(algo->get_name());
    }

    int y_index = 1;
    int color_index = 0;
    int symbol_index = 0;
    
    // Maximum number of algorithms that can be plotted
    const size_t maxAlgos = std::min(SYMBOLS.size(),COLORS.size());
    if ( _selected_algos.size() > maxAlgos )
    {
        if (SYMBOLS.size() != COLORS.size())
        {
            std::cerr << "\n Warning:  number of colors and symbols do not match." << std::endl;
        }
        std::cerr << "\n Warning:  not enough symbols/colors for the number of algo. Let's plot only the first " << std::to_string(maxAlgos) << " algos." << std::endl;
    }
    
    // int repeat_mark = 10;
    size_t nbPlottedAlgos = 0;
    std::string lineStyle = "solid";
    
    // For some profile we may need to plot with only marks
    // lineStyle = "only marks";

    if (RUNNERPOST::Output::Profile_Type::DATA_PROFILE == profile_type ||
        RUNNERPOST::Output::Profile_Type::PERFORMANCE_PROFILE == profile_type ||
        RUNNERPOST::Output::Profile_Type::ACCURACY_PROFILE == profile_type)
    {
        for (const auto & leg : legends )
        {
            out_tex << "  \\addplot [" << lineStyle << ", mark="<< SYMBOLS[symbol_index++] << ", mark repeat = 20, color=" << COLORS[color_index++] << "] table [x index = 0, y index = " << y_index++ << " , header = false ] {" << plain_file_name_step << "}; " << std::endl ;
            out_tex << "\\addlegendentry{" << leg << "};" <<std::endl;
            nbPlottedAlgos++;
            if (nbPlottedAlgos >= maxAlgos)
            {
                break;
            }
        }
    }
    else if (RUNNERPOST::Output::Profile_Type::CONVERGENCE_PROFILE == profile_type)
    {
        out_tex << "  \\addplot [" << lineStyle << ", mark="<< SYMBOLS[0] << ", mark repeat = 20, color=" << COLORS[0] << "] table [x index = 0, y index = 1, header = false ] {" << plain_file_name_step << "}; " << std::endl ;
    }

    out_tex << " \\end{axis} " << std::endl;
    out_tex << " \\end{tikzpicture}" << std::endl;
    out_tex << " \\end{document}" <<std::endl;

    out_tex.close();

    std::cout << " done " << std::endl;

    return true;

}


bool RUNNERPOST::Runner::output_convergence_profile_pgfplots(const Output & out ) const
{
    RUNNERPOST::Output::Profile_Type profile_type = out.get_profile_type();
    
    if (profile_type != RUNNERPOST::Output::Profile_Type::CONVERGENCE_PROFILE)
    {
        std::cerr << "\n Error: Use output_profile_pgfploats." << std::endl;
        return false;
    }
    
    size_t i_pb, i_algo, i_pb_instance;
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    std::vector<std::string> listFileNames;
    std::vector<std::string> listLegends;
    
    for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
    {
        for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                if (! out.plotIsSelected(_selected_algos[i_algo]->get_id(), _selected_pbs[i_pb]->get_id(),i_pb_instance))
                    continue;
                
                auto extension = "."+_selected_algos[i_algo]->get_id()+"."+_selected_pbs[i_pb]->get_id()+".Inst"+std::to_string(i_pb_instance);
                auto legend = "Algo: " + _selected_algos[i_algo]->get_id()+" Pb: "+_selected_pbs[i_pb]->get_id()+" Instance: "+std::to_string(i_pb_instance);
                
                
                listFileNames.push_back(out.get_plain_file_name()+extension);
                listLegends.push_back(RUNNERPOST::removeChar(legend,'_'));
            }
        }
    }
    if (listFileNames.empty())
    {
        std::cerr << "\n Error in output_convergence_profile_pgfplots: No history file to plot in latex." << std::endl;
        return false;
    }
    
    std::string latexFileName = out.get_latex_file_name() ;
    
    std::cout<< "\t Writing of " ;
    for (const auto& plain_file_name: listFileNames)
    {
        std::ifstream infile(plain_file_name);
        if (! infile.good() )
        {
            std::cerr << "\n Error in output_selection: file " << plain_file_name << " does not exist for latex profile " << std::endl;
            infile.close();
            return false;
        }
        
        // Modify plain file name to include the key "step"
        std::string plain_file_name_step = plain_file_name + ".step";
        
        std::cout << plain_file_name_step << " ...";
        
        // Create data for plotting data profile with step
        std::ofstream outfile_step(plain_file_name_step);
        
        // The loop for reading plain_file_name and writing to plain_file_name_step
        std::string line, prevLine, prevLineToken, lineFirstToken;
        while (std::getline(infile, line))
        {
            if (line.empty())
            {
                std::cout << std::endl;
                std::cerr << "Cannot read line from " << plain_file_name << ". Empty lines are present." << std::endl;
                return false;
            }
            if (!prevLine.empty())
            {
                // Read the first token of the line
                std::istringstream iss(line);
                
                iss >> lineFirstToken;
                
                // Write the first token of the previous line to the new line
                outfile_step << lineFirstToken << " ";
                
                // Write all tokens of prevLine, except the first one
                std::istringstream issPrev(prevLine);
                issPrev >> prevLineToken;
                while (std::getline(issPrev, prevLineToken, ' '))
                {
                    if (prevLineToken.empty())
                    {
                        continue;
                    }
                    outfile_step << prevLineToken << " ";
                }
                outfile_step << std::endl;
            }
            prevLine = line;
            outfile_step << line << std::endl;
            
        }
        outfile_step.close();
        infile.close();
    }
    std::cout << " done " << std::endl;
    
    // Graph title
    std::string profileTitle = out.get_title();
    
    std::cout<< "\t Writing of " << latexFileName << " ......";
    
    // Output tex file from previous strings
    std::ofstream out_tex ( latexFileName , std::ofstream::out | std::ofstream::trunc );
    out_tex << "\\documentclass{standalone}" <<std::endl;
    out_tex << "\\usepackage{pgfplots} " <<std::endl;
    out_tex << "\\pgfplotsset{width=10cm,compat=1.16} " <<std::endl;
    out_tex << "\\begin{document}" <<std::endl;
    out_tex << "\\begin{tikzpicture} "<<std::endl;
    out_tex << "\\begin{axis}[ " << std::endl;
    out_tex << "       title = {" << profileTitle << "}," << std::endl;
    
    if (RUNNERPOST::Output::X_Select::EVAL == out.get_x_select())
    {
        out_tex << "       xmin=1," << std::endl;
        out_tex << "       xlabel = {Number of evaluations}, " <<std::endl;
    }
    else if (RUNNERPOST::Output::X_Select::TIME == out.get_x_select())
    {
        out_tex << "       xmin=0," << std::endl;
        out_tex << "       xlabel = Time," <<std::endl;
    }
    else
    {
        std::cerr << "\n Error:  x_select type is not available for latex profile " << std::endl;
        out_tex.close();
        return false;
    }
    
    
    if (RUNNERPOST::Output::Plot_Type::OnlyF == out.get_plot_type())
    {
        out_tex << "       ylabel = {Objective function}," <<std::endl;
    }
    if (RUNNERPOST::Output::Plot_Type::OnlyFFeasible == out.get_plot_type())
    {
        out_tex << "       ylabel = {Objective function (feasible)}," <<std::endl;
    }
    else if (RUNNERPOST::Output::Plot_Type::OnlyHInfeasible == out.get_plot_type())
    {
        out_tex << "       ylabel = {Infeasibility measure}," <<std::endl;
    }
    else
    {
        std::cerr << "\n Error: plot type is not available for latex History profile " << std::endl;
        out_tex.close();
        return false;
    }
    
    
    out_tex << " legend style={ " << std::endl;
    out_tex << "    font=\\small, " <<std::endl;
    out_tex << "    cells={anchor=southeast}, " << std::endl;
    out_tex << "    at={(1,-0.2)}, " <<std::endl;
    out_tex << "   legend cell align=left, } ]" <<std::endl;
    
    int color_index = 0;
    int symbol_index = 0;
    
    // Maximum number of algorithms that can be plotted
    const size_t maxPlots = std::min(SYMBOLS.size(),COLORS.size());
    if ( listFileNames.size() > maxPlots )
    {
        if (SYMBOLS.size() != COLORS.size())
        {
            std::cerr << "\n Warning:  number of colors and symbols do not match." << std::endl;
        }
        std::cerr << "\n Warning:  not enough symbols/colors for the number of algo. Let's plot only the first " << std::to_string(maxPlots) << " plots." << std::endl;
    }
    
    size_t nbPlotted = 0;
    std::string lineStyle = "solid";
    
    std::vector<std::string>::iterator itLeg = listLegends.begin();
    for (const auto & plain_file_name : listFileNames )
    {
        // Modify plain file name to include the key "step"
        std::string plain_file_name_step = plain_file_name + ".step";

        out_tex << "  \\addplot [" << lineStyle << ", mark="<< SYMBOLS[symbol_index++] << ", mark repeat = 20, color=" << COLORS[color_index++] << "] table [x index = 0, y index = 1, header = false ] {" << plain_file_name_step << "}; " << std::endl ;
        out_tex << "\\addlegendentry{" << *itLeg << "};" <<std::endl;
        nbPlotted++;
        itLeg++;
        if (nbPlotted >= maxPlots)
        {
            break;
        }
    }
    
    
    out_tex << " \\end{axis} " << std::endl;
    out_tex << " \\end{tikzpicture}" << std::endl;
    out_tex << " \\end{document}" <<std::endl;
    
    out_tex.close();
    
    std::cout << " done " << std::endl;
    
    return true;
    
}

bool RUNNERPOST::Runner::output_combo_convergence_profile_pgfplots(const Output & out ) const
{
    RUNNERPOST::Output::Profile_Type profile_type = out.get_profile_type();
    
    if (profile_type != RUNNERPOST::Output::Profile_Type::CONVERGENCE_PROFILE)
    {
        std::cerr << "\n Error: Use output_profile_pgfploats." << std::endl;
        return false;
    }
    if ( out.get_plot_type() != RUNNERPOST::Output::Plot_Type::ComboHInfAndFFeas )
    {
        std::cerr << "\n Error: Only for Combo convergence plots." << std::endl;
        return false;
    }
    
    
    size_t i_pb, i_algo, i_pb_instance;
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    std::vector<std::string> listFileNames;
    std::vector<std::string> listLegends;
    
    for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
    {
        for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_nbPbInstances() ; ++i_pb_instance )
            {
                if (! out.plotIsSelected(_selected_algos[i_algo]->get_id(), _selected_pbs[i_pb]->get_id(),i_pb_instance))
                    break;
                
                auto extensionF = "."+_selected_algos[i_algo]->get_id()+"."+_selected_pbs[i_pb]->get_id()+".Inst"+std::to_string(i_pb_instance)+".F";
                auto extensionH = "."+_selected_algos[i_algo]->get_id()+"."+_selected_pbs[i_pb]->get_id()+".Inst"+std::to_string(i_pb_instance)+".H";
                
                auto legend = "Algo. " + _selected_algos[i_algo]->get_id()+" Pb. "+_selected_pbs[i_pb]->get_id()+" Inst. "+std::to_string(i_pb_instance);
                
                listFileNames.push_back(out.get_plain_file_name()+extensionF);
                listFileNames.push_back(out.get_plain_file_name()+extensionH);
                listLegends.push_back(legend);
            }
        }
    }
    if (listFileNames.empty())
    {
        std::cerr << "\n Error in output_convergence_profile_pgfplots: No history file to plot in latex." << std::endl;
        return false;
    }
    
    size_t lastBbe = 0;
    
    std::string latexFileName = out.get_latex_file_name() ;
    
    std::cout<< "\t Writing of " ;
    for (const auto& plain_file_name: listFileNames)
    {
        std::ifstream infile(plain_file_name);
        if (! infile.good() )
        {
            std::cerr << "\n Error in output_selection: file " << plain_file_name << " does not exist for latex profile " << std::endl;
            infile.close();
            return false;
        }
        
        // Modify plain file name to include the key "step"
        std::string plain_file_name_step = plain_file_name + ".step";
        
        std::cout << plain_file_name_step << " ...";
        
        // Create data for plotting data profile with step
        std::ofstream outfile_step(plain_file_name_step);
        
        // The loop for reading plain_file_name and writing to plain_file_name_step
        std::string line, prevLine, prevLineToken, lineFirstToken;
        while (std::getline(infile, line))
        {
            if (line.empty())
            {
                std::cout << std::endl;
                std::cerr << "Cannot read line from " << plain_file_name << ". Empty lines are present." << std::endl;
                return false;
            }
            if (!prevLine.empty())
            {
                // Read the first token of the line
                std::istringstream iss(line);
                
                iss >> lineFirstToken;
                
                // Write the first token of the previous line to the new line
                outfile_step << lineFirstToken << " ";
                
                // Write all tokens of prevLine, except the first one
                std::istringstream issPrev(prevLine);
                issPrev >> prevLineToken;
                while (std::getline(issPrev, prevLineToken, ' '))
                {
                    if (prevLineToken.empty())
                    {
                        continue;
                    }
                    outfile_step << prevLineToken << " ";
                }
                outfile_step << std::endl;
            }
            prevLine = line;
            outfile_step << line << std::endl;
            
        }
        
        // Get last bbe from the line
        if (!prevLine.empty())
        {
            lastBbe = std::max<size_t>(lastBbe, std::stoul(prevLine.substr(0, prevLine.find(" "))));
        }
        else
        {
            std::cerr << "Cannot read last line from " << plain_file_name << ". Empty lines are present. Let's continue." << std::endl;
        }
        outfile_step.close();
        infile.close();
    }
    std::cout << " done " << std::endl;
    
    // Graph title
    std::string profileTitle = out.get_title();
    
    std::cout<< "\t Writing of " << latexFileName << " ......";
    
    // Output tex file from previous strings
    std::ofstream out_tex ( latexFileName , std::ofstream::out | std::ofstream::trunc );
    out_tex << "\\documentclass{standalone}" <<std::endl;
    out_tex << "\\usepackage{pgfplots} " <<std::endl;
    out_tex << "\\pgfplotsset{width=10cm,compat=1.16} " <<std::endl;
    out_tex << "\\begin{document}" <<std::endl;
    out_tex << "\\begin{tikzpicture} "<<std::endl;
    out_tex << "\\begin{axis}[ " << std::endl;
    out_tex << "       title = {" << profileTitle << "}," << std::endl;
    out_tex << "       xmin=1, xmax=" << std::to_string(lastBbe) << "," << std::endl;
    out_tex << "       xlabel = {Number of function evaluations}," <<std::endl;
    out_tex << "       ylabel = {Best objective function value $f$}," <<std::endl;
    out_tex << "       ylabel near ticks," << std::endl;
    out_tex << "       axis y line*=right," << std::endl;
    out_tex << " legend style={ " << std::endl;
    out_tex << "    font=\\small, " <<std::endl;
    out_tex << "    cells={anchor=southeast}, " << std::endl;
    out_tex << "    at={(1,-0.2)}, " <<std::endl;
    out_tex << " legend cell align=left, } ]" <<std::endl;
    
    int color_index = 0;
    int symbol_index = 0;
    
    // Maximum number of algorithms that can be plotted
    const size_t maxPlots = std::min(SYMBOLS.size(),COLORS.size());
    if ( listFileNames.size() > maxPlots )
    {
        if (SYMBOLS.size() != COLORS.size())
        {
            std::cerr << "\n Warning:  number of colors and symbols do not match." << std::endl;
        }
        std::cerr << "\n Warning:  not enough symbols/colors for the number of algo. Let's plot only the first " << std::to_string(maxPlots) << " plots." << std::endl;
    }
    
    size_t nbPlotted = 0;
    std::string lineStyle = "solid";
    size_t j = 0;
    for (size_t i =0 ; i < listFileNames.size()-1 ; i+=2)
    {
        // Modify plain file name to include the key "step"
        std::string plain_file_name_step = listFileNames[i] + ".step";

        out_tex << "  \\addplot [" << lineStyle << ", mark="<< SYMBOLS[symbol_index++] << ", mark repeat = 20, color=" << COLORS[color_index++] << "] table [x index = 0, y index = 1, header = false ] {" << plain_file_name_step << "}; " << std::endl ;
        out_tex << "\\addlegendentry{" << listLegends[j++] << "};" <<std::endl;
        nbPlotted++;
        if (nbPlotted >= maxPlots)
        {
            break;
        }
    }
    out_tex << " \\end{axis} " << std::endl;

    symbol_index = 0;
    color_index = 0;
    nbPlotted = 0;
    out_tex << "\\begin{axis}[ " << std::endl;
    out_tex << "       ylabel = {Constraint violation value $h$ (dotted)}," <<std::endl;
    out_tex << "       xmin=1, xmax=" << std::to_string(lastBbe) << "," << std::endl;
    out_tex << "       axis y line*=left,"  <<std::endl;
    out_tex << "       xlabel near ticks," <<std::endl;
    out_tex << "       hide x axis, ]" <<std::endl;
    std::string lineStyleDotted = "dotted";
    for (size_t i = 1 ; i < listFileNames.size() ; i+=2)
    {
        // Modify plain file name to include the key "step"
        std::string plain_file_name_step = listFileNames[i] + ".step";

        // Only dotted
        out_tex << "  \\addplot [" << lineStyleDotted << ", color=" << COLORS[color_index] << "] table [x index = 0, y index = 1, header = false ] {" << plain_file_name_step << "}; " << std::endl ;
        
        // Only Marks
        out_tex << "  \\addplot [only marks, mark="<< SYMBOLS[symbol_index++] << ", mark repeat = 20, color=" << COLORS[color_index++] << "] table [x index = 0, y index = 1, header = false ] {" << plain_file_name_step << "}; " << std::endl ;
        nbPlotted++;
        if (nbPlotted >= maxPlots)
        {
            break;
        }
    }
    out_tex << " \\end{axis} " << std::endl;
    out_tex << " \\end{tikzpicture}" << std::endl;
    out_tex << " \\end{document}" <<std::endl;
    
    out_tex.close();
    
    std::cout << " done " << std::endl;
    
    return true;
    
}

bool RUNNERPOST::Runner::output_profile_pgfplots(const Output & out) const
{
    if (out.get_latex_file_name().empty())
    {
        return true;
    }
    
    std::string plain_file_name = out.get_plain_file_name();
    
    if (out.get_plain_file_name().empty())
    {
        std::cerr << "\n Error in output_selection: To output in latex the output_plain is mandatory. " << std::endl;
        return false;
    }
    
    if (_selected_algos.empty())
    {
        std::cerr << "\n Error:  no algos available." << std::endl;
        return false;
    }

    
    std::list<std::string> listFileNames;
    if (out.get_profile_type() == RUNNERPOST::Output::Profile_Type::CONVERGENCE_PROFILE)
    {
        if (out.get_plot_type() == RUNNERPOST::Output::Plot_Type::ComboHInfAndFFeas)
        {
            return output_combo_convergence_profile_pgfplots(out);
        }
        else
        {
            return output_convergence_profile_pgfplots(out);
        }
        return output_convergence_profile_pgfplots(out);
    }
    else
    {
        return output_dataperf_profile_pgfplots(out);
    }
    

}



std::string RUNNERPOST::Runner::get_stats_file_name (const Algorithm    & ac      ,
                                                     const Problem      & pb      ,
                                                     const std::string  & pb_inst    )
{
    // When the pb has been obtained by parsing Algo directory, the stats file name is obtained from the pb id.
    if (pb.get_pbFromParse())
    {
        return "";
    }
    
    std::string statsFileName = Algorithm::DEFAULT_STATS_FILE_NAME;
    bool flag_add_pbinstance_to_stats_file = Algorithm::DEFAULT_ADD_PBINSTANCE_TO_STATS_FILE;

    for (const auto & o: ac.get_output_options())
    {
        if ( o.find("STATS_FILE_NAME") !=std::string::npos)
        {
            std::string oTmp = o;
            std::vector<std::string> sWords = RUNNERPOST::extract_words(oTmp);
            if (sWords.size() != 2)
            {
                std::cerr << "Cannot properly extract STATS_FILE_NAME from algorithm output options" << std::endl;
                statsFileName = "";
                return statsFileName;
            }
            statsFileName = sWords[1];
        }
        if ( o.find("ADD_PBINSTANCE_TO_STATS_FILE") !=std::string::npos)
        {
            std::string oTmp = o;
            std::vector<std::string> sWords = RUNNERPOST::extract_words(oTmp);
            if (sWords.size() != 2)
            {
                std::cerr << "Cannot properly extract ADD_PBINSTANCE_TO_STATS_FILE from algorithm output options" << std::endl;
                statsFileName = "";
                return statsFileName;
            }
            RUNNERPOST::toUpperCase(sWords[1]);
            flag_add_pbinstance_to_stats_file = (sWords[1]=="YES" || sWords[1]=="TRUE" || sWords[1] =="1");
        }
    }
    if ( flag_add_pbinstance_to_stats_file )
    {
        add_pbinstance_to_file_name ( pb_inst , statsFileName );
    }
    
    return statsFileName;
}


bool RUNNERPOST::Runner::algo_pb_check_consistency(std::string       & error_msg) const
{
    // The check is only for pb_instance. If more than one instance on one problem
    // we must have the option ADD_PBINSTANCE_TO_STATS_FILE true or
    // the DEFAULT_ADD_PBINSTANCE....
    const std::string base_error_msg = "Pb(s) with more than one pb instance are selected. We need to have the option ADD_PBINSTANCE_TO_STATS_FILE enabled for ";
    error_msg = "";
    
    bool has_pb_instance = false;
    for (const auto & pb: _selected_pbs)
    {
        if (pb->get_pbInstance().size() > 0 )
        {
            has_pb_instance = true;
            break;
        }
    }
    
    if (!has_pb_instance)
    {
        return true;
    }
    
    for (const auto & algo: _selected_algos)
    {
        error_msg = base_error_msg + algo->get_id() + " (" + algo->get_name() + ")\n";
        
        std::vector<std::string> sWords = algo->get_output_option("ADD_PBINSTANCE_TO_STATS_FILE");
        if (sWords.empty() && !RUNNERPOST::Algorithm::DEFAULT_ADD_PBINSTANCE_TO_STATS_FILE)
        {
            return false;
        }
        bool flag_add_pbinstance_to_stats_file = (sWords[1]=="YES" || sWords[1]=="TRUE" || sWords[1] =="1");
        if (!flag_add_pbinstance_to_stats_file)
        {
            return false;
        }
    }
    error_msg = "";
    return true;
}



RUNNERPOST::StatOutputTypeList RUNNERPOST::Runner::composeStatsFileFormat(const RUNNERPOST::StatOutputTypeList & acSotList , const size_t & n, const size_t & m) const
{
    RUNNERPOST::StatOutputTypeList completeSotList;
    for (const auto & acSot: acSotList)
    {
        if (acSot.isOfType(StatOutputType::SOL))
        {
            RUNNERPOST::StatOutputTypeList tmp(n, StatOutputType::SOL);
            completeSotList.insert(completeSotList.end(), tmp.begin(), tmp.end());
        }
        else if (acSot.isOfType(StatOutputType::CST))
        {
            const size_t numberOfConstraints = m - RUNNERPOST::getNbObj(acSotList);
            if (numberOfConstraints <= 0)
            {
                completeSotList.clear();
                return completeSotList;
            }
            RUNNERPOST::StatOutputTypeList tmp(numberOfConstraints, StatOutputType::CST);
            completeSotList.insert(completeSotList.end(), tmp.begin(), tmp.end());
        }
        else
        {
            completeSotList.push_back(acSot);
        }
    }
    return completeSotList;
    
}
