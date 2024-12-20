
#include <gtest/gtest.h>
#include "../src/Runner.hpp"
#include <fstream>

TEST(RunnerTest, DataProfile) {

    // Test if OS is windows
    #ifdef _WIN32
        std::cout << "OS is Windows. Not test is performed" << std::endl;
        return;
    #endif 

    // Create algo_definition.def
    std::ofstream algo_def_file("algo_selection.def");
    algo_def_file << "Algo1 (algo1) [STATS_FILE_NAME stats.txt][STATS_FILE_OUTPUT CNT_EVAL OBJ ] " << std::endl;
    algo_def_file << "Algo2 (algo2) [STATS_FILE_NAME stats.txt][STATS_FILE_OUTPUT CNT_EVAL OBJ ]" << std::endl;
    algo_def_file << "Algo3 (algo3) [STATS_FILE_NAME stats.txt][STATS_FILE_OUTPUT CNT_EVAL OBJ ]" << std::endl;
    algo_def_file.close();

    // Create problem_selection.def
    std::ofstream pb_sel_file("problem_selection.def");
    for (size_t i = 0; i < 20; i++)
    {
        pb_sel_file << "Pb" << i+1 << " (pb" << i << ")  [N " << i+10 << "] [M 1]" << std::endl;
    }
    pb_sel_file.close();

    // Create output_selection.def
    std::ofstream output_sel_file("output_selection.def");
    output_sel_file << "DATA_PROFILE (Data profile on 20 pbs with $\\tau \\; 10^{-2}$) [x_select NP1Eval] [ y_select  OBJ] [tau 0.01   ] [output_plain dp2.txt] [x_max INF]" << std::endl;
    output_sel_file.close();

    // Create directory for Algo et Pb
    for (size_t i = 0 ; i < 3  ; i++)
    {
        for (size_t j = 0 ; j < 20 ; j++)
        {
            std::string dir_name = "Algo" + std::to_string(i+1) + "/Pb" + std::to_string(j+1);
            std::string command = "mkdir -p " + dir_name;
            system(command.c_str());
        }
    }

    // Create stats.txt in each algo and pb directory
    for (size_t i = 0 ; i < 3  ; i++) // Algo# = i +1
    {
        for (size_t j = 0 ; j < 20 ; j++) // Problem #p = j+1
        {
            std::string file_name = "Algo" + std::to_string(i+1) + "/Pb" + std::to_string(j+1) + "/stats.txt";
            std::ofstream stats_file(file_name);
            stats_file << "1 " << j+1 << std::endl; // The starting point is the same for all algos

            // Let's add bbe and f(x) at iteration 10 for all algos
            const size_t maxIt = 10;
            for (size_t it = 0; it < maxIt ; it++)
            {

            if (i == 0) // Algo1
            {
                stats_file << pow(2.0,it+1.0)-1+1 << " " << (j+1)/(1.0+ (it+1)*(j+1)) << std::endl;
            }
            else if (i == 1) // Algo2
            {
                stats_file << pow(it+1.0,3.0)+1 << " " << (j+1)/pow(1+it+1,pow(j+1,0.5)) << std::endl;
            }
            else if (i == 2) // Algo3
            {
                stats_file << 10*pow(it+1.0,2.0)+1 << " " << (j+1)/pow(1.0+j+1.0,it+1.0) << std::endl;
            }
            }
            
            stats_file.close();
        }
    }

    RUNNERPOST::Runner runner;

    std::string error_msg;
    EXPECT_TRUE(runner.read_algo_selection_file ( "algo_selection.def" , error_msg ) );
    runner.display_selected_algos() ;
            
    EXPECT_TRUE(runner.read_problem_selection_file ( "problem_selection.def" , error_msg ) )   ;
    runner.display_selected_problems() ; 

    EXPECT_TRUE(runner.read_output_selection_file ( "output_selection.def" , error_msg ) )   ;

    EXPECT_TRUE(runner.run_post_processing( error_msg ) );

    EXPECT_TRUE(runner.generate_outputs( error_msg ) );



    // Check if the output file is created
    std::ifstream dp_file("dp2.txt");
    EXPECT_TRUE(dp_file.good());

    // Let's check the first line of dp2.txt
    std::string lineTmp, line;
    std::getline(dp_file, line);
    // Remove return carriage from line
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    // Check if the content is correct
    std::string expected_content = "0 0 0 0";
    EXPECT_EQ(line.find(expected_content) != std::string::npos, true);

    // Read all the lines of dp2.txt keeping only the last line 
    while (std::getline(dp_file, lineTmp))
    {
        line = lineTmp;
    }   

    // Check if the content is correct
    expected_content = "94 55 85 100";

    // Remove return carriage from line
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

    // Compare expected_content and dp_content
    EXPECT_EQ(line.find(expected_content) != std::string::npos, true);

    // Close the file
    dp_file.close();


/*
    // Clean up
    system("rm -rf Algo*");
    system("rm -f algo_definition.def");
    system("rm -f problem_selection.def");
    system("rm -f output_selection.def");
    system("rm -f dp2.txt");
*/
}
