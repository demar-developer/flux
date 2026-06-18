# Fetch JUCE 8.0.13 (tag 8.0.13, commit 7c9d378)
# NOTE: Must use tag name (not commit hash) for GIT_SHALLOW to work

include(FetchContent)

FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.13
  GIT_SHALLOW TRUE
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/juce
)

FetchContent_MakeAvailable(juce)

# Add JUCE modules to path
list(APPEND CMAKE_MODULE_PATH ${juce_SOURCE_DIR}/extras/Build/CMake)
