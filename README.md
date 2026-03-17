# RunnerPost
RunnerPost is a C++ and Python interface for post-processing and profiling available optimization results. 

For plotting and visualization, Matplotlib library is an option (TODO: script). Also, LaTeX can produce pdf files from tex files created during post-processing.

The graphes presented in the technical paper [1] were produced using the RunnerPost. RunnerPost uses the concepts and notation of the paper which we suggest to read before using the tool.


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


Ensure you have Python 3.9 or higher:

```sh
python --version 
```
or 
```sh
python3 --version
```

If you don't have Python 3.9 or higher, you can download and install it from https://www.python.org/downloads/

You may want to create a virtual environment:
```sh
python3 -m venv venv
source venv/bin/activate  # On Windows use `venv\Scripts\activate`
```

**To install RunnerPost from TestPyPI:**
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
    cmake -DBUILD_INTERFACE_PYTHON=ON -S . -B build/release
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

To run the example, you need to provide the selection files: `algo_selection.def`, `problem_selection.def`, and `output_selection.def` available in the `example/Unconstrained_Runs_With_Two_Algos` directory. 

First go to the example directory and run the following Python script:

```sh
python3 -m RunnerPost --algo_selection algo_selection.def --problem_selection problem_selection.def --output_selection output_selection.def
```

The content of the selection files is also available in the `post_selection.json` file in the example directory. 

```sh
python3 -m RunnerPost --json_file post_selection.json
```


### Command line

Alternatively, you can use RunnerPost from the command line without Python.

To start using RunnerPost on the given example, run the following commands:
```bash
cd example/
../build/release/bin/runnerPost algo_selection.def problem_selection.def output_selection.def
```

### Syntax of selection files

#### Summary

RunnerPost uses problems, algorithms, and outputs selection files for post-processing and profiling available optimization results.
Each file follows a consistent structure where each line represents a selection with a description and a list of parameter-value pairs enclosed in square brackets. This format allows for easy editing, parsing and processing of the post-processing data.

algo_selection: Defines algorithms with an ID, a name and description, and parameters.
problem_selection: Defines problems with an ID, a name and description, and some parameters.
output_selection: Defines outputs with a type, a description, and parameters.

#### Algorithm selection
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

The `ADD_PBINSTANCE_TO_STATS_FILE` is a boolean parameter (`True` or `Yes` or `1` values are interpreted as True, other values are interpreted as False) is used to specify if the instance names should be automatically added when reading the optimization stats file. If the flag is False (default), the stats file name are not modified and a single instance for each problem run is considered. The problem instance names can be provided in the problem definition (See below).

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
A column type must be among the following: 
- `CNT_EVAL`: the evaluation counter (in the order of the evaluations),
- `SOL`: the vector of input variables; the number of variables is given in the problem selection file,
- `OBJ`: the objective function value (can be more than one),   
- `CST`: the constraints functions value. Even if the problem has several constraints, a single `CST` is required. Indeed the number of constraint can depend on the problem. The number of constraints is given in the problem selection file. 
- `TIME`: the time stamp of the evaluation (in seconds),
- `FEAS`: the infeasibility flag of an evaluation (0: infeasible, 1: feasible),
- `OTHER`: other information that will not be considered in the post-processing (e.g., a user-defined counter).

When `STATS_FILE_OUTPUT` is NOT provided, the column types must be deduced. The stats files can contain all evaluations of an optimization (no need to specify the evaluation counter `CNT_EVAL`) or only the best solution. In the later case, the evaluation counter `CNT_EVAL` must be provided. 

Hence, the stats file can minimally contain the following columns: `CTN_EVAL OBJ CST` or only `OBJ CST` (the `CST` column is optional).
This is determined from the `DISPLAY_ALL_EVAL` parameter. If `DISPLAY_ALL_EVAL` is set to `yes`, the stats file contains all evaluations of an optimization. If `DISPLAY_ALL_EVAL` is set to `no` (column types are `CNT_EVAL OBJ CST` by default), the stats file contains only the best solutions (column types are `OBJ CST`). The default for `DISPLAY_ALL_EVAL` is `false`.


NOTE: Some cross-validation is done on the stats file content to ensure that the file is correctly formatted.


#### Problem selection
Each line in the problem_selection file selects a problem with its configuration in the following format:

`<id> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]`

With:
- `<id>`: A unique identifier for the problem (e.g., 1, 2, 3).
- `<description>`: A brief description of the problem enclosed in parentheses (e.g., (Pb1), (Pb2)).
- `[<parameter> <value>]`: A parameter and its corresponding value(s) enclosed in square brackets.

The following parameters are mandatory:
- `N`, the number of variables must be provided. 
- `M`, the number of outputs must be provided. Outputs include objectives and constraints (inequality and equality). M must be greater than or equal to 1. The number of equality constraints is given by parameter `P`. The default is zero.

In addition, the following parameter is optional:

- PB_INSTANCE, the index of the problem instances solved. There can be more than one instance for problem. This can be achieved by using different initial point or by changing the optimizer seed. The problem instance is undefined when PB_INSTANCE is not provided. If provided the value is used to complement the optimization stats file. 
For example, with the default optimization base stats file name as "stats.txt" (can be changed in optimization_selection file with the STATS_FILE argument), the file containing the optimization stats for the problem instance "1" is named stats.1.txt.

Example of problem_selection file:
```
1 (Pb1) [N 9] [M 1] [PB_INSTANCE 0 1 2 3 4]
2 (Pb2) [N 9] [M 1] [PB_INSTANCE 0 1 2]
```

#### Output selection
Each line in the output_selection file selects an output profile with the following format:

`<type> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]`

With:
- `<type>`: The type of profile or output (e.g., DATA_PROFILE).
- `<description>`: A brief description of the profile or output enclosed in parentheses (e.g., (Data profile on 10 pbs with $\tau\; 10^{-1}$)). This information is used as the title of the output.
- `[<parameter> <value>]`: A parameter and its corresponding value enclosed in square brackets.

Comments can be added at the end of each line starting with the # character.

The current version supports `DATA_PROFILE`, `PERFORMANCE_PROFILE`, `CONVERGENCE_PROFILE` and `ACCURACY_PROFILE`.

The following parameters have default value but it is recommended to provide them for more clarity of the output selection file:
- `x_select`, the type of x-axis:  `NP1Eval` (default) or `EVAL` or `TIME`(algorithm must output the time).  
- `y_select`, the type of y-axis: `OBJ`, `INFEAS` (the infeasibility is computed from the constraints h(x)=Sum_j(max(g_j,0)^2))
- `tau`, the value of the tolerance with respect to the best solution: typical values are 0.1, 0.00001.
- `output_plain`, the plain output file name. A good practice is to name it according to the tau value. For example, `tau 0.1` - > `output_plain dp1.txt`.
- `output_latex`, the latex output file name. 
- `x_max`, the x-axis max value for plotting. `INF` can be used to parse all the evaluations of a problem and deduce the correct maximum value.
- `plot_selection`, to select a specific combination of algorithms, problems and problem instances or select them all (default). The keyword must be followed by an algo name or `*` (or `ALLALGO`), followed by a problem name or `*`(or `ALLPB`), followed by an instance name or `*`(or `ALLINST`). 
- `plot_type` for the convergence profiles (mandatory): can be selected among the following: `OnlyFFeasible`, `OnlyF`, `OnlyHInfeasible`, `ComboHInfAndFFeas` (see the below for the `ComboHInfAndFFeas').  

Example of output_selection file:
```
DATA_PROFILE (Data profile on 10 pbs with $\tau\; 10^{-1}$) [x_select NP1Eval] [y_select OBJ] [tau 0.1] [output_plain dp1.txt] [output_latex dp1.tex] [x_max INF]
DATA_PROFILE (Data profile on 10 pbs with $\tau\; 10^{-3}$) [x_select NP1Eval] [y_select OBJ] [tau 0.001] [output_plain dp3.txt] [output_latex dp3.tex] [x_max INF]
CONVERGENCE_PROFILE (Convergence plots) [output_plain convergenceCombo.txt][output_latex convergenceCombo.tex] [x_max 200][plot_selection * 1]
```

### Convergence plots with constraint violation and objective function improvments
Convergence plots allow a comparison of algorithms on a few instances of a problem. To obtain on the same plot a y-axis for constraint violation and a y-axis for the best objective function value one can specify a plot type `ComboHInfAndFFeas` for that purpose: 
```
CONVERGENCE_PROFILE (Convergence plots) [output_plain convergenceCombo.txt][output_latex convergenceCombo.tex] [x_max 200][plot_selection * 1][plot_type ComboHInfAndFFeas]
```

### Profiling with several problem instances
As previously mentionned, the RunnerPost can manage more than one instance for a given problem. Problem instances can be obtained using different initial points or by changing the optimizer seed.
For the different types of profiles, this can impact what value is considered for `f_*` and the accuray value.

It is possible to consider that all problem instances are different problems. So, we will have the same `f_0` for all problem instances of the same problem but different `f_*`.

But, when using different seeds, all the runs of all algorithms on this problem can be considered to determine `f_*`.

This distinction can be specified whithin the output selection with the keyword `FXBEST_SELECT`. The possible values are `CROSSINSTANCE`, `SINGLEINSTANCE` or `USERPROVIDE` (not yet implemented), the default is to consider all problem instances to determine `f_*` (`CROSSINSTANCE`). 


### Profiling constrained problems
Profiling algorithms on constrained problems with infeasible initial points requires to properly define the accuracy value, which depends on `f_0` and `f_*`. Using `f(x_0)` for `f_0` is not appropriate because this value can be lower than `f_*`. We have three approaches for defining `f_0` that respect that all algorithms and test problem instances are treated equally. We can consider the minimum, the maximum or the average of the first feasible values obtained by all algorithms on the problem instance.

The keyword `FX_FIRST_FEAS` can be used to select the desired approach. The default is `MAX` approach.

```
ACCURACY_PROFILE (Accuracy profiles)[ y_select  OBJ][FX_FIRST_FEAS MIN][output_plain ap_min.txt][output_latex ap_min.tex] [x_max INF]
```


### Profiling for bi-objective optimization 
Several elaborate strategies exist for benchmarking multi-objective optimization algorithms. The set of non-dominated points forms the solution set of a multi-objective problem, known as the Pareto set. Its image in the objectives space is called the Pareto front.

In the RunnerPost, to quantify the quality of Pareto front approximations we have selected the hypervolume indicator. At each evaluation of an algorithm,on a problem we compute the hypervolume indicator. This volume indicator replaces the objective function value to plot any type of profiles.

For now, the RunnerPost is limited to bi-objective problems post-processing only.

Based on the `STATS_FILE_OUTPUT` of algorithm definition, the RunnerPost detects that more than one objective is considered and uses the hypervolume indicator instead of the objective function value:
```
algo2 (Algo 2) [DISPLAY_ALL_EVAL yes][STATS_FILE_OUTPUT OBJ OBJ ]  [STATS_FILE_NAME stats.txt][ADD_PBINSTANCE_TO_STATS_FILE yes]
```


### LaTeX outputs

This step requires to have a LaTeX distribution installed on your machine.

To obtained pdf files from tex file:
```bash
pdflatex dp1.tex
```

## Tests

Unit tests for the Algorithm, Output, Problem and Runner classes are available. To build them, when configuring we must enable it:
```bash
    cmake -DBUILD_TESTS=ON -S . -B build/release
```
Once the project is built, tests are performed in the build/release directory:
```bash
    ctest
```


## Contributing

We welcome contributions! Please read our [contributing guidelines](CONTRIBUTING.md) for more information.


## License

This project is licensed under the GPLv3 License. See the [LICENSE](LICENSE) file for details.

## References

[1] Technical paper on RunnerPost. Available at: https://www.gerad.ca/en/papers/G-2025-36

## Contact

For any questions or feedback, please contact us at nomad@gerad.ca.
