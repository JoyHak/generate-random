option(ENABLE_ICECREAM "If ON the tests to icecream library will be enabled" OFF)

find_package(Catch2 REQUIRED)
enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR})


function(declare_test name)
  add_executable(test_${name} test_${name}.cpp)
  add_test(NAME test_${name} COMMAND test_${name})
  target_link_libraries(test_${name} PRIVATE Catch2::Catch2WithMain)
endfunction()


declare_test(generate)

if (ENABLE_ICECREAM)
  declare_test(icecream)
endif()