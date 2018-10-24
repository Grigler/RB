cmake_minimum_required(VERSION 3.0)
project(RB)

set(CONTRIB ${PROJECT_SOURCE_DIR}/contrib)

if(WIN32)
  link_directories(${CONTRIB}/lib)
else()
  include_directories(/usr/include/)
endif()

include_directories(${CONTRIB}/include)

include_directories(${PROJECT_SOURCE_DIR}/include/rb)
include_directories(${PROJECT_SOURCE_DIR}/include)

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


#-----------------Example-----------------
add_executable(example
	src/example/main.cpp
)

target_link_libraries(example rb)


#------------------Tests------------------
enable_testing()

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

add_executable(tests

	src/tests/main.cpp
)

target_link_libraries(tests rb gtest)

add_test(AllTests tests)
