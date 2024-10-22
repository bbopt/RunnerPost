// October 2024
// Version 1.0

#include "src/Result.hpp"
#include "src/Runner.hpp"

#include <Python.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>



static void printRunnerPostVersion()
{
    std::cout << "-----------------------------------------------------------"  << std::endl;
    std::cout << " Python Interface to RunnerPost version " << RUNNERPOST_VERSION  << std::endl;
    std::cout << "-----------------------------------------------------------"  << std::endl;
}


static void printRunnerPostUsage()
{
    std::cout << "--------------------------------------------------------------"       << std::endl;
    std::cout << " RunnerPost Python interface usage"                                             << std::endl;
    std::cout << "--------------------------------------------------------------"       << std::endl;
    std::cout << "  RunnerPost : result = RunnerPost.read(param)"       << std::endl;
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

static int read( const std::vector<std::string> &params)
{
    
    int runFlag = 0 ;

    std::cout<<"Runner Post "<<std::endl;
    try
    {
        Py_BEGIN_ALLOW_THREADS

        RunnerPost::Runner runner;
        
        RunnerPost::Result result = runner.read(params);

        Py_END_ALLOW_THREADS
        return runFlag;
    }
    catch(std::exception &e)
    {
        printf("RunnerPost exception (report to developper):\n%s\n",e.what());
    }

    return runFlag;

}
