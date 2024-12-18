from . import RunnerPost
from . import postProcess

import os
import argparse

if __name__ == '__main__':
    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Post process RunnerPost output.')

    # Depending on the arguments, we will either run with the user provided 3 definition files or single JSON file

    # Case 1: 3 definition files
    parser.add_argument('--algo_selection', required = False, type=str, help='Path to the algorithm definition file')
    parser.add_argument('--problem_selection',required = False, type=str, help='Path to the problem definition file')
    parser.add_argument('--output_selection', required = False, type=str, help='Path to the output definition file')

    # Case 2: JSON file
    parser.add_argument('--json_file', required = False, type=str, help='Path to the JSON file')
    
    args = parser.parse_args()

    # Manage the case where the user provides 3 definition files
    if args.algo_selection and args.problem_selection and args.output_selection:

        if args.json_file:
            print("Error: JSON file should not be provided when using definition files.")
            exit(1)

        if not args.algo_selection or not args.problem_selection or not args.output_selection:
            print("Error: All three definition files must be provided.")
            exit(1)

        if not os.path.isfile(args.algo_selection):
            print(f"Error: Algorithm definition file '{args.algo_selection}' does not exist.")
            exit(1)

        if not os.path.isfile(args.problem_selection):
            print(f"Error: Problem definition file '{args.problem_selection}' does not exist.")
            exit(1)

        if not os.path.isfile(args.output_selection):
            print(f"Error: Output definition file '{args.output_selection}' does not exist.")
            exit(1)

        #Extract the path from the algo_selection file
        path1 = os.path.dirname(args.algo_selection)
        path2 = os.path.dirname(args.problem_selection)
        path3 = os.path.dirname(args.output_selection)
        if (path1 != path2) or (path2 != path3):
            print("Error: All three definition files must be in the same directory.")
            exit(1)

        with open(args.algo_selection, 'r') as file:
            algo_selection = file.read()

        with open(args.problem_selection, 'r') as file:
            problem_selection = file.read()

        with open(args.output_selection, 'r') as file:
            output_selection = file.read()

        # Get the current working directory
        working_directory = os.getcwd()
        
        if path1:
            # Change the working directory to the directory of the definition files
            os.chdir(path1)

        RunnerPost.run(algo_selection.encode(), problem_selection.encode(), output_selection.encode())

        # Change the working directory back to the original directory
        os.chdir(working_directory)

    # Manage the case where the user provides a JSON file
    elif args.json_file:
        if args.algo_selection or args.problem_selection or args.output_selection:
            print("Error: Definition files should not be provided when using a JSON file.")
            exit(1)

        if not os.path.isfile(args.json_file):
            print(f"Error: JSON file '{args.json_file}' does not exist.")
            exit(1)
    
        # Get the current directory
        current_directory = os.getcwd()

        #Extract the path from the json file
        path = os.path.dirname(args.json_file)
        
        # Change the working directory to the directory of the json file
        if path:
            os.chdir(path)

            # Remove the path from the json file
            json_file = args.json_file.replace(path + "/", "")
        else:
            json_file = args.json_file

        postProcess.postProcess(json_file)
        
        # Change the working directory back to the original directory
        os.chdir(current_directory)

        
    else:
        parser.print_help()
        exit(0)