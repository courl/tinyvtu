#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "internal/DataBlock.hpp"
#include "tinyvtu.hpp"

using namespace tinyvtu::internal;

TEST_CASE("DataBlock creation with various numeric types", "[DataBlock]")
{
    SECTION("Create DataBlock with UInt8 type")
    {
        std::vector<std::uint8_t> data = {1, 2, 3, 4};
        std::string name = "UInt8Data";
        std::uint32_t numComponents = 2;

        DataBlock dataBlock = createBlock<std::uint8_t>(name, data, numComponents, tinyvtu::compression::none);

        REQUIRE(dataBlock.type == DataBlock::UInt8);
        REQUIRE(dataBlock.name == name);
        REQUIRE(dataBlock.number_of_components == numComponents);
        REQUIRE(dataBlock.block.size() == 8);
        // First 32 bit must be size (for uncompressed data)
        REQUIRE(dataBlock.block[0] == data.size());
        REQUIRE(dataBlock.block[1] == 0);
        REQUIRE(dataBlock.block[2] == 0);
        REQUIRE(dataBlock.block[3] == 0);
        // Next comes the data
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            REQUIRE(dataBlock.block[4 + i] == data[i]);
        }
    }

    SECTION("Create DataBlock with Int16 type")
    {
        std::vector<std::int16_t> data = {-100, 200, -300, 400};
        std::string name = "Int16Data";
        std::uint32_t numComponents = 2;

        DataBlock dataBlock = createBlock<std::int16_t>(name, data, numComponents, tinyvtu::compression::none);

        REQUIRE(dataBlock.type == DataBlock::Int16);
        REQUIRE(dataBlock.name == name);
        REQUIRE(dataBlock.number_of_components == numComponents);
        REQUIRE(dataBlock.block.size() == 4 /* block size is stored in 32 bit */ + data.size() * sizeof(std::int16_t));

        REQUIRE(dataBlock.block[0] == data.size() * sizeof(std::int16_t));
        REQUIRE(dataBlock.block[1] == 0);
        REQUIRE(dataBlock.block[2] == 0);
        REQUIRE(dataBlock.block[3] == 0);

        // Verify actual data bytes
        const std::uint8_t* dataPtr = dataBlock.block.data() + 4;
        const auto* int16Ptr = reinterpret_cast<const std::int16_t*>(dataPtr);
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            REQUIRE(int16Ptr[i] == data[i]);
        }
    }

    SECTION("Create DataBlock with Float32 type")
    {
        std::vector<float> data = {1.5f, 2.5f, 3.5f};
        std::string name = "Float32Data";
        std::uint32_t numComponents = 1;

        DataBlock dataBlock = createBlock<float>(name, data, numComponents, tinyvtu::compression::none);

        REQUIRE(dataBlock.type == DataBlock::Float32);
        REQUIRE(dataBlock.name == name);
        REQUIRE(dataBlock.number_of_components == numComponents);
        REQUIRE(dataBlock.block.size() == 4 /* block size is stored in 32 bit */ + data.size() * sizeof(float));
    }
    SECTION("Float32 special values")
    {
        std::vector<float> special_data = {std::numeric_limits<float>::infinity(),
                                           -std::numeric_limits<float>::infinity(),
                                           std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::min(),
                                           std::numeric_limits<float>::max()};

        DataBlock dataBlock = createBlock<float>("SpecialFloat32", special_data, 1, tinyvtu::compression::none);

        // Verify data bytes
        const std::uint8_t* dataPtr = dataBlock.block.data() + 4;
        const auto* floatPtr = reinterpret_cast<const float*>(dataPtr);

        REQUIRE(std::isinf(floatPtr[0]));
        REQUIRE(floatPtr[0] > 0);
        REQUIRE(std::isinf(floatPtr[1]));
        REQUIRE(floatPtr[1] < 0);
        REQUIRE(std::isnan(floatPtr[2]));
        REQUIRE(floatPtr[3] == std::numeric_limits<float>::min());
        REQUIRE(floatPtr[4] == std::numeric_limits<float>::max());
    }

    SECTION("Invalid number of components throws exception")
    {
        std::vector<int> data = {1, 2, 3};
        std::string name = "InvalidComponents";
        std::uint32_t numComponents = 2;

        REQUIRE_THROWS_AS(createBlock<int>(name, data, numComponents, tinyvtu::compression::none),
                          std::invalid_argument);
    }
}
