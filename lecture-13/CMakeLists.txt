cmake_minimum_required(VERSION 3.10)

project(toolproject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_STORAGE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/storage/keys)

add_definitions(-Wall)

set(OPENSSL_ROOT_DIR, /Users/theodorserbana/.conan/data/openssl/1.1.1g/_/_/package/95382baad1eea282b0204228603f9795cf17a845/)

set(OPENSSL_INCLUDE_DIR, ${OPENSSL_ROOT_DIR}/include CACHE FILEPATH "" FORCE)

set(OPENSSL_CRYPTO_LIBRARY, ${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib CACHE FILEPATH "" FORCE)

set(OPENSSL_SSL_LIBRARY, ${OPENSSL_ROOT_DIR}/lib/libssl.dylib CACHE FILEPATH "" FORCE)

find_package(cpprestsdk REQUIRED)

add_executable(toolproject
               src/Toolproject.cpp)

target_include_directories(toolproject
                           PRIVATE
                           ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(toolproject
                      PRIVATE
                      cpprestsdk::cpprest)
