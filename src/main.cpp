#include "Runner.hpp"

#include <map>
#include <fstream>

std::map<double,std::string> map_tau_to_dp_file_name;
std::map<double,std::string> map_tau_to_pp_file_name;
std::map<double,std::string> map_tau_to_time_profile_file_name;
std::map<double,std::string> map_tau_to_tdp_file_name;

// Some constants for the plots
// const std::vector<std::string> symbols = {"diamond","square","o","^","v","+"};
// const std::vector<std::string> colors = {"red","green","blue","cyan","magenta","black"};
// Maximum number of algorithms that can be plotted
// const size_t maxAlgos = colors.size();

/// \brief Toupper utility
void toUpperCase(std::string& str)
{
    for_each(str.begin(), str.end(), [](char& in){ in = std::toupper(in); });
}


std::string get_dp_file_name( double tau )
{
    std::map<double,std::string>::iterator it;
    it = map_tau_to_dp_file_name.find(tau);
    if ( it != map_tau_to_dp_file_name.end() )
        return it->second ;
    else
        return std::string();
}


std::string get_time_profile_file_name(double tau)
{
    std::map<double,std::string>::iterator it;
    it = map_tau_to_time_profile_file_name.find(tau);
    if (it != map_tau_to_time_profile_file_name.end())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}


std::string get_tdp_file_name( double tau )
{
    std::map<double,std::string>::iterator it;
    it = map_tau_to_tdp_file_name.find(tau);
    if ( it != map_tau_to_tdp_file_name.end() )
        return it->second ;
    else
        return std::string();
}


std::string get_pp_file_name( double tau )
{
    std::map<double,std::string>::iterator it;
    it = map_tau_to_pp_file_name.find(tau);
    if ( it != map_tau_to_pp_file_name.end() )
        return it->second ;
    else
        return std::string();
}


void trim(std::string &word)
{
    // Remove starting and ending spaces
    size_t word_size = word.size();
    while (word_size > 0 && (' ' == word[0] || '\t' == word[0]))
    {
        word = word.substr(1, word_size-1);
        word_size = word.size();
    }

    while (word_size > 0 && (' ' == word[word_size-1] || '\t' == word[word_size-1]))
    {
        word = word.substr(0, word_size-1);
        word_size = word.size();
    }

    // remove "runner." before the command
    if (word_size > 0 && 0 == word.find("runner."))
    {
        word = word.substr(7, word_size-7);
        word_size = word.size();
    }

    // Remove starting and ending " (once)
    if (word_size >= 2 && '"' == word[0] && '"' == word[word_size-1])
    {
        word = word.substr(1,word_size-2);
        word_size = word.size();
    }
}

void remove_comments(std::string &line)
{
    // Ignore comments, starting with // 
    size_t i = line.find("//");
    if (i != std::string::npos)
    {
        line = line.substr(0, i);
    }
}

void find_select_command(const std::string line, std::string &select_command, std::vector<std::string> &args)
{
    // First word is the select_command
    size_t args_index = line.find("(");
    select_command = line.substr(0, args_index);
    trim(select_command);

    size_t arg_start_index = args_index;
    size_t arg_end_index = line.find(",", arg_start_index);
    while (std::string::npos != arg_end_index)
    {
        std::string arg = line.substr(arg_start_index+1, arg_end_index-arg_start_index-1);
        trim(arg);
        args.push_back(arg);
        arg_start_index = arg_end_index;
        arg_end_index = line.find(",", arg_start_index+1);
    }
    // last arg
    arg_end_index = line.find(")", arg_start_index);
    std::string arg = line.substr(arg_start_index+1, arg_end_index-arg_start_index-1);
    trim(arg);
    if ( arg.length() >0 )
        args.push_back(arg);

}

//bool read_maintenance_selection_file(Runner &runner)
//{
//    std::ifstream in("maintenance_selection");
//    if ( in.fail() )
//    {
//        in.close();
//        std::cerr << "Error maintenance selection file" << std::endl;
//        return false;
//    }
//
//    while(!in.eof())
//    {
//        std::string line;
//        getline (in , line);
//
//        remove_comments(line);
//        if (line.empty())
//        {
//            continue;
//        }
//
//        std::string maintenance_command;
//        std::vector<std::string> args;
//        find_select_command(line, maintenance_command, args);
//
//        if ("clean_all_tests" == maintenance_command)
//        {
//            runner.clean_all_tests();
//        }
//        else if ("clean_selected_tests" == maintenance_command )
//        {
//            runner.clean_selected_tests();
//        }
//    }
//
//    in.close();
//    return true;
//}


//bool output_profile_pgfplots(Runner& runner,
//                             const std::string& profileName,
//                             const std::vector<std::string>& args)
//{
//    // input plain profile file name is mandatory (1st argument)
//    std::string profile_plain_file_name = args[0]; // Provided profile_plain_file with path
//    if (profile_plain_file_name.empty())
//    {
//        std::cerr << "\n Error in output_selection: output_profile_pgfplots no value is provided for " << profileName << " file name. Generate " << profileName << " plain text file first." << std::endl;
//        return false;
//    }
//
//    // Get root name of file
//    std::string profile_plain_noExt_noPath = NOMAD_BASE::rootname(profile_plain_file_name);
//
//    // Remove path from profile_plain_noExt
//    // std::cout << "profile_plain_noExt_noPath = " << profile_plain_noExt_noPath << std::endl;
//
//    if ("timeProfile" == profileName)
//    {
//        // Use first file as "plain file" for verifications
//        profile_plain_file_name = NOMAD_BASE::dirname(args[0]) + profile_plain_noExt_noPath + "0" + NOMAD_BASE::extension(args[0]);
//    }
//
//    std::ifstream infile(profile_plain_file_name);
//    if (! infile.good() )
//    {
//        std::cerr << "\n Error in output_selection: file " << profile_plain_file_name << " does not exist for profile " << profileName << ". Make sure to generate it first by selecting the command in output_selection." << std::endl;
//        infile.close();
//        return false;
//    }
//
//    infile.close();
//
//    // To be generalized.
//    // Multiple inputs.
//    // Single output.
//
//    // Graph title
//    std::string profileTitle = "not provided";
//    if (args.size() > 1)
//    {
//        profileTitle = args[1];
//        if ("dataProfile" == profileName || "timeDataProfile" == profileName)
//        {
//            profileTitle = "TAU " + profileTitle ;
//        }
//    }
//
//    // Command pdf to use
//    std::string pdflatex_cmd = "pdflatex"; // default
//    if (args.size() > 2)
//    {
//        pdflatex_cmd = args[2];
//    }
//
//    std::string verboseOn = "NO";
//    if (args.size() > 3)
//    {
//        verboseOn = args[3];
//    }
//    toUpperCase(verboseOn);
//
//    // Output tex file and output path
//    std::string pgfplots_output_path = "outputs/pgfplots/"; // default path for script and outputs
//    std::string tex_file_name = pgfplots_output_path + profile_plain_noExt_noPath +".tex"; // default (w path)
//    if (args.size() > 4)
//    {
//        // If provided the script file name contains the path
//        std::string tex_file_name = args[4];
//
//        // Get the output path from the tex file name
//        std::size_t path_index = tex_file_name.find_last_of("/");
//        if (path_index > tex_file_name.size())
//        {
//            pgfplots_output_path = "./";
//            tex_file_name = pgfplots_output_path + tex_file_name ; // add a path for
//        }
//        else
//        {
//            pgfplots_output_path = tex_file_name.substr( 0, path_index+1 ) ; // Get the path and the trailing /
//        }
//    }
//
//    std::string profile_pdf_file_name;
//    bool flag_move_pdf = false;
//    if ( args.size() > 5 )
//    {
//        profile_pdf_file_name = args[5]; // Provided profile_fig_file_name with path
//        flag_move_pdf = true;
//    }
//
//
//    // Output tex file from previous strings
//    std::ofstream out ( tex_file_name.c_str() , std::ofstream::out | std::ofstream::trunc );
//    out << "\\documentclass{standalone}" <<std::endl;
//    out << "\\usepackage{pgfplots} " <<std::endl;
//    out << "\\pgfplotsset{width=10cm,compat=1.16} " <<std::endl;
//    out << "\\begin{document}" <<std::endl;
//    out << "\\begin{tikzpicture} "<<std::endl;
//    out << "\\begin{axis}[ " << std::endl;
//
//    out << "       title = " << profileTitle << "," << std::endl;
//    out << "       xmin=-10, ymin = -5, ymax= 105," << std::endl;
//
//    if ("dataProfile" == profileName)
//    {
//        if (runner.get_use_evals_for_dataprofiles())
//        {
//            out << "       xlabel = Number of evaluations, " <<std::endl;
//        }
//        else
//        {
//            out << "       xlabel = Number of ($n+1$) evaluations," <<std::endl;
//        }
//        out << "       ylabel = Percentage of problems solved," <<std::endl;
//    }
//    else if ("timeProfile" == profileName)
//    {
//        out << "       xlabel = Blackbox evaluation, " <<std::endl;
//        out << "       ylabel = Real time, " <<std::endl;
//        // VRM how to use a ymax that lets results be clear?
//        //out << "       ymax = " << 80 << ", " <<std::endl;
//        // VRM time less than 5 seconds is not interesting
//        //out << "       ymin = " << 5 << ", " <<std::endl;
//    }
//    else if ("timeDataProfile" == profileName)
//    {
//        if (runner.get_use_evals_for_dataprofiles())
//        {
//            out << "       xlabel = Time, " <<std::endl;
//        }
//        else
//        {
//            out << "       xlabel = Time for ($n+1$) evaluations," <<std::endl;
//        }
//        out << "       ylabel = Percentage of problems solved," <<std::endl;
//    }
//
//    out << " legend style={ " << std::endl;
//    out << "    font=\\tiny, " <<std::endl;
//    out << "    cells={anchor=southeast}, " << std::endl;
//    out << "    at={(1,-0.2)}, " <<std::endl;
//    out << "   legend cell align=left, } ]" <<std::endl;
//    const std::vector<std::string> & legends = runner.get_selected_algo_legends();
//    int y_index = 1;
//    int color_index = 0;
//    int symbol_index = 0;
//    // int repeat_mark = 10;
//    size_t nbPlottedAlgos = 0;
//    if (legends.size() > maxAlgos)
//    {
//        std::cout << "Warning: Only " << maxAlgos << " algos will be plotted." << std::endl;
//    }
//    std::string lineStyle = "solid";
//    if ("timeProfile" == profileName)
//    {
//        lineStyle = "only marks";
//    }
//    if ("dataProfile" == profileName || "timeDataProfile" == profileName)
//    {
//        for (auto leg : legends )
//        {
//            out << "  \\addplot [" << lineStyle << ", mark="<< symbols[symbol_index++] << ", mark repeat = 10, color=" << colors[color_index++] << "] table [x index = 0, y index = " << y_index++ << " , header = false ] {" << profile_plain_file_name << "}; " << std::endl ;
//            out << "\\addlegendentry{" << leg << "};" <<std::endl;
//            nbPlottedAlgos++;
//            if (nbPlottedAlgos >= maxAlgos)
//            {
//                break;
//            }
//        }
//    }
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
//    out << " \\end{axis} " << std::endl;
//    out << " \\end{tikzpicture}" << std::endl;
//    out << " \\end{document}" <<std::endl;
//
//    out.close();
//
//
//    std::string cmd = pdflatex_cmd + " -halt-on-error -output-directory=" + pgfplots_output_path  + " " + tex_file_name ;
//
//    if ( verboseOn.compare("FALSE") == 0 || verboseOn.compare("OFF") == 0 || verboseOn.compare("NO") == 0 )
//        cmd += " | grep '^!.*' --color=always";
//
//
//    std::string default_profile_pdf_file_name =  pgfplots_output_path + profile_plain_noExt_noPath  + ".pdf";
//    std::cout << "running the pdflatex command to create " << default_profile_pdf_file_name  << " ..... ";
//    int return_value = system(cmd.c_str());
//    if (0 != return_value)
//    {
//        std::cerr << "\n There might be an error in output_selection running command: " << cmd << std::endl;
//        std::cerr << "\n Let us continue anyway." << cmd << std::endl;
//
//    }
////
//
//    std::ifstream infile_pdf(default_profile_pdf_file_name);
//    if (! infile_pdf.good() )
//    {
//        std::cerr << "\n Error in output_selection using pgfplot: the pdf file was not created. Run with verbose." << std::endl;
//        infile_pdf.close();
//        return false;
//    }
//    infile_pdf.close();
//
//    std::cout << " done " << std::endl;
//
//    if ( flag_move_pdf )
//    {
//        cmd += "mv " + default_profile_pdf_file_name + " " + profile_pdf_file_name ;
//        return_value = system(cmd.c_str());
//
//        std::cout << "moving the pdf file from pgfplots to " << profile_pdf_file_name << " ......" ;
//
//        if ( return_value !=0 )
//        {
//            std::cerr << "\n Error in output_selection: cannot move the pdf file at" << profile_pdf_file_name << std::endl;
//            return false;
//        }
//        std::cout << " done " << std::endl;
//    }
//
//    return true;
//}


//bool output_data_profile_matlab ( Runner & runner , const std::vector<std::string> & args )
//{
//
//    std::cout << "running matlab to create fig and pdf files for dataprofiles .... " << std::endl ;
//    // input plain dp file name is mandatory (1st argument)
//    std::string dp_plain_file_name = args[0]; // Provided dp_plain_file with path
//    if ( dp_plain_file_name.empty() )
//    {
//        std::cerr << "\n Error in output_selection: output_data_profile_matlab no value is provided for dp file name. Generate dp plain text file first." << std::endl;
//        return false;
//    }
//    else
//    {
//        std::ifstream infile(dp_plain_file_name);
//        if (! infile.good() )
//        {
//            std::cerr << "\n Error in output_selection: dp file name does not exist. Make sure to generate dp plain text file first by selecting the command in output_selection." << std::endl;
//            infile.close();
//            return false;
//        }
//
//        infile.close();
//    }
//    // Remove path from dp_plain_file_name
//    std::size_t path_index = dp_plain_file_name.find_last_of("/");
//    if ( path_index > dp_plain_file_name.size() ) // Case without path
//        path_index = -1;  // for substring below
//    std::string dp_plain_noExt_noPath = dp_plain_file_name.substr( path_index+1 , std::string::npos);
//
//    // Remove extension
//    std::size_t extension_index = dp_plain_noExt_noPath.find_last_of(".");
//    extension_index = std::min ( static_cast<int>(extension_index) , static_cast<int>(dp_plain_noExt_noPath.size()) );
//    dp_plain_noExt_noPath = dp_plain_noExt_noPath.substr(0, extension_index ) ;
//
//
//    // Graph title
//    std:: string tauTitle="not provided";
//    if ( args.size() > 1 )
//        tauTitle = args[1];
//
//    std::string matlab_cmd = "matlab"; // default
//    if ( args.size() > 2 )
//        matlab_cmd = args[2];
//
//    // Matlab function name and output path
//    std::string matlab_output_path = "outputs/matlab/"; // default path for script and outputs
//    std::string matlab_function_name = "plotDataProfileTau"; // default (no path)
//    if ( args.size() > 3 )
//    {
//        // If provided the script file name contains the path
//        std::string matlab_script_file_name = args[3];
//
//        // Get the output path from the script file name
//        std::size_t path_index = matlab_script_file_name.find_last_of("/");
//        if ( path_index > matlab_script_file_name.size() )
//        {
//            path_index = 1;  // for substring below
//            matlab_output_path = "./";
//            matlab_script_file_name = matlab_output_path + matlab_script_file_name ; // add a path for
//        }
//        else
//            matlab_output_path = matlab_output_path.substr( 0, path_index+1 ) ; // Get the path and the trailing /
//
//        // Get the matlab function name (without path and without extension)
//        matlab_function_name = matlab_script_file_name.substr( path_index+1 , std::string::npos ) ;
//        std::size_t extension_index = matlab_function_name.find_last_of(".");
//        if ( extension_index > matlab_script_file_name.size() || matlab_script_file_name.size() == 0 )
//        {
//            std::cerr << "\n Error in output_selection: the matlab script file name must be provided with extension" << std::endl;
//            return false;
//        }
//        matlab_function_name = matlab_function_name.substr( 0 , extension_index ) ;
//
//    }
//
//    std::string dp_fig_file_name;
//    if ( args.size() > 4 )
//        dp_fig_file_name = args[4]; // Provided dp_fig_file_name with path
//    else
//        dp_fig_file_name =  matlab_output_path + dp_plain_noExt_noPath  + ".fig";
//
//    std::string dp_pdf_file_name;
//    if ( args.size() > 5 )
//        dp_pdf_file_name = args[5]; // Provided dp_fig_file_name with path
//    else
//        dp_pdf_file_name = matlab_output_path + dp_plain_noExt_noPath + ".pdf";
//
//    // Name of tested algos
//    // TODO
//
//    // Construct command from previous strings
//    std::string cmd = matlab_cmd +" -nosplash -noFigureWindows -nodesktop ";
//    cmd += " -r \"  try ; addpath(genpath('" + matlab_output_path + "')) ;" ;
//    cmd += matlab_function_name + "(";
//    cmd += " '" + tauTitle + "' ," ;
//    cmd += " '" + dp_plain_file_name + "' ," ;
//    cmd += " '" + dp_fig_file_name + "' ,";
//    cmd += " '" + dp_pdf_file_name + "'" ;
//
//    const std::vector<std::string> & legends = runner.get_selected_algo_legends();
//    for (auto leg : legends )
//    {
//        cmd +=  ", '" + leg + "'";
//    }
//    cmd += ") ; catch ; end  ; exit ; \" ";
//
//    int return_value = system(cmd.c_str());
//
//    if ( return_value !=0 )
//    {
//        std::cerr << "\n Error in output_selection: there is a problem with matlab" << std::endl;
//        return false;
//    }
//
//    std::cout << std::endl;
//    return true;
//}

bool read_output_selection_file( Runner &runner )
{
    std::ifstream in("output_selection");
    if ( in.fail() )
    {
        in.close();
        std::cerr << "\n Error reading output_selection file" << std::endl;
        return false;
    }
    
    while(!in.eof())
    {
        std::string line;
        getline (in , line);
        
        remove_comments(line);
        if (line.empty())
        {
            continue;
        }
        
        std::string select_command;
        std::vector<std::string> args;
        find_select_command(line, select_command, args);
    
        
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

//bool read_problem_selection_file(Runner &runner)
//{
//    std::ifstream in("problem_selection");
//    if ( in.fail() )
//    {
//        in.close();
//        std::cerr << "Error reading file problem_selection" << std::endl;
//        return false;
//    }
//
//    bool selectOkByName=true;
//    std::string selectPbName="";
//
//    while(!in.eof())
//    {
//        std::string line;
//        getline (in , line);
//
//        remove_comments(line);
//        if (line.empty())
//        {
//            continue;
//        }
//
//        std::string select_command;
//        std::vector<std::string> args;
//        find_select_command(line, select_command, args);
//
//        if ("select_all_problems" == select_command)
//        {
//            bool bb_eval=false;
//            if (args.size() > 0)
//            {
//                bb_eval = NOMAD_BASE::stringToBool(args[1]);
//            }
//            runner.select_all_problems(bb_eval);
//        }
//        else if ("select_problem_by_name" == select_command
//                 || "select_problems_by_keyword" == select_command)
//        {
//            bool bb_eval = false;
//            if (args.size() >= 2)
//            {
//                bb_eval = NOMAD_BASE::stringToBool(args[1]);
//            }
//
//            if ("select_problem_by_name" == select_command)
//            {
//                if ( ! runner.select_problem_by_name(args[0], bb_eval) )
//                {
//                    selectOkByName = false;
//                    selectPbName = args[0];
//                }
//            }
//            else if ("select_problems_by_keyword" == select_command)
//            {
//                if ( ! runner.select_problems_by_keyword(args[0], bb_eval) )
//                {
//                    selectOkByName = false;
//                    selectPbName = args[0];
//                }
//            }
//        }
//        else if ("select_problems_by_size" == select_command)
//        {
//            bool bb_eval = false;
//            int n_min, n_max;
//            NOMAD_BASE::atoi(args[0], n_min);
//            NOMAD_BASE::atoi(args[1], n_max);
//            if (args.size() >= 3)
//            {
//                bb_eval = NOMAD_BASE::stringToBool(args[2]);
//            }
//            runner.select_problems_by_size(n_min, n_max, bb_eval);
//        }
//        else if ("refine_problems_by_keyword" == select_command)
//        {
//            runner.refine_problems_by_keyword(args[0]);
//        }
//        else if ("exclude_problem_by_name" == select_command)
//        {
//            runner.exclude_problem_by_name(args[0]);
//        }
//        else if ("exclude_problems_by_keyword" == select_command)
//        {
//            runner.exclude_problems_by_keyword(args[0]);
//        }
//        else if ("exclude_problems_with_infeasible_x0" == select_command)
//        {
//            runner.exclude_problems_with_infeasible_x0();
//        }
//        else if ("exclude_problems_by_size" == select_command)
//        {
//            int n_min, n_max;
//            NOMAD_BASE::atoi(args[0], n_min);
//            NOMAD_BASE::atoi(args[1], n_max);
//            runner.exclude_problems_by_size(n_min,n_max);
//        }
//        //
//        // Special options to control the runner
//        //
//        else if ("set_use_avg_fx_first_feas" == select_command )
//        {
//            runner.set_use_avg_fx_first_feas();
//        }
//        else if ("set_use_h_for_profiles" == select_command )
//        {
//            runner.set_use_h_for_profiles() ;
//        }
//        else if ("set_use_hypervolume_for_profiles" == select_command )
//        {
//            runner.set_use_hypervolume_for_profiles() ;
//        }
//        else if ("setOverallMaxBBEvals" == select_command)
//        {
//            int overallMaxBBEvals;
//            bool success = NOMAD_BASE::atoi(args[0],overallMaxBBEvals);
//            if ( ! success )
//            {
//                std::cerr << "Error in output_selection: setOverallMaxBBEvals=" << args[0] << " cannot be converted into integer." << std::endl << std::endl;
//                return false;
//            }
//            Problem::setOverallMaxBBEvals(overallMaxBBEvals);
//        }
//        else if ("setNbSimplexEvals" == select_command)
//        {
//            int nbSimplexEvals;
//            bool success = NOMAD_BASE::atoi(args[0],nbSimplexEvals);
//            if ( ! success )
//            {
//                std::cerr << "Error in output_selection: setNbSimplexEvals=" << args[0] << " cannot be converted into integer." << std::endl << std::endl;
//                return false;
//            }
//            Problem::setNbSimplexEvals(nbSimplexEvals);
//        }
//        else if ("setNomadRunSeeds" == select_command )
//        {
//            int seed;
//            runner.clearAlgoRunSeeds();
//            for ( auto s : args )
//            {
//                if ( ! NOMAD_BASE::atoi( s, seed) )
//                {
//                    in.close();
//                    std::cerr << "Error reading file problem_selection: in setNomadRunSeeds the seeds must be integers" << std::endl;
//                    return false;
//                }
//                runner.appendAlgoRunSeed(seed);
//            }
//        }
//        else if ("setFeasibilityThreshold" == select_command)
//        {
//            if ( args.size() != 1 )
//            {
//                std::cerr << "\n Error in setFeasibilityThreshold: 1 argument (threshold)" << std::endl;
//                return false;
//            }
//            double threshold;
//            if ( ! threshold.atof(args[0]) || !threshold.isDefined() || threshold <= 0 )
//            {
//                std::cerr << "\n Error in setFeasibilityThreshold: threshold should be a strictly positive real" << std::endl;
//                return false;
//            }
//            runner.setFeasibilityThreshold(threshold);
//
//        }
//    }
//
//    if ( ! selectOkByName )
//        std::cerr << std::endl << "WARNING: The problem " << selectPbName << " does not exist. Continuing anyway" << std::endl;
//
//    in.close();
//    return true;
//}
    
/*-------------------------------------*/
/*          RUNNER main function       */
/*-------------------------------------*/
int main ( int argc , char ** argv )
{

    
    // Runner object:
    try {
        
        Runner runner;
        
        
        // Display special options ( Use h for data/perf profiles instead of f (default), use use the average fx value of all the first feasible points instead of the max (default) )
        runner.display_special_options();
        
        // runner.display_selected_problems();
        
        // select algo params config file :
        if ( argc == 1 )
            std::cout << "Error: at least one algo parameters config file must be provided" << std::endl;
//        else
//        {
//            for ( int i = 1 ; i < argc ; ++i )
//            {
//                std::string error_msg;
//                if ( !runner.read_algo_params_file ( argv[i] , error_msg ) )
//                {
//                    return -1;
//                }
//            }
//        }
        
        // display test configs:
        // runner.display_selected_algos() ;
        
        // run:
        std::string error_msg;
        if ( !runner.run ( error_msg ) )
        {
            std::cout << "runner.run() returned the error \"" << error_msg << "\"" << std::endl;
            std::cout << "runner is stopped prematurely" << std::endl;
        }
        // display stats:
        else
        {
            // Read and perform output tasks by functions from output_selection
            read_output_selection_file( runner );
        }
    } catch ( std::exception & e )
    {
        std::string error = std::string ( "ERROR: " ) + e.what();
        std::cerr << std::endl << error << std::endl << std::endl;
        return 0;
    }
    
    return 0;
}

// MENU:
// -----
//
//   RUNNER version X.X
//
//   1. Select problems
//      1. Select all problems
//      2. Select a single problem
//      3. Select a group of problems
//      1. Load selection
//      2. Save selection
//   2. Define test configurations
//      1. New test configuration
//      2. Load configurations
//      3. Save configurations
//   3. Display data
//      1. All problems
//      2. Selected problems
//      3. Test configurations
//      4. Available tests
//   4. Clear data
//      1. Selected problems
//      2. Test configurations
//      3. Remove invalid tests
//      4. Remove all tests
//   5. Execute tests
//   6. Display results
//
//   7. Quit
//
//   > _
