# RunnerPost
RunnerPost is a C++ and Python interface for post-processing and profiling optimization results.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Development](#development)
- [Tests](#tests)
- [Contributing](#contributing)
- [License](#license)


## Installation

To install RunnerPost, follow these steps:

### From PyPI

To install the Python package from PyPI, ensure you have Python 3.8 or higher:

```sh
pip install RunnerPost

### From Source

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/RunnerPost.git
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
    pip install --user --force-reinstall dist/*whl

## Usage

### Python

```python
import RunnerPost

# Example usage
RunnerPost.info()
```

### Command line


To start using RunnerPost, run the following command:
```bash
npm start
```

## Contributing

We welcome contributions! Please read our [contributing guidelines](CONTRIBUTING.md) for more information.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or feedback, please contact us at support@runnerpost.com.