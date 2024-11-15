#include "Runner.hpp"

#include <map>
#include <fstream>

std::map<double,std::string> map_tau_to_dp_file_name;
std::map<double,std::string> map_tau_to_pp_file_name;
std::map<double,std::string> map_tau_to_time_profile_file_name;
std::map<double,std::string> map_tau_to_tdp_file_name;



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


//void trim(std::string &word)
//{
//    // Remove starting and ending spaces
//    size_t word_size = word.size();
//    while (word_size > 0 && (' ' == word[0] || '\t' == word[0]))
//    {
//        word = word.substr(1, word_size-1);
//        word_size = word.size();
//    }
//
//    while (word_size > 0 && (' ' == word[word_size-1] || '\t' == word[word_size-1]))
//    {
//        word = word.substr(0, word_size-1);
//        word_size = word.size();
//    }
//
//    // remove "runner." before the command
//    if (word_size > 0 && 0 == word.find("runner."))
//    {
//        word = word.substr(7, word_size-7);
//        word_size = word.size();
//    }
//
//    // Remove starting and ending " (once)
//    if (word_size >= 2 && '"' == word[0] && '"' == word[word_size-1])
//    {
//        word = word.substr(1,word_size-2);
//        word_size = word.size();
//    }
//}

//void remove_comments(std::string &line)
//{
//    // Ignore comments, starting with //
//    size_t i = line.find("//");
//    if (i != std::string::npos)
//    {
//        line = line.substr(0, i);
//    }
//}

//void find_select_command(const std::string line, std::string &select_command, std::vector<std::string> &args)
//{
//    // First word is the select_command
//    size_t args_index = line.find("(");
//    select_command = line.substr(0, args_index);
//    trim(select_command);
//
//    size_t arg_start_index = args_index;
//    size_t arg_end_index = line.find(",", arg_start_index);
//    while (std::string::npos != arg_end_index)
//    {
//        std::string arg = line.substr(arg_start_index+1, arg_end_index-arg_start_index-1);
//        trim(arg);
//        args.push_back(arg);
//        arg_start_index = arg_end_index;
//        arg_end_index = line.find(",", arg_start_index+1);
//    }
//    // last arg
//    arg_end_index = line.find(")", arg_start_index);
//    std::string arg = line.substr(arg_start_index+1, arg_end_index-arg_start_index-1);
//    trim(arg);
//    if ( arg.length() >0 )
//        args.push_back(arg);
//
//}

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
        
        RUNNERPOST::Runner runner;
        
        // TODO
        // Select special options
        
        // Display special options ( Use h for data/perf profiles instead of f (default), use use the average fx value of all the first feasible points instead of the max (default) )
        // runner.display_special_options();
        
        // Algo config file :
        if ( argc < 4 )
        {
            std::cout << "Error: at least one algo selection file, one problem selection file and one output selection file must be provided" << std::endl;
            return 1;
        }
        
        std::string error_msg;
        if ( !runner.read_algo_selection_file ( argv[1] , error_msg ) )
        {
            std::cerr << "Cannot read algo config file \"" << argv[1] << "\"" << std::endl;
            std::cerr << "Stop prematurely with error \"" << error_msg << "\"" << std::endl;
            return 1;
        }
        
        // display test configs:
        runner.display_selected_algos() ;
        
        if ( !runner.read_problem_selection_file ( argv[2] , error_msg ) )
        {
            std::cerr << "Cannot read pbs config file \"" << argv[2] << "\"" << std::endl;
            std::cerr << "Stop prematurely with error \"" << error_msg << "\"" << std::endl;
            return 1;
        }
        
        // display test configs:
        runner.display_selected_problems() ;
        
        if (!runner.algo_pb_check_consistency(error_msg))
        {
            std::cout << "runner check failed: \n" << error_msg << std::endl;
            std::cout << "runner is stopped prematurely" << std::endl;
            return 1;
        }
        
        
        // run:
        if ( !runner.run_post_processing( error_msg ) )
        {
            std::cout << "runner.post_processing() returned the error \"" << error_msg << "\"" << std::endl;
            std::cout << "runner is stopped prematurely" << std::endl;
            return 1;
        }
        
        
        // Output
        
        // Read output_selection file
        if ( !runner.read_output_selection_file ( argv[3] , error_msg ) )
        {
            std::cerr << "Cannot read output config file \"" << argv[3] << "\"" << std::endl;
            std::cerr << "Stop prematurely with error \"" << error_msg << "\"" << std::endl;
            return 1;
        }
        
        runner.display_selected_outputs();
        
        if ( !runner.generate_outputs( error_msg ) )
        {
            std::cout << "runner.post_processing() returned the error \"" << error_msg << "\"" << std::endl;
            std::cout << "runner is stopped prematurely" << std::endl;
            return 1;
        }
        
        
    }
    catch ( std::exception & e )
    {
        std::string error = std::string ( "ERROR: " ) + e.what();
        std::cerr << std::endl << error << std::endl << std::endl;
        return 1;
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
