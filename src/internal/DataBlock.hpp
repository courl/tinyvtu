#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "tinyvtu.hpp"

namespace tinyvtu::internal {
    using Data = std::vector<std::uint8_t>;

    struct DataBlock {
        enum Type {
            Float32,
            Float64,
            Int8,
            UInt8,
            Int16,
            UInt16,
            Int32,
            UInt32,
            Int64,
            UInt64,
        };

        Type type;
        std::string name;
        std::uint32_t number_of_components;
        Data block;
    };

    /**
     * Compresses the passed data if the compression info allows for it.
     *
     * @param source       Pointer to the source data
     * @param size         The size of the data
     * @param compression  The compression information
     * @return             The compressed data
     */
    Data compressData(const std::uint8_t *source, std::uint64_t size, const compression::Info &compression);

    template<std::integral T>
    DataBlock createBlock(const std::string &name, const std::vector<T> &data, std::uint32_t number_of_components,
                          const compression::Info &compression) {
        const auto compressedData = compressData(reinterpret_cast<const std::uint8_t *>(data.data()),
                                                 data.size() * sizeof(T), compression);
        if constexpr (sizeof(T) == sizeof(std::uint8_t) && std::is_unsigned_v<T>)
            return {DataBlock::UInt8, name, number_of_components, compressedData};
        else if constexpr (sizeof(T) == sizeof(std::uint8_t))
            return {DataBlock::Int8, name, number_of_components, compressedData};
        else if constexpr (sizeof(T) == sizeof(std::uint16_t) && std::is_unsigned_v<T>)
            return {DataBlock::UInt16, name, number_of_components, compressedData};
        else if constexpr (sizeof(T) == sizeof(std::uint16_t))
            return {DataBlock::Int16, name, number_of_components, compressedData};
        else if constexpr (sizeof(T) == sizeof(std::uint32_t) && std::is_unsigned_v<T>)
            return {DataBlock::UInt32, name, number_of_components, compressedData};
        else if constexpr (sizeof(T) == sizeof(std::uint32_t))
            return {DataBlock::Int32, name, number_of_components, compressedData};
        else if constexpr (sizeof(T) == sizeof(std::uint64_t) && std::is_unsigned_v<T>)
            return {DataBlock::UInt64, name, number_of_components, compressedData};
        else if constexpr (sizeof(T) == sizeof(std::uint64_t))
            return {DataBlock::Int64, name, number_of_components, compressedData};
        else
            static_assert(std::is_integral_v<T>, "Unsupported data type");
        return {};
    }

    template<std::floating_point T>
    DataBlock createBlock(const std::string &name, const std::vector<T> &data, std::uint32_t number_of_components,
                          const compression::Info &compression) {
        const auto compressedData = compressData(reinterpret_cast<const std::uint8_t *>(data.data()),
                                                 data.size() * sizeof(T), compression);
        if constexpr (sizeof(T) == sizeof(float))
            return {DataBlock::Float32, name, number_of_components, compressedData};
        else if constexpr (sizeof(T) == sizeof(double))
            return {DataBlock::Float64, name, number_of_components, compressedData};
        else
            static_assert(std::is_floating_point_v<T>, "Unsupported data type");
        return {};
    }
}
