cmake_minimum_required(VERSION 3.10)
project(rootHistogramViewer)

set(CMAKE_CXX_STANDARD 11)

find_package(ROOT REQUIRED COMPONENTS Gui)
include(${ROOT_USE_FILE})

ROOT_GENERATE_DICTIONARY(MainFrameDictionary include/MainFrame.hpp LINKDEF include/MainFrameLinkDef.h)

add_library(RootHistogramViewer SHARED MainFrameDictionary.cxx source/MainFrame.cpp)
target_include_directories(RootHistogramViewer SYSTEM BEFORE PUBLIC include PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_libraries(RootHistogramViewer ${ROOT_LIBRARIES} Gui)
set_target_properties(RootHistogramViewer PROPERTIES PUBLIC_HEADER include/MainFrame.hpp)

add_executable(rootHistogramViewer source/main.cpp)
target_include_directories(rootHistogramViewer SYSTEM BEFORE PUBLIC include PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_libraries(rootHistogramViewer RootHistogramViewer)

install(TARGETS rootHistogramViewer RootHistogramViewer RUNTIME DESTINATION bin LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/RootHistogramViewer/)