cmake_minimum_required(VERSION 3.28)
project(generate-random VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 26)


include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
option(BUILD_TESTING "If ON the tests will be enabled" ON)
include(CTest)

add_library(generate-random INTERFACE)

install(
  FILES "${CMAKE_CURRENT_SOURCE_DIR}/generate.hpp"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
  TARGETS generate-random
  EXPORT generate-random-targets
)

install(
  EXPORT generate-random-targets
  FILE generate-random-targets.cmake
  NAMESPACE generate-random::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/generate-random"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/generate-random-config-version.cmake"
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT
)

install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/generate-random-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/generate-random-config-version.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/generate-random"
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()


