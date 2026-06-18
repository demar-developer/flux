# Dummy CMake to pre-build JUCE and Tracktion dependencies
# This populates ccache so subsequent FLUX builds are fast

cmake_minimum_required(VERSION 3.22)
project(FluxDepsPrebuild LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Pre-build JUCE
FetchContent_Declare(
  juce
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/juce
)

# Pre-build Tracktion (depends on JUCE)
set(JUCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/juce)
FetchContent_Declare(
  tracktion_engine
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tracktion_engine
)

# Make them available (this triggers the build)
FetchContent_MakeAvailable(juce tracktion_engine)

# Create a minimal target to force compilation
add_executable(dummy_prebuild dummy.cpp)
target_link_libraries(dummy_prebuild PRIVATE
  juce_core
  tracktion_engine
)
