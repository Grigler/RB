cmake_minimum_required(VERSION 3.0)
project(RB)

set(CONTRIB ${PROJECT_SOURCE_DIR}/contrib)

if(WIN32)
  link_directories(${CONTRIB}/lib)
else()
  include_directories(/usr/include/)
  
  #if(NOT CMAKE_BUILD_TYPE)
  #set(CMAKE_BUILD_TYPE Release)
  #endif()

  #set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall --coverage")
  #set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  
endif()

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

#allows use of quaternions
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set(CMAKE_CXX_STANDARD 17)

#-----------------Library-----------------
add_library(rb STATIC
  include/rb/World.h
  include/rb/Body.h
  include/rb/AABB.h
  include/rb/BVH.h
  include/rb/IntegratorFactory.h
  
  include/rb/Integrator.h
  include/rb/ForwardEuler.h
  include/rb/SemiImplicitEuler.h
  include/rb/RK4.h

  src/rb/World.cpp
  src/rb/Body.cpp
  src/rb/AABB.cpp
  src/rb/BVH.cpp
  
  src/rb/IntegratorFactory.cpp
  src/rb/ForwardEuler.cpp
  src/rb/SemiImplicitEuler.cpp
  src/rb/RK4.cpp
)


target_include_directories(rb PUBLIC
  ${CONTRIB}/include
  ${PROJECT_SOURCE_DIR}/include/rb
)


#------------------Tests------------------
enable_testing()

#Forces gtest to link to C runtime libraries statically
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
)

if(NOT WIN32)
  #Just testing
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall --coverage")
endif()

add_executable(tests

	src/tests/main.cpp
)
target_include_directories(tests PUBLIC
  #${CONTRIB}/include
  ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(tests rb gtest_main)

add_test(AllTests ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/tests)


#-----------------NGL/RB Example-----------------

add_definitions(-DNO_DLL)

add_executable(example
  src/example/main.cpp

  include/example/Renderer.h
  include/example/GameClock.h
  
  src/example/Renderer.cpp
  src/example/GameClock.cpp
  
  src/example/gl3w.c
)

target_include_directories(example PUBLIC
  ${CONTRIB}/include
  ${PROJECT_SOURCE_DIR}/include/rb
  ${PROJECT_SOURCE_DIR}/include/example
)

target_link_libraries(example rb SDL2 SDL2main optimized NGL)
target_link_libraries(example debug NGLD)