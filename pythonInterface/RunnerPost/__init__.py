
import os
import json
import pkg_resources

from . import RunnerPost
from . import postProcess

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
    postProcess.postProcess(json_file)

# Run on test data set included
def run_test():

    json_file_name = pkg_resources.resource_filename(__name__, f'Data/post_selection.json')

    # Get the current directory
    current_directory = os.getcwd()

    #Extract the path from the json file
    path = os.path.dirname(json_file_name)
        
    # Change the working directory to the directory of the json file
    if path:
        os.chdir(path)

        # Remove the path from the json file
        json_file = json_file_name.replace(path + "/", "")
    else:
        json_file = json_file_name

    postProcess.postProcess(json_file)

    # Change the working directory back to the original directory
    os.chdir(current_directory)

    