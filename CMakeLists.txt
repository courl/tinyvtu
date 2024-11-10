cmake_minimum_required(VERSION 3.29)
project(tinyvtu)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_VERSION 0.0.1)

find_package(ZLIB REQUIRED)

add_library(tinyvtu STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyvtu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/DataBlock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/XMLWriter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/DataBlock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/GridData.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/GridData.cpp)

target_include_directories(tinyvtu PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
target_link_libraries(tinyvtu PUBLIC ZLIB::ZLIB)

# Installation rules
include(GNUInstallDirs)

install(TARGETS tinyvtu
        EXPORT tinyvtu_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/tinyvtu.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export the target for find_package
install(EXPORT tinyvtu_targets
        FILE tinyvtuTargets.cmake
        NAMESPACE tinyvtu::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinyvtu)


# Generate a Config file for find_package
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/tinyvtuConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion

)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tinyvtuConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/tinyvtuConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinyvtu
)


install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/tinyvtuConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/tinyvtuConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinyvtu
)

# Add a test executable
add_executable(tetra
        examples/example_tetra.cpp
)

target_link_libraries(tetra PRIVATE tinyvtu)