#include "EngineWrapper.h"

#include <juce_core/juce_core.h>

namespace flux {

EngineWrapper::EngineWrapper() = default;

EngineWrapper::~EngineWrapper() {
  shutdown();
}

bool EngineWrapper::initialize() {
  if (initialized_) {
    return true;
  }

  // Create the engine with application properties
  juce::PropertiesFile::Options options;
  options.applicationName = "FLUX";
  options.folderName = "FLUXAudio";
  options.filenameSuffix = "settings";

  auto* properties = new juce::ApplicationProperties();
  properties->setStorageParameters(options);

  // Create the Tracktion Engine
  engine_ = std::make_unique<tracktion::engine::Engine>(
      "FLUX", std::make_unique<tracktion::engine::EngineBehaviour>(),
      std::unique_ptr<juce::ApplicationProperties>(properties));

  if (!engine_) {
    DBG("FLUX: Failed to create Tracktion Engine");
    return false;
  }

  // Initialize the device manager (don't open devices in headless mode)
  auto& deviceManager = engine_->getDeviceManager();

  // In headless/CI mode, we don't need to open audio devices
  // Just ensure the manager is set up
  if (deviceManager.getCurrentAudioDevice() == nullptr) {
    // No device available — this is OK for headless/render-only operation
    DBG("FLUX: No audio device available — running in headless mode");
  }

  initialized_ = true;
  DBG("FLUX: Engine initialized successfully");
  return true;
}

void EngineWrapper::shutdown() {
  if (!initialized_ || !engine_) {
    return;
  }

  // The engine cleans up its own resources
  engine_.reset();
  initialized_ = false;

  DBG("FLUX: Engine shut down");
}

tracktion::engine::Engine& EngineWrapper::getEngine() {
  jassert(engine_ != nullptr);
  return *engine_;
}

tracktion::engine::DeviceManager& EngineWrapper::getDeviceManager() {
  jassert(engine_ != nullptr);
  return engine_->getDeviceManager();
}

bool EngineWrapper::isInitialized() const {
  return initialized_ && engine_ != nullptr;
}

}  // namespace flux
