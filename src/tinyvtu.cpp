#include "tinyvtu.hpp"

#include <ranges>

#include "internal/GridData.hpp"

namespace tinyvtu {
	UnstructuredGrid::UnstructuredGrid(std::unique_ptr<internal::GridData> &&gridData)
		: grid_data_(std::move(gridData)) {
	}

	UnstructuredGrid::~UnstructuredGrid() = default;

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<float> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<double> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<std::int8_t> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<std::uint8_t> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<std::int16_t> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<std::uint16_t> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<std::int32_t> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<std::uint32_t> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<std::int64_t> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addPointData(const std::string &name, const std::vector<std::uint64_t> &pointData,
	                                    std::uint32_t numberOfComponents) {
		grid_data_->addData<false>(name, pointData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<float> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<double> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<std::int8_t> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<std::uint8_t> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<std::int16_t> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<std::uint16_t> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<std::int32_t> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<std::uint32_t> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<std::int64_t> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::addCellData(const std::string &name, const std::vector<std::uint64_t> &cellData,
	                                   std::uint32_t numberOfComponents) {
		grid_data_->addData<true>(name, cellData, numberOfComponents);
	}

	void UnstructuredGrid::write(const std::filesystem::path &file_path) const {
		grid_data_->write(file_path);
	}


	UnstructuredGrid createGrid(const std::vector<std::array<float, 3> > &points, CellType cellType,
	                            const std::vector<std::vector<std::int32_t> > &cells, compression::Info compression) {
		if (points.size() > std::numeric_limits<std::uint32_t>::max() || cells.size() > std::numeric_limits<
			    std::uint32_t>::max()) {
			throw std::invalid_argument("Unsupported number of points or cells");
		}
		std::vector<float> pointsVector;
		pointsVector.reserve(points.size() * 3);
		for (const auto &point: points) {
			pointsVector.insert(pointsVector.end(), point.begin(), point.end());
		}

		const auto cellCount = static_cast<std::uint32_t>(cells.size());

		std::uint32_t connectivitySize = 0;
		for (const auto &cell: cells)
			connectivitySize += static_cast<std::uint32_t>(cell.size());

		std::vector<std::int32_t> connectivity;
		connectivity.resize(connectivitySize);

		std::vector<std::int32_t> offsets;
		offsets.resize(cellCount);

		std::vector<std::uint8_t> types;
		types.resize(cellCount);

		std::int32_t connectivityOffset = 0;
		for (auto i = 0ull; i < cells.size(); ++i) {
			std::ranges::copy(cells[i], connectivity.begin() + connectivityOffset);
			connectivityOffset += static_cast<std::int32_t>(cells[i].size());

			offsets[i] = connectivityOffset;
			types[i] = static_cast<std::uint8_t>(cellType);
		}

		// We set numberOfComponents to zero, as these RawData have special meaning
		auto pointBlock = internal::createBlock("", pointsVector, 3, compression);
		auto connectivityBlock = internal::createBlock("connectivity", connectivity, 0, compression);
		auto offsetsBlock = internal::createBlock("offsets", offsets, 0, compression);
		auto typesBlock = internal::createBlock("types", types, 0, compression);
		return UnstructuredGrid(std::make_unique<internal::GridData>(
				static_cast<std::uint32_t>(points.size()), std::move(pointBlock), cellCount,
				std::move(connectivityBlock), std::move(offsetsBlock), std::move(typesBlock), compression)
		);
	}

	void write(const UnstructuredGrid &grid, const std::filesystem::path &file_path) {
		grid.write(file_path);
	}
}
