# RunnerPost
RunnerPost is a C++ and Python interface for post-processing and profiling optimization results.

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

To install the Python package from TestPyPI, ensure you have Python 3.8 or higher:

```sh
pip install RunnerPost
```

### From Source

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

### Python

```python
import RunnerPost

# Example to get usage
RunnerPost.info()

# Produce data profile from provided example
# And selection file (in the example dir)
RunnerPost.run("algo_selection", "problem_selection", "output_selection")

```

TODO: give an example with selection parameters and run.

### Command line

To start using RunnerPost on the given example, run the following commands:
```bash
cd example/
../build/release/bin/runnerPost algo_selection problem_selection output_selection
```

### Syntax of selection files

#### Summary

RunnerPost uses problems, algorithms, and outputs selection files for post-processing and profiling optimization results.
Each file follows a consistent structure where each line represents a selection with a description and a list of parameter-value pairs enclosed in square brackets. This format allows for easy editing, parsing and processing of the post-processing data.

algo_selection: Defines algorithms with an ID, a name and description, and parameters.
problem_selection: Defines problems with an ID, a name and description, and some parameters.
output_selection: Defines outputs with a type, a description, and parameters.

#### Algorithm selection file syntax
Each line in the algo_selection file selects an algorithm with its configuration in the following format:

<id> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]

- `<id>`: A unique identifier for the algorithm (e.g., Algo_1, Algo_2).
- `<description>`: A brief description of the algorithm enclosed in parentheses.
- `[<parameter> <value>]`: A list of parameters and their corresponding values enclosed in square brackets.

The <id> and <description> fields are mandatory, the rest are optional.

Example of algo_selection file:
```
Algo_1 (DAMON X: Default) [STATS_FILE_NAME history.txt][STATS_FILE_OUTPUT SOL OBJ CST] [ADD_PBINSTANCE_TO_STATS_FILE yes]
Algo_2 (DAMON Y: Default + 2n dirs) [DISPLAY_ALL_EVAL yes] [STATS_FILE_NAME stats.txt] [ADD_PBINSTANCE_TO_STATS_FILE yes]
````

#### Problem selection file syntax
Each line in the problem_selection file selects a problem with its configuration in the following format:

<id> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]

<id>: A unique identifier for the problem (e.g., 1, 2, 3).
<description>: A brief description of the problem enclosed in parentheses (e.g., (Pb1), (Pb2)).
[<parameter> <value>]: A list of parameters and their corresponding values enclosed in square brackets.

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

<type> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]

<type>: The type of profile or output (e.g., DATA_PROFILE).
<description>: A brief description of the profile or output enclosed in parentheses (e.g., (Data profile on 10 pbs with $\tau\; 10^{-1}$)). This information is used as the title of the output.
[<parameter> <value>]: A list of parameters and their corresponding values enclosed in square brackets.

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
TODO

## Contributing

We welcome contributions! Please read our [contributing guidelines](CONTRIBUTING.md) for more information.


## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or feedback, please contact us at nomad@gerad.ca.
