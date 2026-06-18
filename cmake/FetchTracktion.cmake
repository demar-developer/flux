# Fetch Tracktion Engine v3.2.0 (tag v3.2.0, commit 0a5f4e6)
# NOTE: Must use tag name (not commit hash) for GIT_SHALLOW to work
# Tracktion Engine depends on JUCE, which should be fetched first

include(FetchContent)

# Tracktion Engine uses JUCE that we already fetched
FetchContent_Declare(
  tracktion_engine
  GIT_REPOSITORY https://github.com/Tracktion/tracktion_engine.git
  GIT_TAG v3.2.0
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
