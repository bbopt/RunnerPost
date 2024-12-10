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
Summary

RunnerPost uses selection files to select problems, algorithms, and outputs for post-processing and profiling optimization results.
Each file follows a consistent structure where each line represents a selection with a description and a list of parameter-value pairs enclosed in square brackets. This format allows for easy parsing and processing of the configuration data.

problem_selection: Defines problems with an ID, a name and description, and some parameters.
algo_selection: Defines algorithms with an ID, a name and description, and parameters.
output_selection: Defines output profiles with a type, a description, and parameters.

Problem selection file Syntax
Each line in the problem_selection file selects an optimization problem with the following format:

<id> (<description>) [<parameter1> <value1>] [<parameter2> <value2>] ... [<parameterN> <valueN>]

<id>: A unique identifier for the problem (e.g., 1, 2, 3).
<description>: A brief description of the problem enclosed in parentheses (e.g., (Pb1), (Pb2)). 
[<parameter> <value>]: A list of parameters and their corresponding values enclosed in square brackets. 


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
