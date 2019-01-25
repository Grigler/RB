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
  #set(CMAKE_CXX_FLAGS "-g -O0 -Wall --coverage")
  #set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  
  #Used to link to the NGL lib location (assuming it is built to home/NGL/lib)
  link_directories($ENV{HOME}/NGL/lib)

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

  include/rb/Constraint.h
  include/rb/LCPFactory.h
  include/rb/LCPSolver.h
  include/rb/SISolver.h
  
  include/rb/GreedyCollider.h
  include/rb/SphereCollider.h

  src/rb/World.cpp
  src/rb/Body.cpp
  src/rb/AABB.cpp
  src/rb/BVH.cpp
  
  src/rb/IntegratorFactory.cpp
  src/rb/ForwardEuler.cpp
  src/rb/SemiImplicitEuler.cpp
  
  src/rb/Constraint.cpp
  src/rb/LCPFactory.cpp
  src/rb/LCPSolver.cpp
  src/rb/SISolver.cpp
  
  src/rb/GreedyCollider.cpp
  src/rb/SphereCollider.cpp
)


target_include_directories(rb PUBLIC
  ${CONTRIB}/include
  ${PROJECT_SOURCE_DIR}/include/rb
)


#------------------Tests------------------
enable_testing()

#Forces gtest to link to C runtime libraries statically
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#Downloads gtest as part of the configuration steps
include(cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
)

if(NOT WIN32)
  #Just testing
  #set(CMAKE_CXX_FLAGS "-g -O0 -Wall --coverage")
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
add_definitions(-D_USE_MATH_DEFINES)

add_executable(example
  src/example/main.cpp

  include/example/Renderer.h
  include/example/GameClock.h
  include/example/KeyInput.h
  include/example/Transform.h
  include/example/Camera.h
  include/example/Scene.h
  include/example/Object.h
  include/example/DrawObject.h
  
  include/example/Sphere.h
  include/example/Cube.h
  include/example/SphereEmitter.h
  
  src/example/Renderer.cpp
  src/example/GameClock.cpp
  src/example/KeyInput.cpp
  src/example/Scene.cpp
  src/example/Transform.cpp
  src/example/Camera.cpp
  src/example/Object.cpp
  src/example/DrawObject.cpp

  src/example/Sphere.cpp
  src/example/Cube.cpp
  src/example/SphereEmitter.cpp
  
  #src/example/gl3w.c
)

target_include_directories(example PUBLIC
  ${CONTRIB}/include
  ${PROJECT_SOURCE_DIR}/include/rb
  ${PROJECT_SOURCE_DIR}/include/example
)

target_link_libraries(example rb SDL2 SDL2main NGL)
#target_link_libraries(example debug NGLD)


#Moving data directory into build directory for VS ease
#file(COPY ${PROJECT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
if(WIN32)
message(STATUS 
  "Copying data folder to ${CMAKE_BINARY_DIR}/data for VS ease of use"
)
add_custom_command(TARGET example PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${PROJECT_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
)
endif()

#Postbuild command to copy required data folder to build/bin for easy running
message(STATUS "Copying data folder to ${CMAKE_BINARY_DIR}/bin/data")
add_custom_command(TARGET example POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${PROJECT_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/bin/data
)
