cmake_minimum_required(VERSION 3.16)

project(lablib3
    LANGUAGES C CXX)

if(NOT ${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    message("Please switch to x64 build.")
    return()
endif()

add_library(lablib3 SHARED src/lablib3.c src/oamalloc.c)

target_include_directories(lablib3
	PUBLIC
	./include
)

add_compile_definitions(
	BUILDING_LABLIB3
)
