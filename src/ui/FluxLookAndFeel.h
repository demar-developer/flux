#pragma once

#include "Fonts.h"

#include <juce_gui_basics/juce_gui_basics.h>

namespace flux {

/**
 * FLUX Design System LookAndFeel.
 *
 * Colors:
 * - Hero accent: #FF8A4C (warm amber)
 * - Background: Dark gray (FL Studio inspired)
 * - Text: Light gray/white
 *
 * Fonts:
 * - Space Grotesk: display/headers
 * - Inter: UI labels/controls
 * - JetBrains Mono: numeric readouts
 */
class FluxLookAndFeel : public juce::LookAndFeel_V4 {
public:
  FluxLookAndFeel();
  ~FluxLookAndFeel() override = default;

  /**
   * Get the accent color (#FF8A4C).
   */
  static juce::Colour getAccentColour();

  /**
   * Get the background color.
   */
  static juce::Colour getBackgroundColour();

  /**
   * Get the surface color (for panels, cards).
   */
  static juce::Colour getSurfaceColour();

  /**
   * Get the primary text color.
   */
  static juce::Colour getTextColour();

  /**
   * Get the secondary/muted text color.
   */
  static juce::Colour getMutedTextColour();

  // JUCE LookAndFeel overrides
  juce::Font getLabelFont(juce::Label& label) override;
  juce::Font getComboBoxFont(juce::ComboBox& box) override;
  juce::Font getPopupMenuFont() override;
  void drawButtonBackground(juce::Graphics& g,
                           juce::Button& button,
                           const juce::Colour& backgroundColour,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override;
  void fillTextEditorBackground(juce::Graphics& g,
                               int width,
                               int height,
                               juce::TextEditor& textEditor) override;
  void drawTextEditorOutline(juce::Graphics& g,
                            int width,
                            int height,
                            juce::TextEditor& textEditor) override;

private:
  void initializeColours();
};

}  // namespace flux
