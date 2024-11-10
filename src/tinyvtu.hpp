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
		struct Info {
			bool active = true;
			std::uint32_t blockSize = 32768;
			std::int32_t level = 2;
		};

		static constexpr Info none = {false, 0, 0};
		static constexpr Info zlib = {true, 32768, 2};
	}

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

		void write(const std::filesystem::path &file_path) const;

	private:
		std::unique_ptr<internal::GridData> grid_data_;
	};

	UnstructuredGrid createGrid(const std::vector<std::array<float, 3> > &points, CellType cellType,
	                            std::vector<std::vector<std::int32_t> > &cells,
	                            compression::Info compression = compression::zlib);


	void write(const UnstructuredGrid &grid, const std::filesystem::path &file_path);
}
