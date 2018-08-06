cmake_minimum_required(VERSION 3.10)
project(rootHistogramViewer)

set(CMAKE_CXX_STANDARD 11)

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

ROOT_GENERATE_DICTIONARY(MainFrameDictionary include/MainFrame.hpp LINKDEF include/MainFrameLinkDef.h)

add_library(MainFrame SHARED MainFrameDictionary.cxx source/MainFrame.cpp)
target_include_directories(MainFrame SYSTEM BEFORE PUBLIC include/ PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_libraries(MainFrame ${ROOT_LIBRARIES} Gui)

add_executable(rootHistogramViewer source/main.cpp)
target_include_directories(rootHistogramViewer SYSTEM BEFORE PUBLIC include/ PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_libraries(rootHistogramViewer MainFrame)
