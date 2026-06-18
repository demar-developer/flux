#pragma once

#include "../engine/EngineWrapper.h"
#include "MainComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

namespace flux {

/**
 * Main application window.
 */
class MainWindow : public juce::DocumentWindow {
public:
  MainWindow(const juce::String& name, EngineWrapper& engine);
  ~MainWindow() override;

  // DocumentWindow interface
  void closeButtonPressed() override;

private:
  std::unique_ptr<MainComponent> mainComponent_;
};

}  // namespace flux
