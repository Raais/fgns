cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_RELEASE "-s -Os")

project(fgns)

find_package(LibLZMA REQUIRED)

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third-party)
set(CORE_DIR ${CMAKE_SOURCE_DIR}/core)
set(CRYPTO_DIR ${CMAKE_SOURCE_DIR}/crypto)
set(XZ_DIR ${CMAKE_SOURCE_DIR}/xz)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)

add_subdirectory(${THIRD_PARTY_DIR}/cryptopp)
add_subdirectory(${THIRD_PARTY_DIR}/rapidfuzz)

file(GLOB_RECURSE CORE_DIR_GLOB ${CORE_DIR}/*.cc)
file(GLOB_RECURSE CRYPTO_DIR_GLOB ${CRYPTO_DIR}/*.cc)
file(GLOB_RECURSE XZ_DIR_GLOB ${XZ_DIR}/*.cpp)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/fgns.cc
                    ${CORE_DIR_GLOB}
                        ${CRYPTO_DIR_GLOB}
                              ${XZ_DIR_GLOB})

add_executable(fgns ${SOURCE_FILES})

target_include_directories(fgns PRIVATE ${CMAKE_SOURCE_DIR}/include
                                        ${CMAKE_SOURCE_DIR}/include/crypto
                                       ${XZ_DIR}
                                       ${LIBS_DIR}/cryptopp/include
                                       ${LIBLZMA_INCLUDE_DIRS}
                                       #${LIBS_DIR}/lzma/include
                                       #${LIBS_DIR}/lzma/include/lzma
                                       ${THIRD_PARTY_DIR}/cereal/include
                                       ${THIRD_PARTY_DIR}/cxxopts
                                       ${THIRD_PARTY_DIR}/filesystem/include
                                       )

#target_link_libraries(fgns -static rapidfuzz::rapidfuzz ${LIBS_DIR}/cryptopp/bin/libcryptopp.a ${LIBS_DIR}/lzma/bin/linux/release/liblzma.a)
#target_link_libraries(fgns rapidfuzz::rapidfuzz ${LIBS_DIR}/cryptopp/bin/libcryptopp.a ${LIBLZMA_LIBRARIES})
target_link_libraries(fgns rapidfuzz::rapidfuzz cryptopp-static ${LIBLZMA_LIBRARIES})

set(INSTALL_DIR /usr/local/bin)
install(TARGETS fgns DESTINATION ${INSTALL_DIR})
