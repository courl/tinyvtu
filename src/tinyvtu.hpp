#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace tinyvtu::internal {
class GridData;
}

namespace tinyvtu::compression {
/**
 * @brief Represents compression information.
 *
 * This structure holds the essential information for managing data compression.
 */
struct Info
{
    /**
     * @brief Indicates the activation state of compression.
     *
     * This boolean flag determines whether compression is active or not.
     */
    bool active = true;

    /**
     * @brief Defines the size of a block for compression.
     *
     * This variable determines the size, in bytes, of each block used during the compression process.
     */
    std::uint32_t blockSize = 32768;

    /**
     * @brief Specifies the compression level.
     *
     * This integer value controls the level of compression to be applied, where higher values typically result
     * in better compression ratio but might require more processing time.
     * Valid range: 0-9, where 0=fastest, 9=best compression
     */
    std::int32_t level = 2;
};

/**
 * @brief Disables compression.
 *
 * This constant provides an Info object that indicates no compression is applied.
 */
static constexpr Info none = {false, 0, 0};

/**
 * @brief Default compression.
 *
 * This constant provides an Info object that configures compression using zlib with a default block size of
 * 32KB and a compression level of 2.
 */
static constexpr Info zlib = {true, 32768, 2};
}  // namespace tinyvtu::compression

namespace tinyvtu {
/**
 * @brief Enumerates different cell types used in grid structures.
 *
 * Represents the various types of cells that can be used in a grid for data structures, enabling a wide range of
 * geometric representations. These must be consistent with the vtk definitions.
 * @see https://vtk.org/doc/nightly/html/vtkCellType_8h.html
 */
enum class CellType : std::uint8_t
{
    Vertex = 1,
    PolyVertex = 2,
    Line = 3,
    PolyLine = 4,
    Triangle = 5,
    TriangleStrip = 6,
    Polygon = 7,
    Pixel = 8,
    Quad = 9,
    Tetra = 10,
    Voxel = 11,
    Hexahedron = 12,
    Wedge = 13,
    Pyramid = 14
};

/**
 * @brief Represents an unstructured grid that can hold and manage various types of grid data.
 *
 * The UnstructuredGrid class is used for handling unstructured grid data, allowing users to add point and cell data
 * of different types and write the grid to a file.
 *
 * To create an UnstructuredGrid, use the @see tinyvtu::createGrid() function
 */
class UnstructuredGrid final
{
public:
    /**
     * @brief Constructs an UnstructuredGrid object.
     *
     * Initializes the UnstructuredGrid with the provided grid data, transferring ownership of the data. Use
     * `createGrid` instead of calling it directly
     *
     * @param gridData A unique pointer to the internal GridData object containing the grid's data.
     *                 The ownership of gridData is moved to the constructed UnstructuredGrid object.
     */
    explicit UnstructuredGrid(std::unique_ptr<internal::GridData> &&gridData);

    /**
     * @brief Destructor for the UnstructuredGrid class.
     *
     * Cleans up any resources associated with the UnstructuredGrid object.
     */
    ~UnstructuredGrid();

    /**
     * @brief Adds point data to the unstructured grid.
     *
     * This method allows adding point-associated data to the unstructured grid. The data is specified
     * by its name, the actual data points, and the number of components per point.
     *
     * @param name The name of the point data.
     * @param pointData A vector containing the data points.
     * @param numberOfComponents The number of components for each point.
     */
    template <class T>
    requires std::is_arithmetic_v<T>
    void addPointData(const std::string &name, const std::vector<T> &pointData, std::uint32_t numberOfComponents = 1);

    /**
     * @brief Adds cell data to the unstructured grid.
     *
     * This method allows adding data associated with the cells of the unstructured grid.
     * The data should be passed as a vector and the type of the data should be an arithmetic type.
     *
     * @tparam T The type of the data, must be an arithmetic type.
     * @param name The name associated with the cell data.
     * @param cellData A vector containing the cell data values.
     * @param numberOfComponents The number of components each cell data point has.
     */
    template <class T>
    requires std::is_arithmetic_v<T>
    void addCellData(const std::string &name, const std::vector<T> &cellData, std::uint32_t numberOfComponents = 1);

    /**
     * @brief Writes the unstructured grid to a file.
     *
     * This method writes the unstructured grid data to the specified file path in VTU (VTK Unstructured Grid)
     * format.
     *
     * @param file_path The file path where the grid data will be written.
     */
    void write(const std::filesystem::path &file_path) const;

private:
    std::unique_ptr<internal::GridData> grid_data_;
};

/**
 * @brief Creates an unstructured grid from given points, cell type, and cells.
 *
 * This function constructs an UnstructuredGrid object using the provided points, cell
 * type, and cells with specified compression information.
 *
 * @pre points vector must not be empty and must be smaller than 2^32-1 elements
 * @pre cells vector must not be empty and must be smaller than 2^32-1 elements
 * @pre All indices in cells must be valid indices into the points vector
 * @pre Number of vertices in each cell must match the requirements for the specified cell type
 *
 * @param points A vector containing arrays of 3D points.
 * @param cellType Enum representing the type of cell to create.
 * @param cells A vector of vectors containing cell connectivity information.
 * @param compression Structure holding the compression information.
 * @return An UnstructuredGrid object constructed from the provided data.
 * @throws std::invalid_argument if preconditions are not met
 */
UnstructuredGrid createGrid(const std::vector<std::array<float, 3> > &points, CellType cellType,
                            const std::vector<std::vector<std::int32_t> > &cells,
                            compression::Info compression = compression::zlib);

/**
 * @brief Writes an unstructured grid to a specified file path.
 *
 * This is a convenience wrapper around UnstructuredGrid::write that allows writing a grid without calling the
 * member function directly.
 *
 * @param grid The unstructured grid containing the data to be written.
 * @param file_path The file path where the grid data will be written.
 * @see UnstructuredGrid::write
 */
void write(const UnstructuredGrid &grid, const std::filesystem::path &file_path);
}  // namespace tinyvtu
