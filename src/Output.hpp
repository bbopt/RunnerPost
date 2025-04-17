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
        CONVERGENCE_PROFILE,
        ACCURACY_PROFILE,
        UNDEFINED_PROFILE
    };
    enum Y_Select
    {
        OBJ,
        INFEAS,
        UNDEFINED_Y
    };
    enum X_Select
    {
        EVAL,
        NP1EVAL,
        TIME,
        UNDEFINED_X
    };
    
    enum Plot_Type
    {
        OnlyFFeasible,
        OnlyF,
        OnlyHInfeasible,
        ComboHInfAndFFeas,
        UNDEFINED_PLOT_TYPE
    };
    
    enum Fx_First_Feas_Method
    {
        max,
        min,
        avg,
        UNDEFINED_FFFM
    };
    
    
private:
    
    Profile_Type                       _pType = Profile_Type::UNDEFINED_PROFILE;
    Y_Select                           _ySel = Y_Select::OBJ;
    X_Select                           _xSel = X_Select::EVAL;
    int                                _xMax;
    double                             _tau = -1.0; // Default value is not allowed. Must be set explicitly depending on profile type. This is tested.
    std::string                        _plainFileName;
    std::string                        _latexFileName;
    std::string                        _title="";
    Fx_First_Feas_Method               _FFFeasM = Fx_First_Feas_Method::max;
    
    std::vector<std::string>           _profile_type_options;
    
    std::vector<std::string>           _plotSelection = {"ALLALGO","ALLPB", "ALLINST"}; // Default value, only for convergence profile
    Plot_Type                          _plotType = Plot_Type::UNDEFINED_PLOT_TYPE;
    
    
public:
    
    
    // Constructor #1
    Output (std::string single_output_description, std::string & error_msg);
    
    // destructor:
    virtual ~Output ( void ) {}
    
    // SET methods:
    bool setSingleAttribute(const std::pair<std::string,std::vector<std::string>> & att);
    bool setProfileType(const std::string & s) { _pType = stringToProfileType(s); if (_pType == Profile_Type::UNDEFINED_PROFILE) return false; return true; }
    void setProfileType(const Output::Profile_Type & pt) { _pType = pt; }
    bool setTau(const double & tau) { if (tau<=0) return false; _tau = tau ; return true; }
    bool setTau(const std::string & s) { return setTau(std::stod(s));}
    bool setYSelect(const std::string & s);
    void setYSelect(const Output::Y_Select & ys) { _ySel = ys;}
    bool setXSelect(const std::string & s);
    void setXSelect(const Output::X_Select & xs) { _xSel = xs;}
    bool setXMax(const int & xMax) { if (xMax<=0) return false; _xMax = xMax ; return true; }
    bool setFFFeasMeth(const std::string & s) { _FFFeasM = stringToFFFeasMeth(s); if (_FFFeasM == Fx_First_Feas_Method::UNDEFINED_FFFM) return false; return true; }
    
    bool setPlotSelection(const std::vector<std::string> & s);
    
    bool setPlotType(const std::string & s){ _plotType = stringToPlotType(s); if (_plotType == Plot_Type::UNDEFINED_PLOT_TYPE) return false; return true; }
    void setPlotType(const Plot_Type & pt) { _plotType = pt;}
    
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
    const Fx_First_Feas_Method & get_FFFeasMeth() const { return _FFFeasM;}

    const std::vector<std::string> & get_plot_selection() const { return _plotSelection;}
    const Plot_Type &           get_plot_type() const { return _plotType;}
    
    bool plotIsSelected(const std::string & algoId, const std::string & pbId, size_t i_pbInstance) const ;
    
    const std::vector<std::string> & get_profile_type_options() const { return _profile_type_options;}
    
    static Profile_Type stringToProfileType(const std::string & s) ;
    static std::string profileTypeToString(const Profile_Type & pType) ;
    
    static Fx_First_Feas_Method stringToFFFeasMeth(const std::string & s) ;
    static std::string fFeasMethToString(const Fx_First_Feas_Method & fffm) ;
    
    static Plot_Type stringToPlotType(const std::string & sConst);
    static std::string plotTypeToString(const Plot_Type & pType);
    
};

#include "runnerpost_nsend.hpp"

#endif


