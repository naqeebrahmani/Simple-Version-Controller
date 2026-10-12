cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(nit)

add_executable(nit)

set_property(TARGET nit PROPERTY CXX_STANDARD 20)

target_sources(nit
    PRIVATE
        nit.cpp)

add_library(zlib IMPORTED STATIC)

include_directories(
    "C:/zlib-1.3.2")


set_property(TARGET zlib PROPERTY
             IMPORTED_LOCATION "C:/zlib-1.3.2/libz.a")

target_link_libraries(nit
    PRIVATE
        zlib)