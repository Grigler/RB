cmake_minimum_required(VERSION 3.0)
project(RB)

set(CONTRIB ${PROJECT_SOURCE_DIR}/contrib)

if(WIN32)
  link_directories(${CONTRIB}/lib)
else()

endif()

include_directories(${CONTRIB}/include)


#-----------------Library-----------------
add_library(rb STATIC
  src/rb/rb.cpp
)


#-----------------Example-----------------
add_executable(example
	src/example/main.cpp
)

target_link_libraries(example rb)


#------------------Tests------------------
enable_testing()

add_executable(tests
	src/tests/main.cpp
)

target_link_libraries(tests rb gtest)

add_test(AllTests tests)
