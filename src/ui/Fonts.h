#pragma once

#include <juce_graphics/juce_graphics.h>

namespace flux {

/**
 * Font management for FLUX.
 * Uses JUCE's BinaryData for embedded fonts.
 */
class Fonts {
public:
  /**
   * Font types available in FLUX.
   */
  enum class Type {
    Display,  // Space Grotesk - headers and display text
    UI,       // Inter - UI labels and controls
    Mono      // JetBrains Mono - numeric readouts (BPM, dB, time)
  };

  /**
   * Get a font of the specified type and size.
   * Falls back to system fonts if custom fonts not loaded.
   */
  static juce::Font getFont(Type type, float size);

  /**
   * Load custom fonts from BinaryData.
   * Should be called once at application startup.
   */
  static void loadCustomFonts();

  /**
   * Check if custom fonts are available.
   */
  static bool hasCustomFonts();

private:
  static bool customFontsLoaded_;
  static juce::Font::Ptr spaceGrotesk_;
  static juce::Font::Ptr inter_;
  static juce::Font::Ptr jetBrainsMono_;
};

}  // namespace flux
