cmake_minimum_required(VERSION 3.0)
project(RB)

set(CONTRIB ${PROJECT_SOURCE_DIR}/contrib)

if(WIN32)
  link_directories(${CONTRIB}/lib)
else()
  include_directories(/usr/include/)
  
  if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  endif()

  set(CMAKE_CXX_FLAGS "-Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  
endif()

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

#include_directories(${CONTRIB}/include)

#include_directories(${PROJECT_SOURCE_DIR}/include/rb)
#include_directories(${PROJECT_SOURCE_DIR}/include)

#allows use of quaternions
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set(CMAKE_CXX_STANDARD 17)

#-----------------Library-----------------
add_library(rb STATIC
  include/rb/World.h
  include/rb/Body.h
  include/rb/AABB.h
  include/rb/BVH.h

  src/rb/World.cpp
  src/rb/Body.cpp
  src/rb/AABB.cpp
  src/rb/BVH.cpp
)


target_include_directories(rb PUBLIC
  ${CONTRIB}/include
  ${PROJECT_SOURCE_DIR}/include/rb
)

#-----------------Example-----------------
add_executable(example
	src/example/main.cpp
)

target_link_libraries(example rb)


#------------------Tests------------------
enable_testing()

#Forces gtest to link to C runtime libraries statically
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
)

add_executable(tests

	src/tests/main.cpp
)
target_include_directories(tests PUBLIC
  #${CONTRIB}/include
  ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(tests rb gtest_main)

add_test(AllTests tests)
