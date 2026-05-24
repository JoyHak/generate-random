option(ENABLE_ICECREAM "If ON the tests to icecream library will be enabled" OFF)
option(ENABLE_DUPLICATES "If ON the tests for duplicates in std::set will be enabled" OFF)
option(ENABLE_UNIQ "If ON the stress tests for unique elements will be enabled" OFF)

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


declare_test(array)
declare_test(vector)
declare_test(list)
declare_test(forward_list)
declare_test(deque)
declare_test(stack)
declare_test(set)
declare_test(unordered_set)

if (ENABLE_ICECREAM)
  declare_test(icecream)
endif()
if (ENABLE_DUPLICATES)
  declare_test(duplicates)
endif()
if (ENABLE_UNIQ)
  declare_test(uniq)
endif()