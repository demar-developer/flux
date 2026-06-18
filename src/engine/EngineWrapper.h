#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_core/juce_core.h>
#include <tracktion_engine/tracktion_engine.h>

namespace flux {

/**
 * EngineWrapper manages the Tracktion Engine and its device manager.
 * This is the entry point for all audio engine functionality.
 */
class EngineWrapper {
public:
  EngineWrapper();
  ~EngineWrapper();

  // Non-copyable, non-movable (Engine is a singleton-like entity)
  EngineWrapper(const EngineWrapper&) = delete;
  EngineWrapper& operator=(const EngineWrapper&) = delete;
  EngineWrapper(EngineWrapper&&) = delete;
  EngineWrapper& operator=(EngineWrapper&&) = delete;

  /**
   * Initialize the engine. Must be called before use.
   * Returns true on success, false on failure.
   */
  bool initialize();

  /**
   * Shutdown the engine and release resources.
   */
  void shutdown();

  /**
   * Get the underlying Tracktion Engine.
   */
  tracktion::engine::Engine& getEngine();

  /**
   * Get the device manager for audio I/O.
   */
  tracktion::engine::DeviceManager& getDeviceManager();

  /**
   * Check if the engine is initialized.
   */
  bool isInitialized() const;

private:
  std::unique_ptr<tracktion::engine::Engine> engine_;
  bool initialized_ = false;
};

}  // namespace flux
