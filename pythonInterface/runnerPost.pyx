# RunnerPost

# TODO: make the code compatible with Cython v3

from libcpp cimport bool
from libcpp.string cimport string
from libcpp.vector cimport vector

from cython.operator cimport dereference as deref

def version():
    printRunnerPostVersion()

# Define the interface function to display nomad general information
def usage():
    printRunnerPostUsage()

# Define the interface function to display nomad general information
def info():
    printRunnerPostInfo()

def __doc__():
    cdef string about;
    printRunnerPostUsage()


# Define the interface function to perform run post processing
def run(algo_selection, pb_selection, output_selection):
    readAndPostprocess(algo_selection, pb_selection, output_selection)

cdef extern from "RunnerPostCythonInterface.cpp":
    void printRunnerPostVersion()
    void printRunnerPostUsage()
    void printRunnerPostInfo()
    int readAndPostprocess(string & algo_selection,
                           string & pb_selection,
                           string & output_selection) except+
