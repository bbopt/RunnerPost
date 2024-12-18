from . import RunnerPost
from . import postProcess

import os
import argparse

if __name__ == '__main__':
    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Post process RunnerPost output.')

    # Depending on the number of arguments, we will either run with the user provided 3 definition files or single JSON file

    # Case 1: 3 definition files
    parser.add_argument('--algo_definition', type=str, help='Path to the algorithm definition file')
    parser.add_argument('--problem_definition', type=str, help='Path to the problem definition file')
    parser.add_argument('--output_definition', type=str, help='Path to the output definition file')

    # Case 2: JSON file
    parser.add_argument('json_file', type=str, help='Path to the JSON file')
   
    args = parser.parse_args()

    # Manage the argument is -h
    if args.json_file == '-h':
        parser.print_help()
        exit(0)

    # Manage the case where the user provides 3 definition files
    if args.algo_definition and args.problem_definition and args.output_definition:

        if args.json_file:
            print("Error: JSON file should not be provided when using definition files.")
            exit(1)

        if not args.algo_definition or not args.problem_definition or not args.output_definition:
            print("Error: All three definition files must be provided.")
            exit(1)

        if not os.path.isfile(args.algo_definition):
            print(f"Error: Algorithm definition file '{args.algo_definition}' does not exist.")
            exit(1)

        if not os.path.isfile(args.problem_definition):
            print(f"Error: Problem definition file '{args.problem_definition}' does not exist.")
            exit(1)

        if not os.path.isfile(args.output_definition):
            print(f"Error: Output definition file '{args.output_definition}' does not exist.")
            exit(1)

        with open(args.algo_definition, 'r') as file:
            algo_definition = file.read()

        with open(args.problem_definition, 'r') as file:
            problem_definition = file.read()

        with open(args.output_definition, 'r') as file:
            output_definition = file.read()

        RunnerPost.run(algo_definition.encode(), problem_definition.encode(), output_definition.encode())


    # Manage the case where the user provides a JSON file
    else:
        if not os.path.isfile(args.json_file):
            print(f"Error: JSON file '{args.json_file}' does not exist.")
            exit(1)

        postProcess.postProcess(args.json_file)