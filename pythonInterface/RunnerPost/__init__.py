
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

    json_file = pkg_resources.resource_filename(__name__, f'Data/post_selection.json')


    # json_file = os.path.join(os.path.abspath(os.path.dirname(__file__)),"Data","post_selection.json")

    run(json_file)
