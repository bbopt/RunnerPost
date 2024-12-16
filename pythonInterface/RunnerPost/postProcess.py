import os
import json
import argparse

import RunnerPost.RunnerPost as RunnerPost

import matplotlib.pyplot as plt


def toConfig(JSonFile):

    # Read runnerPost json file.
    # If the file does not exist, return an empty dictionary.
    # This will be caught in the main function.
    # The json contains the following: algo_selection, problem_selection, output_selection
    # The format is given in the README file.
    with open(JSonFile, 'r') as file:
        data = json.load(file)

    return data

def postProcess(json_file):

    postConfig = toConfig(json_file)

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


def plot(file_path,tau):

    # Read data from file
    x = []
    algo1 = []
    algo2 = []

    with open(file_path, 'r') as file:
        for line in file:
            parts = line.split()
            x.append(int(parts[0]))
            algo1.append(float(parts[1]))
            algo2.append(float(parts[2]))

    # Plot the data
    plt.figure(figsize=(10, 6))
    plt.plot(x, algo1, label='Algo1', marker='o')
    plt.plot(x, algo2, label='Algo2', marker='s')

    # Add titles and labels
    plt.title('$\tau$ = ')
    plt.xlabel('Number of (n+1) evaluations')
    plt.ylabel('Proportion of instances $\tau$-solved')

    # Add legend
    plt.legend()

    # Show grid
    plt.grid(True)

    # Show plot
    plt.show()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Convert JSON file to config format.')
    parser.add_argument('json_file', type=str, help='Path to the JSON file')
    args = parser.parse_args()

    # test if json file exists
    if not os.path.exists(args.json_file):
        print("JSON file does not exist: ", args.json_file)
    else:
        postProcess(args.json_file)
