# FLUX Tests

# Test executable - includes engine sources needed for tests
target_sources(flux_tests
  PRIVATE
    test_main.cpp
    test_render_smoke.cpp
    test_gui_smoke.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/EngineWrapper.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/EngineWrapper.h
    ${CMAKE_SOURCE_DIR}/src/engine/EditController.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/EditController.h
)

target_include_directories(flux_tests
  PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(flux_tests
  PRIVATE
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  TRACKTION_ENABLE_SINGLETONS=1
  FLUX_TEST_BIN_DIR="${CMAKE_BINARY_DIR}"
)

target_link_libraries(flux_tests
  PRIVATE
  GTest::gtest_main
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
  tracktion::tracktion_engine
)

# Add tests to CTest
add_test(NAME EngineSmokeTest COMMAND flux_tests --gtest_filter=EngineSmoke*)
add_test(NAME RenderSmokeTest COMMAND flux_tests --gtest_filter=RenderSmoke*)
add_test(NAME GUISmokeTest COMMAND flux_tests --gtest_filter=GUISmoke*)

# GUI smoke test needs virtual display on headless systems
set_property(TEST GUISmokeTest PROPERTY ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
