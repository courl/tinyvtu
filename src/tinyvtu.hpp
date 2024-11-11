#pragma once

#include <cstdint>
#include <filesystem>
#include <array>
#include <string>
#include <vector>
#include <memory>

namespace tinyvtu {
	namespace internal {
		class GridData;
	}

	namespace compression {
		/**
		 * @brief Represents compression information.
		 *
		 * This structure holds the essential information for managing data compression.
		 */
		struct Info {
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
	}

	/**
	 * @brief Enumerates different cell types used in grid structures.
	 *
	 * Represents the various types of cells that can be used in a grid for data structures, enabling a wide range of
	 * geometric representations. These must be consistent with the vtk definitions.
	 */
	enum class CellType : std::uint8_t {
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
	 * To create an UnstructuredGrid, use the @see createGrid function
	 */
	class UnstructuredGrid final {
	public:
		explicit UnstructuredGrid(std::unique_ptr<internal::GridData> &&gridData);

		~UnstructuredGrid();

		void addPointData(const std::string &name, const std::vector<float> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<double> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<std::int8_t> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<std::uint8_t> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<std::int16_t> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<std::uint16_t> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<std::int32_t> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<std::uint32_t> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<std::int64_t> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addPointData(const std::string &name, const std::vector<std::uint64_t> &pointData,
		                  std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<float> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<double> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<std::int8_t> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<std::uint8_t> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<std::int16_t> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<std::uint16_t> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<std::int32_t> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<std::uint32_t> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<std::int64_t> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		void addCellData(const std::string &name, const std::vector<std::uint64_t> &cellData,
		                 std::uint32_t numberOfComponents = 1);

		/**
		 * @brief Writes the unstructured grid to a file.
		 *
		 * This method writes the unstructured grid data to the specified file path in the appropriate format.
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
	 * @param points A vector containing arrays of 3D points.
	 * @param cellType Enum representing the type of cell to create.
	 * @param cells A vector of vectors containing cell connectivity information.
	 * @param compression Structure holding the compression information.
	 * @return An UnstructuredGrid object constructed from the provided data.
	 */
	UnstructuredGrid createGrid(const std::vector<std::array<float, 3> > &points, CellType cellType,
	                            const std::vector<std::vector<std::int32_t> > &cells,
	                            compression::Info compression = compression::zlib);


	/**
	 * @brief Writes an unstructured grid to a specified file path.
	 *
	 * This function writes the data contained in the provided UnstructuredGrid object
	 * to a file specified by the given file path.
	 *
	 * @param grid The unstructured grid containing the data to be written.
	 * @param file_path The file path where the grid data will be written.
	 */
	void write(const UnstructuredGrid &grid, const std::filesystem::path &file_path);
}
