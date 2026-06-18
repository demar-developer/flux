# Fetch Tracktion Engine v3.2.0 (commit 0a5f4e6)
# Tracktion Engine depends on JUCE, which should be fetched first

include(FetchContent)

# Tracktion Engine uses JUCE that we already fetched
FetchContent_Declare(
  tracktion_engine
  GIT_REPOSITORY https://github.com/Tracktion/tracktion_engine.git
  GIT_TAG 0a5f4e6a5f53d09c89b414a44386a12df7fa1ec6
  GIT_SHALLOW TRUE
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/tracktion_engine
)

# Tracktion Engine needs to know where JUCE is
set(JUCE_DIR ${juce_SOURCE_DIR} CACHE PATH "Path to JUCE" FORCE)

# Make Tracktion Engine available
FetchContent_MakeAvailable(tracktion_engine)

# Tracktion creates a namespaced alias
define_property(TARGET PROPERTY JUCE_COMPANY_NAME)
define_property(TARGET PROPERTY JUCE_COMPANY_COPYRIGHT)
define_property(TARGET PROPERTY JUCE_COMPANY_WEBSITE)
define_property(TARGET PROPERTY JUCE_COMPANY_EMAIL)
