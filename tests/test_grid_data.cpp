#include <catch2/catch_test_macros.hpp>
#include <fstream>

#include "internal/DataBlock.hpp"
#include "internal/GridData.hpp"

using namespace tinyvtu::internal;

TEST_CASE("GridData Constructor", "[GridData]")
{
    // Mock data for the constructor
    std::uint32_t point_count = 4;
    std::vector<double> points_data = {
        0.0, 0.0, 0.0,  // Point 1
        1.0, 0.0, 0.0,  // Point 2
        1.0, 1.0, 0.0,  // Point 3
        0.0, 1.0, 0.0   // Point 4
    };
    DataBlock points = createBlock("Points", points_data, 3, tinyvtu::compression::none);

    std::uint32_t cell_count = 1;
    std::vector<std::uint32_t> connectivity = {0, 1, 2, 3};
    DataBlock cell_connectivity = createBlock("Connectivity", connectivity, 1, tinyvtu::compression::none);

    std::vector<std::uint32_t> offsets = {4};
    DataBlock cell_offsets = createBlock("Offsets", offsets, 1, tinyvtu::compression::none);

    std::vector<std::uint8_t> cell_types = {9};  // VTK_QUAD = 9
    DataBlock cell_types_block = createBlock("Types", cell_types, 1, tinyvtu::compression::none);

    SECTION("Construct GridData")
    {
        REQUIRE_NOTHROW(GridData(point_count, std::move(points), cell_count, std::move(cell_connectivity),
                                 std::move(cell_offsets), std::move(cell_types_block), tinyvtu::compression::none));
    }
}

TEST_CASE("GridData addData", "[GridData]")
{
    // Prepare GridData object
    std::uint32_t point_count = 4;
    std::vector<double> points_data = {
        0.0, 0.0, 0.0,  // Point 1
        1.0, 0.0, 0.0,  // Point 2
        1.0, 1.0, 0.0,  // Point 3
        0.0, 1.0, 0.0   // Point 4
    };
    DataBlock points = createBlock("Points", points_data, 3, tinyvtu::compression::none);

    std::uint32_t cell_count = 1;
    std::vector<std::uint32_t> connectivity = {0, 1, 2, 3};
    DataBlock cell_connectivity = createBlock("Connectivity", connectivity, 1, tinyvtu::compression::none);

    std::vector<std::uint32_t> offsets = {4};
    DataBlock cell_offsets = createBlock("Offsets", offsets, 1, tinyvtu::compression::none);

    std::vector<std::uint8_t> cell_types = {9};  // VTK_QUAD = 9
    DataBlock cell_types_block = createBlock("Types", cell_types, 1, tinyvtu::compression::none);

    GridData grid_data(point_count, std::move(points), cell_count, std::move(cell_connectivity),
                       std::move(cell_offsets), std::move(cell_types_block), tinyvtu::compression::none);

    SECTION("Add Point Data with Correct Size")
    {
        std::vector<float> point_scalars = {1.0f, 2.0f, 3.0f, 4.0f};
        REQUIRE_NOTHROW(grid_data.addData<false>("PointScalars", point_scalars, 1));
    }

    SECTION("Add Point Data with Incorrect Size")
    {
        std::vector<float> point_scalars = {1.0f, 2.0f, 3.0f};  // Missing one element
        REQUIRE_THROWS_AS(grid_data.addData<false>("PointScalars", point_scalars, 1), std::invalid_argument);
    }

    SECTION("Add Cell Data with Correct Size")
    {
        std::vector<int> cell_scalars = {42};
        REQUIRE_NOTHROW(grid_data.addData<true>("CellScalars", cell_scalars, 1));
    }

    SECTION("Add Cell Data with Incorrect Size")
    {
        std::vector<int> cell_scalars = {42, 24};  // Extra element
        REQUIRE_THROWS_AS(grid_data.addData<true>("CellScalars", cell_scalars, 1), std::invalid_argument);
    }
}

TEST_CASE("GridData write", "[GridData]")
{
    // Prepare GridData object
    std::uint32_t point_count = 4;
    std::vector<double> points_data = {
        0.0, 0.0, 0.0,  // Point 1
        1.0, 0.0, 0.0,  // Point 2
        1.0, 1.0, 0.0,  // Point 3
        0.0, 1.0, 0.0   // Point 4
    };
    DataBlock points = createBlock("Points", points_data, 3, tinyvtu::compression::none);

    std::uint32_t cell_count = 1;
    std::vector<std::uint32_t> connectivity = {0, 1, 2, 3};
    DataBlock cell_connectivity = createBlock("Connectivity", connectivity, 1, tinyvtu::compression::none);

    std::vector<std::uint32_t> offsets = {4};
    DataBlock cell_offsets = createBlock("Offsets", offsets, 1, tinyvtu::compression::none);

    std::vector<std::uint8_t> cell_types = {9};  // VTK_QUAD = 9
    DataBlock cell_types_block = createBlock("Types", cell_types, 1, tinyvtu::compression::none);

    GridData grid_data(point_count, std::move(points), cell_count, std::move(cell_connectivity),
                       std::move(cell_offsets), std::move(cell_types_block), tinyvtu::compression::none);

    // Add sample point data
    std::vector<float> point_scalars = {1.0f, 2.0f, 3.0f, 4.0f};
    grid_data.addData<false>("PointScalars", point_scalars, 1);

    // Add sample cell data
    std::vector<int> cell_scalars = {100};
    grid_data.addData<true>("CellScalars", cell_scalars, 1);

    SECTION("Write GridData to File")
    {
        std::filesystem::path file_path = "test_grid_data.vtu";
        REQUIRE_NOTHROW(grid_data.write(file_path));

        // Check if file exists
        REQUIRE(std::filesystem::exists(file_path));

        // Clean up
        std::filesystem::remove(file_path);
    }
    SECTION("Write GridData File Handling")
    {
        std::filesystem::path file_path = "test_grid_data.vtu";

        // Test writing to existing file
        {
            std::ofstream dummy(file_path);
            dummy.close();
            REQUIRE_NOTHROW(grid_data.write(file_path));
            std::filesystem::remove(file_path);
        }

        // Test invalid path (assuming write permissions)
        std::filesystem::path invalid_path = "/nonexistent/directory/test.vtu";
        REQUIRE_THROWS(grid_data.write(invalid_path));

        // Test file content (basic validation)
        grid_data.write(file_path);
        std::ifstream file(file_path);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        REQUIRE(content.find("<VTKFile") != std::string::npos);
        REQUIRE(content.find("PointScalars") != std::string::npos);
        REQUIRE(content.find("CellScalars") != std::string::npos);
        file.close();
        std::filesystem::remove(file_path);
    }
}
