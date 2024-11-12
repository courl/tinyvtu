#include <array>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <filesystem>
#include <vector>

#include "tinyvtu.hpp"

using namespace tinyvtu;

TEST_CASE("UnstructuredGrid Constructor and Write", "[UnstructuredGrid]")
{
    // Prepare sample data
    std::vector<std::array<float, 3>> points = {
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};

    CellType cellType = CellType::Quad;

    std::vector<std::vector<std::int32_t>> cells = {{0, 1, 2, 3}};

    SECTION("Create UnstructuredGrid and Write to File")
    {
        REQUIRE_NOTHROW(
            [&points, &cellType, &cells]
            {
                UnstructuredGrid grid = createGrid(points, cellType, cells);
                // Optionally, add point data or cell data
                grid.addPointData("PointScalarData", std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f});
                grid.addCellData("CellScalarData", std::vector<int>{42});

                // Write to file
                std::filesystem::path filePath = "test_unstructured_grid.vtu";
                grid.write(filePath);

                // Check if file was created
                REQUIRE(std::filesystem::exists(filePath));

                // Clean up
                std::filesystem::remove(filePath);
            }());
    }
}

TEST_CASE("createGrid Function", "[createGrid]")
{
    std::vector<std::array<float, 3>> points;
    CellType cellType = CellType::Triangle;
    std::vector<std::vector<std::int32_t>> cells;

    SECTION("Empty Points and Cells do not throw an exception")
    {
        REQUIRE_NOTHROW(createGrid(points, cellType, cells));
    }

    SECTION("Valid Points and Cells")
    {
        points = {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
        cells = {{0, 1, 2}};
        REQUIRE_NOTHROW(createGrid(points, cellType, cells));
    }
}

TEST_CASE("Compression Info Defaults", "[compression]")
{
    using namespace tinyvtu::compression;

    SECTION("Default Compression Info")
    {
        Info defaultCompression;
        REQUIRE(defaultCompression.active == true);
        REQUIRE(defaultCompression.blockSize == 32768);
        REQUIRE(defaultCompression.level == 2);
    }

    SECTION("No Compression")
    {
        REQUIRE(none.active == false);
        REQUIRE(none.blockSize == 0);
        REQUIRE(none.level == 0);
    }

    SECTION("Zlib Compression")
    {
        REQUIRE(zlib.active == true);
        REQUIRE(zlib.blockSize == 32768);
        REQUIRE(zlib.level == 2);
    }
}
