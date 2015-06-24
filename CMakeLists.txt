
cmake_minimum_required(VERSION 2.8)
project(xfsx CXX)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8")
    message(FATAL_ERROR "You need at least version 4.8 when compiling with GCC")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")


find_package(Boost 1.58
  COMPONENTS
    regex
    system
    filesystem
    unit_test_framework
  REQUIRED)

add_executable(mkvariant
  mkvariant.cc
)

set_property(TARGET mkvariant PROPERTY INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIRS}
#  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(example
  example/main.cc
  example/variant.cc
)

target_link_libraries(mkvariant
  ${Boost_REGEX_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
#  ${Boost_LIBRARIES} # or
#  ${Boost_LOG_LIBRARY}
)

macro(VARIANT_TARGET Name Input Output)
  set(VARIANT_TARGET_usage "VARIANT_TARGET(<Name> <Input> <Output>")
  get_filename_component(VARIANT_TARGET_basename ${Output} NAME_WE)
  add_custom_command(OUTPUT ${Output} ${VARIANT_TARGET_basename}.hh
    DEPENDS mkvariant ${Input}
    COMMAND ./mkvariant ${Input} -o ${Output}
    COMMENT "[mkvariant][${Name}] Generating variant"
    )
endmacro()

set(INFS
  basic_variant
  non_default
  namespace
  copy
  )
foreach(INF ${INFS})
  VARIANT_TARGET(${INF} ${CMAKE_SOURCE_DIR}/test/${INF}.inf ${INF}.cc)
endforeach()
VARIANT_TARGET(json ${CMAKE_SOURCE_DIR}/test/json.json json.cc)

add_executable(ut
  test/main.cc
  test/basic.cc
  test/non_default.cc
  test/namespace.cc
  test/json.cc
  test/copy.cc
  basic_variant.cc
  non_default.cc
  namespace.cc
  json.cc
  copy.cc
)
set_property(TARGET ut PROPERTY INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
)
target_link_libraries(ut
  ${Boost_LIBRARIES}
)

