// October 2024
// Version 1.0


#include "Runner.hpp"

#include <Python.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>


 
static void printRunnerPostVersion()
{
    std::cout << "-----------------------------------------------------------"  << std::endl;
    std::cout << " Python Interface to RunnerPost version " << RUNNERPOST_VERSION_NUMBER  << std::endl;
    std::cout << "-----------------------------------------------------------"  << std::endl;
}


static void printRunnerPostUsage()
{
    std::cout << "--------------------------------------------------------------"       << std::endl;
    std::cout << " RunnerPost Python interface usage"                                             << std::endl;
    std::cout << "--------------------------------------------------------------"       << std::endl;
    std::cout << "  >>> RunnerPost.run(runnerPostSelection.json)"       << std::endl;
    std::cout << "--------------------------------------------------------------"       << std::endl;
    
}


static void printRunnerPostInfo()
{
    std::cout << "-----------------------------------------------------------"          << std::endl;
    std::cout << " RunnerPost: Optimization post processing benchmark profiles"              << std::endl;
    std::cout << " Released under the GNU General Public License v3:"               << std::endl;
    std::cout << " https://www.gnu.org/licenses/gpl-3.0.html"                             << std::endl;
    std::cout << " Source available at: https://github.com/bbopt/RunnerPost" << std::endl;

    printRunnerPostVersion();

    printRunnerPostUsage();
}

static int readAndPostprocess( const std::string & algo_selection_file, const std::string & pb_selection_file, const std::string & output_selection_file)
{
    
    int runFlag = 0 ;

    std::cout<<"Runner Post "<<std::endl;
    try
    {
        Py_BEGIN_ALLOW_THREADS

        RUNNERPOST::Runner runner;
        
        std::string error_msg;
        if ( !runner.read_algo_selection_file ( algo_selection_file , error_msg ) )
        {
            std::cerr << "Cannot read algo config file \"" << algo_selection_file << "\"" << std::endl;
            std::cerr << "Stop prematurely with error \"" << error_msg << "\"" << std::endl;
            return 1;
        }
        
        // display test configs:
        runner.display_selected_algos() ;
        
        if ( !runner.read_problem_selection_file ( pb_selection_file , error_msg ) )
        {
            std::cerr << "Cannot read pbs config file \"" << pb_selection_file << "\"" << std::endl;
            std::cerr << "Stop prematurely with error \"" << error_msg << "\"" << std::endl;
            return 1;
        }
        
        // display test configs:
        runner.display_selected_problems() ;
        
        
        // run:
        if ( !runner.run_post_processing( error_msg ) )
        {
            std::cout << "runner.post_processing() returned the error \"" << error_msg << "\"" << std::endl;
            std::cout << "runner is stopped prematurely" << std::endl;
            return 1;
        }
        
        
        // Output
        
        // Read output_selection file
        if ( !runner.read_output_selection_file ( output_selection_file , error_msg ) )
        {
            std::cerr << "Cannot read output config file \"" << output_selection_file << "\"" << std::endl;
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


        Py_END_ALLOW_THREADS
        return runFlag;
    }
    catch(std::exception &e)
    {
        printf("RunnerPost exception (report to developper):\n%s\n",e.what());
    }

    return runFlag;

}
