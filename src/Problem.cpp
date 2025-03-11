#include "Problem.hpp"
#include "Utils.hpp"

#include <cctype>

#include <fstream>

#include <filesystem>


// Default values. Can be set.
// TODO: this is available from Algo
int RUNNERPOST::Problem::_nbSimplexEvals = 150;
int RUNNERPOST::Problem::_overallMaxBBEvals = 20000;

// TODO: accept the syntax * () to use all directory names as pb ids. This should work only when output has no constraint, a single objective. Can be default format: SOL OBJ for all evals or EVAL SOL OBJ. We need SOL to determine the dimenstion from the outputs.

RUNNERPOST::Problem::Problem(std::string  s, std::string & error_msg)
{
    if (s.empty())
    {
        error_msg = "Error: Empty output description";
        return;
    }

    std::string sO = s;
    
    // Remove trailing comments
    size_t i = s.find("#");
    if ( i != std::string::npos)
    {
        s.erase(i,s.length());
    }
    
    // Remove commented lines
    i = s.find("//");
    if ( i != std::string::npos)
    {
        s.erase(i,s.length());
    }
    
    // Case where the line is empty
    if (s.empty())
    {
        error_msg = "Found an empty line in problem selection file";
        return;
    }

    // Get pb id as the first word on the line
    i = s.find_first_not_of(" ");
    if (i > 0)
    {
        s.erase(i) ;  // Remove initial white spaces
    }
    i = s.find(" ");
    std::string pType_str = s.substr(0,i);
    if ( !set_id(pType_str))
    {
        error_msg = "Error: Cannot read output profile type in \"" + sO + "\"" ;
    }
    
    s.erase(0,i);
    
    // Output title provided between parenthesis
    size_t i0 = s.find("(",0);
    size_t i1 = s.find(")",i0+1);

    if ( i0 == std::string::npos || i1 == std::string::npos )
    {
        error_msg = "Error(2) in output selection file. Output title must be provided between parenthesis in " + sO ;
        return ;
    }
    set_name(s.substr(i0+1,i1-i0-1));
    s.erase(0,i1+1);
    
        
    size_t pos = 0;
    // Parse for Key + Value(s) given between brackets [Key Val1]
    while ((pos = s.find("[")) != std::string::npos)
    {
        
        // Strip any character (should only be empty spaces) before a [
        pos = s.find_first_not_of(" ");
        s.erase(0, pos);
        
        auto p = RUNNERPOST::extract_from_bracket(s);
        
        if ( p.first.empty() || p.second.empty())
        {
            error_msg = "Error: Cannot read output bracket value in " + s;
            break;
        }
        
        if ( !setSingleAttribute(p) )
        {
            error_msg = "Error: Cannot read output bracket value for " + p.first;
        }
        
    }
    
    if (_pbInstance.empty())
    {
        _nbPbInstances = 1;
    }
    else
    {
        _nbPbInstances = _pbInstance.size();
    }
    
    // TODO: check inconsistencies. Example: tau provided but default output file name are used. At least give a warning
    
}

RUNNERPOST::Problem::Problem(const std::string&  result_file, RUNNERPOST::StatOutputTypeList & sotList, const std::string & pbInst, bool pbFromParse, std::string & error_msg)
{
    bool hasConst = (std::count(sotList.begin(),sotList.end(),StatOutputType::CST) > 0);
    bool hasSol = (std::count(sotList.begin(),sotList.end(),StatOutputType::SOL) > 0);
    size_t nbObj = std::count(sotList.begin(),sotList.end(),StatOutputType::OBJ);
    
    if (hasConst && hasSol)
    {
        error_msg = "Error. Cannot create pb. Constraints and solution are in " + result_file;
        return;
    }
    if (nbObj > 1)
    {
        error_msg = "Error. Cannot create pb. More than one objective in " + result_file;
        return;
    }
    
    
    std::ifstream in ( result_file, std::ios::in );
    if ( in.fail() )
    {
        in.close();
        error_msg = "Error. Cannot read pb result file " + result_file;
        return;
    }
    
    std::string line;
    getline (in , line);
    
    if (line.empty())
    {
        error_msg = "Error. Cannot read pb file. First line is empty. " + result_file;
        in.close();
        return;
    }
    
    // Let's try to read the pb dimension provided in the first line of the result file
    size_t dimPb = 0;
    if (!hasSol)
    {
        auto first_line_words = RUNNERPOST::extract_words(line, true /*to upper case*/);
        if (first_line_words[0] !="DIM" && first_line_words[1] != "=")
        {
            error_msg = "Error. Cannot read pb from result file " + result_file + ". First line does not start with DIM = xx, where xx is the number of variables.";
            in.close();
            return;
        }
        dimPb = std::stoi(first_line_words[2]);
        
    }
    else
    {
        
        //Count the number of words in the line
        size_t nbWords = RUNNERPOST::extract_words(line).size();
        
        // Deduce the dimension of pb
        dimPb = nbWords - 1; // Single objective, no constraints. The rest of the words can be the solution.
        if (std::count(sotList.begin(),sotList.end(),StatOutputType::CNT_EVAL) > 0)
        {
            dimPb = int(nbWords) - 2; // Single objective, no constraints and eval counter. The rest of the words can be the solution.
        }
    }
    if (dimPb < 1)
    {
        error_msg = "Error. Cannot read pb result file. Cannot deduce the number of variables in " + result_file;
        in.close();
        return;
    }
    
    _n = int(dimPb);
    _m = 1;
    
    _pbFromParse = pbFromParse;
    
    // Extract the id of the pb from the path in pb_dirs
    std::string pbId = RUNNERPOST::split(result_file, RUNNERPOST::DIR_SEP)[1];
    set_id(pbId);
    set_name("Pb"+pbId);
    
    set_pbInstance(pbInst);
    _nbPbInstances = _pbInstance.size();
    
    in.close();
}
    



bool RUNNERPOST::Problem::setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att)
{
    if (att.second.empty())
    {
        return false;
    }
    
    if (att.first == "N")
    {
        if (att.second.size() != 1)
        {
            return false;
        }
        return set_n(att.second[0]);
    }
    else if (att.first =="M")
    {
        if (att.second.size() != 1)
        {
            return false;
        }
        return set_m(att.second[0]);
    }
    else if (att.first == "PB_INSTANCE")
    {
        return set_pbInstance(att.second);
    }
    else
    {
        return false;
    }
    
}

bool RUNNERPOST::Problem::set_pbInstance(const std::vector<std::string> &vs)
{
    _pbInstance.clear();
    
    // Remove white spaces
    std::vector<std::string> tmpVS=vs;
    
    for (auto &s: tmpVS)
    {
        auto tmpS = RUNNERPOST::extract_words(s);
        _pbInstance.insert(_pbInstance.end(),tmpS.begin(), tmpS.end());
    }
    return true;
}

bool RUNNERPOST::Problem::set_pbInstance(const std::string &s)
{
    _pbInstance.clear();
    
    // Remove white spaces
    std::string tmpS=s;
    
    auto tmp2S = RUNNERPOST::extract_words(tmpS);
    
    if (tmp2S.size() > 0)
    {
        _pbInstance.insert(_pbInstance.end(),tmp2S.begin(), tmp2S.end());
        return true;
    }
    return false;
}

    
int RUNNERPOST::Problem::getMaxBBEvals() const
{
    int maxBBEvals = -1;

    if ( _overallMaxBBEvals > 0 )
    {
        if ( _nbSimplexEvals > 0 )
        {
            maxBBEvals = std::min( _overallMaxBBEvals , (_n+1) * _nbSimplexEvals );
        }
        else
        {
            maxBBEvals = _overallMaxBBEvals;
        }
    }

    return maxBBEvals;
}

///*----------------------------------------------*/
///*           set blackbox input types           */
///*----------------------------------------------*/
//bool Problem::set_bbit ( int i , NOMAD_BASE::BBInputType t )
//{
//
//    if ( i < 0 || i >= _n )
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ ,
//        "Problem::set_bbit, index not in [0;n-1]." );
//
//    if ( _bbit.size() < (size_t)_n )
//        _bbit.resize(_n);
//
//    _bbit[i] = t;
//
//// TODO
////    if ( t == NOMAD_BASE::CATEGORICAL )
////    {
////        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ ,
////                                "categorical variables are not handled" );
////        return false;
////    }
////    else if ( t == NOMAD_BASE::BBInputType::INTEGER )
//    if ( t == NOMAD_BASE::BBInputType::INTEGER )
//    {
//        if ( ! _has_integers )
//        {
//            add_keyword ( "integer_variables" );
//            _has_integers = true;
//        }
//    }
//
//    else if ( t == NOMAD_BASE::BBInputType::BINARY )
//    {
//        if ( ! _has_binaries )
//        {
//            _has_binaries = true;
//            add_keyword ( "binary_variables" );
//        }
//    }
//
//    return true;
//}



///*----------------------------------------------*/
///*         set bb executable                    */
///*     !!!! Batch eval if _batch_eval = true    */
///*----------------------------------------------*/
//void Problem::set_bb_exe ( const std::string & s )
//{
//    _bb_exe = s;
//    if ( ! _bb_exe.empty() )
//        add_keyword ( "batch" );
//}
//
///*----------------------------------------------*/
///*                  set bounds                  */
///*----------------------------------------------*/
//void Problem::set_bounds ( const NOMAD_BASE::Point & lb , const NOMAD_BASE::Point & ub )
//{
//    if ( has_bounds() )
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ ,
//                                "Problem::set_bounds can only be called once" );
//    _lb = lb;
//    _ub = ub;
//
//    if ( _lb.isDefined() && (int)_lb.size() != _n )
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ ,
//        "Problem::set_bounds, inconsistent dimensions" );
//
//    if ( _ub.isDefined() && (int)_ub.size() != _n )
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ ,
//        "Problem::set_bounds, inconsistent dimensions" );
//
//    if ( has_bounds() )
//        add_keyword ( "bounded" );
//}

///*-----------------------*/
///*  get the problem id   */
///*-----------------------*/
//std::string RUNNERPOST::Problem::get_id ( ) const
//{
//    std::string s;
//    size_t n = _id.size() , k;
//    for ( k = 0 ; k < n ; ++k ) {
//        if ( _id[k] == '_' )
//            s.push_back ( '\\' );
//        s.push_back ( _id[k] );
//    }
//    return s;
//}

/*----------------------------------------------*/
/*                    display                   */
/*----------------------------------------------*/
void RUNNERPOST::Problem::display ( void ) const
{
    std::cout << _id << ": "
    << "(" << _name << ") [N " << _n
    << "] [ M " << _m << "]";
    
    if (_pbInstance.size() > 0)
    {
        std::cout << " [ PB_INSTANCE ";
        for (const auto &s: _pbInstance)
        {
            std::cout << s << " " ;
        }
        std::cout << " ]";
    }
    
//    << "] [cstr=" << _has_constraints
//    << "] [f*=" << _fxe << "]";
}

///*----------------------------------------------*/
///*       display keywords (static, private)     */
///*----------------------------------------------*/
//void Problem::display_keywords ( const std::set<std::string> & keywords )
//{
//    size_t n = keywords.size();
//    
//    if ( n == 0 )
//    {
//        std::cout << "no keywords" << std::endl;
//        return;
//    }
//    
//    std::ostringstream msg;
//    msg << "keywords(" << n << ")";
//    std::set<std::string>::const_iterator it , end = keywords.end();
//    int i = 1;
//    for ( it = keywords.begin() ; it != end ; ++it , ++i ) {
//        std::cout << i << ": " << *it << std::endl;
//    }
//}


///*----------------------------------------------*/
///*         update the best known solution       */
///*----------------------------------------------*/
//bool Problem::update_xe ( const NOMAD_BASE::Point  & xe  ,
//                         const NOMAD_BASE::Double & fxe   ) {
//
//    if ( (int)xe.size() != _n || !xe.isComplete() || !fxe.isDefined() )
//        return false;
//
//    if ( !_fxe.isDefined() || fxe < _fxe )
//    {
//        _xe  = xe;
//        _fxe = fxe;
//
//        std::string   xe_file_name = _pb_dir + _xe_file_name;
//        std::ofstream out ( xe_file_name.c_str() );
//
//        out.setf      ( std::ios::fixed );
//        out.precision ( NOMAD_BASE::DISPLAY_PRECISION_STD );
//        out << xe;
//
//        out << std::endl << std::endl << fxe << std::endl;
//
//        out.close();
//
//        if ( out.fail() )
//            std::cerr << "Warning: cannot update best known solution "
//            << "for problem " << get_id() << std::endl;
//
//        return true;
//    }
//    return false;
//}


//void Problem::read_x0( const std::string & input_file_name , NOMAD_BASE::Point & x0 )
//{
//
//    size_t size_x0_before = x0.size();
//    NOMAD_BASE::ArrayOfDouble aod(size_x0_before);
//
//    if ( size_x0_before == 0 )
//    {
//        std::string err = "Cannot read alternate " + input_file_name + ". Size of point x0 to be read must be given.";
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ , err );
//    }
//
//    std::ifstream fin ( input_file_name.c_str() );
//    if ( fin.fail() )
//    {
//        std::string err = "Cannot read alternate " + input_file_name ;
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ , err );
//    }
//    try
//    {
//        fin >> aod ;
//    }
//    catch ( NOMAD_BASE::Exception  & e)
//    {
//        std::string err = "Cannot read alternate " + input_file_name + " \n" + std::string(e.what());
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ , err );
//    }
//
//    x0 = aod;
//
//    if ( !x0.isComplete() )
//    {
//        std::string err = "Cannot read alternate " + input_file_name + "(inconplete point)";
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ ,err );
//    }
//    fin.close();
//}
