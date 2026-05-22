cmake_minimum_required(VERSION 3.28)
project(CPP)

set(CMAKE_CXX_STANDARD 26)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE)
endif()

set(VCPKG_TARGET_TRIPLET x64-mingw-static)
set(CMAKE_PREFIX_PATH "C:/Users/ToYu/.vcpkg-clion/vcpkg/installed/x64-mingw-static")
set(CMAKE_TOOLCHAIN_FILE "C:/Users/ToYu/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(fmt)
include_directories(
    "C:/Configs and settings/CPP/icecream-cpp"
    "C:/Configs and settings/CPP/generate-random"
    "Lib"
)

file(GLOB SOURCES
    "21.05/*.cpp"
    "generate.cpp")

# Преобразуем все пути в относительные
set(REL_SOURCES)
foreach(source IN LISTS SOURCES)
    file(RELATIVE_PATH rel_source "${CMAKE_CURRENT_SOURCE_DIR}" "${source}")
    if(rel_source MATCHES "^\\.")
        continue()
    endif()
    list(APPEND REL_SOURCES ${rel_source})
endforeach()
set(SOURCES ${REL_SOURCES})
#message("SOURCES after filter: ${SOURCES}")

foreach(source_file IN LISTS SOURCES)
    get_filename_component(file_name ${source_file} NAME_WE)
    get_filename_component(dir ${source_file} DIRECTORY)

    if(NOT dir STREQUAL "")
        string(REPLACE " " "_" dir ${dir})
        string(REPLACE "/" "_" dir ${dir})
        set(target_name ${dir}_${file_name})
    else()
        set(target_name ${file_name})
    endif()

    add_executable(${target_name} ${source_file})
    target_compile_options(${target_name} PRIVATE -Wno-system-headers)
    target_link_libraries(
        ${target_name}
        fmt::fmt
    )
endforeach()