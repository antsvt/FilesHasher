cmake_minimum_required(VERSION 3.12.0)
set(CMAKE_CXX_STANDARD 17)

# Main project 
set(PROJECT_NAME hasher)

project(${PROJECT_NAME} VERSION 0.0.1)
aux_source_directory(./src SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

# boost
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_DEBUG_LIBS        OFF)
set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(THREADS_PREFER_PTHREAD_FLAG  ON)


find_package(Boost 1.66.0 COMPONENTS program_options REQUIRED)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} Threads::Threads)

# Tests 
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

aux_source_directory(./tests TESTS)
aux_source_directory(./src SOURCES_FOR_TEST)
list(REMOVE_ITEM SOURCES_FOR_TEST ./src/main.cpp)
message(${SOURCES_FOR_TEST})

include_directories(./src)
add_executable(${PROJECT_TEST_NAME} ${TESTS} ${SOURCES_FOR_TEST})
target_link_libraries(${PROJECT_TEST_NAME} GTest::gtest_main ${Boost_LIBRARIES})

add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})
