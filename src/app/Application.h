#pragma once

#include "../engine/EngineWrapper.h"
#include "MainWindow.h"

#include <juce_gui_basics/juce_gui_basics.h>

namespace flux {

/**
 * FLUX Application entry point.
 */
class Application : public juce::JUCEApplication {
public:
  Application();
  ~Application() override;

  // JUCEApplication interface
  const juce::String getApplicationName() override;
  const juce::String getApplicationVersion() override;
  bool moreThanOneInstanceAllowed() override;
  void initialise(const juce::String& commandLine) override;
  void shutdown() override;
  void systemRequestedQuit() override;
  void anotherInstanceStarted(const juce::String& commandLine) override;

  /**
   * Check if running in smoke test mode.
   */
  static bool isSmokeTestMode();

  /**
   * Signal that smoke test passed.
   */
  static void signalSmokeTestSuccess();

  /**
   * Get the engine wrapper.
   */
  EngineWrapper& getEngine();

  /**
   * Get the application instance.
   */
  static Application* getApp();

private:
  std::unique_ptr<EngineWrapper> engine_;
  std::unique_ptr<MainWindow> mainWindow_;

  static bool smokeTestMode_;
  static bool smokeTestSuccess_;
};

}  // namespace flux
