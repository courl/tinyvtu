# TinyVTU

Welcome to **TinyVTU**, a tiny yet powerful VTU file creation library written in modern C++20. This library allows for
efficient writing of [VTU (VTK Unstructured Grid)](https://docs.vtk.org/en/latest/design_documents/VTKFileFormats.html)
files, commonly used in scientific computing and data visualization. This format is especially useful to output
simulations of two- or three-dimensional finite element computations.

## Table of Contents

- [Why another library?](#why-another-library)
- [Features](#design-goals)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Why another library?

Contrary to the vtk library, this one is "tiny". This means minimal dependencies (only one, zlib) and easy integration.
It can generate both compressed and uncompressed binary data in vtk's XML file format for unstructured grids. Hence, the
benefits of data compression are handled while the created files can be analysed with mighty visualization software like
Paraview.

## Design goals

- **Lightweight and Fast**: Designed for efficiency and speed.
- **Modern C++20**: Utilizes the latest C++ features for optimal performance and readability.
- **Easy Integration**: Seamlessly integrate with your existing projects.
- **Extensible**: Designed with extensibility in mind, allowing easy additions of new features.

## Installation

### Prerequisites

- **C++20**: Ensure you have a compatible C++20 compiler.
- **CMake 3.29+**: Required for building the project.
- **Git**: For cloning the repository.
- **zlib**: TinyVTU's only dependency - an up to date zlib version

### Building from Source

1. **Clone the Repository**
   ```sh
   git clone https://github.com/yourusername/tinyvtu.git
   cd tinyvtu
   ```

2. **Create a Build Directory**
   ```sh
   mkdir build
   cd build
   ```

3. **Run CMake**
   ```sh
   cmake ..
   ```
   Note: you might need to add the paths to your zlib installation

4. **Build the Project**
   ```sh
   cmake --build .
   ```

### Installation

After successfully building the project, you can install it using:

```sh
cmake --install .
```

### Integration with Your Project

To use TinyVTU in your project, add the following lines to your `CMakeLists.txt`:

```cmake
find_package(TinyVTU REQUIRED)
target_link_libraries(your_project PRIVATE TinyVTU::TinyVTU)
```

## Usage

### Basic Example

Here's a basic example of how to use TinyVTU:

```cpp
#include "tinyvtu.h"
#include <cmath>

int main() {
    static std::vector<std::array<float, 3> > points{
        {0., 0., 0.}, {1., 0., 0.}, {0., 1., 0.}, {0., 0., 1.},
        {1., 0., 1.}, {1., 1., 0.}, {0., 1., 1.}, {1., 1., 1.}
    };
    static std::vector<std::vector<std::int32_t> > cells{
        {0, 1, 5, 7}, {0, 1, 7, 4}, {5, 7, 6, 0}, {5, 0, 6, 2}, {0, 7, 3, 6}, {7, 3, 0, 4}
    };
    auto grid = tinyvtu::createGrid(points, tinyvtu::CellType::Tetra, cells);

    grid.addPointData("u", std::vector<float>{
                          0.f, std::sqrt(1.f), std::sqrt(1.f), std::sqrt(1.f), std::sqrt(2.f), std::sqrt(2.f),
                          std::sqrt(2.f), std::sqrt(3.f)
                      });
    grid.addCellData("number", std::vector<std::uint8_t>{0, 1, 2, 3, 4, 5});

    grid.write("out.vtu");

    return 0;
}
```

## Contributing

We welcome contributions to the TinyVTU project. If you have an idea for an enhancement or have found a bug, please open
an issue or a pull request.

### Steps to Contribute:

1. Fork the repository.
2. Create a new branch: `git checkout -b feature-branch`
3. Commit your changes: `git commit -m 'Add some feature'`
4. Push to the branch: `git push origin feature-branch`
5. Open a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

Feel free to reach out if you have any questions or need further assistance!