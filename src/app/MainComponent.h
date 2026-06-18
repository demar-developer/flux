#pragma once

#include "../engine/EngineWrapper.h"
#include "../engine/EditController.h"

#include <juce_gui_basics/juce_gui_basics.h>

namespace flux {

/**
 * Main component containing the FLUX interface.
 * For Session 1 (foundation), this is a placeholder showing the theme.
 */
class MainComponent : public juce::Component {
public:
  explicit MainComponent(EngineWrapper& engine);
  ~MainComponent() override;

  // Component interface
  void paint(juce::Graphics& g) override;
  void resized() override;

private:
  void initializeEdit();

  EngineWrapper& engine_;
  std::unique_ptr<EditController> editController_;

  // UI elements for Session 1
  juce::Label titleLabel_;
  juce::Label statusLabel_;
  juce::TextButton testButton_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

}  // namespace flux
