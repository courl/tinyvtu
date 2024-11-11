#pragma once
#include <utility>
#include <filesystem>

#include "internal/DataBlock.hpp"

namespace tinyvtu::internal {
    class GridData final {
    public:
        /**
         * Constructs a GridData object.
         *
         * @param point_count The number of points in the grid.
         * @param points A DataBlock containing the points' coordinates.
         * @param cell_count The number of cells in the grid.
         * @param cell_connectivity A DataBlock containing the cell connectivity information.
         * @param cell_offsets A DataBlock containing the offsets to the start of each cell in the connectivity array.
         * @param cell_types A DataBlock containing the types of the cells.
         * @param compression Compression settings to be applied on the grid data.
         */
        GridData(std::uint32_t point_count, DataBlock &&points, std::uint32_t cell_count, DataBlock &&cell_connectivity,
                 DataBlock &&cell_offsets, DataBlock &&cell_types, compression::Info const &compression) noexcept
            : point_count_(point_count), cell_count_(cell_count), points_(std::move(points)),
              cell_connectivity_(std::move(cell_connectivity)), cell_offsets_(std::move(cell_offsets)),
              cell_types_(std::move(cell_types)), compression_(compression) {
        }

        /**
         * Adds data to the grid, either as cell data or point data.
         *
         * @tparam is_cell_data Boolean indicating whether the data being added is for cells (true) or points (false).
         * @tparam T Type of the data elements, which must be an arithmetic type.
         * @param name The name associated with the data block.
         * @param data A vector containing the data elements.
         * @param number_of_components The number of components per data element.
         */
        template<bool is_cell_data, typename T> requires NumericType<T>
        void addData(std::string const &name, std::vector<T> const &data, const std::uint32_t number_of_components) {
            const auto expected_size = (is_cell_data ? cell_count_ : point_count_) * number_of_components;
            if (data.size() != expected_size) {
                throw std::invalid_argument("Data size mismatch");
            }
            if constexpr (is_cell_data) {
                cell_data_.emplace_back(createBlock(name, data, number_of_components, compression_));
            } else {
                point_data_.emplace_back(createBlock(name, data, number_of_components, compression_));
            }
        }

        /**
         * Writes the GridData to a file specified by the given file path.
         *
         * @param file_path The file path where the GridData will be written.
         */
        void write(const std::filesystem::path &file_path) const;

    private:
        const std::uint32_t point_count_ = 0;
        const std::uint32_t cell_count_ = 0;
        const DataBlock points_;
        const DataBlock cell_connectivity_;
        const DataBlock cell_offsets_;
        const DataBlock cell_types_;
        std::vector<DataBlock> point_data_;
        std::vector<DataBlock> cell_data_;

        const compression::Info compression_;
    };
}
