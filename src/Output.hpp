#ifndef __OUTPUT__
#define __OUTPUT__

#include "runner_defines.hpp"
#include <set>

#include "runnerpost_nsbegin.hpp"

class Output {


public:
    
    enum Profile_Type
    {
        DATA_PROFILE,
        PERFORMANCE_PROFILE,
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
    double                             _tau =0.001;
    std::string                        _fileName ="";
    
    
public:
    
    // constructor #1
    Output (const Profile_Type & ptype, const double tau = 0.001,   const std::string fileName = "", const Y_Select ySel = Y_Select::OBJ, const X_Select xSel = X_Select::NP1EVAL  )
    : _pType              ( ptype    ) ,
      _fileName           ( fileName ) ,
      _ySel               ( ySel ) ,
      _xSel               ( xSel ) ,
      _tau                ( tau     )
    {};
    
    // Constructor #2
    Output (std::string & single_output_description, std::string & error_msg);

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
    bool setSingleAttribute(const std::pair<std::string,std::string> & att);
    bool setProfileType(const std::string & s) { _pType = stringToProfileType(s); if (_pType == Profile_Type::UNDEFINED) return false; return true; }
    void setProfileType(const Output::Profile_Type & pt) { _pType = pt; }
    bool setTau(const double & tau) { if (tau<=0) return false; _tau = tau ; return true; }
    bool setTau(const std::string & s) { return setTau(std::stod(s));}
    bool setYSelect(const std::string & s);
    void setYSelect(const Output::Y_Select & ys) { _ySel = ys;}
    bool setXSelect(const std::string & s);
    void setXSelect(const Output::X_Select & xs) { _xSel = xs;}
    bool setFileName(const std::string & s) { if (s.empty()) return false; _fileName = s; return true; }
    
    void display ( void ) const;
    
    // GET methods:
    const Profile_Type &        get_profile_type( ) const { return _pType;}
    const Y_Select &            get_y_select( ) const { return _ySel;}
    const X_Select &            get_x_select( ) const { return _xSel;}
    const double &              get_tau         ( void ) const { return _tau;}
    const std::string &         get_file_name() const { return _fileName;}
    
    
    
    static Profile_Type stringToProfileType(const std::string & s) ;
    static std::string profileTypeToString(const Profile_Type & pType) ;
    
//    static Y_Select stringToYSelect(const std::string & s) ;
//    static std::string ySelectToString(const Y_Select & ySel) ;
//    
//    static X_Select stringToXSelect(const std::string & s) ;
//    static std::string xSelectToString(const X_Select & xSel) ;
    
    
    
    static std::pair<std::string,std::string> extract_from_bracket(std::string &s);
};

#include "runnerpost_nsend.hpp"

#endif


