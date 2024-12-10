# /Users/christophe/Development/RunnerPost/pythonInterface/RunnerPost/__init__.py


import os
import json

# Initialize the RunnerPost package

from . import RunnerPost

def version():
    return RunnerPost.version()

# Define the interface function to display nomad general information
def usage():
    return RunnerPost.usage()

# Define the interface function to display nomad general information
def info():
    return RunnerPost.info()

def doc():
    return RunnerPost.__doc__()

def run(json_file):

    # test if json file exists
    if not os.path.exists(json_file):
        print("JSON file does not exist: ", json_file)
        return

    # Read runnerPost json file.
    # If the file does not exist, return an empty dictionary.
    # This will be caught in the main function.
    # The json contains the following: algo_selection, problem_selection, output_selection
    # The format is given in the README file.
    with open(json_file, 'r') as file:
        postConfig = json.load(file)


    # Create the config strings for each selection file
    algo_selection = ""
    problem_selection = ""
    output_selection = ""

    for output in postConfig['algo_selection']:
        algo_id = next((value for key, value in output.items() if key.lower() == 'id'), '')
        # error message if algo_id is not found
        if not algo_id:
            print("Algo ID not found in the JSON file.",args.json_file)
            return

        algo_name = next((value for key, value in output.items() if key.lower() == 'name'), '')
        if not algo_name:
            print("Algo name and description not found in the JSON file.",args.json_file)
            return

        algo_selection += f"{algo_id} ({algo_name})"

        params = {k.lower(): v for k, v in output['parameters'].items()}
        for key, value in params.items():
            algo_selection += f" [{key.upper()} {value}]"

        # Add a carriage return in between each selection
        algo_selection += "\n"

        # print(algo_selection)


    for output in postConfig['problem_selection']:
        problem_id = next((value for key, value in output.items() if key.lower() == 'id'), '')
        # error message if problem_id is not found
        if not problem_id:
            print("Problem ID not found in the JSON file.",args.json_file)
            return

        problem_name = next((value for key, value in output.items() if key.lower() == 'name'), '')
        if not problem_name:
            print("Problem name and description not found in the JSON file.",args.json_file)
            return

        problem_selection += f"{problem_id} ({problem_name})"

        if 'parameters' in output:
            params = {k.lower(): v for k, v in output['parameters'].items()}
            for key, value in params.items():
                problem_selection += f" [{key.upper()} {value}]"

        # Add a carriage return in between each selection
        problem_selection += "\n"
        
        # print(problem_selection)

    for output in postConfig['output_selection']:

        output_id = next((value for key, value in output.items() if key.lower() == 'type'), '')
        # error message if output_id is not found
        if not output_id:
            print("Output ID not found in the JSON file.",args.json_file)
            return

        output_name = next((value for key, value in output.items() if key.lower() == 'description'), '')
        if not output_name:
            print("Output description not found in the JSON file.",args.json_file)
            return

        output_selection += f"{output_id} ({output_name})"

        if 'parameters' in output:
            params = {k.lower(): v for k, v in output['parameters'].items()}
            for key, value in params.items():
                output_selection += f" [{key.upper()} {value}]"

        # Add a carriage return in between each selection
        output_selection += "\n"
        #print(output_selection)

    # Encode strings before calling RunnerPost
    RunnerPost.run(algo_selection.encode(), problem_selection.encode(), output_selection.encode())


def run_test():
    HERE = os.path.abspath(os.path.dirname(__file__))

    print(HERE)
    # Read runnerPost json file  

    