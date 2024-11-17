#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "tinyvtu.hpp"

namespace tinyvtu::internal {
using Data = std::vector<std::uint8_t>;

struct DataBlock
{
    enum Type
    {
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

/**
 * Creates a DataBlock object from provided data, name, number of components, and compression info.
 *
 * @param name                  The name of the data block
 * @param data                  The data to be stored in the data block
 * @param number_of_components  The number of components per data entry (can be zero for internal data)
 * @param compression           The compression information
 * @return                      A DataBlock object containing the specified data
 * @throws std::invalid_argument if the data size is not divisible by the number of components
 */
template <typename T>
requires std::is_arithmetic_v<T>
DataBlock createBlock(const std::string &name, const std::vector<T> &data, std::uint32_t number_of_components,
                      const compression::Info &compression)
{
    if (number_of_components != 0 && (data.size() % number_of_components != 0))
    {
        throw std::invalid_argument("data size must be divisible by number_of_components");
    }
    const auto compressedData =
        compressData(reinterpret_cast<const std::uint8_t *>(data.data()), data.size() * sizeof(T), compression);
    if constexpr (sizeof(T) == sizeof(std::uint8_t) && std::is_integral_v<T> && std::is_unsigned_v<T>)
        return {DataBlock::UInt8, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(std::uint8_t) && std::is_integral_v<T>)
        return {DataBlock::Int8, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(std::uint16_t) && std::is_integral_v<T> && std::is_unsigned_v<T>)
        return {DataBlock::UInt16, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(std::uint16_t) && std::is_integral_v<T>)
        return {DataBlock::Int16, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(std::uint32_t) && std::is_integral_v<T> && std::is_unsigned_v<T>)
        return {DataBlock::UInt32, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(std::uint32_t) && std::is_integral_v<T>)
        return {DataBlock::Int32, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(std::uint64_t) && std::is_integral_v<T> && std::is_unsigned_v<T>)
        return {DataBlock::UInt64, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(std::uint64_t) && std::is_integral_v<T>)
        return {DataBlock::Int64, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(float) && std::is_floating_point_v<T>)
        return {DataBlock::Float32, name, number_of_components, compressedData};
    else if constexpr (sizeof(T) == sizeof(double) && std::is_floating_point_v<T>)
        return {DataBlock::Float64, name, number_of_components, compressedData};
    else
        static_assert(!sizeof(T), "Unsupported data type");
    return {};
}

}  // namespace tinyvtu::internal
