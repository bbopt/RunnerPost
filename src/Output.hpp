#ifndef __OUTPUT__
#define __OUTPUT__

#include "runner_defines.hpp"
#include <set>

#include "runnerpost_nsbegin.hpp"


// This class is for the output options of the runner.
// It is used to specify the output format of the runner.
class Output {


public:
    
    enum Profile_Type
    {
        DATA_PROFILE,
        PERFORMANCE_PROFILE,
        HISTORY,
        ACCURACY_PROFILE,
        UNDEFINED
    };
    enum Y_Select
    {
        OBJ,
        INFEAS,
    };
    enum X_Select
    {
        EVAL,
        NP1EVAL,
        TIME
    };
    

private:

    Profile_Type                       _pType = Profile_Type::UNDEFINED;
    Y_Select                           _ySel = Y_Select::OBJ ;
    X_Select                           _xSel = X_Select::NP1EVAL;
    int                                _xMax = P_INF_INT;
    double                             _tau =0.001;
    std::string                        _plainFileName ="dp3.txt";
    std::string                        _latexFileName ="dp3.tex";
    std::string                        _title="";

    
public:
    
    // constructor #1
    Output (const Profile_Type & ptype, const std::string title = "", const double tau = 0.001,   const std::string plainFileName = "dp3.txt", const std::string latexFileName = "dp3.tex", const Y_Select ySel = Y_Select::OBJ, const X_Select xSel = X_Select::NP1EVAL, const int xMax = P_INF_INT)
    : _pType              ( ptype    ) ,
      _plainFileName      ( plainFileName ) ,
      _latexFileName      ( latexFileName ) ,
      _ySel               ( ySel ) ,
      _xSel               ( xSel ) ,
      _xMax               ( xMax ),
      _tau                ( tau     ),
      _title              ( title )
    {
        // TODO: check inconsistencies. Example: tau provided but default output file name are used. At least give a warning
        
    };
    
    // Constructor #2
    Output (std::string single_output_description, std::string & error_msg);

    // Maybe more constructors
//    // constructor #2:
//    Output (const Profile_Type & ptype )
//    : _pType              ( ptype    ) ,
//      _fileName           ( fileName ) ,
//      _ySel               ( Y_Select::OBJ ) ,
//      _xSel               ( X_Select::NP1EVAL ) ,
//      _tau                ( tau     )
//    {};
//
//    // constructor #3:
//    Output (const Profile_Type & ptype, const double & tau, const Y_Select & ySel )
//    : _pType              ( ptype    ) ,
//      _fileName           ( fileName ) ,
//      _ySel               ( ySel ) ,
//      _xSel               ( X_Select::NP1EVAL ) ,
//      _tau                ( tau     )
//    {};
//
//    // constructor #4:
//    Output (const Profile_Type & ptype, const double & tau, const Y_Select & ySel, const X_Select & xSel)
//    : _pType              ( ptype    ) ,
//      _ySel               ( ySel ) ,
//      _xSel               ( xSel ) ,
//      _tau                ( tau     )
//    {};
    
    // destructor:
    virtual ~Output ( void ) {}

    // SET methods:
    bool setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att);
    bool setProfileType(const std::string & s) { _pType = stringToProfileType(s); if (_pType == Profile_Type::UNDEFINED) return false; return true; }
    void setProfileType(const Output::Profile_Type & pt) { _pType = pt; }
    bool setTau(const double & tau) { if (tau<=0) return false; _tau = tau ; return true; }
    bool setTau(const std::string & s) { return setTau(std::stod(s));}
    bool setYSelect(const std::string & s);
    void setYSelect(const Output::Y_Select & ys) { _ySel = ys;}
    bool setXSelect(const std::string & s);
    void setXSelect(const Output::X_Select & xs) { _xSel = xs;}
    bool setXMax(const int & xMax) { if (xMax<=0) return false; _xMax = xMax ; return true; }
    bool setXMax(const std::string & s) {return setXMax(std::stoi(s));}
    
    bool setPlainFileName(const std::string & s) { if (s.empty()) return false; _plainFileName = s; return true; }
    bool setLatexFileName(const std::string & s) { if (s.empty()) return false; _latexFileName = s; return true; }
    
    bool setTitle(const std::string & s) { if (s.empty()) return false; _title = s; return true; }
    
    void display ( void ) const;
    
    // GET methods:
    const Profile_Type &        get_profile_type( ) const { return _pType;}
    const Y_Select &            get_y_select( ) const { return _ySel;}
    const X_Select &            get_x_select( ) const { return _xSel;}
    const int      &            get_x_max( ) const { return _xMax;}
    const double &              get_tau         ( void ) const { return _tau;}
    const std::string &         get_plain_file_name() const { return _plainFileName;}
    const std::string &         get_latex_file_name() const { return _latexFileName;}
    const std::string &         get_title() const { return _title;}
    
    
    static Profile_Type stringToProfileType(const std::string & s) ;
    static std::string profileTypeToString(const Profile_Type & pType) ;
    
//    static Y_Select stringToYSelect(const std::string & s) ;
//    static std::string ySelectToString(const Y_Select & ySel) ;
//    
//    static X_Select stringToXSelect(const std::string & s) ;
//    static std::string xSelectToString(const X_Select & xSel) ;
    
    
};

#include "runnerpost_nsend.hpp"

#endif


