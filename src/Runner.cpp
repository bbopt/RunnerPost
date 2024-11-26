#include "Runner.hpp"

#include <ctime>
#include <fstream>

/*----------------------------------*/
/*            constructor           */
/*----------------------------------*/
RUNNERPOST::Runner::Runner ( ) :
_results    ( NULL ) ,
_test_id    ( NULL ) ,
_use_avg_fx_first_feas( false ) ,
_use_evals_for_dataprofiles ( false ) ,
_use_h_for_profiles ( false ),
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
        
        auto n_pb_inst = _selected_pbs[i_pb]->get_pbInstance().size();
        if (n_pb_inst == 0)
        {
            n_pb_inst = 1;
        }
        
        // loop on the algorithm parameters:
        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            
            // result:
            _results [i_pb][i_algo] = new Result [ n_pb_inst ];
            
            for (size_t i_pb_inst = 0 ; i_pb_inst < n_pb_inst ; i_pb_inst++)
            {
                _results[i_pb][i_algo][i_pb_inst].reset( _use_hypervolume_for_profiles, _use_h_for_profiles );
            }
            
            if ( !RUNNERPOST::Runner::get_results ( _test_id              [i_pb][i_algo]  ,
                                                   *_selected_pbs         [i_pb]          ,
                                                   *_selected_algos       [i_algo]        ,
                                                   _results               [i_pb][i_algo]   ) )
            {
                display_instance_name ( *_selected_pbs[i_pb] , *_selected_algos[i_algo] );
                std::cout << ": cannot get result" << std::endl << std::endl;
                error_msg = "cannot get result";
                return false;
            }
            
            // set the result:
            set_result ( _test_id              [i_pb][i_algo] ,
                        _results               [i_pb][i_algo] ,
                        *_selected_pbs         [i_pb  ]       ,
                        *_selected_algos       [i_algo]         );
        }
    }
    
    
    // 4- Set the combined pareto results
    if ( _use_hypervolume_for_profiles )
    {
        std::cerr << "Hyper volume results are not yet available" << std::endl;
        // set_hypervolume_result();
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
        auto pt = out->get_profile_type();
        
        if (Output::Profile_Type::DATA_PROFILE == pt)
        {
            auto xSel = out->get_x_select();
            auto ySel = out->get_y_select();
            if (Output::X_Select::TIME == xSel)
            {
                output_time_data_profile_plain(*out);
            }
            else
            {
                output_data_profile_plain(*out);
            }
            output_profile_pgfplots(*out);
        }
        else if (Output::Profile_Type::PERFORMANCE_PROFILE == pt)
        {
            error_msg = "Performance profile. TODO.";
            return false;
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


/*-----------------------------------------------------*/
/*  check if fx is at alpha % relatively close to fxe  */
/*  (static, private)                                  */
/*-----------------------------------------------------*/
bool RUNNERPOST::Runner::is_within ( const double & fx    ,
                        const double & fxe   ,
                        const double & alpha   )
{
    if ( fxe != 0.0 )
        return ( std::fabs( (fx - fxe) / fxe ) <= alpha / 100.0 );

    return ( fabs(fx) <= alpha / 100.0 );
}

/*---------------------------------------------*/
/*  compute relative error between fx and fxe  */
/*  (static, private)                          */
/*---------------------------------------------*/
double RUNNERPOST::Runner::compute_alpha ( const double & fx  ,
                                          const double & fxe   )
{
    if ( fxe != 0.0 )
        return std::fabs(100.0 * ( (fx - fxe) / fxe ));
    return std::fabs(fx) * 100.0;
}

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
                        !(*_results[k][i] == *_results[k][j]) /* TODO: implement comparison of result */ ) {
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
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance )
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
        std::cout << "... the following results are missing" ;
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
    ArrayOfDouble fx0s = get_fx0s();

    // get the best solution for each problem:
    ArrayOfDouble fxe = get_best_fx();


    // compute tpsMin and alpha_max (Moré and Wild  2009, eq. 2.1)
    // -------------------------
    std::vector<size_t> tpsMin;
    size_t tpsMinTmp;
    double alpha_max = INF;
    size_t bbe_max=get_bbe_max();
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        tpsMinTmp=bbe_max+1;
        if ( fx0s[i_pb] < INF )
        {
            for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
            {
                for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance )
                {
                    if ( _results[i_pb][i_algo][i_pb_instance].has_solution() )
                    {
                        size_t bbe = 1;
                        for ( bbe = 1 ; bbe <= bbe_max ; ++bbe )
                        {
                            if (fx0s[i_pb]-_results[i_pb][i_algo][i_pb_instance].get_sol(bbe) >= (1-out.get_tau())*(fx0s[i_pb]-fxe[i_pb]) )
                            {
                                if (bbe<tpsMinTmp)
                                {
                                    tpsMinTmp=bbe;
                                }
                                break;
                            }
                        }
                        if ( alpha_max == INF ||
                            (tpsMinTmp!=bbe_max+1 && alpha_max < bbe/tpsMinTmp))
                        {
                            alpha_max=bbe/tpsMinTmp;

                        }
                    }
                }
            }
        }
        tpsMin.push_back(tpsMinTmp);
    }


    size_t cnt;
    
    const size_t PP_NB_LINES = 100; // TEMP for building
    double dalpha = 20 / (PP_NB_LINES-1.0);
    double alpha                = 0.0;

    for (size_t i = 0 ; i < PP_NB_LINES ; ++i)
    {
        for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
        {
            cnt = 0;
            for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
                for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_algo]->get_pbInstance().size(); ++i_pb_instance )
                    if ( _results[i_pb][i_algo][i_pb_instance].has_solution() )
                    {
                        for ( size_t bbe = 1 ; bbe <= bbe_max ; ++bbe )
                        {
                            if (fx0s[i_pb]-_results[i_pb][i_algo][i_pb_instance].get_sol(bbe) >= (1-out.get_tau())*(fx0s[i_pb]-fxe[i_pb]) )
                            {
                                if (bbe/tpsMin[i_pb]<=alpha)
                                    cnt++;
                                break;
                            }
                        }
                    }

            fout << " ";
        }
        fout << std::endl;
        alpha += dalpha;
    }


    fout.close();

    std::cout << "... done" << std::endl;
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
    
    if ( out.get_tau() < 0 || n_pb == 0 || n_algo == 0 )
    {
        std::cerr << "Error: cannot compute data profile for tau < 0 or n_pb ==0 or n_algo == 0" << std::endl;
        return false;
    }


    std::ofstream fout ( out.get_plain_file_name() );
    if ( fout.fail() ) {
        std::cerr << "Error: cannot create data profile output file "
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
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance )
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
    ArrayOfDouble fx0s = get_fx0s();
    
    // TODO Failsafe for Fx0
//    if ( fx0s.isComplete())
//    {
//        std::cerr << "Error: Undefined fx0" << std::endl;
//        fout << "failed to obtain data profile" << std::endl;
//        fout.close();
//        return false;
//    }
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        if ( fx0s[i_pb]==INF )
        {
            std::cout << "pb #" << i_pb+1 << " ---> fx0=Inf --> un-resolved"<<std::endl;
        }
    }

    // get the best solution for each problem:
    ArrayOfDouble fxe = get_best_fx();

    if ( _use_h_for_profiles )
    {
        bool flag_ok = false;
        for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
        {
            if ( !flag_ok && fxe[i_pb] == 0 )
                flag_ok = true;
            if ( fxe[i_pb] == INF )
                std::cout << "pb #" << i_pb+1 << " ---> no run returned a feasible point!"<<std::endl;
        }
        if ( ! flag_ok )
        {
            std::cerr << "Error: at least one run should return a feasible point"<<std::endl;
            return false;
        }
    }

    // compute the data profile:
    // -------------------------
    int max_alpha = Problem::getNbSimplexEvals() ;

    // Update the range for x axis according to all problems considered (take the max of all problems max_bb_eval)
    // Warning pb max_bb_eval may depend on the value of _nb_simplex_evals
    if ( _use_evals_for_dataprofiles )
    {
        max_alpha = 0;
        for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
        {
            max_alpha = std::max (max_alpha , _selected_pbs[i_pb]->getMaxBBEvals() );
        }
    }
//    int dim_all_pb=0;
//    if ( _scale_xaxis_by_np1_for_dataprofiles )
//    {
//        if ( ! _use_evals_for_dataprofiles )
//        {
//            std::cerr << "Error: Option scale_axis_by_np1 available only with option use_evals_for_dataprofiles" << std::endl;
//            fout.close();
//            return false;
//        }
//        dim_all_pb = _selected_pbs[0]->get_n();
//        for ( i_pb = 1 ; i_pb < _n_pb ; ++i_pb )
//        {
//            if ( dim_all_pb != _selected_pbs[i_pb]->get_n() )
//            {
//                std::cerr << "Error: Option scale_axis_by_np1 not available when problems have different dimension" << std::endl;
//                fout.close();
//                return false;
//            }
//        }
//    }
    

    size_t cnt, cnt_pb_instance;
    for (int alpha = 0 ; alpha <= max_alpha ; ++alpha )
    {

            fout << alpha << " ";

        for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
        {
            cnt = 0;
            cnt_pb_instance = 0;
            for (i_pb = 0 ; i_pb < n_pb ; ++i_pb)
            {
                auto n_pb_instance = _selected_pbs[i_pb]->get_pbInstance().size();
                cnt_pb_instance += n_pb_instance;
                
                // Use evals instead of (n+1)*evals
                size_t dimPb= ( _use_evals_for_dataprofiles ) ? 0 : _selected_pbs[i_pb]->get_n();
                if ( fx0s[i_pb] < INF && fxe[i_pb] < INF )
                {
                    for ( i_pb_instance = 0 ; i_pb_instance < n_pb_instance ; ++i_pb_instance )
                    {
                        if ( fx0s[i_pb]-_results[i_pb][i_algo] [i_pb_instance].get_sol(alpha*(dimPb+1)) >= (1-out.get_tau())*(fx0s[i_pb]-fxe[i_pb]) )
                            ++cnt;
                    }
                }
            }
            fout << (100.0 * cnt ) / (cnt_pb_instance) << " " ;
        }
        fout << std::endl;

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
    std::list<int> miss_list;
    for (size_t i_pb = 0; i_pb < n_pb; ++i_pb)
    {
        for (size_t i_algo = 0; i_algo < n_algo; ++i_algo)
        {
            for (size_t i_pb_instance = 0; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size(); ++i_pb_instance)
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
        std::cout << "... the following results are missing" ;
        std::list<int>::const_iterator it;
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
            auto n_pb_instance = _selected_pbs[i_pb]->get_pbInstance().size();
            
            for (size_t i_pb_instance = 0 ; i_pb_instance < n_pb_instance ; ++i_pb_instance)
            {
                int bbe = _results[i_pb][i_algo][i_pb_instance].getTotalBbe();
                double time = _results[i_pb][i_algo][i_pb_instance].getTotalTime();
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
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance )
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
    ArrayOfDouble fx0s = get_fx0s();
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        if ( fx0s[i_pb] == INF )
        {
            std::cout << "pb #" << i_pb+1 << " ---> fx0=Inf --> un-resolved"<<std::endl;
        }
    }
    // get the best solution for each problem:
    ArrayOfDouble fxe = get_best_fx();
    if ( _use_h_for_profiles )
    {
        bool flag_ok = false;
        for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
        {
            if ( !flag_ok && fxe[i_pb] == 0 )
                flag_ok = true;
            if ( fxe[i_pb] == INF )
                std::cout << "pb #" << i_pb+1 << " ---> no run returned a feasible point!"<<std::endl;
        }
        if ( ! flag_ok )
        {
            std::cerr << "Error: at least one run should return a feasible point"<<std::endl;
            return false;
        }
    }
    // compute the time data profile:
    // -------------------------
    // Compute max time taken by all problems
    int max_beta = 0;
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        size_t dimPb = (_use_evals_for_dataprofiles) ? 0 : _selected_pbs[i_pb]->get_n();
        for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance )
            {
                int beta = std::round(_results[i_pb][i_algo][i_pb_instance].get_time()) / dimPb;
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
            cnt = 0;
            for (i_pb = 0 ; i_pb < n_pb ; ++i_pb)
            {
                auto n_pb_instance = _selected_pbs[i_pb]->get_pbInstance().size();
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
            fout << (100.0 * cnt ) / (n_pb*cnt_pb_instance) << " " ;
        }
        fout << std::endl;
    }
    fout.close();
    std::cout << "... done" << std::endl << std::endl;
    return true;
}

void RUNNERPOST::Runner::output_problems_unsolved ( const double & tau , const double & nbSimplexEval ) const
{
    const size_t n_algo = _selected_algos.size();
    const size_t n_pb = _selected_pbs.size();
    
    if ( tau < 0 || n_pb == 0 || n_algo == 0 )
    {
        std::cerr << "Error: cannot compute data profile for tau < 0 or n_pb ==0 or n_algo == 0" << std::endl;
        return ;
    }
    
    std::cout << "Detecting unsolved problems at precision tau=" << tau << " for " << ((nbSimplexEval<0) ? "max": std::to_string(nbSimplexEval)) << " nb simplex evals" << std::endl;
    
    size_t i_pb, i_algo, i_pb_instance;
    
    // check that best solution and all results are available:
    std::list<size_t> miss_list;
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        for ( i_algo = 0 ; i_algo < n_algo ; ++i_algo )
        {
            for ( i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size(); ++i_pb_instance )
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
    
    // Get fx0s for all problems
    ArrayOfDouble fx0s = get_fx0s();
    for ( i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        if ( fx0s[i_pb]==INF )
        {
            std::cout << "pb #" << i_pb+1 << " ---> fx0=Inf --> un-resolved"<<std::endl;
        }
    }
    
    // get the best solution for each problem:
    ArrayOfDouble fxe = get_best_fx();
    
    // Search for unsolved problems:
    // -------------------------
    int alpha = (nbSimplexEval < 0 ) ?  Problem::getNbSimplexEvals(): std::round(nbSimplexEval) ;
    std::vector<size_t> nbUnsolved(n_algo,0);
    for (i_pb = 0 ; i_pb < n_pb ; ++i_pb)
    {
        std::vector<size_t> nbUnsolvedByPb(n_algo,0);
        if ( fx0s[i_pb] < INF && fxe[i_pb] < INF )
        {
            size_t dimPb= _selected_pbs[i_pb]->get_n();
            size_t n_pb_instance = _selected_pbs[i_pb]->get_pbInstance().size();
            for (i_algo = 0 ; i_algo < n_algo ; ++i_algo)
            {
                for ( i_pb_instance = 0 ; i_pb_instance < n_pb_instance ; ++i_pb_instance )
                {
                    if ( fx0s[i_pb]-_results[i_pb][i_algo] [i_pb_instance].get_sol(alpha*(dimPb+1)) < (1-tau)*(fx0s[i_pb]-fxe[i_pb]) )
                    {
                        display_instance_name ( *_selected_pbs[i_pb] , *_selected_algos[i_algo] );
                        if ( n_pb_instance > 1)
                        {
                            std::cout << " pb run instance #" << i_pb_instance << std::endl;
                        }
                        nbUnsolved[i_algo]++;
                        nbUnsolvedByPb[i_algo]++;
                    }
                }
            }
            for (size_t i_algo=0; i_algo < n_algo ; i_algo++  )
            {
                if (nbUnsolvedByPb[i_algo] > 0)
                {
                    std::cout << "     ->  Algo #" << i_algo+1 << " -> " << nbUnsolvedByPb[i_algo] << " unsolved instances for pb #" << i_pb+1 << std::endl;
                }
            }
        }
    }
    for (size_t i_algo=0; i_algo < n_algo ; i_algo++  )
    {
        std::cout << "  Algo #" << i_algo+1 << " -> " << nbUnsolved[i_algo] << " overall unsolved instances" << std::endl;
    }
    std::cout << "... done" << std::endl << std::endl;
}


/*-------------------------------------------------------*/
/* get the value of f at x0 for all problems (private)   */
/*-------------------------------------------------------*/
RUNNERPOST::ArrayOfDouble RUNNERPOST::Runner::get_fx0s() const
{
    const size_t n_pb = _selected_pbs.size();
    const size_t n_algo = _selected_algos.size();
    
    ArrayOfDouble fx0s(n_pb, INF);
    double fx0;

    // Get fx0 for all problems
    for (size_t i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        fx0 = _results[i_pb][0][0].get_sol(1);
        if ( fx0 == INF)
        {
            std::cout << "... problem with starting point of problem #" << i_pb+1 << std::endl;
            return fx0s;
        }
        fx0s[i_pb] = fx0;
        for (size_t i_algo = 1 ; i_algo < n_algo ; ++i_algo )
        {
            for (size_t i_pb_instance=1 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance)
            {
                fx0 = _results[i_pb][i_algo][i_pb_instance].get_sol(1);

                if ( fx0 == INF || std::fabs(fx0 - fx0s[i_pb]) > 1e-10 )
                {
                    std::cout << "... inconsistent starting points between instance runs for problem " << _selected_pbs[i_pb]->get_id() << " and algo " << _selected_algos[i_algo]->get_id() << " fx0=" << fx0 << " fx0s[ipb]=" << fx0s[i_pb] << std::endl;
                    fx0s.clear();
                    return fx0s;
                }
            }
        }

        // fx0 for problems with constraints may not be available (case infeasible initial point -- > NINF)
        // fx0--> average of first feasible point obj
        //     or max first feasible point obj
        if ( fx0s[i_pb]==INF )
        {
            double first_fx;
            size_t nb_first_fx = 0;
            fx0s[i_pb]=0.0;
            for (size_t  i_algo = 0 ; i_algo < n_algo ; ++i_algo )
            {
                for (size_t i_pb_instance=0 ; i_pb_instance < _selected_pbs[i_pb_instance]->get_pbInstance().size() ; ++i_pb_instance)
                {
                    first_fx = _results[i_pb][i_algo][i_pb_instance].get_first_fx();
                    if ( first_fx != INF )
                    {
                        if ( ! _use_avg_fx_first_feas )
                        {
                            if ( nb_first_fx == 0 )
                                fx0s[i_pb] = first_fx;
                            else
                                fx0s[i_pb] = std::max( first_fx , fx0s[i_pb] );

                        }
                        else
                            fx0s[i_pb] += first_fx;

                        nb_first_fx++;
                    }
                }
            }
            if ( nb_first_fx > 0 )
            {
                if ( _use_avg_fx_first_feas )
                    fx0s[i_pb]/=nb_first_fx;
            }
            else
                fx0s[i_pb]=INF;

            std::cout << "pb #"<< i_pb+1 << " has infeasible starting point ---> " << ( ( _use_avg_fx_first_feas  ) ? "average" : "max" ) << " of first feasible point objective functions is used: fx0="<<fx0s[i_pb] << std::endl;
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
    double fxe_tmp, fxe_bb;
    size_t nbDomRefObj; // not used here
    for (size_t i_pb = 0; i_pb < n_pb ; ++i_pb)
    {
        if ( _use_hypervolume_for_profiles )
        {
            std::cerr << "Error: MULTIOBJ NOT YET HANDLED. TODO " << std::endl;
//            // Get the best hypervolume using the combined paretos of algos
//            fxe[i_pb] = - Result::compute_hv( _combinedParetoAllAlgos[i_pb],
//                                            _refParetoIdealPtAllAlgos[i_pb],
//                                            _refParetoNadirPtAllAlgos[i_pb],
//                                            nbDomRefObj );
        }
        else
        {
            for (size_t i_algo = 0 ; i_algo < n_algo ; ++i_algo )
            {
                for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance )
                {
                    size_t max_bb_evals=_selected_pbs[i_pb]->getMaxBBEvals();
                    fxe_tmp = _results[i_pb][i_algo][i_pb_instance].get_sol ( max_bb_evals );
                    
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
            for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size(); ++i_pb_instance)
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
            for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size(); ++i_pb_instance)
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
            for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance )
            {
                tmp = _results[i_pb][i_algo][i_pb_instance].get_sol_bbe();
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
    const size_t n_algo = _selected_algos.size();
    
    size_t tmp = 0;
    size_t bbe_max = 0;
    for (size_t i_pb = 0 ; i_pb < n_pb ; ++i_pb )
    {
        for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance)
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
    for (size_t i_pb_instance = 0 ; i_pb_instance < _selected_pbs[i_pb]->get_pbInstance().size() ; ++i_pb_instance)
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

/*---------------------------------------*/
/*           display all problems        */
/*---------------------------------------*/
void RUNNERPOST::Runner::display_special_options ( void ) const
{

    if ( _use_h_for_profiles || _use_avg_fx_first_feas || _use_evals_for_dataprofiles )
    {
        std::cout <<std::endl << "Special options for runner:" <<std::endl;
        if ( _use_h_for_profiles )
        {
            std::cout  << "\t Use infeasibility h(x)=sum_j ( max(c_j(x),0)^2) for data and performance profiles" <<std::endl;

        }
        if ( _use_avg_fx_first_feas )
        {
            std::cout << "\t Use average fx of all first feasible points on all instance of a problem (default uses max) as a reference for convergence test." <<std::endl;
        }
        if ( _use_evals_for_dataprofiles )
        {
            std::cout  << "\t Use number of evals instead of nbevals/(n+1)=number of simplex grad for x axis on data profiles" <<std::endl;

        }
    }

}


/*------------------------------------------------*/
/*          display instance name (private)       */
/*------------------------------------------------*/
void RUNNERPOST::Runner::display_instance_name (const Problem & pb, const  Algorithm & ac, size_t i_pb_instance) const
{
    std::cout << "\t (" << pb.get_id() << ", " << ac.get_id() << ")";
    if (i_pb_instance < INF_SIZE_T && pb.get_pbInstance().size()> 1)
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





///*---------------------------------------*/
///*  construct a list of sub-directories  */
///*  (static, private)                    */
///*---------------------------------------*/
//bool Runner::construct_list_of_subdirs ( std::list<std::string> & list_of_dirs ,
//                                        const std::string      & directory      )
//{
//
//
//    if ( ! list_of_dirs.empty() )
//        list_of_dirs.clear();
//
//
//
//    std::string s , cmd = "ls " + directory + " > " + TMP_FILE;
//
//
//    if (0 != system ( cmd.c_str() ))
//    {
//        std::cerr << "Error listing files in directory: " << directory << std::endl;
//        return false;
//    }
//
//    std::ifstream fin ( TMP_FILE.c_str() );
//
//    while ( !fin.eof() )
//    {
//        fin >> s >> std::ws;
//        if ( !s.empty() )
//            list_of_dirs.push_back ( s );
//    }
//
//    fin.close();
//    remove ( TMP_FILE.c_str() );
//
//    if ( fin.fail() )
//    {
//        list_of_dirs.clear();
//        return false;
//    }
//    return true;
//}

/*------------------------------------------------*/
/*              set a result (private)            */
/*------------------------------------------------*/
void RUNNERPOST::Runner::set_result (const std::string        & test_id /*not used*/ ,
                         Result                     result[],
                         const Problem            & pb      ,
                         const  Algorithm         & ac        )
{
    // Set single objective result
    
    if ( _use_hypervolume_for_profiles )
        return;
    
    size_t  bbe = pb.getMaxBBEvals();
    
    // bbe corresponds to the desired max number of bb evaluations,
    //  and not necessarily to the last entry in the stats file, while
    //  xe and fxe  correspond to the best solution (last entry in the
    //  stats file).
    auto pb_instances = pb.get_pbInstance();
    for (size_t i_pb_instance=0 ; i_pb_instance < pb_instances.size() ; ++i_pb_instance )
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
//            if ( pb.update_xe ( result[i_seed].get_sol_xe() , result[i_seed].get_sol_fxe() ) )
//                std::cout << " (new best solution)";
            std::cout << std::endl;
        }
        else
        {
            std::cout << "no solution" << std::endl;
        }
    }
}

///*------------------------------------------------*/
///*              set a result (private)            */
///*------------------------------------------------*/
//void Runner::set_hypervolume_result ()
//{
//
//    if (!_use_hypervolume_for_profiles)
//    {
//        std::cout << "This is for hypervoluem results. Cannot set regular results." << std::endl;
//    }
//
//
//    // Initialize reference combined pareto (empty) for all pbs
//    _refParetoIdealPtAllAlgos = std::vector<NOMAD_BASE::Point>(_n_pb);
//    _refParetoNadirPtAllAlgos = std::vector<NOMAD_BASE::Point>(_n_pb);
//    _combinedParetoAllAlgos = std::vector<std::vector<NOMAD_BASE::Point>>(_n_pb);
//
//    // loop on the problems:
//    for ( size_t i_pb = 0 ; i_pb < _n_pb ; ++i_pb )
//    {
//        const auto & pb = *_selected_pbs[i_pb];
//        size_t pbIndex = pb.get_index();
//        size_t nb_obj = pb.getNbObj();
//
//        // bbe corresponds to the desired max number of bb evaluations,
//        //  and not necessarily to the last entry in the stats file.
//        size_t  bbe = pb.getMaxBBEvals();
//
//        int n = pb.get_n();
//
//        if (nb_obj < 2)
//        {
//            std::cout << "/t Pb #" << pbIndex << " is not multi-objective. Cannot compute hypervolume." << std::endl;
//            return;
//        }
//
//        // loop on the algorithm:
//        // For a problem combine all pareto front obtained and the ideal and nadir reference
//        // This is use to compute hypervolume
//        for ( size_t i_algo = 0 ; i_algo < _n_algo ; ++i_algo )
//        {
//            const auto & ap = *_selected_algos[i_algo];
//
//            // This is partial because it is for all seeds of a single algo
//            std::vector<NOMAD_BASE::Point> partialCombinedPareto;
//
//            // Multi-objective: Check file for combined pareto of all seeds
//            std::string combinedParetoFile = Runner::get_test_dir ( _test_id[i_pb][i_algo] , pb ) + "combinedPareto.txt";
//            std::ifstream finPareto( combinedParetoFile.c_str());
//            if (! finPareto.fail())
//            {
//                std::cout << "\t pb #"  << pbIndex+1 << " algo #" << ap.getIndex()+1 << ". Read partial combined pareto file: " << combinedParetoFile;
//                NOMAD_BASE::ArrayOfDouble pt(pb.getNbObj());
//                while ( !finPareto.eof())
//                {
//                    finPareto >> pt;
//                    partialCombinedPareto.push_back(pt);
//                }
//                std::cout << ": " << partialCombinedPareto.size() << " pts " << std::endl;
//            }
//            else
//            {
//
//                std::cout << "\t pb #" << i_pb + 1 << " algo #" << ap.getIndex() +1 << ". Partial pareto of seeds runs: " <<std::endl;
//                for (size_t i_seed=0 ; i_seed < _n_seed_run ; ++i_seed )
//                {
//                    _results[i_pb][i_algo][i_seed].update_pareto (  bbe         ,
//                                                                  partialCombinedPareto);
//                    std::cout << "\t \t - Seeds 1->" << i_seed+1 << ", " << partialCombinedPareto.size() << " pts" << std::endl;
//                }
//
//                // We write the partial combined pareto
//                writePartialCombinedPareto(_test_id[i_pb][i_algo], pb, ap, partialCombinedPareto);
//
//            }
//
//            std::cout << std::endl;
//
//            // Clock
//            time_t t0,t1;
//            time(&t0);
//
//            // Update combined pareto with partial
//            addToCombinedPareto(partialCombinedPareto, i_pb);
//
//            time(&t1);
//
//            std::cout << "\t pb #" << i_pb + 1 << " algo 1->" << ap.getIndex() +1 <<  ". Combined pareto of seeds runs: " << _combinedParetoAllAlgos[i_pb].size() << " pts. Comput. time: " << difftime(t1,t0) << " s" << std::endl;
//
//            std::cout << std::endl;
//
//            finPareto.close();
//        }
//
//
//        // Set the reference ideal and nadir pts for pb.
//        if (_combinedParetoAllAlgos[i_pb].size() > 0 && _combinedParetoAllAlgos[i_pb][0].isComplete() )
//        {
//            _refParetoIdealPtAllAlgos[i_pb] = NOMAD_BASE::Point(nb_obj, NOMAD_BASE::INF);
//            _refParetoNadirPtAllAlgos[i_pb] = NOMAD_BASE::Point(nb_obj, -NOMAD_BASE::INF);
//            for (const auto & p : _combinedParetoAllAlgos[i_pb])
//            {
//                for (size_t j=0; j < nb_obj ; j++)
//                {
//                    _refParetoIdealPtAllAlgos[i_pb][j] = min( p[j], _refParetoIdealPtAllAlgos[i_pb][j]);
//                    _refParetoNadirPtAllAlgos[i_pb][j] = max( p[j], _refParetoNadirPtAllAlgos[i_pb][j]);
//                }
//            }
//        }
//        else
//        {
//            std::cout << "/t Cannot set results (ref pts) for hypervolumes of pb " << _selected_pbs[i_pb]->get_id() << ". Reference hypervolume is not defined." << std::endl;
//        }
//
//        // Compute hypervolume solution for all seeds and all algos
//        for ( size_t i_algo = 0 ; i_algo < _n_algo ; ++i_algo )
//        {
//            for (size_t i_seed=0 ; i_seed < _n_seed_run ; ++i_seed )
//            {
//                Result * result = &_results[i_pb][i_algo][i_seed];
//                display_instance_name ( i_pb , i_algo , i_seed );
//                std::cout << ": found in " << Runner::get_test_dir ( _test_id[i_pb][i_algo] , pb ) << ": ";
//                // Clock
//                time_t t0,t1;
//                time(&t0);
//                if (result->compute_hypervolume_solution(n, bbe, _combinedParetoAllAlgos[i_pb], _refParetoIdealPtAllAlgos[i_pb], _refParetoNadirPtAllAlgos[i_pb]) )
//                {
//                    time(&t1);
//                    std::cout << "bbe="   << result->get_sol_bbe ()
//                    << " optim time=" << result->get_time(result->get_sol_bbe())
//                    << " nb_pareto_points="    << result->get_nb_pareto_points  ()
//                    << " pareto_dominating_ref_obj="    << result->get_nb_dominating_ref_obj()
//                    << " HV_f="    << result->get_sol_fx  ()
//                    << " HV_fx0=" << result->get_sol(1)
//                    << " compute time=" << difftime(t1,t0) << " s" << std::endl;
//                }
//                else
//                {
//                    std::cout << "no solution" << std::endl;
//                }
//            }
//        }
//    }
//}

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

//std::vector<std::string> RUNNERPOST::Runner::get_selected_algo_options ( void ) const
//{
//    std::vector<std::string> algosOptions;
//
//    for (const auto& algo: _selected_algos)
//    {
//        algosOptions.push_back(algo->get_output_options());
//    }
//    return algosOptions;
//}


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
        
        _selected_pbs.push_back(new Problem(line, error_msg));
        if (!error_msg.empty())
        {
            in.close();
            return false;
        }
    }
    in.close();

//    std::string s;
//    while(!fin.eof())  // std::getline(fin, s) && !s.empty())
//    {
//        getline (fin , s);
//
//        if (s.empty())
//        {
//            continue;
//        }
//
//        // Get pb id as the first word on the line
//        size_t i = s.find_first_not_of(" ");
//        if (i > 0)
//        {
//            s.erase(i) ;  // Remove initial white spaces
//        }
//        i = s.find(" ");
//        std::string id = s.substr(0,i);
//
//        // pb name is provided between parenthesis
//        size_t i0 = s.find("(",0);
//        size_t i1 = s.find(")",i0+1);
//
//        if ( i0 == std::string::npos || i1 == std::string::npos )
//        {
//            error_msg = "Error(2) in file " + pb_selection_file_name + ". Line #" + std::to_string(_n_algo+1) + ". Pb name must be provided between parentheses after pb id.";
//            return false;
//        }
//        std::string name = s.substr(i0+1,i1-i0-1);
//
//        // TODO parse bbot
//        s.erase(0,i1);
//        size_t pos = 0;
//        // Parse for n and m given as bracket values
//        while ((pos = s.find("[")) != std::string::npos)
//        {
//
//            // Strip empty spaces before a [
//            pos = s.find_first_not_of("[");
//            if (pos == std::string::npos)
//            {
//                break;
//            }
//            s.erase(0, pos+1);
//
//            n = extract_from_bracket("n",s);
//
//            if ( n==M_INF_INT)
//            {
//                error_msg = "Error(4) in file " + pb_selection_file_name + ". Cannot read problem bracket value n." ;
//                return false;
//            }
//
//            m = extract_from_bracket("m",s);
//            if ( m==M_INF_INT)
//            {
//                error_msg = "Error(4) in file " + pb_selection_file_name + ". Cannot read problem bracket value m." ;
//                return false;
//            }
//
//        }
//
//
//        _selected_pbs.push_back ( new Problem ( id , name , n, m /* todo bbot */) );
//
//        _n_pb++ ;
//
//    }
//    fin.close();
    
    if (_selected_pbs.empty())
    {
        in.close();
        error_msg = "Error(1) in file " + pb_selection_file_name + ". Cannot read a single pb config. First line in file must contain an algo config." ;
        return false;
    }
    

    return true;
}

//bool Runner::addToCombinedPareto(const std::vector<NOMAD_BASE::Point> & paretoPoints, const size_t & pbIndex )
//{
//    bool updated_pareto = false;
//
//    if (_combinedParetoAllAlgos[pbIndex].size() == 0)
//    {
//        _combinedParetoAllAlgos[pbIndex] = paretoPoints;
//        updated_pareto = true;
//    }
//    else
//    {
//        for (const auto & pt: paretoPoints)
//        {
//            updated_pareto = addToCombinedPareto(NOMAD_BASE::ArrayOfDouble(pt), pbIndex) || updated_pareto ;
//        }
//    }
//    return updated_pareto;
//}
//
//bool Runner::addToCombinedPareto(const NOMAD_BASE::ArrayOfDouble & pt, const size_t & pbIndex )
//{
//
//    bool updated_pareto = false;
//
//    bool insert = true;
//    std::vector<NOMAD_BASE::Point>::iterator itPf = _combinedParetoAllAlgos[pbIndex].begin();
//    NOMAD_BASE::EvalPoint tempEP(1); // Fake number of variables
//    NOMAD_BASE::EvalPoint ep(1);
//    ep.setBBO(pt.display(),NOMAD_BASE::BBOutputTypeList(pt.size(), NOMAD_BASE::BBOutputType::OBJ), NOMAD_BASE::EvalType::BB, true /* eval is ok*/);
//    while (itPf != _combinedParetoAllAlgos[pbIndex].end())
//    {
//        // Create a fake eval point for using compMO
//        tempEP.setBBO(itPf->displayNoPar(), NOMAD_BASE::BBOutputTypeList(pt.size(), NOMAD_BASE::BBOutputType::OBJ), NOMAD_BASE::EvalType::BB, true /* eval is ok*/);
//        auto compFlag = ep.compMO(tempEP, NOMAD_BASE::defaultFHComputeType);
//        if (compFlag == NOMAD_BASE::CompareType::DOMINATED || compFlag == NOMAD_BASE::CompareType::EQUAL)
//        {
//            return false;
//        }
//        if (compFlag == NOMAD_BASE::CompareType::DOMINATING)
//        {
//            itPf = _combinedParetoAllAlgos[pbIndex].erase(itPf);
//            updated_pareto = true;
//            continue;
//        }
//        itPf++;
//    }
//    if (insert)
//    {
//        _combinedParetoAllAlgos[pbIndex].push_back(pt);
//        updated_pareto = true;
//    }
//    return updated_pareto;
//
//}

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
            
        
        // TODO
//        if ( "output_data_profile_plain" == select_command )
//        {
//            if ( args.size() != 2 )
//            {
//                std::cerr << "\n Error in output_selection: number of arguments in output_data_profile_plain should be 2 (tau,dp_file_name)" << std::endl;
//                return false;
//            }
//            double tau = stod(args[0]);
//            if ( tau < 0 )
//            {
//                std::cerr << "\n Error in output_selection: output_data_profile_plain first argument (tau value) should be a positive real" << std::endl;
//                return false;
//            }
//            if ( tau == 0 && ! runner.get_use_h_for_profiles() )
//            {
//                std::cerr << "\n Error in output_selection: data profile for tau=0 will be executed only if set_use_h_for_profiles has been set in problem_selection file" << std::endl;
//                return false;
//            }
//            if ( runner.get_use_h_for_profiles() && runner.get_use_hypervolume_for_profiles() )
//            {
//                std::cerr << "\n Error in output_selection: data profile for hypervolume is only for pareto points for the objectives for feasible points. Cannot be use in combination with use_h_for_profiles" << std::endl;
//                return false;
//            }
//
//            if ( ! get_dp_file_name(tau).empty() )
//            {
//                std::cerr << "\n Error in output_selection: data profile for tau=" << tau << " was already processed." << std::endl << std::endl;
//                return false;
//            }
//
//            bool success = runner.output_data_profile_plain( tau ,args[1] );
//            if ( ! success )
//                return false;
//
//            // Register the dp_file_name in the file for duplicate testing and other outputs (matlab, pgfplots)
//            map_tau_to_dp_file_name[tau] = args[1];
//
//        }
//        else if ( "output_time_profile_plain" == select_command )
//        {
//            if ( args.size() != 1 )
//            {
//                std::cerr << "\n Error in output_selection: number of arguments in output_time_profile_plain should be 1 (time_profile_file_name)" << std::endl;
//                return false;
//            }
//            bool success = runner.output_time_profile_plain(args[0]);
//            if ( ! success )
//            {
//                return false;
//            }
//        }
//        else if ( "output_time_data_profile_plain" == select_command )
//        {
//            if ( args.size() != 2 )
//            {
//                std::cerr << "\n Error in output_selection: number of arguments in output_time_data_profile_plain should be 2 (tau,tdp_file_name)" << std::endl;
//                return false;
//            }
//            double tau;
//            if ( ! tau.atof(args[0]) || !tau.isDefined() || tau < 0 )
//            {
//                std::cerr << "\n Error in output_selection: output_time_data_profile_plain first argument (tau value) should be a positive real" << std::endl;
//                return false;
//            }
//            if ( tau == 0 && ! runner.get_use_h_for_profiles() )
//            {
//                std::cerr << "\n Error in output_selection: data profile for tau=0 will be executed only if set_use_h_for_profiles has been set in problem_selection file" << std::endl;
//                return false;
//            }
//            if ( runner.get_use_h_for_profiles() && runner.get_use_hypervolume_for_profiles() )
//            {
//                std::cerr << "\n Error in output_selection: data profile for hypervolume is only for pareto points for the objectives for feasible points. Cannot be use in combination with use_h_for_profiles" << std::endl;
//                return false;
//            }
//
//            if ( ! get_tdp_file_name(tau).empty() )
//            {
//                std::cerr << "\n Error in output_selection: data profile for tau=" << tau << " was already processed." << std::endl << std::endl;
//                return false;
//            }
//
//            bool success = runner.output_time_data_profile_plain( tau ,args[1] );
//            if ( ! success )
//                return false;
//
//            // Register the dp_file_name in the file for duplicate testing and other outputs (matlab, pgfplots)
//            map_tau_to_tdp_file_name[tau] = args[1];
//
//        }
//
//        else if ("set_use_evals_for_dataprofiles" == select_command )
//        {
//            runner.set_use_evals_for_dataprofiles() ;
//        }
//
//        else if ( "output_perf_prof_plain" == select_command )
//        {
//            std::cerr << "\n Error: output_perf_prof_plain not yet implemented" << std::endl;
//            return false;
//
//// TODO
////            std::map<double,string>::iterator it;
////            it = map_tau_to_dp_file_name.find(tau);
////            if (it != map_tau_to_dp_file_name.end())
////            {
////                std::cerr << "Error in output_selection: data profile for tau=" << tau << " was already processed." << std::endl;
////                return false;
////            }
//
//            // runner.output_perf_profile_plain( tau ,args[1] );
//
//            // Register the pp_file_name in the file for duplicate testing and other outputs (matlab, pgfplots)
//            // map_tau_to_pp_file_name[tau] = args[1];
//        }
//
//        if ("output_data_profile_pgfplots" == select_command)
//        {
//            if ( args.size() < 1 || args.size() > 6 )
//            {
//                std::cerr << "\n Error in output_selection: number of arguments in output_data_profile_pgfplots should be greater than 1." <<std::endl;
//                std::cerr << " Usage: output_data_profile_pgfplots (tau, pdflatex_cmd, tex_file_name,dp_plain_file_name, tex_file_name, dp_pdf_file_name); All parameters except tau are optional." << std::endl;
//                return false;
//            }
//            bool success = output_profile_pgfplots(runner, "dataProfile", args);
//            if (!success)
//            {
//                return false;
//            }
//        }
//
//        else if ("output_time_profile_pgfplots" == select_command)
//        {
//            if ( args.size() < 1 || args.size() > 6 )
//            {
//                std::cerr << "\n Error in output_selection: number of arguments in output_time_profile_pgfplots should be greater than 1." <<std::endl;
//                std::cerr << " Usage: output_time_profile_pgfplots(pdflatex_cmd, tex_file_name, time_profile_plain_file_name, tex_file_name, time_profile_pdf_file_name); All parameters are optional." << std::endl;
//                return false;
//            }
//            bool success = output_profile_pgfplots(runner, "timeProfile", args);
//            if (!success)
//            {
//                return false;
//            }
//        }
//        if ("output_time_data_profile_pgfplots" == select_command)
//        {
//            if ( args.size() < 1 || args.size() > 6 )
//            {
//                std::cerr << "\n Error in output_selection: number of arguments in output_time_data_profile_pgfplots should be greater than 1." <<std::endl;
//                std::cerr << " Usage: output_time_data_profile_pgfplots (tau, pdflatex_cmd, tex_file_name,tdp_plain_file_name, tex_file_name, tdp_pdf_file_name); All parameters except tau are optional." << std::endl;
//                return false;
//            }
//            bool success = output_profile_pgfplots(runner, "timeDataProfile", args);
//            if (!success)
//            {
//                return false;
//            }
//        }
//
//        else if ( "display_algo_diff" == select_command )
//        {
//            runner.display_algo_diff();
//        }
//        else if( "output_problems_unsolved" == select_command)
//        {
//            if ( args.size() != 2 )
//            {
//                std::cerr << "\n Error in output_problems_solved: number of arguments should be 2." <<std::endl;
//                std::cerr << " Usage: output_problems_solved (tau, nbSimplexEval); All parameters are mandatory. If nbSimplexEval == -1, the last eval point is used." << std::endl;
//                return false;
//            }
//            double tau;
//            if ( ! tau.atof(args[0]) || !tau.isDefined() || tau < 0 )
//            {
//                std::cerr << "\n Error in output_selection: output_problems_unsolved first argument (tau value) should be a positive real" << std::endl;
//                return false;
//            }
//            double nbSimplexEval;
//            if ( ! nbSimplexEval.atof(args[1]) || !nbSimplexEval.isDefined() )
//            {
//                std::cerr << "\n Error in output_selection: output_problems_unsolved second argument (nbSimplex value) should be a real" << std::endl;
//                return false;
//            }
//            runner.output_problems_unsolved(tau,nbSimplexEval);
//
//        }
    }
    
    
    
    in.close();
    return true;
}

void RUNNERPOST::Runner::display_selected_outputs   ( void ) const
{
    // TODO: similar to algo and pbs.
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
            result[i_pb_instance].reset(_use_hypervolume_for_profiles, _use_h_for_profiles);
            return false;
        }
        
        // Prepare the stats file complete format from the pb info and the algo stat output type
        StatOutputTypeList statsFileFormat = composeStatsFileFormat(ac.get_stats_output_type_list(), pb.get_n(), pb.get_m());
        
        // Read the stats file into results
        if ( !result[i_pb_instance].read ( fin , pb.getMaxBBEvals ( ) , statsFileFormat, _feasibilityThreshold )  )
        {
            fin.close();
            result[i_pb_instance].reset( _use_hypervolume_for_profiles, _use_h_for_profiles);
            return false;
        }
        fin.close();

        size_t res_bbe = result[i_pb_instance].get_last_bbe();

        if ( res_bbe <= 0 )
        {
            result[i_pb_instance].reset( _use_hypervolume_for_profiles, _use_h_for_profiles);
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



bool RUNNERPOST::Runner::output_profile_pgfplots(const Output & out) const
{
    if (out.get_latex_file_name().empty())
    {
        return true;
    }
    
    std::string plain_file_name = out.get_plain_file_name();
    std::string latex_file_name = out.get_latex_file_name();
    
    if (out.get_plain_file_name().empty())
    {
        std::cerr << "\n Error in output_selection: To output in latex the output_plain is mandatory. " << std::endl;
        return false;
    }

    std::ifstream infile(plain_file_name);
    if (! infile.good() )
    {
        std::cerr << "\n Error in output_selection: file " << plain_file_name << " does not exist for latex profile " << std::endl;
        infile.close();
        return false;
    }
    
    // Modify plain file name to include the key "step"
    std::string plain_file_name_step = plain_file_name + ".step";
    
    std::cout<< "\t Writing of " << latex_file_name << " and " << plain_file_name_step << " ......";
    
    // Create data for plotting data profile with step
    std::ofstream outfile_step(plain_file_name_step);
    
    // The loop for reading plain_file_name and writing to plain_file_name_step
    std::string line, prevLine, prevLineToken, lineFirstToken;
    while (std::getline(infile, line))
    {
        if (line.empty())
        {
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

    // To be generalized.
    // Multiple inputs.
    // Single output.

    // Graph title
    std::string profileTitle = out.get_title();
    
    
    RUNNERPOST::Output::Profile_Type profile_type = out.get_profile_type();


    // Output tex file from previous strings
    std::ofstream out_tex ( latex_file_name , std::ofstream::out | std::ofstream::trunc );
    out_tex << "\\documentclass{standalone}" <<std::endl;
    out_tex << "\\usepackage{pgfplots} " <<std::endl;
    out_tex << "\\pgfplotsset{width=10cm,compat=1.16} " <<std::endl;
    out_tex << "\\begin{document}" <<std::endl;
    out_tex << "\\begin{tikzpicture} "<<std::endl;
    out_tex << "\\begin{axis}[ " << std::endl;

    out_tex << "       title = " << profileTitle << "," << std::endl;
    out_tex << "       xmin=-10, ymin = -5, ymax= 105," << std::endl;

    if (RUNNERPOST::Output::Profile_Type::DATA_PROFILE == profile_type)
    {
        if (RUNNERPOST::Output::X_Select::EVAL == out.get_x_select())
        {
            out_tex << "       xlabel = Number of evaluations, " <<std::endl;
        }
        else if (RUNNERPOST::Output::X_Select::NP1EVAL == out.get_x_select())
        {
            out_tex << "       xlabel = Number of ($n+1$) evaluations," <<std::endl;
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
            out_tex << "       ylabel = Percentage of problems solved," <<std::endl;
        }
        else if (RUNNERPOST::Output::Y_Select::INFEAS == out.get_y_select())
        {
            out_tex << "       ylabel = Percentage of problems solved," <<std::endl;
        }
        else
        {
            std::cerr << "\n Error:  y_select type is not available for latex profile " << std::endl;
            out_tex.close();
            return false;
        }
        
        
        
    }
    else
    {
        std::cerr << "\n Error:  profile type " << RUNNERPOST::Output::profileTypeToString(out.get_profile_type()) << " is not yet available for latex profile " << std::endl;
        out_tex.close();
        return false;
    }


    out_tex << " legend style={ " << std::endl;
    out_tex << "    font=\\tiny, " <<std::endl;
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

    if (RUNNERPOST::Output::Profile_Type::DATA_PROFILE == profile_type)
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

//    else if ("timeProfile" == profileName)
//    {
//        for (size_t i_algo = 0; i_algo < legends.size(); i_algo++)
//        {
//            std::string algoFileName = NOMAD_BASE::dirname(args[0]);
//            algoFileName += profile_plain_noExt_noPath;
//            algoFileName += NOMAD_BASE::itos(i_algo) + NOMAD_BASE::extension(args[0]);
//            // VRM TODO
//            out << "  \\addplot [" << lineStyle << ", mark="<< symbols[symbol_index++] << ", color=" << colors[color_index++] << "] table [x index = 0, y index = 1, header = false ] {" << algoFileName << "}; " << std::endl ;
//            out << "\\addlegendentry{" << legends[i_algo] << "};" <<std::endl;
//        }
//    }
    out_tex << " \\end{axis} " << std::endl;
    out_tex << " \\end{tikzpicture}" << std::endl;
    out_tex << " \\end{document}" <<std::endl;

    out_tex.close();

    std::cout << " done " << std::endl;

    return true;
}



std::string RUNNERPOST::Runner::get_stats_file_name (const Algorithm    & ac      ,
                                                     const Problem      & pb      ,
                                                     const std::string  & pb_inst    )
{
    
    
    std::string statsFileName = DEFAULT_STATS_FILE;
    bool flag_add_pbinstance_to_stats_file = DEFAULT_ADD_PBINSTANCE_TO_STATS_FILE;

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
        if (sWords.empty() && !RUNNERPOST::DEFAULT_ADD_PBINSTANCE_TO_STATS_FILE)
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
            if (numberOfConstraints < 0)
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
