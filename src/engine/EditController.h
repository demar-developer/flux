#pragma once

#include "EngineWrapper.h"

#include <tracktion_engine/tracktion_engine.h>

namespace flux {

/**
 * EditController manages a Tracktion Edit (the project/document).
 * Provides operations for creating tracks, adding clips, transport control, etc.
 */
class EditController {
public:
  explicit EditController(EngineWrapper& engine);
  ~EditController();

  // Non-copyable, non-movable
  EditController(const EditController&) = delete;
  EditController& operator=(const EditController&) = delete;
  EditController(EditController&&) = delete;
  EditController& operator=(EditController&&) = delete;

  /**
   * Create a new empty Edit.
   */
  bool createNewEdit();

  /**
   * Get the underlying Tracktion Edit.
   */
  tracktion::engine::Edit* getEdit();

  /**
   * Get the transport control for playback.
   */
  tracktion::engine::TransportControl* getTransport();

  /**
   * Create an audio track.
   */
  tracktion::engine::AudioTrack* createAudioTrack();

  /**
   * Get the tempo sequence (for setting BPM).
   */
  tracktion::engine::TempoSequence* getTempoSequence();

  /**
   * Render the edit to a file (offline).
   */
  bool renderToFile(const juce::File& outputFile,
                   double durationSeconds,
                   juce::String& error);

private:
  EngineWrapper& engine_;
  std::unique_ptr<tracktion::engine::Edit> edit_;
};

}  // namespace flux
