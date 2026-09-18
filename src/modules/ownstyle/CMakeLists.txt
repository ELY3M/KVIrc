cmake_minimum_required(VERSION 3.16)
project(ownstyle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt dependencies (Match the version KVIrc was built with, usually Qt5 or Qt6)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Find KVIrc headers. If installed in a non-standard path, 
# you can pass -DKVIRC_INCLUDE_DIR=/path/to/include to cmake.
find_path(KVIRC_INCLUDE_DIR KviModule.h PATH_SUFFIXES kvirc)

if(NOT KVIRC_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find KviModule.h. Please specify -DKVIRC_INCLUDE_DIR")
endif()

# Define your module library
add_library(ownstyle MODULE 
    libownstyle.cpp
    libownstyle.h
)

# Include directories for KVIrc and Qt
target_include_directories(ownstyle PRIVATE 
    ${KVIRC_INCLUDE_DIR}
)

# Link Qt libraries and apply position-independent code (required for plugins)
target_link_libraries(ownstyle PRIVATE Qt5::Widgets)
set_target_properties(ownstyle PROPERTIES 
    PREFIX "lib"
    POSITION_INDEPENDENT_CODE ON
)
