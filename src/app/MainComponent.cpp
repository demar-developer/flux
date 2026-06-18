#include "MainComponent.h"

#include "../ui/FluxLookAndFeel.h"
#include "../ui/Fonts.h"

namespace flux {

MainComponent::MainComponent(EngineWrapper& engine)
    : engine_(engine),
      titleLabel_("titleLabel", "FLUX"),
      statusLabel_("statusLabel", "Initializing..."),
      testButton_("testButton", "Click Me") {
  // Initialize fonts
  Fonts::loadCustomFonts();

  // Set up the title label
  titleLabel_.setFont(Fonts::getFont(Fonts::Type::Display, 48.0f));
  titleLabel_.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(titleLabel_);

  // Set up the status label
  statusLabel_.setFont(Fonts::getFont(Fonts::Type::UI, 16.0f));
  statusLabel_.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(statusLabel_);

  // Set up the test button
  testButton_.setClickingTogglesState(true);
  testButton_.onClick = [this]() {
    bool toggled = testButton_.getToggleState();
    statusLabel_.setText(toggled ? "Button: ON (Accent color active)"
                                 : "Button: OFF",
                        juce::dontSendNotification);
  };
  addAndMakeVisible(testButton_);

  // Initialize the edit
  initializeEdit();

  DBG("FLUX: MainComponent created");
}

MainComponent::~MainComponent() = default;

void MainComponent::initializeEdit() {
  // Create an EditController and set up an empty edit
  editController_ = std::make_unique<EditController>(engine_);

  if (editController_->createNewEdit()) {
    statusLabel_.setText("Engine initialized | Edit created | BPM: 120",
                        juce::dontSendNotification);
    DBG("FLUX: Edit created successfully");
  } else {
    statusLabel_.setText("Engine initialized | Edit creation failed",
                        juce::dontSendNotification);
    DBG("FLUX: Failed to create edit");
  }
}

void MainComponent::paint(juce::Graphics& g) {
  // Fill background with FLUX dark color
  g.fillAll(FluxLookAndFeel::getBackgroundColour());

  // Draw a subtle accent line at the top
  auto accentY = 60.0f;
  g.setColour(FluxLookAndFeel::getAccentColour());
  g.drawLine(0.0f, accentY, getWidth(), accentY, 3.0f);

  // Draw a subtle grid pattern (FL Studio-inspired)
  g.setColour(FluxLookAndFeel::getSurfaceColour().withAlpha(0.5f));
  const float gridSize = 20.0f;
  for (float x = 0; x < getWidth(); x += gridSize) {
    g.drawLine(x, 0.0f, x, getHeight(), 1.0f);
  }
  for (float y = 0; y < getHeight(); y += gridSize) {
    g.drawLine(0.0f, y, getWidth(), y, 1.0f);
  }

  // Draw a placeholder "coming soon" box in the center
  auto centerBox = getLocalBounds()
                      .withSizeKeepingCentre(400, 200)
                      .reduced(10);

  g.setColour(FluxLookAndFeel::getSurfaceColour());
  g.fillRoundedRectangle(centerBox.toFloat(), 8.0f);

  g.setColour(FluxLookAndFeel::getAccentColour().withAlpha(0.5f));
  g.drawRoundedRectangle(centerBox.toFloat(), 8.0f, 2.0f);

  // Center box text
  g.setColour(FluxLookAndFeel::getMutedTextColour());
  g.setFont(Fonts::getFont(Fonts::Type::UI, 14.0f));
  g.drawFittedText(
      "Session 1: Foundation\n\n"
      "Engine: OK\n"
      "Edit: OK\n"
      "Theme: Loaded (FF8A4C)\n"
      "Fonts: System fallbacks",
      centerBox.reduced(20),
      juce::Justification::centred,
      5);
}

void MainComponent::resized() {
  auto bounds = getLocalBounds();

  // Title at top
  titleLabel_.setBounds(bounds.removeFromTop(80).reduced(20, 10));

  // Status bar at bottom
  auto bottomBar = bounds.removeFromBottom(40).reduced(20, 5);
  statusLabel_.setBounds(bottomBar);

  // Test button in the top-right area
  testButton_.setBounds(getWidth() - 140, 70, 120, 30);
}

}  // namespace flux
