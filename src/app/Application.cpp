#include "Application.h"

#include "../ui/FluxLookAndFeel.h"

namespace flux {

bool Application::smokeTestMode_ = false;
bool Application::smokeTestSuccess_ = false;

Application::Application() = default;

Application::~Application() = default;

const juce::String Application::getApplicationName() {
  return "FLUX";
}

const juce::String Application::getApplicationVersion() {
  return "0.1.0";
}

bool Application::moreThanOneInstanceAllowed() {
  return true;
}

void Application::initialise(const juce::String& commandLine) {
  DBG("FLUX: Initializing application...");

  // Check for smoke test mode
  if (commandLine.contains("--smoke-test")) {
    smokeTestMode_ = true;
    DBG("FLUX: Running in smoke test mode");
  }

  // Initialize the LookAndFeel
  juce::LookAndFeel::setDefaultLookAndFeel(new FluxLookAndFeel());

  // Initialize the engine
  engine_ = std::make_unique<EngineWrapper>();
  if (!engine_->initialize()) {
    DBG("FLUX: Failed to initialize engine!");
    quit();
    return;
  }

  // Create the main window
  mainWindow_ = std::make_unique<MainWindow>(getApplicationName(), *engine_);

  // In smoke test mode, start a timer to exit after window is shown
  if (smokeTestMode_) {
    DBG("FLUX: Smoke test - scheduling exit");
    juce::Timer::callAfterDelay(500, [this]() {
      DBG("FLUX: Smoke test window verification complete");
      signalSmokeTestSuccess();
      systemRequestedQuit();
    });
  }

  DBG("FLUX: Application initialized successfully");
}

void Application::shutdown() {
  DBG("FLUX: Shutting down...");

  // Clean up in reverse order
  mainWindow_.reset();

  if (engine_) {
    engine_->shutdown();
    engine_.reset();
  }

  DBG("FLUX: Shutdown complete");
}

void Application::systemRequestedQuit() {
  DBG("FLUX: System requested quit");
  quit();
}

void Application::anotherInstanceStarted(const juce::String& commandLine) {
  juce::ignoreUnused(commandLine);
  // Bring window to front if another instance tries to start
  if (mainWindow_ != nullptr) {
    mainWindow_->toFront(true);
  }
}

bool Application::isSmokeTestMode() {
  return smokeTestMode_;
}

void Application::signalSmokeTestSuccess() {
  smokeTestSuccess_ = true;
}

EngineWrapper& Application::getEngine() {
  jassert(engine_ != nullptr);
  return *engine_;
}

Application* Application::getApp() {
  return dynamic_cast<Application*>(JUCEApplication::getInstance());
}

}  // namespace flux

// Entry point
START_JUCE_APPLICATION(flux::Application)
