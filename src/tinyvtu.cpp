#include "tinyvtu.hpp"

#include <ranges>

#include "internal/GridData.hpp"

namespace tinyvtu {
UnstructuredGrid::UnstructuredGrid(std::unique_ptr<internal::GridData> &&gridData) : grid_data_(std::move(gridData)) {}

UnstructuredGrid::~UnstructuredGrid() = default;

template <class T>
requires std::is_arithmetic_v<T>
void UnstructuredGrid::addPointData(const std::string &name, const std::vector<T> &pointData,
                                    std::uint32_t numberOfComponents)
{
    grid_data_->addData<false, T>(name, pointData, numberOfComponents);
}

template <class T>
requires std::is_arithmetic_v<T>
void UnstructuredGrid::addCellData(const std::string &name, const std::vector<T> &pointData,
                                   std::uint32_t numberOfComponents)
{
    grid_data_->addData<true, T>(name, pointData, numberOfComponents);
}

template void UnstructuredGrid::addPointData<float>(const std::string &, const std::vector<float> &, std::uint32_t);
template void UnstructuredGrid::addPointData<double>(const std::string &, const std::vector<double> &, std::uint32_t);
template void UnstructuredGrid::addPointData<std::uint8_t>(const std::string &, const std::vector<std::uint8_t> &,
                                                           std::uint32_t);
template void UnstructuredGrid::addPointData<std::int8_t>(const std::string &, const std::vector<std::int8_t> &,
                                                          std::uint32_t);
template void UnstructuredGrid::addPointData<std::uint16_t>(const std::string &, const std::vector<std::uint16_t> &,
                                                            std::uint32_t);
template void UnstructuredGrid::addPointData<std::int16_t>(const std::string &, const std::vector<std::int16_t> &,
                                                           std::uint32_t);
template void UnstructuredGrid::addPointData<std::uint32_t>(const std::string &, const std::vector<std::uint32_t> &,
                                                            std::uint32_t);
template void UnstructuredGrid::addPointData<std::int32_t>(const std::string &, const std::vector<std::int32_t> &,
                                                           std::uint32_t);
template void UnstructuredGrid::addPointData<std::uint64_t>(const std::string &, const std::vector<std::uint64_t> &,
                                                            std::uint32_t);
template void UnstructuredGrid::addPointData<std::int64_t>(const std::string &, const std::vector<std::int64_t> &,
                                                           std::uint32_t);

template void UnstructuredGrid::addCellData<float>(const std::string &, const std::vector<float> &, std::uint32_t);
template void UnstructuredGrid::addCellData<double>(const std::string &, const std::vector<double> &, std::uint32_t);
template void UnstructuredGrid::addCellData<std::uint8_t>(const std::string &, const std::vector<std::uint8_t> &,
                                                          std::uint32_t);
template void UnstructuredGrid::addCellData<std::int8_t>(const std::string &, const std::vector<std::int8_t> &,
                                                         std::uint32_t);
template void UnstructuredGrid::addCellData<std::uint16_t>(const std::string &, const std::vector<std::uint16_t> &,
                                                           std::uint32_t);
template void UnstructuredGrid::addCellData<std::int16_t>(const std::string &, const std::vector<std::int16_t> &,
                                                          std::uint32_t);
template void UnstructuredGrid::addCellData<std::uint32_t>(const std::string &, const std::vector<std::uint32_t> &,
                                                           std::uint32_t);
template void UnstructuredGrid::addCellData<std::int32_t>(const std::string &, const std::vector<std::int32_t> &,
                                                          std::uint32_t);
template void UnstructuredGrid::addCellData<std::uint64_t>(const std::string &, const std::vector<std::uint64_t> &,
                                                           std::uint32_t);
template void UnstructuredGrid::addCellData<std::int64_t>(const std::string &, const std::vector<std::int64_t> &,
                                                          std::uint32_t);

void UnstructuredGrid::write(const std::filesystem::path &file_path) const { grid_data_->write(file_path); }

UnstructuredGrid createGrid(const std::vector<std::array<float, 3> > &points, CellType cellType,
                            const std::vector<std::vector<std::int32_t> > &cells, compression::Info compression)
{
    if (points.size() > std::numeric_limits<std::uint32_t>::max() ||
        cells.size() > std::numeric_limits<std::uint32_t>::max())
    {
        throw std::invalid_argument("Unsupported number of points or cells");
    }
    std::vector<float> pointsVector;
    pointsVector.reserve(points.size() * 3);
    for (const auto &point : points)
    {
        pointsVector.insert(pointsVector.end(), point.begin(), point.end());
    }

    const auto cellCount = static_cast<std::uint32_t>(cells.size());

    std::uint32_t connectivitySize = 0;
    for (const auto &cell : cells) connectivitySize += static_cast<std::uint32_t>(cell.size());

    std::vector<std::int32_t> connectivity;
    connectivity.resize(connectivitySize);

    std::vector<std::int32_t> offsets;
    offsets.resize(cellCount);

    std::vector<std::uint8_t> types;
    types.resize(cellCount);

    std::int32_t connectivityOffset = 0;
    for (auto i = 0ull; i < cells.size(); ++i)
    {
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
        static_cast<std::uint32_t>(points.size()), std::move(pointBlock), cellCount, std::move(connectivityBlock),
        std::move(offsetsBlock), std::move(typesBlock), compression));
}

void write(const UnstructuredGrid &grid, const std::filesystem::path &file_path) { grid.write(file_path); }
}  // namespace tinyvtu
