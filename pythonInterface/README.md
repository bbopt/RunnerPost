# RunnerPost
RunnerPost is a C++ and Python interface for post-processing and profiling available optimization results. 

For plotting and visualization, Matplotlib library is an option (TODO: script). Also, LaTeX can produce pdf files from tex files created during post-processing.


## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Tests](#tests)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)



## Installation

To install RunnerPost, follow these steps:

### From TestPyPI

# Ensure you have Python 3.8 or higher
```sh
python --version 
```
or 
```sh
python3 --version
```

If you don't have Python 3.8 or higher, you can download and install it from https://www.python.org/downloads/

# You may want to create a virtual environment:
```sh
python3 -m venv venv
source venv/bin/activate  # On Windows use `venv\Scripts\activate`
```

# Install RunnerPost from TestPyPI:
```sh
pip3 install -i https://test.pypi.org/simple/ RunnerPost
```

### From Source

Alternatively, you can build RunnerPost from source.

On Linux, Windows and Mac OS X, RunnerPost can be compiled using *CMake*, a tool to manage building of source code.

The minimum version of *CMake* is 3.15. *CMake* will detect which compiler is available. A recent C++ compiler supporting C++14 is also required.


Follow these steps for getting the source and build RunnerPost:

1. Clone the repository:
    ```bash
    git clone https://github.com/bbopt/RunnerPost.git
    ```
2. Navigate to the project directory:
    ```bash
    cd RunnerPost
    ```
3. Configure and build the project:
    ```bash
    cmake -DBUILD_INTERFACE_PYTHON=ON -DTEST_OPENMP=OFF -S . -B build/release
    cmake --build build/release --config Release
    cmake --install build/release
    ```

4. Install locally the Python package (optional)
    ```bash
    pip install --user --force-reinstall dist/*whl
    ```


## Usage

RunnerPost is a post-processing tool that produces text data profiles from existing optimization results. 
It requires three selection files: algo_selection, problem_selection, and output_selection. 
The selection files define the algorithms, problems, and outputs to be profiled.

### Python

A simple sanity check using provided data is available in the RunnerPost package:

```python
import RunnerPost

RunnerPost.run_test()
```

The following example demonstrates how to use RunnerPost to produce text data profiles from the provided example obtained when downloading the source from GitHub.

To run the example, you need to provide the selection files: 'algo_selection', 'problem_selection', and 'output_selection' available in the example/Run directory. 

First go to the example directory and run the following Python script:

```sh
python3 -m RunnerPost --algo_selection algo_selection.txt --problem_selection problem_selection.txt --output_selection output_selection.txt
```

The content of the selection files is also available in the 'post_selection.json' file in the example directory. 

```sh
python3 -m RunnerPost --json_file post_selection.json
```


### Command line

Alternatively, you can use RunnerPost from the command line without Python.

To start using RunnerPost on the given example, run the following commands:
```bash
cd example/
../build/release/bin/runnerPost algo_selection problem_selection output_selection
```

### Syntax of selection files

#### Summary

RunnerPost uses problems, algorithms, and outputs selection files for post-processing and profiling available optimization results.
Each file follows a consistent structure where each line represents a selection with a description and a list of parameter-value pairs enclosed in square brackets. This format allows for easy editing, parsing and processing of the post-processing data.

algo_selection: Defines algorithms with an ID, a name and description, and parameters.
problem_selection: Defines problems with an ID, a name and description, and some parameters.
output_selection: Defines outputs with a type, a description, and parameters.

#### Algorithm selection file syntax
Each line in the algo_selection file selects an algorithm with its configuration in the following format:

`<id> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]`

- `<id>`: A unique identifier for the algorithm (e.g., Algo_1, Algo_2).
- `<description>`: A brief description of the algorithm enclosed in parentheses.
- `[<parameter> <value>]`: A parameter and its corresponding value enclosed in square brackets.

The `<id>` and `<description>` fields are mandatory, the rest are optional. The description is used as legends of the latex output files.

Example of algo_selection file:
```
Algo_1 (DAMON X: Default for all params) [STATS_FILE_OUTPUT SOL OBJ CST] [ADD_PBINSTANCE_TO_STATS_FILE yes]
Algo_2 (DAMON Y: Default + 2n dirs) [DISPLAY_ALL_EVAL yes] [STATS_FILE_NAME stats.txt] [ADD_PBINSTANCE_TO_STATS_FILE yes]
````

The `STATS_FILE_NAME` parameter is used to specify the name of the files containing the optimization stats for all runs of an algorithm. If not provided, the default name is "history.txt". 
The files must be in directories constructed with the algorithm and the problem id-s. The given file name is the same for all problems and for all instances of a problem. An instance number is automatically added to the file name.

The `ADD_PBINSTANCE_TO_STATS_FILE` is a boolean parameter (`True` or `Yes` or `1` values are interpreted as True, other values are interpreted as False) is used to specify if the instance names should be automatically added when reading the optimization stats file. If the flag is False (default), the stats file name are not modified and a single instance for each problem run is considered.

With this option enabled, The structure of the run directories follows the pattern:
```
 MyRuns/
 ├── Algo_1/
 │   ├── Pb1/
 │   │   ├── stats.A.txt
 │   │   ├── stats.B.txt
 │   │   └── stats.C.txt
 │   └──  Pb2/
 │       ├── stats.0.txt
 │       ├── stats.5.txt
 │       └── stats.6.txt
 └── Algo_2/
     ├── Pb1/
     │   ├── history.A.txt
     │   ├── history.B.txt
     │   └── history.C.txt
     └── Pb2/
         ├── history.0.txt
         ├── history.5.txt
         └── history.6.txt
```

The `STATS_FILE_OUTPUT` is used to describe the stats file content, i.e the outputs of the optimizer. The evaluation outputs are given column-wise. The columns must be separated by a space.
Obviously, the number of columns varies depending on the problem considered. The `STATS_FILE_OUTPUT` parameters describes the organization of the columns.
The column organization must be respected for all problems (and on all the instances) solved by an algorithm. 
The type of columns must be among the following: 
    -- `CNT_EVAL`: the evaluation counter (in the order of the evaluations),
    -- `SOL`: the vector of input variables; the number of variables is given in the problem selection file,
    -- `OBJ`: the objective function value (can be more than one),   
    -- `CST`: the constraints functions value. Even if the problem has several constraints, a single `CST` is required. Indeed the number of constraint can depend on the problem. The number of constraints is given in the problem selection file. 
    -- `TIME`: the time of the evaluation (in seconds),
    -- `FEAS`: the infeasibility flag of an evaluation (0: infeasible, 1: feasible),
    -- `OTHER`: other information (e.g., the number of function evaluations, the number of iterations, the number of constraints evaluations, the number of gradient evaluations, the number of hessian evaluations, the number of hessian-vector evaluations, the number of jacobian evaluations, the number of jacobian-vector evaluations, the number of jacobian

When `STATS_FILE_OUTPUT` is NOT provided, the column types must be deduced. The stats files can contain all evaluations of an optimization (no need to specify the evaluation counter `CNT_EVAL`) or only the best solutions (the evaluation counter `CNT_EVAL`must be provided). 
Hence, the stats file can minimally contain the following columns: `CTN_EVAL OBJ CST` or only `OBJ CST` (the `CST` column is optional).
This is determined from the `DISPLAY_ALL_EVAL` parameter. If `DISPLAY_ALL_EVAL` is set to `yes`, the stats file contains all evaluations of an optimization. If `DISPLAY_ALL_EVAL` is set to `no` (column types are `CNT_EVAL OBJ CST` by default), the stats file contains only the best solutions (column types are `OBJ CST`). The default for `DISPLAY_ALL_EVAL` is `false`.


NOTE: Some cross-validation is done on the stats file content to ensure that the file is correctly formatted.


#### Problem selection file syntax
Each line in the problem_selection file selects a problem with its configuration in the following format:

`<id> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]`

`<id>`: A unique identifier for the problem (e.g., 1, 2, 3).
`<description>`: A brief description of the problem enclosed in parentheses (e.g., (Pb1), (Pb2)).
`[<parameter> <value>]`: A parameter and its corresponding value(s) enclosed in square brackets.

The following parameters are mandatory:
- N, the number of variables must be provided. 
- M, the number of outputs must be provided. Outputs include objectives and constraints. M must be greater than or equal to 1.

In addition, the following parameter is optional:

- PB_INSTANCE, the index of the problem instances solved. There can be more than one instance for problem. This can be achieved by using different initial point or by changing the optimizer seed. The problem instance is undefined when PB_INSTANCE is not provided. If provided the value is used to complement the optimization stats file. 
For example, with the default optimization base stats file name as "stats.txt" (can be changed in optimization_selection file with the STATS_FILE argument), the file containing the optimization stats for the problem instance "1" is named stats.1.txt.

Example of problem_selection file:
```
1 (Pb1) [N 9] [M 1] [PB_INSTANCE 0 1 2 3 4]
2 (Pb2) [N 9] [M 1] [PB_INSTANCE 0 1 2]
```

#### Output selection file syntax
Each line in the output_selection file selects an output profile with the following format:

`<type> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]`

`<type>`: The type of profile or output (e.g., DATA_PROFILE).
`<description>`: A brief description of the profile or output enclosed in parentheses (e.g., (Data profile on 10 pbs with $\tau\; 10^{-1}$)). This information is used as the title of the output.
`[<parameter> <value>]`: A parameter and its corresponding value enclosed in square brackets.

Comments can be added at the end of each line starting with the # character.

Example of output_selection file:
```
DATA_PROFILE (Data profile on 10 pbs with $\tau\; 10^{-1}$) [x_select NP1Eval] [y_select OBJ] [tau 0.1] [output_plain dp1.txt] [output_latex dp1.tex] [x_max INF]
DATA_PROFILE (Data profile on 10 pbs with $\tau\; 10^{-3}$) # default (tau=0.001, np1eval, obj, dp3.txt, INF, dp3.tex)

### LaTeX outputs
This step requires to have a LaTeX distribution installed on your machine.

To obtained pdf files from tex file:
```bash
pdflatex dp1.tex
```

## Tests
TODO. In progress.

## Contributing

We welcome contributions! Please read our [contributing guidelines](CONTRIBUTING.md) for more information.


## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or feedback, please contact us at nomad@gerad.ca.
