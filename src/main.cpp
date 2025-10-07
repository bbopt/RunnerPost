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


    
/*-------------------------------------*/
/*          RUNNER main function       */
/*-------------------------------------*/
int main ( int argc , char ** argv )
{

    
    // Runner object:
    try {

        RUNNERPOST::Runner runner;
        
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
        
        // Read output_selection file
        if ( !runner.read_output_selection_file ( argv[3] , error_msg ) )
        {
            std::cerr << "Cannot read output config file \"" << argv[3] << "\"" << std::endl;
            std::cerr << "Stop prematurely with error \"" << error_msg << "\"" << std::endl;
            return 1;
        }
        runner.display_selected_outputs();
        
        if ( !runner.read_problem_selection_file ( argv[2] , error_msg ) )
        {
            std::cerr << std::endl << "Cannot read pbs config file \"" << argv[2] << "\"" << std::endl;
            std::cerr << "Stop prematurely with error \"" << error_msg << "\"" << std::endl;
            return 1;
        }
        runner.display_selected_problems() ;
        
        if (!runner.algo_pb_check_consistency(error_msg))
        {
            std::cout << "runner check failed: \n" << error_msg << std::endl;
            std::cout << "runner is stopped prematurely" << std::endl;
            return 1;
        }
        
        
        // Post processing of results is done after reading the outputs.
        // For example, the max bbe can be used for post processing.
        if ( !runner.run_post_processing( error_msg ) )
        {
            std::cout << "runner.post_processing() returned the error \"" << error_msg << "\"" << std::endl;
            std::cout << "runner is stopped prematurely" << std::endl;
            return 1;
        }
        
        // Generate outputs
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

