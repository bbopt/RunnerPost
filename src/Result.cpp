#include "Result.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

//#ifdef LIB_HYPERVOLUME
//#include "hv.h"
//#endif

/*----------------------------------*/
/*               reset              */
/*----------------------------------*/
void RUNNERPOST::Result::reset ( bool use_hypervolume_for_obj, bool use_h_for_obj )
{
    _bbe.clear();
    _obj.clear();
    // _mobj.clear();
    _last_x.clear();
    
    if (_use_h_for_obj && _use_hypervolume_for_obj)
    {
        std::cout << "RUNNERPOST::Result::reset. Cannot use both h AND hypervolume for obj" <<std::endl;
    }
    
    _use_h_for_obj = use_h_for_obj;
    _use_hypervolume_for_obj = use_hypervolume_for_obj;
    // For now use_std_h is always true;      // _use_std_h = use_std_h ;  
    _nb_obj = 0; // Need to be updated
    clear_solution();
}

/*-----------------------------------*/
/*      clear solution (private)     */
/*-----------------------------------*/
void RUNNERPOST::Result::clear_solution ( void )
{
    _sol_bbe    = INF_SIZE_T;
    _sol_fx     = INF;
    // _sol_xe.clear();
    _sol_fxe    = INF;
    _has_sol = false;
    _is_infeas = false;
}

/*----------------------------------*/
/*          read results            */
/*----------------------------------*/
bool RUNNERPOST::Result::read ( std::ifstream & in , size_t max_bbe , int m , int n, const double & feasibilityThreshold )
{
    if (_use_h_for_obj && _use_hypervolume_for_obj)
    {
        std::cout << "Cannot use both h AND hypervolume for obj" <<std::endl;
        return false;
    }
    
    
    std::string   s,s2, line;
    size_t        bbe =0, bbe_plus =0;
    double time = 0 , obj = INF, obj_prev = INF ;
    bool valid=true;
    
    // TODO: manage bbo and output type.
    // Complex to deduce all cases.
    // Only one default output type: CNT OBJ (not for multi-obj). Each line is an improving feasible point. This gives the smallest stats files.
    // TODO: User provides output type with algo and pass it to this read (some compliance tests with m can be done).
    ArrayOfDouble bbo(m);
    if (m==1)
    {
        _outputType.push_back(OBJ);
    }
    else
    {
        std::cout << "TODO: manage output type" << std::endl;
        return false;
    }
    
    bool first_line = true;
    bool first_line_is_infeasible = false;
    
    // Number of objectives
    _nb_obj = std::count(_outputType.begin(),_outputType.end(),OutputType::OBJ);
    
    // NOMAD_BASE::getNbObj(bbot);
    //    // Bbot for MultiObjective EvalPoint (stored in _mobj)
//    NOMAD_BASE::BBOutputTypeList bbotMO(_nb_obj, NOMAD_BASE::BBOutputType::OBJ);

    if (_use_hypervolume_for_obj && _nb_obj < 2)
    {
        std::cout << "Cannot compute hypervolume when nb objective is not greater than 1" <<std::endl;
        return false;
    }
    
    // For multi objective, for now, we can only compute profiles with hypervolume
    if (!_use_hypervolume_for_obj && _nb_obj > 1)
    {
        std::cout << "Nb objective is greater than 1. Need to enable option to compute hypervolume of pareto fronts" <<std::endl;
        return false;
    }
    
    int nb_fields=0; // Number of fields in the file. For history file we have nb_fields = n + m. For stats files nb_fields will be larger. This is determined from the first line.
    bool flag_history_file = false;
    
    while ( !in.eof() )
    {
        if (!std::getline(in, line))
        {
            if (first_line)
            {
                std::cout << "Result file is empty" <<std::endl;
                return false;
            }
        }

        // Put the line in a string stream for reading values.
        std::istringstream iss(line);
        
        valid=true;
        if ( first_line )
        {
            iss >> s;
            
            // no feasible solution has been found in X evaluations:
            if ( s == "no" )
            {
                iss >> s >> s >> s >> s >> s >> s >> _sol_bbe;
                if ( in.fail() )
                {
                    _sol_bbe = INF_SIZE_T;
                }
                else
                {
                    if ( max_bbe < INF_SIZE_T && max_bbe < _sol_bbe )
                    {
                        _sol_bbe = max_bbe;
                    }
                }
                _bbe.push_back (_sol_bbe);
                _obj.push_back (INF);
                _time.push_back(time);
                
                return true;
            }
            else
            {
                // number of fields.
                size_t pos = 0;
                nb_fields = 0;
                while ((pos = line.find(" ")) != std::string::npos)
                {
                    
                    // Strip empty spaces before a field
                    pos = line.find_first_not_of(" ");
                    if (pos == std::string::npos)
                    {
                        break;
                    }
                    line.erase(0, pos);
                     
                    // We have a field
                    nb_fields ++;
                    
                    // Strip a single field value
                    pos = line.find_first_of(" ");
                    if (pos != std::string::npos)
                    {
                        line.erase(0, pos);
                    }
                }
                // We have a history file: x1 x2 ... xn bbo1 bbo2 ... bbbom
                if (nb_fields == n+m)
                {
                    flag_history_file = true;
                    
                    // Take the xi (not used). x0 was already taken.
                    for (size_t i =0 ; i < n-1 ; i++)
                    {
                        iss >> s;
                    }
                    bbe= 1;
                    
                }
                else
                {
                    time = std::stod(s);
                    iss >> bbe >> obj;  // This is obj from getF but if merit function is a F+penalty, this is not correct. We get the output for OBJ later.
                }
            }
            
        }
        else
        {
            // No more lines to read.
            if (line.empty())
            {
                break;
            }
            
            if (flag_history_file)
            {
                bbe++;
                // Take the xi (not used). Unlike for the first line, x0 was NOT taken.
                s2="";
                for (size_t i =0 ; i < n ; i++)
                {
                    iss >> s;
                    s2+=s+" ";
                }
            }
            else
            {
                iss >> s;
                
                // no feasible solution has been found in X evaluations (case optimization for h and suboptimizations (VNS,...):
                if ( s == "no" )
                {
                    // skip the line
                    std::getline ( in , line );
                    std::getline ( in , line );
                    in >> std::ws;
                    continue;
                }
                else
                {
                    time = std::stod(s);
                }
                
                
                iss >> bbe >> s2;
                if ( s2.compare("+")==0 )
                {
                    iss >> bbe_plus >> obj ;
                    
                    // update the bb counter
                    bbe += bbe_plus;
                }
                else
                {
                    obj = std::stod(s2);
                }
            }
        }
        
//        if ( in.fail() || !valid )
//        {
//            return false;
//        }
        
        
        // NOTE: for multi obj, we have a single f value displayed BUT the bb outputs contain all objectives
        // Get bb_outputs (f again and constraints)
        for (int i = 0; i < m ; i++)
        {
            iss >> bbo[i];
            if (iss.fail())
            {
                bbo[i] = INF;
            }
            // Do not use the F from the stats file. For penalized objective this may not be equal to the output OBJ.
            if ( _nb_obj == 1 && _outputType[i] == OutputType::OBJ )
            {
                obj = bbo[i];
            }
        }
        
        if (!flag_history_file)
        {
            // In stats file, the xi are at the endl of the line.
            s2="";
            for (size_t i =0 ; i < n ; i++)
            {
                iss >> s;
                s2+=s+ " ";
            }
        }
        
        if ( _use_h_for_obj )
        {
            if ( !_use_std_h )
            {
                std::cout << "Other than standard h calculation not implemented" <<std::endl;
                return false;
            }
            // Convert bbo (except f) into obj
            obj = 0;
            for ( int i=1; i < m ; i++ )
            {
                if ( _outputType[i] != OutputType::OBJ )
                {
                    obj += pow( std::max( bbo[i], feasibilityThreshold ),2);
                }
            }
        }

        
        // Keep only feasible points. Use flag isFeas.
        bool isFeas = true;
        if ( m > _nb_obj)
        {
            for ( int i=0; i < m ; i++ )
            {
                if ( _outputType[i] != OutputType::OBJ && bbo[i] > feasibilityThreshold )
                {
                    isFeas = false;
                    if ( first_line )
                        first_line_is_infeasible = true;
                    break;
                }
            }
        }
        
        if (_use_hypervolume_for_obj)
        {
            std::cerr << "Result::read for hypervolumem not yet implemented" << std::endl;
            return false;
            // Keep all feasible points
//            if (isFeas)
//            {
//                std::string objs;
//                for ( int i=0; i < m ; i++ )
//                {
//                    if ( _outputType[i] == OutputType::OBJ )
//                    {
//                        objs += std::to_string(bbo[i]) + " ";
//                    }
//                }
//                NOMAD_BASE::EvalPoint ev; // An eval point with nothing
//                ev.setBBO(objs, bbotMO, NOMAD_BASE::EvalType::BB);
//                if ( ev.getEvalStatus(NOMAD_BASE::EvalType::BB) != NOMAD_BASE::EvalStatusType::EVAL_OK )
//                {
//                    std::cout << "Evaluation point is not eval ok. " << std::endl;
//                    return false;
//                }
//
//                _bbe.push_back( bbe );
//                // _mobj.push_back( ev );
//                _time.push_back(time);
//            }
        }
        else
        {
            // Keep only improving feasible values
            if ( isFeas && ( first_line || obj < obj_prev ) )
            {
                _bbe.push_back( bbe );
                _obj.push_back( obj );
                _time.push_back(time);
                obj_prev = obj;
            }
        }
            

         // Total time and total bbe - for time stats.
        _totalBbe = bbe;
        _totalTime = time; // When using history file, time is not available (use 0)

        if ( first_line )
        {
            first_line = false;
        }
        
    }
    
    _last_x = s2;
    
    // The first line displays an infeasible initial point and no feasible point was found.
    if ( _bbe.empty() && first_line_is_infeasible )
    {
        _bbe.push_back (INF_SIZE_T);
        _obj.push_back (INF);
        // _mobj.push_back (NOMAD_BASE::EvalPoint()); // ChT TEMP maybe we need a fake EvalPoint with obj=INF
        _time.push_back(time);
        _last_x.clear();
    }
    
    return true;
}

/*-----------------------------------*/
/*       get the last bbe entry      */
/*-----------------------------------*/
size_t RUNNERPOST::Result::get_last_bbe ( void ) const
{
    if ( _bbe.empty() )
        return 0 ;
    return _bbe[_bbe.size()-1];
}




//bool RUNNERPOST::Result::update_pareto_single ( const NOMAD_BASE::EvalPoint & evalPoint ,
//                                   std::vector<NOMAD_BASE::Point> & combinedPareto) const
//{
//    bool updated_pareto = false;
//
//    NOMAD_BASE::Point bboEP = evalPoint.getFs(NOMAD_BASE::defaultFHComputeType);
//    bool insert = true;
//    std::vector<NOMAD_BASE::Point>::iterator itPf = combinedPareto.begin();
//    NOMAD_BASE::EvalPoint tempEP(1); // Fake number of variables
//    while (itPf != combinedPareto.end())
//    {
//        // Create a fake eval point for using compMO
//        tempEP.setBBO(itPf->displayNoPar(), NOMAD_BASE::BBOutputTypeList(_nb_obj, NOMAD_BASE::BBOutputType::OBJ), NOMAD_BASE::EvalType::BB, true /* eval is ok*/);
//        auto compFlag = evalPoint.compMO(tempEP, NOMAD_BASE::defaultFHComputeType);
//        if (compFlag == NOMAD_BASE::CompareType::DOMINATED || compFlag == NOMAD_BASE::CompareType::EQUAL)
//        {
//            return false;
//        }
//        if (compFlag == NOMAD_BASE::CompareType::DOMINATING)
//        {
//            itPf = combinedPareto.erase(itPf);
//            updated_pareto = true;
//            continue;
//        }
//        itPf++;
//    }
//    if (insert)
//    {
//        combinedPareto.push_back(bboEP);
//        updated_pareto = true;
//    }
//    return updated_pareto;
//}


//bool RUNNERPOST::Result::update_pareto ( const size_t bbe ,
//                            std::vector<NOMAD_BASE::Point> & pareto) const
//{
//    bool updated_pareto = false;
//
//    std::vector<size_t>::const_iterator itBBE = _bbe.begin();
//    std::vector<NOMAD_BASE::EvalPoint>::const_iterator itMobj ;
//
//    // udpate pareto using all multi objective evaluations
//    for ( itMobj = _mobj.begin() ; itMobj < _mobj.end() ; itMobj++, itBBE++ )
//    {
//        if ( *itBBE > bbe)
//            break;
//
//        NOMAD_BASE::Point bboEP = itMobj->getFs(NOMAD_BASE::defaultFHComputeType);
//        if (pareto.size() == 0)
//        {
//            pareto.push_back(bboEP);
//            updated_pareto = true;
//            continue;
//        }
//        if (pareto.size() == 1 && !pareto[0].isComplete())
//        {
//            pareto[0] = bboEP;
//            updated_pareto = true;
//            continue;
//        }
//        updated_pareto = update_pareto_single(*itMobj, pareto) || updated_pareto;
//    }
//
////
////    if (updateCombinedPareto)
////    {
////        bool flagEmptyCombinedPareto = (combinedPareto.size() == 0);
////
////        // If partial pareto is not updated we don't need to update combined pareto
////        // Update combined pareto using only partial pareto
////        if (updated_partial_pareto)
////        {
////            for (const auto & pt: partialCombinedPareto )
////            {
////                // If the combine pareto was empty, partial = combined
////                if (flagEmptyCombinedPareto)
////                {
////                    combinedPareto.push_back(pt);
////                    updated_pareto = true;
////                    continue;
////                }
////                if (combinedPareto.size() == 1 && !combinedPareto[0].isComplete())
////                {
////                    combinedPareto[0] = pt;
////                    updated_pareto = true;
////                    continue;
////                }
////                NOMAD_BASE::EvalPoint tempEP(1);
////                tempEP.setBBO(pt.displayNoPar(), NOMAD_BASE::BBOutputTypeList(_nb_obj, NOMAD_BASE::BBOutputType::OBJ), NOMAD_BASE::EvalType::BB, true /* eval is ok*/);
////                updated_pareto = update_pareto_single(tempEP, combinedPareto) || updated_pareto;
////
////            }
////        }
////    }
//
//    return updated_pareto;
//}

//// Compute scaled hypervolume of pareto front with respect to ref ideal and nadir points. See LS paper.
//NOMAD_BASE::Double RUNNERPOST::Result::compute_hv (const std::vector<NOMAD_BASE::Point> & pareto,
//                                       const NOMAD_BASE::Point              & refParetoIdealPt,
//                                       const NOMAD_BASE::Point              & refParetoNadirPt,
//                                       size_t & nb_dominating_ref_obj)
//{
//    NOMAD_BASE::Double scaledHV;
//    nb_dominating_ref_obj = 0;
//
//#ifdef LIB_HYPERVOLUME
//
//    // data for hv
//    const int nb_obj = static_cast<int>(refParetoIdealPt.size());
//    if (nb_obj != refParetoNadirPt.size() || nb_obj != pareto[0].size())
//    {
//        std::cout << "Inconsistent dimension of the number of objectives" <<std::endl;
//        return NOMAD_BASE::INF;
//    }
//    double * dpareto = new double[nb_obj*pareto.size()];
//    size_t k=0;
//    std::vector<NOMAD_BASE::Point> Tpareto; // Transformed (scaled) pareto (see LS paper)
//
//// TEMP for testing hv computation
////    std::ifstream fin ( "viennet.txt" );
////    size_t lines=0;
////    double l,m,r,lmax=-100000000,mmax=-10000000,rmax=-1000000000;
////
////    while ( !fin.eof() )
////    {
////        lines ++;
////        fin >> l  ;
////        fin >> m  ;
////        fin >> r  ;
////        dpareto[k++] = l;
////        dpareto[k++] = m;
////        dpareto[k++] = r;
////        if ( l > lmax)
////        {
////            lmax = l;
////        }
////        if ( m > mmax)
////        {
////            mmax = m;
////        }
////        if ( r > rmax)
////        {
////            rmax = r;
////        }
////
////    }
////    fin.close();
////    double reference[3] = { lmax, mmax, rmax };
////    scaledHV = fpli_hv(dpareto, 3, 874, reference);
////    if (abs(scaledHV - 3.86877) > 0.01)
////      std::cout << "ERROR in hv compuation" <<std::endl;
//
//
//    for (const auto & p : pareto)
//    {
//        NOMAD_BASE::Point Tpt(nb_obj);
//        bool dominating = true;
//        for (size_t j =0 ; j < nb_obj; j++)
//        {
//            Tpt[j] = p[j].todouble()-refParetoIdealPt[j].todouble();
//            if (refParetoIdealPt[j] != refParetoNadirPt[j])
//            {
//                Tpt[j] /= (refParetoNadirPt[j].todouble() - refParetoIdealPt[j].todouble());
//            }
//            if ( dominating && Tpt[j] > 1.0)
//                dominating = false;
//            dpareto[k++] = Tpt[j].todouble();
//        }
//        if (dominating)
//            nb_dominating_ref_obj++;
//    }
//
//    double reference[nb_obj]; // scaled reference point is 1 for all objectives
//    for (size_t j =0 ; j < nb_obj; j++)
//    {
//        reference[j] = 1.0;
//    }
//
//    scaledHV = fpli_hv(dpareto, static_cast<int>(nb_obj), static_cast<int>(pareto.size()), reference);
//    delete[] dpareto;
//
//#else
//    std::cout << "Hypervolume computation library (HV) is not available." <<std::endl;
//    return NOMAD_BASE::INF;
//
//#endif
//
//    return scaledHV;
//}


//// Replaces the objectives fs by single f values with monotonic decrease. The value is computed as the hypervolume between
//bool RUNNERPOST::Result::compute_hypervolume_for_obj ( const size_t bbeMax         ,
//                     const std::vector<NOMAD_BASE::Point> & refCombinedPareto,
//                     const NOMAD_BASE::Point              & refParetoIdealPt,
//                     const NOMAD_BASE::Point              & refParetoNadirPt)
//{
//    std::vector<size_t>             bbeTmp;
//    std::vector<NOMAD_BASE::Double> timeTmp;
//
//    // Hypervolume values are put into a new obj vector
//    _obj.clear();
//
//    // Compute pareto profiles for increasing values of bbe
//    std::vector<NOMAD_BASE::EvalPoint>::const_iterator itMobj = _mobj.begin();
//    std::vector<NOMAD_BASE::Point> pareto(1, itMobj->getFs(NOMAD_BASE::defaultFHComputeType)); // First fs point alway inserted for starting
//    std::vector<size_t>::const_iterator itBBE = _bbe.begin();
//    bbeTmp.push_back(*itBBE);
//    size_t nb_dom;
//    _obj.push_back(0.0); // We must have f(x0)=0
//    _nb_dominating_ref_obj.push_back(0); // Laziness. We should test if fs(x0) are below ref objs.
//    ++itBBE;
//    ++itMobj;
//    for ( itMobj ; itMobj < _mobj.end() ; itMobj++, itBBE++ )
//    {
//        if ( update_pareto_single(*itMobj, pareto) )
//        {
//            bbeTmp.push_back(*itBBE);
//            NOMAD_BASE::Double hv = -compute_hv(pareto, refParetoIdealPt, refParetoNadirPt, nb_dom);
//            _obj.push_back(hv); // We want a decrease in the objective function
//            _nb_dominating_ref_obj.push_back(nb_dom);
//        }
//    }
//
//    _bbe=bbeTmp;
//    _sol_fx = _obj.back();
//
//    // Transfer into a single value hypervolume is complete. No need for mobj anymore.
//    _mobj.clear();
//
//    // Update number of pareto points for this
//    _nb_pareto_points = pareto.size();
//    _has_sol = true;
//
//    return true;
//}

///*-----------------------------------*/
///*          compute solution         */
///*-----------------------------------*/
//bool RUNNERPOST::Result::compute_hypervolume_solution ( int n    ,
//                                        size_t  bbe,
//                                        const std::vector<NOMAD_BASE::Point> & combinedPareto,
//                                        const NOMAD_BASE::Point              & refParetoIdealPt,
//                                        const NOMAD_BASE::Point              & refParetoNadirPt)
//{
//
//    // NOTE: sol_xe has no sense for multi-obj
//
//    if ( _bbe.empty() || _mobj.empty())
//    {
//        size_t sol_bbe = _sol_bbe;
//        clear_solution();
//        _sol_bbe = sol_bbe;
//        return false;
//    }
//
//    clear_solution();
//
//    _sol_bbe = bbe;
//    _nb_pareto_points = 0;
//
//    size_t p = _bbe.size();
//
//    // Test if no feasible point has been obtained
//    if ( _last_x.empty() && ! _bbe.empty() && ! _mobj.empty() && p==_mobj.size() )
//    {
//        // Cases where the last line of stats file contains "no feasible ...." or if the only line in the file is the initial point and is not feasible
//        if ( _use_h_for_obj ) // Case where f= h and h != Inf
//        {
//            _is_infeas = true;
//        }
//        else if ( _mobj.back().getFs(NOMAD_BASE::defaultFHComputeType)[0]==NOMAD_BASE::INF)
//        {
//            _is_infeas  = true ; // Case multi obj, h > 0 and f[0] = Inf
//        }
//        else
//        {
//            _is_infeas  = true ;  // Case where run failed
//        }
//        return false;
//    }
//
//    if (_last_x.empty()                        ||
//        _bbe.empty()                           ||
//        _mobj.empty()                          ||
//        p != _mobj.size()                      ||
//        ( _sol_bbe < NOMAD_BASE::INF_SIZE_T && _bbe[0] > _sol_bbe )    )
//    {
//        clear_solution();
//        _sol_bbe = bbe;
//        return false;
//    }
//
//    if ( _sol_bbe == NOMAD_BASE::INF_SIZE_T )
//    {
//        _sol_bbe = _bbe[p-1];
//    }
//
//    // Compute hypervolume and update the solution. Also computes the number of pareto
//    // points and the number of points undominated by the nadir ref
//
//    if (! compute_hypervolume_for_obj(bbe, combinedPareto, refParetoIdealPt, refParetoNadirPt))
//    {
//        clear_solution();
//        return false;
//    }
//
//    _has_sol = true;
//    _sol_bbe = _bbe.back();
//    _sol_fxe = _obj.back();
//    _sol_fx  = _sol_fxe;
//
//    return true;
//}

/*-----------------------------------*/
/*          compute solution         */
/*-----------------------------------*/
bool RUNNERPOST::Result::compute_solution ( int n    ,
                               size_t  bbe)
{
    
    if ( _bbe.empty() || _obj.empty() )
    {
        size_t sol_bbe = _sol_bbe;
        clear_solution();
        _sol_bbe = sol_bbe;
        return false;
    }
    
    clear_solution();
    
    _sol_bbe = bbe;
    // _nb_pareto_points = 0;
    
    size_t p = _bbe.size();
    
    // Test if no feasible point has been obtained
    if ( _last_x.empty() && ! _bbe.empty() && ! _obj.empty() && p==_obj.size() )
    {
        // Cases where the last line of stats file contains "no feasible ...." or if the only line in the file is the initial point and is not feasible
        // P
        if ( _use_h_for_obj ) // Case where f= h and h != Inf
        {
            _is_infeas = true;
        }
        else if ( _obj.back()==INF ) // Case f !=h and h > 0 and f = Inf
        {
            _is_infeas = true;
        }
        else
        {
            _is_infeas  = true ;  // Case where run failed
        }
        return false;
    }
    
    if ( _last_x.empty()                       ||
        _bbe.empty()                           ||
        _obj.empty()                           ||
        p != _obj.size()                       ||
        ( _sol_bbe < INF_SIZE_T && _bbe[0] > _sol_bbe )    )
    {
        clear_solution();
        _sol_bbe = bbe;
        return false;
    }
    
    if ( _sol_bbe == INF_SIZE_T )
    {
        _sol_bbe = _bbe[p-1];
    }

    
    // TODO
//    if ( n != (int)_last_x.size() )
//    {
//        clear_solution();
//        return false;
//    }
//    _sol_xe.resize ( n );
//    n = 0;
//
//    _sol_xe[n++] = std::stod(_last_x);
//    if (_sol_xe[n++] == INF)
//    {
//        clear_solution();
//        return false;
//    }
    
    
    _has_sol = true;
    _sol_fxe = _obj[p-1];
    
    // get fx
    _sol_fx   = _obj[0];
    
    if ( p == 1 )
        return true;
    
    for ( size_t k = 1 ; k < p ; ++k )
    {
        if ( _bbe[k] > _sol_bbe )
            return true;
        _sol_fx = _obj[k];
    }
    
    _sol_bbe = _bbe[p-1];
    _sol_fx  = _sol_fxe;
    
    return true;
}





/*------------------------------------------------------*/
/*  get the solution for a given number of evaluations  */
/*------------------------------------------------------*/
double RUNNERPOST::Result::get_sol ( const size_t bbe) const
{
    double cur = INF;
    int n = static_cast<int> ( _bbe.size() );
    
    for ( int k = 0 ; k < n ; ++k )
    {
        if ( _bbe[k] > bbe )
            return cur;
        cur = _obj[k];
    }
    
    return cur;
}

/*--------------------------------------------------------*/
/*  get the time for a given number of evaluations (bbe)  */
/*  if bbe = INF, get the maximum (i.e., total) time.      */
/*--------------------------------------------------------*/
double RUNNERPOST::Result::get_time(const size_t bbe) const
{
    double cur = 0;
    int n = static_cast<int>(_bbe.size());

    for (int k = 0; k < n; ++k)
    {
        cur = _time[k];
        if (bbe < INF_SIZE_T && _bbe[k] > bbe)
        {
            break;
        }
    }

    return cur;
}

/*-----------------------------------*/
/* get the solution for a given time */
/*-----------------------------------*/
double RUNNERPOST::Result::get_sol_by_time(const size_t time) const
{
    double cur = INF;
    int n = static_cast<int> ( _bbe.size() );
    for ( int k = 0 ; k < n ; ++k )
    {
        if ( _time[k] > time )
            return cur;
        cur = _obj[k];
    }
    return cur;
}


/*----------------------------------*/
/*               display            */
/*----------------------------------*/
void RUNNERPOST::Result::display ( ) const
{
    int n = static_cast<int> ( _bbe.size() );
    for ( int k = 0 ; k < n ; ++k )
        std::cout << _bbe[k] << " " << _obj[k] << std::endl;
}
