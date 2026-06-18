#include "FluxLookAndFeel.h"

namespace flux {

// FLUX Design System Colors
static constexpr juce::uint8 ACCENT_R = 0xFF;
static constexpr juce::uint8 ACCENT_G = 0x8A;
static constexpr juce::uint8 ACCENT_B = 0x4C;

static constexpr juce::uint8 BG_R = 0x1A;
static constexpr juce::uint8 BG_G = 0x1A;
static constexpr juce::uint8 BG_B = 0x1A;

static constexpr juce::uint8 SURFACE_R = 0x2D;
static constexpr juce::uint8 SURFACE_G = 0x2D;
static constexpr juce::uint8 SURFACE_B = 0x2D;

static constexpr juce::uint8 TEXT_R = 0xF0;
static constexpr juce::uint8 TEXT_G = 0xF0;
static constexpr juce::uint8 TEXT_B = 0xF0;

static constexpr juce::uint8 MUTED_R = 0x99;
static constexpr juce::uint8 MUTED_G = 0x99;
static constexpr juce::uint8 MUTED_B = 0x99;

FluxLookAndFeel::FluxLookAndFeel() {
  initializeColours();
}

juce::Colour FluxLookAndFeel::getAccentColour() {
  return juce::Colour(ACCENT_R, ACCENT_G, ACCENT_B);
}

juce::Colour FluxLookAndFeel::getBackgroundColour() {
  return juce::Colour(BG_R, BG_G, BG_B);
}

juce::Colour FluxLookAndFeel::getSurfaceColour() {
  return juce::Colour(SURFACE_R, SURFACE_G, SURFACE_B);
}

juce::Colour FluxLookAndFeel::getTextColour() {
  return juce::Colour(TEXT_R, TEXT_G, TEXT_B);
}

juce::Colour FluxLookAndFeel::getMutedTextColour() {
  return juce::Colour(MUTED_R, MUTED_G, MUTED_B);
}

void FluxLookAndFeel::initializeColours() {
  // Set up the colour scheme
  setColour(juce::ResizableWindow::backgroundColourId, getBackgroundColour());
  setColour(juce::DocumentWindow::backgroundColourId, getBackgroundColour());

  // Button colours
  setColour(juce::TextButton::buttonColourId, getSurfaceColour());
  setColour(juce::TextButton::buttonOnColourId, getAccentColour());
  setColour(juce::TextButton::textColourOffId, getTextColour());
  setColour(juce::TextButton::textColourOnId, juce::Colours::white);

  // Toggle button
  setColour(juce::ToggleButton::tickColourId, getAccentColour());
  setColour(juce::ToggleButton::tickDisabledColourId, getMutedTextColour());
  setColour(juce::ToggleButton::textColourId, getTextColour());

  // Combo box
  setColour(juce::ComboBox::backgroundColourId, getSurfaceColour());
  setColour(juce::ComboBox::textColourId, getTextColour());
  setColour(juce::ComboBox::arrowColourId, getAccentColour());
  setColour(juce::ComboBox::outlineColourId, getAccentColour().withAlpha(0.5f));

  // Text editor
  setColour(juce::TextEditor::backgroundColourId, getSurfaceColour());
  setColour(juce::TextEditor::textColourId, getTextColour());
  setColour(juce::TextEditor::highlightColourId, getAccentColour().withAlpha(0.3f));
  setColour(juce::TextEditor::focusedOutlineColourId, getAccentColour());
  setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);

  // Label
  setColour(juce::Label::textColourId, getTextColour());
  setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);

  // List box
  setColour(juce::ListBox::backgroundColourId, getBackgroundColour());
  setColour(juce::ListBox::textColourId, getTextColour());
  setColour(juce::ListBox::outlineColourId, getAccentColour().withAlpha(0.5f));

  // Popup menu
  setColour(juce::PopupMenu::backgroundColourId, getSurfaceColour());
  setColour(juce::PopupMenu::textColourId, getTextColour());
  setColour(juce::PopupMenu::highlightedBackgroundColourId, getAccentColour());
  setColour(juce::PopupMenu::highlightedTextColourId, juce::Colours::white);

  // Scrollbar
  setColour(juce::ScrollBar::thumbColourId, getAccentColour().withAlpha(0.5f));
  setColour(juce::ScrollBar::trackColourId, getSurfaceColour());

  // Slider
  setColour(juce::Slider::thumbColourId, getAccentColour());
  setColour(juce::Slider::trackColourId, getAccentColour().withAlpha(0.3f));
  setColour(juce::Slider::backgroundColourId, getSurfaceColour());
  setColour(juce::Slider::textBoxTextColourId, getTextColour());
  setColour(juce::Slider::textBoxBackgroundColourId, getSurfaceColour());
  setColour(juce::Slider::textBoxHighlightColourId, getAccentColour().withAlpha(0.3f));
  setColour(juce::Slider::textBoxOutlineColourId, getAccentColour().withAlpha(0.5f));

  // Tooltip
  setColour(juce::TooltipWindow::backgroundColourId, getSurfaceColour());
  setColour(juce::TooltipWindow::textColourId, getTextColour());
  setColour(juce::TooltipWindow::outlineColourId, getAccentColour().withAlpha(0.5f));

  // TreeView
  setColour(juce::TreeView::backgroundColourId, getBackgroundColour());
  setColour(juce::TreeView::linesColourId, getAccentColour().withAlpha(0.3f));
  setColour(juce::TreeView::selectedItemBackgroundColourId, getAccentColour().withAlpha(0.3f));

  // Tabbed component
  setColour(juce::TabbedComponent::backgroundColourId, getBackgroundColour());
  setColour(juce::TabbedComponent::outlineColourId, getAccentColour().withAlpha(0.5f));
  setColour(juce::TabbedButtonBar::frontColourId, getAccentColour());
  setColour(juce::TabbedButtonBar::tabOutlineColourId, getAccentColour().withAlpha(0.5f));
  setColour(juce::TabbedButtonBar::tabTextColourId, getTextColour());
}

juce::Font FluxLookAndFeel::getLabelFont(juce::Label& label) {
  return Fonts::getFont(Fonts::Type::UI, 14.0f);
}

juce::Font FluxLookAndFeel::getComboBoxFont(juce::ComboBox& box) {
  return Fonts::getFont(Fonts::Type::UI, 14.0f);
}

juce::Font FluxLookAndFeel::getPopupMenuFont() {
  return Fonts::getFont(Fonts::Type::UI, 14.0f);
}

void FluxLookAndFeel::drawButtonBackground(juce::Graphics& g,
                                          juce::Button& button,
                                          const juce::Colour& backgroundColour,
                                          bool shouldDrawButtonAsHighlighted,
                                          bool shouldDrawButtonAsDown) {
  auto bounds = button.getLocalBounds().toFloat();
  auto cornerSize = 4.0f;

  juce::Colour baseColour = backgroundColour;

  if (shouldDrawButtonAsDown) {
    baseColour = getAccentColour().withMultipliedBrightness(0.8f);
  } else if (shouldDrawButtonAsHighlighted) {
    baseColour = baseColour.withMultipliedBrightness(1.1f);
  }

  // If button is toggle and on, use accent color
  if (auto* tb = dynamic_cast<juce::TextButton*>(&button)) {
    if (tb->getToggleState()) {
      baseColour = getAccentColour();
    }
  }

  g.setColour(baseColour);
  g.fillRoundedRectangle(bounds, cornerSize);

  // Subtle border
  if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown) {
    g.setColour(getAccentColour());
    g.drawRoundedRectangle(bounds, cornerSize, 1.5f);
  } else {
    g.setColour(getSurfaceColour().brighter(0.1f));
    g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
  }
}

void FluxLookAndFeel::fillTextEditorBackground(juce::Graphics& g,
                                            int width,
                                            int height,
                                            juce::TextEditor& textEditor) {
  juce::ignoreUnused(textEditor);

  g.setColour(getSurfaceColour());
  g.fillRoundedRectangle(0, 0, width, height, 4.0f);
}

void FluxLookAndFeel::drawTextEditorOutline(juce::Graphics& g,
                                           int width,
                                           int height,
                                           juce::TextEditor& textEditor) {
  if (textEditor.isEnabled() && textEditor.hasKeyboardFocus(true)) {
    g.setColour(getAccentColour());
    g.drawRoundedRectangle(0.5f, 0.5f, width - 1, height - 1, 4.0f, 1.5f);
  } else {
    g.setColour(getSurfaceColour().brighter(0.2f));
    g.drawRoundedRectangle(0.5f, 0.5f, width - 1, height - 1, 4.0f, 1.0f);
  }
}

}  // namespace flux
