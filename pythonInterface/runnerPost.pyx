# RunnerPost

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


# Define the interface function to perform optimization
# For now, we only show one best solution.
def run(algo_selection_file, pb_selection_file, output_selection_file):
    readAndPostprocess(algo_selection_file, pb_selection_file, output_selection_file)


cdef extern from "RunnerPostCythonInterface.cpp":
    void printRunnerPostVersion()
    void printRunnerPostUsage()
    void printRunnerPostInfo()
    int readAndPostprocess(string & algo_selection_file,
                           string & pb_selection_file,
                           string & output_selection_file) except+
