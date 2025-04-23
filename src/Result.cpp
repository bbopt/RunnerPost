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
void RUNNERPOST::Result::reset (  )
{
    _bbe.clear();
    _obj.clear();
    _mobj.clear();
    _last_x.clear();
    
    //_use_hypervolume_for_obj = use_hypervolume_for_obj;
    // For now use_std_h is always true;      // _use_std_h = use_std_h ;  
    _nb_obj = 0; // Need to be updated
    clear_solution();
}

/*------------------------*/
/*      clear solution    */
/*------------------------*/
void RUNNERPOST::Result::clear_solution ( void )
{
    _sol_bbe    = INF_SIZE_T;
    _sol_fx     = INF;
    _sol_xe.clear();
    _sol_fxe    = INF;

    _has_sol = false;
    _is_infeas = false;
}
/*--------------------------------*/
/*      clear best infeasible     */
/*--------------------------------*/
void RUNNERPOST::Result::clear_best_infeas ( void )
{
    
    _bestInf_h  = INF;
    _bestInf_xe.clear();
    _bestInf_bbe = INF_SIZE_T;

}


/*----------------------------------*/
/*          read results            */
/*----------------------------------*/
bool RUNNERPOST::Result::read ( std::ifstream & in , size_t max_bbe , const RUNNERPOST::StatOutputTypeList & sotList , const double & feasibilityThreshold )
{

    std::string   s, line;
    size_t        bbe =0;
    double time = 0 , obj, obj_prev = INF, h, h_prev = INF ;
    size_t nbExtra = 0;
    
    bool first_line = true;
    
    bool hasFeasEval = false;
    
    // Number of objectives
    _nb_obj = std::count(sotList.begin(),sotList.end(),StatOutputType::OBJ);
    
    const size_t m = _nb_obj + std::count(sotList.begin(),sotList.end(),StatOutputType::CST);
    if (m == 0)
    {
        std::cerr << "Result::read. Output format has no objective and no constraint." << std::endl;
        return false;
    }
    
    const bool hasExtraO = (std::count(sotList.begin(),sotList.end(),StatOutputType::EXTRA_O) > 0);
    
    // Test the last element of sotList for EXTRA_O
    if (hasExtraO && !sotList.back().isOfType(StatOutputType::Type::EXTRA_O))
    {
        std::cerr << "Result::read. Output EXTRA_O should the last element." << std::endl;
        return false;
    }
    
    const bool hasCntEval = (std::count(sotList.begin(),sotList.end(),StatOutputType::CNT_EVAL) > 0);
    
    if ( m-_nb_obj > 0 )
    {
        if ( !_use_std_h )
        {
            std::cout << "Other than standard h calculation not implemented" <<std::endl;
            return false;
        }
    }
    
    int nb_fields=0; // Number of fields in the file. For history file we have nb_fields = n + m. For stats files nb_fields will be larger. This is determined from the first line.
    
    double *bbo = new double[m];
    
    while ( !in.eof() )
    {
        if (!std::getline(in, line))
        {
            if (first_line)
            {
                std::cout << "Result file is empty" <<std::endl;
                delete [] bbo;
                return false;
            }
        }
        
        // No more lines to read.
        if (line.empty())
        {
            break;
        }

        // Put the line in a string stream for reading values.
        std::istringstream iss(line);

        // This is mostly for managing the case of a number of fields not compatible with format
        if ( first_line )
        {
            const std::string lineTmp=line;
            
            // number of fields.
            size_t pos = 0;
            nb_fields = 0;
            
            // Detect some decoration in the first line of the result file
            // We can have a comment line starting with # or %
            // We can have some extra information : DIM = 10, for example
            RUNNERPOST::toUpperCase(line);
            if (line.find("DIM") != std::string::npos || line.find("#") != std::string::npos || line.find("%") != std::string::npos)
            {
                continue;
            }
            
            // At least we have single field
            if (line.find(" ") == std::string::npos && !line.empty())
            {
                nb_fields = 1;
            }
            
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
            if (nb_fields != sotList.size())
            {
                if (hasExtraO)
                {
                    nbExtra = nb_fields - m;
                }
                else
                {
                    std::cout << "(1) Result file does not comply with stats_file_output type: " << std::endl;
                    std::cout << "     + Line read is \"" << lineTmp << "\"" << std::endl;
                    std::cout << "     + Expected stats output is \"" << sotList << "\"" <<std::endl;
                    delete [] bbo;
                    return false;
                }
            }
        }
        
        // Case without eval count. Auto-increment bbe. One line=one eval
        if (!hasCntEval) // That is no CNT_EVAL output type
        {
            bbe++;
        }
        
        size_t i= 0;
        obj = 0;
        h = 0;
        time = 0.0;
        _last_x.clear();
        
        // Read according to the format
        for (const auto & sot: sotList)
        {
            if ( sot.isOfType(StatOutputType::Type::CNT_EVAL) )
            {
                iss >> bbe;
            }
            else if(sot.isOfType(StatOutputType::Type::SOL))
            {
                iss >> s;
                _last_x.append(s+" ");
            }
            else if(sot.isOfType(StatOutputType::Type::TIME))
            {
                iss >> time;
            }
            else if(sot.isOfType(StatOutputType::Type::OBJ) || sot.isOfType(StatOutputType::Type::CST))
            {
                iss >> bbo[i];
                if (iss.fail())
                {
                    bbo[i] = INF;
                    obj = INF;
                }
                if (sot.isOfType(StatOutputType::Type::CST))
                {
                    h += pow( std::max( bbo[i], feasibilityThreshold ),2);
                }
                else
                {
                    // For multi-obj, obj is not used. bbo[i] are processed instead
                    obj = bbo[i];
                }
                i++;
            }
            else if(sot.isOfType(StatOutputType::Type::FEAS))
            {
                iss >> bbo[i];
                if (iss.fail() || bbo[i] > 0)
                {
                    bbo[i] = INF;
                    h = INF;
                }
            }
            else if (sot.isOfType(StatOutputType::Type::EXTRA_O)) // the extra outputs are ignored. They must be the last info in the line. Just skip the remaining.
            {
                break;
            }
            else
            {
                std::cout << "(2) Result file does not comply with stats_file_output type: " << std::endl;
                std::cout << "      + Line read is \"" << line << "\"" << std::endl;
                std::cout << "      + Expected stats output is \"" << sotList << "\"" <<std::endl;
                delete [] bbo;
                return false;
            }
        }
        
        // Consistency check
        if ( first_line && bbe!=1)
        {
            std::cout << "Warning: Result file has a first line with evaluation counter bbe = " << bbe << ". This can affect post-processing." << std::endl;
        }

        // Keep improving feasible evaluations
        if (h <= feasibilityThreshold)
        {
            // For multi-objectif, all evals are kept.
            // Post-processing will create the pareto front
            if ( _nb_obj > 1)
            {
                int i = 0;
                std::vector<double> objs;
                for (const auto & sot: sotList)
                {
                    if (sot.isObjective())
                    {
                        objs.push_back(bbo[i]);
                    }
                    i++;
                }
                _bbe.push_back( bbe );
                _mobj.push_back(objs);
            }
            else if ( obj < obj_prev && bbe <= max_bbe )
            {
                _bbe.push_back( bbe );
                _obj.push_back( obj );
                _time.push_back(time);
                obj_prev = obj;
            }
            
            if (!hasFeasEval)
            {
                hasFeasEval = true; // Once we have the first feasible eval, we are not interested in infeasible evals
                _bbeForH.push_back( bbe );
                _infH.push_back( 0.0 );
                _timeForH.push_back(time);
                h_prev = 0.0;
            }
        }
        // and improving infeasible evaluations
        else
        {
            if ( !hasFeasEval && h < h_prev && bbe <= max_bbe )
            {
                _bbeForH.push_back( bbe );
                _infH.push_back( h );
                _timeForH.push_back(time);
                h_prev = h;
            }
        }
        
        // No need to parse for more eval than required
        if (bbe > max_bbe)
        {
            break;
        }

         // Total time and total bbe - for time stats.
        _totalBbe = bbe;
        _totalTime = time; // Note: when using history file, time stays 0

        if ( first_line )
        {
            first_line = false;
        }
    }
    
    delete [] bbo;
    
    return true;
}

/*-----------------------------------*/
/*       get the last bbe entry      */
/*-----------------------------------*/
size_t RUNNERPOST::Result::get_last_bbe ( void ) const
{
    if ( _bbe.empty() && _bbeForH.empty())
        return 0 ;
    if ( _bbe.empty() )
        return _bbeForH[_bbeForH.size()-1];
    else
        return _bbe[_bbe.size()-1];
}

/*-----------------------------------*/
/*       get the last time entry      */
/*-----------------------------------*/
size_t RUNNERPOST::Result::get_last_time ( void ) const
{
    if ( _time.empty())
        return 0 ;
    else
        return _time.size();
}

void RUNNERPOST::Result::writeToStatsFile(size_t i_pb, size_t i_algo, size_t i_pb_inst, size_t pb_size) const
{
    std::string statsFileName = "stats."+std::to_string(i_pb)+"."+std::to_string(i_algo)+"."+std::to_string(i_pb_inst)+".txt";
    std::ofstream out(statsFileName, std::ios::out);;
    if (!out.is_open())
    {
        std::cerr << "Error opening stats file " << statsFileName << std::endl;
        return;
    }
    out << " DIM = " << pb_size << std::endl;
    for (size_t i=0; i<_bbe.size(); i++)
    {
        out << _bbe[i] << " " << _obj[i] << std::endl;
    }
    out.close();
}


// CHT TEMP FOR DATA MANIPULATION -- DO NOT KEEP -- DANGEROUS
void RUNNERPOST::Result::TMPtransform()
{

    // MANIPULATION OF BBE --> for PP in paper
    for (size_t i=1; i<_obj.size(); i++)
    {
        double fact = 0.005*_bbe[i];
        _bbe[i] = std::floor(_bbe[i] * pow(2.0,fact));
        if (_bbe[i] < _bbe[i-1])
        {
            _bbe[i] = _bbe[i-1];
        }
    }

//    // MANUPULATION OF OBJ --> for DP in paper
//    for (size_t i=0; i<_obj.size(); i++)
//    {
//        double fact = 1.0*i/(_obj.size()-1);
//        double tmp = _obj[i] + 0.14*fact*_obj[i];
//        if (i>0 && tmp > _obj[i-1])
//        {
//            tmp = _obj[i-1]*0.9999999;
//        }
//        _obj[i] = tmp;
//    }
}


RUNNERPOST::MOCompareType RUNNERPOST::Result::compMultiObjForDominate(const std::vector<double> & f1, const std::vector<double> & f2)
{
    // COMPARAISON ONLY BETWEEN FEASIBLE POINTS
    // f1 dominates f2 ?
    
    MOCompareType compareFlag = MOCompareType::INDIFFERENT;
    
    // Comparing objective vectors of different size is undefined
    if (f1.size() != f2.size())
    {
        std::cerr << "ERROR: Results::compMultiObjForDominate. Incompatible number of objective." << std::endl;
        return compareFlag;
    }
    
    bool isbetter = false;
    bool isworse = false;
    for (size_t i = 0; i < f1.size(); ++i)
    {
        if (f1[i] < f2[i])
        {
            isbetter = true;
        }
        if (f2[i] < f1[i])
        {
            isworse = true;
        }
        if (isworse && isbetter)
        {
            break;
        }
    }
    if (isworse)
    {
        compareFlag = isbetter ? MOCompareType::INDIFFERENT : MOCompareType::DOMINATED;
    }
    else
    {
        compareFlag = isbetter ? MOCompareType::DOMINATING : MOCompareType::EQUAL;
    }
    return compareFlag;
}


bool RUNNERPOST::Result::update_pareto_single ( const std::vector<double> & fs ,
                                               std::vector<std::vector<double>> & combinedPareto) const
{
    bool updated_pareto = false;

    bool insert = true;
    std::vector<std::vector<double>>::iterator itPf = combinedPareto.begin();
    while (itPf != combinedPareto.end())
    {
        
        MOCompareType moCompFlag = compMultiObjForDominate(fs, *itPf);
        if (moCompFlag == MOCompareType::DOMINATED || moCompFlag == MOCompareType::EQUAL)
        {
            return false;
        }
        if (moCompFlag == MOCompareType::DOMINATING)
        {
            itPf = combinedPareto.erase(itPf);
            updated_pareto = true;
            continue;
        }
        itPf++;
    }
    if (insert)
    {
        combinedPareto.push_back(fs);
        updated_pareto = true;
    }
    return updated_pareto;
}


bool RUNNERPOST::Result::update_pareto ( const size_t bbeMax ,
                                        std::vector<std::vector<double>> & pareto) const
{
    bool updated_pareto = false;

    std::vector<std::vector<double>>::const_iterator itMobj ;

    size_t bbeMobj = 0 ;
    // udpate pareto using all multi objective evaluations
    // All evaluations are stored
    for ( itMobj = _mobj.begin() ; itMobj < _mobj.end() ; itMobj++, bbeMobj++ )
    {
        if ( bbeMobj > bbeMax)
            break;

        if (pareto.size() == 0)
        {
            pareto.push_back(*itMobj);
            updated_pareto = true;
            continue;
        }
        updated_pareto = update_pareto_single(*itMobj, pareto) || updated_pareto;
    }
    return updated_pareto;
}

// Compute scaled hypervolume of pareto front with respect to ref ideal and nadir points. See LS paper.
double RUNNERPOST::Result::compute_hv (const std::vector<std::vector<double>> & pareto,
                                                   const std::vector<double>  & refParetoIdealPt,
                                                   const std::vector<double>  & refParetoNadirPt,
                                                   size_t & nb_dominating_ref_obj)
{
    double scaledHV;
    nb_dominating_ref_obj = 0;

    // data for hv
    const int nb_obj = static_cast<int>(refParetoIdealPt.size());
    if (nb_obj != refParetoNadirPt.size() || nb_obj != pareto[0].size())
    {
        std::cout << "Inconsistent dimension of the number of objectives" <<std::endl;
        return RUNNERPOST::INF;
    }

    
#ifdef LIB_HYPERVOLUME
    
    double * dpareto = new double[nb_obj*pareto.size()];
    size_t k=0;
    std::vector<std::vector<double>> Tpareto; // Transformed (scaled) pareto (see LS paper)

    for (const auto & p : pareto)
    {
        std::vector<double> Tpt(nb_obj);
        bool dominating = true;
        for (size_t j =0 ; j < nb_obj; j++)
        {
            Tpt[j] = p[j]-refParetoIdealPt[j];
            if (refParetoIdealPt[j] != refParetoNadirPt[j])
            {
                Tpt[j] /= (refParetoNadirPt[j] - refParetoIdealPt[j]);
            }
            if ( dominating && Tpt[j] > 1.0)
                dominating = false;
            dpareto[k++] = Tpt[j];
        }
        if (dominating)
            nb_dominating_ref_obj++;
    }

    double reference[nb_obj]; // scaled reference point is 1 for all objectives
    for (size_t j =0 ; j < nb_obj; j++)
    {
        reference[j] = 1.0;
    }


// TEMP for testing hv computation
//    std::ifstream fin ( "viennet.txt" );
//    size_t lines=0;
//    double l,m,r,lmax=-100000000,mmax=-10000000,rmax=-1000000000;
//
//    while ( !fin.eof() )
//    {
//        lines ++;
//        fin >> l  ;
//        fin >> m  ;
//        fin >> r  ;
//        dpareto[k++] = l;
//        dpareto[k++] = m;
//        dpareto[k++] = r;
//        if ( l > lmax)
//        {
//            lmax = l;
//        }
//        if ( m > mmax)
//        {
//            mmax = m;
//        }
//        if ( r > rmax)
//        {
//            rmax = r;
//        }
//
//    }
//    fin.close();
//    double reference[3] = { lmax, mmax, rmax };
//    scaledHV = fpli_hv(dpareto, 3, 874, reference);
//    if (std::fabs(scaledHV - 3.86877) > 0.01)
//      std::cout << "ERROR in hv compuation" <<std::endl;
    
    scaledHV = fpli_hv(dpareto, static_cast<int>(nb_obj), static_cast<int>(pareto.size()), reference);
    delete[] dpareto;

#else
    if (nb_obj > 2)
    {
        std::cout << "Number of objective is greater than 2. Requires to enable hypervolume computation library (HV)." <<std::endl;
        return RUNNERPOST::INF;
    }
    
    double VolR = 1.0;
    for (size_t j =0 ; j < nb_obj; j++)
    {
        if (std::fabs(refParetoIdealPt[j] - refParetoNadirPt[j]) > 1.E-16)
        {
            VolR *= (refParetoNadirPt[j] - refParetoIdealPt[j]);
        }
    }
    
    double io = 0.0;
    size_t i ;
    for (i = 0; i < pareto.size()-1 ; i++)
    {
        io += (pareto[i+1][0]-pareto[i][0])*std::max(refParetoNadirPt[1]-pareto[i][1],0.0);
    }

    
    io += std::max(refParetoNadirPt[0]-pareto[i][0],0.0)*std::max(refParetoNadirPt[1]-pareto[i][1],0.0);
    
    scaledHV = -io /VolR;

#endif
    

    return scaledHV;
}


// Replaces the objectives fs by single f values with monotonic decrease. The value is computed as the hypervolume between ...
bool RUNNERPOST::Result::compute_hypervolume_for_obj ( const size_t bbeMax         ,
                     const std::vector<std::vector<double>> & refCombinedPareto,
                     const std::vector<double>              & refParetoIdealPt,
                     const std::vector<double>              & refParetoNadirPt)
{
    std::vector<size_t>             bbeTmp;
    std::vector<std::vector<double>> timeTmp;

    // Hypervolume values are put into a new obj vector
    _obj.clear();

    // Compute pareto profiles for increasing values of bbe
    std::vector<std::vector<double>>::const_iterator itMobj = _mobj.begin();
    std::vector<std::vector<double>> pareto(1, *itMobj); // First fs point alway inserted for starting
    std::vector<size_t>::const_iterator itBBE = _bbe.begin();
    bbeTmp.push_back(*itBBE);
    size_t nb_dom;
    _obj.push_back(0.0); // We must have f(x0)=0
    _nb_dominating_ref_obj.push_back(0); // Laziness. We should test if fs(x0) are below ref objs.
    ++itBBE;
    ++itMobj;
    for ( itMobj ; itMobj < _mobj.end() ; itMobj++, itBBE++ )
    {
        if ( update_pareto_single(*itMobj, pareto) )
        {
            bbeTmp.push_back(*itBBE);
            double hv = -compute_hv(pareto, refParetoIdealPt, refParetoNadirPt, nb_dom);
            _obj.push_back(hv); // We want a decrease in the objective function
            _nb_dominating_ref_obj.push_back(nb_dom);
        }
    }

    _bbe=bbeTmp;
    _sol_fx = _obj.back();

    // Transfer into a single value hypervolume is complete. No need for mobj anymore.
    _mobj.clear();

    // Update number of pareto points for this
    _nb_pareto_points = pareto.size();
    _has_sol = true;

    return true;
}

/*-----------------------------------*/
/*          compute solution         */
/*-----------------------------------*/
bool RUNNERPOST::Result::compute_hypervolume_solution ( int n    ,
                                        size_t  bbe,
                                        const std::vector<std::vector<double>> & combinedPareto,
                                        const std::vector<double>              & refParetoIdealPt,
                                        const std::vector<double>              & refParetoNadirPt)
{

    // NOTE: sol_xe has no sense for multi-obj

    if ( _bbe.empty() || _mobj.empty())
    {
        size_t sol_bbe = _sol_bbe;
        clear_solution();
        _sol_bbe = sol_bbe;
        return false;
    }

    clear_solution();

    _sol_bbe = bbe;
    _nb_pareto_points = 0;

    size_t p = _bbe.size();

//    // Test if no feasible point has been obtained
//    if (! _bbe.empty() && ! _mobj.empty() && p==_mobj.size() )
//    {
//        // Cases where the last line of stats file contains "no feasible ...." or if the only line in the file is the initial point and is not feasible
//        if ( _use_h_for_obj ) // Case where f= h and h != Inf
//        {
//            _is_infeas = true;
//        }
//        else if ( _mobj.back()[0]==RUNNERPOST::INF)
//        {
//            _is_infeas  = true ; // Case multi obj, h > 0 and f[0] = Inf
//        }
//        else
//        {
//            _is_infeas  = true ;  // Case where run failed
//        }
//        return false;
//    }

    if (_bbe.empty()                           ||
        _mobj.empty()                          ||
        p != _mobj.size()                      ||
        ( _sol_bbe < RUNNERPOST::INF_SIZE_T && _bbe[0] > _sol_bbe )    )
    {
        clear_solution();
        _sol_bbe = bbe;
        return false;
    }

    if ( _sol_bbe == RUNNERPOST::INF_SIZE_T )
    {
        _sol_bbe = _bbe[p-1];
    }

    // Compute hypervolume and update the solution. Also computes the number of pareto
    // points and the number of points undominated by the nadir ref

    if (! compute_hypervolume_for_obj(bbe, combinedPareto, refParetoIdealPt, refParetoNadirPt))
    {
        clear_solution();
        return false;
    }

    _has_sol = true;
    _sol_bbe = _bbe.back();
    _sol_fxe = _obj.back();
    _sol_fx  = _sol_fxe;

    return true;
}

/*-----------------------------------*/
/*          compute solution         */
/*-----------------------------------*/
bool RUNNERPOST::Result::compute_solution_prev ( int n    ,
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
    

    _nb_pareto_points = 0;
    
    size_t p = _bbe.size();
    
//    // Test if no feasible point has been obtained
//    if ( ! _bbe.empty() && ! _obj.empty() && p==_obj.size() )
//    {
//        // Cases where the last line of stats file contains "no feasible ...." or if the only line in the file is the initial point and is not feasible
//        // P
//        if ( _use_h_for_obj ) // Case where f= h and h != Inf
//        {
//            _is_infeas = true;
//        }
//        else if ( _obj.back()==INF ) // Case f !=h and h > 0 and f = Inf
//        {
//            _is_infeas = true;
//        }
//        else
//        {
//            _is_infeas  = true ;  // Case where run failed
//        }
//        return false;
//    }
    
    if ( _bbe.empty()                           ||
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

    _sol_bbe = _bbe.back();
    
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
    _sol_fx = _obj.back();
    
//    // get fx
//    _sol_fx   = _obj[0];
    
//    if ( p == 1 )
//        return true;
    
//    for ( size_t k = 1 ; k < p ; ++k )
//    {
//        if ( _bbe[k] > _sol_bbe )
//            return true;
//        _sol_fx = _obj[k];
//    }
//
    
//    _sol_fx  = _sol_fxe;
    
    return true;
}
bool RUNNERPOST::Result::compute_solution ( int n    ,
                                           size_t  bbe)
{
    if (_bbe.size() != _obj.size())
    {
        std::cout << "Compute_solution: Inconsistent number of evaluations and objectives" <<std::endl;
        return false;
    }
    
    
    if ( _bbe.empty() )
    {
        size_t sol_bbe = _sol_bbe;
        clear_solution();
        _sol_bbe = sol_bbe;
    
        _is_infeas = true;
        return false;
    }
    
    clear_solution();
    
    _nb_pareto_points = 0;
    
    
    
// More test to identify that no feasible point has been obtained
    if ( _obj.back()==INF ) // Case f = Inf
    {
        clear_solution();
        _sol_bbe = INF_SIZE_T;
        _is_infeas = true;
    }

    // We have a feasible solution
    // Update the characteristics of the solution (bbe, f, x)
    
    _sol_bbe = _bbe.back();
    _has_sol = true;
    _sol_fx = _obj.back();
     
    // If last_x is not empty we can extract the solution
    _sol_xe.clear();
    if (!_last_x.empty())
    {
        _sol_xe.resize(n);
        
        // Extract the last x given as a string with space separated values into a vector of doubles
        std::istringstream iss(_last_x);
        for (int i = 0; i < n; i++)
        {
            iss >> _sol_xe[i];
            if (iss.fail())
            {
                _sol_xe.clear();
                break;
            }
        }
    }
    
    return true;
}

bool RUNNERPOST::Result::compute_best_infeasible ( int n    ,
                                                   size_t  bbe)
{
    // We already have computed a solution (feasible)
    if (!_is_infeas)
    {
        return true;
    }
    
    clear_best_infeas();
    
    if (_bbeForH .size() != _infH.size())
    {
        std::cout << "Compute_solution: Inconsistent number of evaluations and infeasibility h" <<std::endl;
        return false;
    }
    
    
    if ( _bbeForH.empty() )
    {
        return false;
    }

    _nb_pareto_points = 0;
    
    // More test to identify that no infeasible point has been obtained
    if ( _infH.back()==INF ) // Case h = Inf
    {
        clear_best_infeas();
        _bestInf_bbe = INF_SIZE_T;
    }

    // We have a valid infeasible solution
    // Update the characteristics of the best infeasible (bbe, h, x)
    
    _bestInf_bbe = _bbeForH.back();
    _has_sol = false;
    _bestInf_h = _infH.back();
     
    // If last_x is not empty we can extract the solution
    _bestInf_xe.clear();
    if (!_last_x.empty())
    {
        _bestInf_xe.resize(n);
        
        // Extract the last x given as a string with space separated values into a vector of doubles
        std::istringstream iss(_last_x);
        for (int i = 0; i < n; i++)
        {
            iss >> _bestInf_xe[i];
            if (iss.fail())
            {
                _bestInf_xe.clear();
                break;
            }
        }
    }
    
    return true;
}





/*-----------------------------------------------------------------*/
/*  get the solution (feasible) for a given number of evaluations  */
/*-----------------------------------------------------------------*/
double RUNNERPOST::Result::get_sol ( const size_t bbe) const
{
    double cur = INF;
    if (_obj.empty())
    {
        return cur;
    }
    int n = static_cast<int> ( _bbe.size() );
    if (n > 0 && _bbe[n-1] <= bbe)
    {
        return _obj[n-1];
    }
    for ( int k = 0 ; k < n ; ++k )
    {
        if ( _bbe[k] > bbe )
            return cur;
        cur = _obj[k];
    }
    
    return cur;
}

/*-----------------------------------------------------------------*/
/*  get the solution (feasible) for a given time                   */
/*-----------------------------------------------------------------*/
double RUNNERPOST::Result::get_sol_by_time ( const double &time) const
{
    double cur = INF;
    int n = static_cast<int> ( _time.size() );
    if (n > 0 && _time[n-1] <= time)
    {
        return _obj[n-1];
    }
    for ( int k = 0 ; k < n ; ++k )
    {
        if ( _time[k] > time )
            return cur;
        cur = _obj[k];
    }
    
    return cur;
}

/*------------------------------------------------------*/
/*  get the solution for a given number of evaluations  */
/*------------------------------------------------------*/
double RUNNERPOST::Result::get_best_infeas ( const size_t bbe) const
{
    double cur = INF;
    int n = static_cast<int> ( _bbeForH.size() );
    if (n > 0 && _bbeForH[n-1] <= bbe)
    {
        return _infH[n-1];
    }
    for ( int k = 0 ; k < n ; ++k )
    {
        if ( _bbeForH[k] > bbe )
            return cur;
        cur = _infH[k];
    }
    
    return cur;
}

/*------------------------------------------------------*/
/*  get the solution for a time                         */
/*------------------------------------------------------*/
double RUNNERPOST::Result::get_best_infeas_by_time ( const double & time) const
{
    double cur = INF;
    int n = static_cast<int> ( _timeForH.size() );
    if (n > 0 && _timeForH[n-1] <= time)
    {
        return _infH[n-1];
    }
    for ( int k = 0 ; k < n ; ++k )
    {
        if ( _timeForH[k] > time )
            return cur;
        cur = _infH[k];
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
    if (_time.size() > 0)
    {
        int n = static_cast<int>(_bbe.size());
        
        for (int k = 0; k < n; ++k)
        {
            cur = _time[k];
            if (bbe < INF_SIZE_T && _bbe[k] > bbe)
            {
                break;
            }
        }
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
