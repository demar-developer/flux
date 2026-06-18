// FLUX Test Main

#include <gtest/gtest.h>
#include <juce_core/juce_core.h>

int main(int argc, char** argv) {
  // Initialize JUCE (needed for Tracktion tests)
  juce::ScopedJuceInitialiser_GUI juceInitializer;

  // Initialize Google Test
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
