cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_RELEASE "-s -Os")

project(fgns)

option(FGNS_STATIC "Build static" OFF)

set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PARTY_DIR ${BASE_DIR}/third-party)
set(CORE_DIR ${BASE_DIR}/core)
set(MISC_DIR ${BASE_DIR}/misc)
set(LIBS_DIR ${BASE_DIR}/libs)

list(APPEND CMAKE_MODULE_PATH "${BASE_DIR}/cmake")

if (NOT FGNS_STATIC)
    find_package(Sodium REQUIRED)
    find_package(LibLZMA REQUIRED)
endif()

add_subdirectory(${THIRD_PARTY_DIR}/rapidfuzz)

file(GLOB_RECURSE CORE_DIR_GLOB ${CORE_DIR}/*.cc)
file(GLOB_RECURSE MISC_DIR_GLOB ${MISC_DIR}/*.cpp)
set(SOURCE_FILES ${CORE_DIR_GLOB}
                    ${MISC_DIR_GLOB})

add_library(fgns ${SOURCE_FILES})

if (NOT FGNS_STATIC)
    set(LIB_INCLUDES ${sodium_INCLUDE_DIR} ${LIBLZMA_INCLUDE_DIRS})
else()
    set(LIBSODIUM ${LIBS_DIR}/libsodium-stable/src/libsodium)
    set(LIBLZMA ${LIBS_DIR}/lzma)
    set(LIB_INCLUDES ${LIBSODIUM}/include
                     ${LIBLZMA}/include
                     )
endif()

file(GLOB_RECURSE INCLUDE_DIR_GLOB ${BASE_DIR}/include/*.h)
file(GLOB_RECURSE MISC_DIR_H_GLOB ${MISC_DIR}/*.hpp)
set(HEADER_FILES ${INCLUDE_DIR_GLOB}
                 ${MISC_DIR_H_GLOB}
                 )

set_target_properties(fgns PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")
target_include_directories(fgns PUBLIC ${BASE_DIR}/include
                                       ${MISC_DIR}
                                       ${LIB_INCLUDES}
                                       ${THIRD_PARTY_DIR}/cereal/include
                                       ${THIRD_PARTY_DIR}/cxxopts
                                       ${THIRD_PARTY_DIR}/filesystem/include
                                       )

add_executable(fgnsx ${BASE_DIR}/fgns.cc)
set_target_properties(fgnsx PROPERTIES OUTPUT_NAME fgns)

if (NOT FGNS_STATIC)
    target_link_libraries(fgns sodium ${LIBLZMA_LIBRARIES} rapidfuzz::rapidfuzz)
    target_link_libraries(fgnsx fgns)
else()
    target_link_libraries(fgns -static ${LIBSODIUM}/.libs/libsodium.a
                                       ${LIBLZMA}/bin/linux/release/liblzma.a
                                       rapidfuzz::rapidfuzz
                                       )
    target_link_libraries(fgnsx -static fgns)
endif()

set(INSTALL_DIR /usr/local/bin)
install(TARGETS fgns DESTINATION ${INSTALL_DIR})
