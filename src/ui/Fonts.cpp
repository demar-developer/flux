#include "Fonts.h"

namespace flux {

bool Fonts::customFontsLoaded_ = false;
juce::Font::Ptr Fonts::spaceGrotesk_;
juce::Font::Ptr Fonts::inter_;
juce::Font::Ptr Fonts::jetBrainsMono_;

void Fonts::loadCustomFonts() {
  // For Session 1 (foundation), we use JUCE's built-in fonts
  // as fallback. In a full build, these would be loaded from
  // BinaryData (fonts embedded as binary resources).
  //
  // TODO: Embed actual font files:
  // - SpaceGrotesk-*.ttf
  // - Inter-*.ttf
  // - JetBrainsMono-*.ttf
  //
  // For now, we map to similar system fonts:
  // - Space Grotesk → sans-serif display fonts
  // - Inter → system sans-serif
  // - JetBrains Mono → monospace

  customFontsLoaded_ = false;  // Using fallbacks for now

  // When fonts are embedded, load them like:
  //
  // juce::MemoryInputStream spaceGroteskStream(
  //     BinaryData::SpaceGroteskRegular_ttf,
  //     BinaryData::SpaceGroteskRegular_ttfSize, false);
  // spaceGrotesk_ = new juce::Font(juce::Typeface::createSystemTypefaceFor(
  //     spaceGroteskStream));
}

juce::Font Fonts::getFont(Type type, float size) {
  if (!customFontsLoaded_) {
    // Use system font fallbacks
    switch (type) {
      case Type::Display:
        // Use a nice sans-serif display font
        #if JUCE_MAC
        return juce::Font("Helvetica Neue", size, juce::Font::bold);
        #elif JUCE_WINDOWS
        return juce::Font("Segoe UI", size, juce::Font::bold);
        #else
        return juce::Font("DejaVu Sans", size, juce::Font::bold);
        #endif

      case Type::UI:
        // Standard UI font
        #if JUCE_MAC
        return juce::Font("SF Pro Text", size, juce::Font::plain);
        #elif JUCE_WINDOWS
        return juce::Font("Segoe UI", size, juce::Font::plain);
        #else
        return juce::Font("DejaVu Sans", size, juce::Font::plain);
        #endif

      case Type::Mono:
        // Monospace font for numbers
        #if JUCE_MAC
        return juce::Font("SF Mono", size, juce::Font::plain);
        #elif JUCE_WINDOWS
        return juce::Font("Consolas", size, juce::Font::plain);
        #else
        return juce::Font("DejaVu Sans Mono", size, juce::Font::plain);
        #endif
    }
  }

  // When custom fonts are loaded, use them
  switch (type) {
    case Type::Display:
      return spaceGrotesk_->withHeight(size);
    case Type::UI:
      return inter_->withHeight(size);
    case Type::Mono:
      return jetBrainsMono_->withHeight(size);
  }

  // Fallback
  return juce::Font(size);
}

bool Fonts::hasCustomFonts() {
  return customFontsLoaded_;
}

}  // namespace flux
