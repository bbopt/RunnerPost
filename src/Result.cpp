#include "Result.hpp"
#include "Output.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>


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

    _has_feas_sol = false;
}

std::string RUNNERPOST::Result::read_file_to_string_fast(const std::string& filename)
{
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
    if (!ifs)
    {
        throw std::system_error(errno, std::generic_category(), "opening file failed: " + filename);
    }

    std::ifstream::pos_type end = ifs.tellg();
    if (end < 0)
    {
        // Could not determine size; fall back to streambuf method below
        ifs.close();
        std::ostringstream ss;
        std::ifstream ifs2(filename, std::ios::binary);
        ss << ifs2.rdbuf();
        return ss.str();
    }

    const std::size_t file_size = static_cast<std::size_t>(end);
    std::string contents;
    contents.resize(file_size);

    ifs.seekg(0, std::ios::beg);
    if (!ifs.read(&contents[0], static_cast<std::streamsize>(file_size)))
    {
        // If read fails but some bytes were read, contents may be partially filled.
        throw std::system_error(errno, std::generic_category(), "reading file failed: " + filename);
    }
    return contents;
}


/*----------------------------------*/
/*          read results            */
/*----------------------------------*/
bool RUNNERPOST::Result::read ( std::ifstream & in , size_t max_bbe , const RUNNERPOST::StatOutputTypeList & sotList , const double & ineqConsFeasibilityThreshold, const double & eqConsFeasibilityThreshold  )
{

    std::string   s, line;
    size_t        bbe =0;
    double time = 0 , obj, obj_prev = INF, h, h_prev = INF ;
    size_t nbExtra = 0;
    
    bool first_line = true;
    
    bool hasFeasEval = false;
    
    // Number of objectives
    _nb_obj = std::count(sotList.begin(),sotList.end(),StatOutputType::OBJ);
    
    const size_t m = _nb_obj + std::count(sotList.begin(),sotList.end(),StatOutputType::CST) + std::count(sotList.begin(),sotList.end(),StatOutputType::EQCST);
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
    
    // Read entire file into a single string
    std::string buf;
    in.seekg(0, std::ios::end);
    buf.reserve(static_cast<size_t>(in.tellg()));
    in.seekg(0);
    buf.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    
    std::istringstream iss(std::move(buf));
    while (std::getline(iss, line))
    {
        
        // No more lines to read.
        if (line.empty())
        {
            break;
        }

        // Replace tabs by spaces
        std::replace(line.begin(), line.end(), '\t', ' ');
        
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
            else if(sot.isOfType(StatOutputType::Type::OBJ) || sot.isOfType(StatOutputType::Type::CST) || sot.isOfType(StatOutputType::Type::EQCST))
            {
                iss >> bbo[i];
                if (iss.fail())
                {
                    bbo[i] = INF;
                    obj = INF;
                }
                if (sot.isOfType(StatOutputType::Type::CST))
                {
                    if (bbo[i] > ineqConsFeasibilityThreshold)
                    {
                        h += pow( bbo[i],2);
                    }
                }
                else if (sot.isOfType(StatOutputType::Type::EQCST))
                {
                    if (abs(bbo[i]) > eqConsFeasibilityThreshold)
                    {
                        h += pow( bbo[i],2);
                    }
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
        if (h <= 0)
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
                hasFeasEval = true; // Once we have the first feasible eval, we are not interested in infeasible evals. Even when y axis plot is for h.
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

         // Total time and total bbe - for time stats.
        _totalBbe = bbe;
        _totalTime = time; // Note: when using history file, time stays 0

        if ( first_line )
        {
            first_line = false;
        }
        
        // No need to parse for more eval than required
        if (bbe > max_bbe)
        {
            break;
        }
    }
    
    
    if (!std::getline(in, line))
    {
        if (first_line)
        {
            std::cout << "Result file is empty" <<std::endl;
            delete [] bbo;
            return false;
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
    if ( _bbe.empty() ) // No feasible solution found. But we have infeasible points (used when h is of interest).
        return _bbeForH.back();
    else
        return _bbe.back();
}

/*-----------------------------------*/
/*       get the last time entry      */
/*-----------------------------------*/
size_t RUNNERPOST::Result::get_last_time ( void ) const
{
    if ( _time.empty() && _timeForH.empty())
        return 0 ;
    
    if (_time.empty())
        return _timeForH.back();
    else
        return _time.back();
}

double RUNNERPOST::Result::get_first_feas_fx() const
{
    double fx=INF;
    
    if (! _infH.empty() && _infH[0] == 0.0)
    {
        return _obj[0];
    }
    else
    {
        std::vector<double>::const_iterator it = std::find (_infH.begin(), _infH.end(), 0.0);
        if (it != _infH.end())
        {
            auto index = std::distance(_infH.begin(), it);
            auto bbe = _bbeForH[index];
            fx = get_sol(bbe, false);
        }
    }
    return fx;
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


//// CHT TEMP FOR DATA EXPERIMENTAL MANIPULATION -- DO NOT KEEP 
//void RUNNERPOST::Result::TMPtransform()
//{
//
//    // MANIPULATION OF BBE --> for PP in paper
//    for (size_t i=1; i<_obj.size(); i++)
//    {
//        double fact = 0.005*_bbe[i];
//        _bbe[i] = std::floor(_bbe[i] * pow(2.0,fact));
//        if (_bbe[i] < _bbe[i-1])
//        {
//            _bbe[i] = _bbe[i-1];
//        }
//    }
//
////    // MANUPULATION OF OBJ --> for DP in paper
////    for (size_t i=0; i<_obj.size(); i++)
////    {
////        double fact = 1.0*i/(_obj.size()-1);
////        double tmp = _obj[i] + 0.14*fact*_obj[i];
////        if (i>0 && tmp > _obj[i-1])
////        {
////            tmp = _obj[i-1]*0.9999999;
////        }
////        _obj[i] = tmp;
////    }
//}


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
                                               std::vector<std::vector<double>> & combinedPareto,
                                               bool sort) const
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
        
        // For bi-objective, let's sort.
        // This is required for bi-obj compute_hv
        if (sort && combinedPareto.size() > 1 && fs.size()==2)
        {
            std::sort(combinedPareto.begin(),combinedPareto.end(),[](const std::vector<double> & a, const std::vector<double> & b)
                      {
                        if (a[0] > b[0])
                        {
                          return false;
                        }
                        else if(b[0] > a[0])
                        {
                            return true;
                        }
                        else
                        {
                            // a[0]==b[0]
                            if (a[1] > b[1])
                            {
                                return false;
                            }
                            else if (b[1] > a[1])
                            {
                                return true;
                            }
                            else
                            {
                                // a[0] == b[0] && a[1]==b[1])
                                return false;
                            }
                        }
                          });
        }
        
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
        updated_pareto = update_pareto_single(*itMobj, pareto, true /*true: sort, false: do not sort*/) || updated_pareto;
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
        VolR *= (refParetoNadirPt[j] - refParetoIdealPt[j]);
    }
    if ( VolR < 1.E-16)
    {
        return 0.0;
    }
    
    double io = 0.0;
    size_t i ;
    for (i = 0; i < pareto.size()-1 ; i++)
    {
        // Test: f1 increasing, f2 decreasing
        if (pareto[i+1][0] < pareto[i][0])
        {
            std::cerr << "Result::compute_hv (bi-obj): pareto points should have f1 strictly increasing" << std::endl;
            return 0.0;
        }
        if (pareto[i+1][1] > pareto[i][1])
        {
            std::cerr << "Result::compute_hv (bi-obj): pareto points should have f2 strictly decreasing" << std::endl;
            return 0.0;
        }
        if (pareto[i][0] < refParetoIdealPt[0] || pareto[i][1] < refParetoIdealPt[1] )
        {
            std::cerr << "ERROR: Result::compute_hv (bi-obj): pareto points should be above Ideal" << std::endl;
            return 0.0;
            // This point is outside [I,N] hyperrectangle, below I. This is not supposed to happen
        }
        
        if ( pareto[i][1] > refParetoNadirPt[1])
        {
            continue;
            // This point is outside [I,N] hyperrectangle above N.
            // Not considered in io computation. Move to next point.
        }
        
        if (pareto[i][0] > refParetoNadirPt[0])
        {
            // Break because remaining points are above Nadir. We are done with io summation.
            break;
        }
        if (pareto[i+1][0] > refParetoNadirPt[0])
        {
            // Break because remaining points are above Nadir.
            // We are NOT done with io summation because
            // previous point is below N
            io += (refParetoNadirPt[0]-pareto[i][0])*(refParetoNadirPt[1]-pareto[i][1]);
            
            i++; // This is to prevent last part of the summation (i=m)
            break;
        }
        io += (pareto[i+1][0]-pareto[i][0])*(refParetoNadirPt[1]-pareto[i][1]);

    }
    // Last point of the pareto (i=m in the paper)
    if ( i < pareto.size() && pareto[i][0] < refParetoNadirPt[0] && pareto[i][1] < refParetoNadirPt[1]  )
    {
        io += (refParetoNadirPt[0]-pareto[i][0])*(refParetoNadirPt[1]-pareto[i][1]);
    }

    
    scaledHV = io /VolR;

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
        std::vector<std::vector<double>> pareto_tmp = pareto;
        if ( update_pareto_single(*itMobj, pareto , true /* true: sort if bi-obj*/) )
        {
            double hv = -compute_hv(pareto, refParetoIdealPt, refParetoNadirPt, nb_dom);
            if (hv-_obj.back() < -1E-10)
            {
                bbeTmp.push_back(*itBBE);
                _obj.push_back(hv); // We want a decrease in the objective function
                _nb_dominating_ref_obj.push_back(nb_dom);
            }
            if (hv-_obj.back() > 1E-10)
            {
                std::cout << "ERROR Result::compute_hypervolume_for_obj: Increase of hv" << std::endl;
                return false;
                // double hv_tmp = compute_hv(pareto, refParetoIdealPt, refParetoNadirPt, nb_dom);
            }
            
        }
    }

    _bbe=bbeTmp;
    _sol_fx = _obj.back();

    // Transfer into a single value hypervolume is complete. No need for mobj anymore.
    _mobj.clear();

    // Update number of pareto points for this
    _nb_pareto_points = pareto.size();
    
    // For multi-objective we consider only feasible solution
    _has_feas_sol = true;

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
    clear_solution();
    if ( _bbe.empty() || _mobj.empty())
    {
        size_t sol_bbe = _sol_bbe;
        _sol_bbe = sol_bbe;
        return false;
    }

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

    // We can compute hypervolume, hence we have a feasible solution
    _has_feas_sol = true;
    
    _sol_bbe = _bbe.back();
    _sol_fx = _obj.back();

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
    
    
    clear_solution();
    if ( _bbe.empty() )
    {
        _has_feas_sol = false;
        return false;
    }
    
    _nb_pareto_points = 0;
    
// More test to identify that no feasible point has been obtained
    if ( _obj.back()==INF ) // Case f = Inf
    {
        clear_solution();
        _sol_bbe = INF_SIZE_T;
        _has_feas_sol = false;
    }

    // We have a feasible solution
    // Update the characteristics of the solution (bbe, f, x)
    
    _sol_bbe = _bbe.back();
    _has_feas_sol = true;
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
    clear_solution();
    
    // We already have computed a solution (feasible)
    if (_has_feas_sol)
    {
        // Something is probably wrong.
        // compute_best_infeasible is called if compute_solution returns false!
        return false;
    }
    
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
    
    // No infeasible point has been obtained
    if (_infH.back() == INF) // Case h = Inf
    {
        return false;
    }

    // We have a valid infeasible solution
    _has_feas_sol = false;
    
    return true;
}





/*-----------------------------------------------------------------*/
/*  get the solution (feasible) for a given number of evaluations  */
/*-----------------------------------------------------------------*/
double RUNNERPOST::Result::get_sol ( const size_t & tBbe, bool forH) const
{
    double cur = INF;
    
    
    const auto & sol = (forH)? _infH:_obj;
    const auto & bbe = (forH)? _bbeForH:_bbe;
        
    if (sol.empty())
    {
        return cur;
    }
    int n = static_cast<int> ( bbe.size() );
    if (n > 0 && bbe[n-1] <= tBbe)
    {
        return sol[n-1];
    }
    for ( int k = 0 ; k < n ; ++k )
    {
        if ( bbe[k] > tBbe )
            return cur;
        cur = sol[k];
    }
    
    return cur;
}

/*-----------------------------------------------------------------*/
/*  get the solution (feasible) for a given time                   */
/*-----------------------------------------------------------------*/
double RUNNERPOST::Result::get_sol_by_time ( const double &tTime, bool forH) const
{
    
    
    const auto & sol = (forH)? _infH:_obj;
    const auto & time = (forH)? _timeForH:_time;
    
    double cur = INF;
    int n = static_cast<int> ( _time.size() );
    if (n > 0 && time[n-1] <= tTime)
    {
        return sol[n-1];
    }
    for ( int k = 0 ; k < n ; ++k )
    {
        if ( time[k] > tTime )
            return cur;
        cur = sol[k];
    }
    
    return cur;
}


/*--------------------------------------------------------*/
/*  get the time for a given number of evaluations (bbe)  */
/*  if bbe = INF, get the maximum (i.e., total) time.      */
/*--------------------------------------------------------*/
double RUNNERPOST::Result::get_time(const size_t & tBbe, bool forH) const
{
    const auto & bbe = (forH)? _bbeForH:_bbe;
    const auto & time = (forH)? _timeForH:_time;
    
    double cur = 0;
    if (time.size() > 0)
    {
        int n = static_cast<int>(bbe.size());
        
        for (int k = 0; k < n; ++k)
        {
            cur = time[k];
            if (tBbe < INF_SIZE_T && bbe[k] > tBbe)
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
