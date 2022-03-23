cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_RELEASE "-s -Os")

project(fgns)

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third-party)
set(CORE_DIR ${CMAKE_SOURCE_DIR}/core)
set(CRYPTO_DIR ${CMAKE_SOURCE_DIR}/crypto)
set(MISC_DIR ${CMAKE_SOURCE_DIR}/misc)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Sodium REQUIRED)
find_package(LibLZMA REQUIRED)

add_subdirectory(${THIRD_PARTY_DIR}/rapidfuzz)

file(GLOB_RECURSE CORE_DIR_GLOB ${CORE_DIR}/*.cc)
file(GLOB_RECURSE MISC_DIR_GLOB ${MISC_DIR}/*.cpp)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/fgns.cc
                    ${CORE_DIR_GLOB}
                        ${MISC_DIR_GLOB})

add_executable(fgns ${SOURCE_FILES})

target_include_directories(fgns PRIVATE ${CMAKE_SOURCE_DIR}/include
                                        ${MISC_DIR}
                                        ${sodium_INCLUDE_DIR}
                                        ${LIBLZMA_INCLUDE_DIRS}
                                        ${THIRD_PARTY_DIR}/cereal/include
                                        ${THIRD_PARTY_DIR}/cxxopts
                                        ${THIRD_PARTY_DIR}/filesystem/include
                                        )

target_link_libraries(fgns sodium rapidfuzz::rapidfuzz ${LIBLZMA_LIBRARIES})

set(INSTALL_DIR /usr/local/bin)
install(TARGETS fgns DESTINATION ${INSTALL_DIR})
