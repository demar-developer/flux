cmake_minimum_required(VERSION 3.22)

project(FLUX VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CCache support
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# Include dependency fetching
include(cmake/FetchJUCE.cmake)
include(cmake/FetchTracktion.cmake)
include(FetchContent)

# Fetch GoogleTest for tests
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
  GIT_SHALLOW TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# JUCE modules required by FLUX
set(FLUX_JUCE_MODULES
  juce_core
  juce_events
  juce_graphics
  juce_gui_basics
  juce_gui_extra
  juce_audio_basics
  juce_audio_devices
  juce_audio_formats
  juce_audio_processors
  juce_audio_utils
)

# Main application target
juce_add_gui_app(FLUX
  PRODUCT_NAME "FLUX"
  COMPANY_NAME "FLUX Audio"
  VERSION "${PROJECT_VERSION}"
  BUNDLE_ID "com.fluxaudio.flux"
  NEEDS_WEB_BROWSER FALSE
  NEEDS_WEB_SERVER FALSE
  NEEDS_CURL FALSE
  ICON_BIG ""
  ICON_SMALL ""
)

target_sources(FLUX
  PRIVATE
    src/app/Application.cpp
    src/app/Application.h
    src/app/MainWindow.cpp
    src/app/MainWindow.h
    src/app/MainComponent.cpp
    src/app/MainComponent.h
    src/engine/EngineWrapper.cpp
    src/engine/EngineWrapper.h
    src/engine/EditController.cpp
    src/engine/EditController.h
    src/ui/FluxLookAndFeel.cpp
    src/ui/FluxLookAndFeel.h
    src/ui/Fonts.cpp
    src/ui/Fonts.h
)

target_include_directories(FLUX
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(FLUX
  PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="FLUX"
    JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}"
    JUCE_MODAL_LOOPS_PERMITTED=1
    TRACKTION_ENABLE_SINGLETONS=1
)

target_link_libraries(FLUX
  PRIVATE
    ${FLUX_JUCE_MODULES}
    tracktion::tracktion_engine
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# Tests
add_subdirectory(tests)
