#include "Problem.hpp"
#include "Utils.hpp"

#include <cctype>

// static members initialization:
// std::set<std::string> Problem::_all_keywords;

// Default values. Can be set.
// TODO: this is available from Algo
int RUNNERPOST::Problem::_nbSimplexEvals = 100;
int RUNNERPOST::Problem::_overallMaxBBEvals = 20000;



/*----------------------------------------------*/
/*                   constructor #1             */
/*----------------------------------------------*/
//Problem::Problem ( const std::string & pb_dir       )
//: _index           ( -1                          ) ,
//  _id              ( ""                          ) ,
//  _pb_dir          ( pb_dir ) ,
//   _xe_file_name    ( ""                          ) ,
//  _n               ( -1                          ) ,
//  _m               ( -1                          )
//  _bb_exe          (""                           ) ,
//  _batch_eval      ( false                       ) ,
//  _has_constraints ( false                       ) ,
//  _has_integers    ( false                       ) ,
//  _has_binaries    ( false                       ) ,
//  _x0_feas_info    ( false                       ) ,
//  _x0_is_feas      ( false                       )
//{};


/*----------------------------------------------*/
/*                   constructor #2             */
/*----------------------------------------------*/
//Problem::Problem ( const std::string & id           ,
//                  const std::string & pb_dir       ,
//                  const std::string & xe_file_name ,
//                  int                 n            ,
//                  int                 m              )
//: _index           ( -1                          ) ,
//_id              ( id                            ) ,
//_pb_dir          ( PROBLEMS_DIR + pb_dir + "/" ) ,
//_xe_file_name    ( xe_file_name                ) ,
//_n               ( n                           ) ,
//_m               ( m                           )
//_bb_exe          (""                           ) ,
//_batch_eval      ( false                       ) ,
//_bbit            ( n                           ) ,
//_bbot            ( m                           ) ,
//_has_constraints ( false                       ) ,
//_has_trend_matrix( false                       ) ,
//_has_integers    ( false                       ) ,
//_has_binaries    ( false                       ) ,
//_lb              ( n                           ) ,
//_ub              ( n                           ) ,
//_x0              ( n                           ) ,
//_xe              ( n                           ) ,
//_fxe             (                             ) ,
//_keywords        (                             ) ,
//_trend_matrix    ( m                           ) ,
//_x0_feas_info    ( false                       ) ,
//_x0_is_feas      ( false                       )
//{
    
//    std::ifstream in ( (_pb_dir + xe_file_name).c_str() );
//
//    if ( !in.fail() )
//    {
//        try
//        {
//            in >> _xe >> _fxe;
//        }
//        catch ( ... ) { }
//
//        if ( !_xe.isComplete() )
//            _xe.reset();
//
//        if ( !_fxe.isDefined() )
//        {
//            _xe.reset();
//            _fxe.clear();
//        }
//    }
//    in.close();
    
//}

///*----------------------------------------------*/
///*     add a keyword describing the problem     */
///*----------------------------------------------*/
//void Problem::add_keyword ( std::string s )
//{
//    std::toupper                  ( s );
//    _keywords.insert              ( s );
//    Problem::_all_keywords.insert ( s );
//}

///*----------------------------------------------*/
///*        search for a particular keyword       */
///*----------------------------------------------*/
//bool Problem::has_keyword ( const std::string & kw )
//{
//    std::set<std::string>::const_iterator
//    it = _keywords.find ( kw );
//    return ( it != _keywords.end() );
//}

RUNNERPOST::Problem::Problem(std::string & s, std::string & error_msg)
{
    std::string sO = s;
    
    // Remove trailing comments
    size_t i = s.find("#");
    if ( i != std::string::npos)
    {
        s.erase(i,s.length());
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
        
        auto p = RUNNERPOST::extract_from_bracket(s,"=" /*key/val separator is = */);
        
        if ( p.first.empty() || p.second.empty())
        {
            error_msg = "Error: Cannot read output bracket value in " + s;
            break;
        }
        
        if ( !setSingleAttribute(p) )
        {
            error_msg = "Error: Cannot read output bracket value in " + s;
        }
        
    }
    
    // TODO: check inconsistencies. Example: tau provided but default output file name are used. At least give a warning
    
}

bool RUNNERPOST::Problem::setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att)
{
    if (att.second.size() != 1)
    {
        return false;
    }
    
    if (att.first.find("N") != std::string::npos)
    {
        return set_n(att.second[0]);
    }
    else if (att.first.find("M") != std::string::npos)
    {
        return set_m(att.second[0]);
    }
    else
    {
        return false;
    }
    
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
///*          set blackbox output types           */
///*----------------------------------------------*/
//bool Problem::set_bbot ( int i , NOMAD_BASE::BBOutputType t )
//{
//
//    if ( i < 0 || i >= _m )
//        throw NOMAD_BASE::Exception ( "RUNNER: Problem.cpp" , __LINE__ ,
//        "Problem::set_bbot, index not in [0;m-1]." );
//
//    if ( _bbot.size() < (size_t)_m )
//        _bbot.resize(_m);
//
//    _bbot[i] = t;
//
//    if ( t == NOMAD_BASE::BBOutputType::PB || t == NOMAD_BASE::BBOutputType::EB )
//    {
//        if ( ! _has_constraints )
//        {
//            _has_constraints = true;
//            add_keyword ( "constrained" );
//        }
//    }
//
//    // Append from NOMAD_BASE::BBOutputType to string
//    std::ostringstream s;
//    s << " " << _bbot[i];
//    _bbotS.append( s.str() );
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

/*-----------------------*/
/*  get the problem id   */
/*-----------------------*/
std::string RUNNERPOST::Problem::get_id ( ) const
{
    std::string s;
    size_t n = _id.size() , k;
    for ( k = 0 ; k < n ; ++k ) {
        if ( _id[k] == '_' )
            s.push_back ( '\\' );
        s.push_back ( _id[k] );
    }
    return s;
}

/*----------------------------------------------*/
/*                    display                   */
/*----------------------------------------------*/
void RUNNERPOST::Problem::display ( void ) const
{
    std::cout << _id << ": "
    << "(" << _name << ") [n=" << _n
    << "] [m=" << _m << "]";;
//    << "] [bnds=" << has_bounds()
//    << "] [cstr=" << _has_constraints
//    << "] [trend=" << _has_trend_matrix
//    << "] [int=" << _has_integers
//    << "] [bin=" << _has_binaries << "] ["
//    << ( is_batch() ? "batch" : "lib" )
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
