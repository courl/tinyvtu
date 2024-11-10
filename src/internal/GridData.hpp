#pragma once
#include <utility>
#include <filesystem>

#include "internal/DataBlock.hpp"

namespace tinyvtu::internal {
    class GridData final {
    public:
        GridData(std::uint64_t point_count, DataBlock &&points, std::uint64_t cell_count, DataBlock &&cell_connectivity,
                 DataBlock &&cell_offsets, DataBlock &&cell_types, compression::Info const &compression)
            : point_count_(point_count), cell_count_(cell_count), points_(std::move(points)),
              cell_connectivity_(std::move(cell_connectivity)), cell_offsets_(std::move(cell_offsets)),
              cell_types_(std::move(cell_types)),
              compression_(compression) {
        }

        template<bool is_cell_data, std::integral T>
        void addData(std::string const &name, std::vector<T> const &data, const std::uint32_t number_of_components) {
            if constexpr (is_cell_data) {
                cell_data_.emplace_back(createBlock(name, data, number_of_components, compression_));
            } else {
                point_data_.emplace_back(createBlock(name, data, number_of_components, compression_));
            }
        }

        template<bool is_cell_data, std::floating_point T>
        void addData(std::string const &name, std::vector<T> const &data, const std::uint32_t number_of_components) {
            if constexpr (is_cell_data) {
                cell_data_.emplace_back(createBlock(name, data, number_of_components, compression_));
            } else {
                point_data_.emplace_back(createBlock(name, data, number_of_components, compression_));
            }
        }

        void write(const std::filesystem::path &file_path) const;

    private:
        std::uint32_t point_count_ = 0;
        std::uint32_t cell_count_ = 0;
        DataBlock points_;
        DataBlock cell_connectivity_;
        DataBlock cell_offsets_;
        DataBlock cell_types_;
        std::vector<DataBlock> point_data_;
        std::vector<DataBlock> cell_data_;

        compression::Info compression_;
    };
}
