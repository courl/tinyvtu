cmake_minimum_required(VERSION 3.29)

set(TINYVTU_PROJECT_VERSION 0.0.1)
set(TINYVTU_WITH_TESTS "Build tests" CACHE BOOL ON)

project(TinyVTU VERSION ${TINYVTU_PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(ZLIB REQUIRED)

add_library(TinyVTU STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyvtu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/DataBlock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/XMLWriter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/DataBlock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/GridData.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/GridData.cpp)

target_include_directories(TinyVTU PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
target_link_libraries(TinyVTU PUBLIC ZLIB::ZLIB)

# Installation rules
include(GNUInstallDirs)

install(TARGETS TinyVTU
        EXPORT TinyVTU_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyvtu.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export the target for find_package
install(EXPORT TinyVTU_targets
        FILE TinyVTUTargets.cmake
        NAMESPACE TinyVTU::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyVTU)


# Generate a Config file for find_package
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/TinyVTUConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion

)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TinyVTUConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/TinyVTUConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyVTU
)


install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TinyVTUConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TinyVTUConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TinyVTU
)

# Add tests
if (${TINYVTU_WITH_TESTS})
    Include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(tinyvtu_tests
            tests/test_datablock.cpp
            tests/test_grid_data.cpp
            tests/test_tinyvtu.cpp
            tests/test_xml_writer.cpp
    )

    target_link_libraries(tinyvtu_tests PRIVATE TinyVTU Catch2::Catch2WithMain)
    target_include_directories(tinyvtu_tests PRIVATE src)

    include(CTest)
    include(Catch)
    catch_discover_tests(tinyvtu_tests)
endif ()
