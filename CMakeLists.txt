cmake_minimum_required(VERSION 3.14)

project(offalloc)

set(SOURCE_FILES
   offalloc.c
   offalloc.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
