#include "EditController.h"

namespace flux {

EditController::EditController(EngineWrapper& engine)
    : engine_(engine), edit_(nullptr) {
  jassert(engine_.isInitialized());
}

EditController::~EditController() = default;

bool EditController::createNewEdit() {
  jassert(engine_.isInitialized());

  auto& engine = engine_.getEngine();

  // Create a new empty Edit with default properties
  juce::ValueTree editState(tracktion::engine::IDs::EDIT);
  editState.setProperty(tracktion::engine::IDs::APPNAME, "FLUX", nullptr);

  // Create the Edit instance
  edit_ = std::make_unique<tracktion::engine::Edit>(
      engine, editState, tracktion::engine::Edit::forEditing, nullptr, 0);

  if (!edit_) {
    DBG("FLUX: Failed to create Edit");
    return false;
  }

  // Set default tempo (120 BPM)
  if (auto* tempoSequence = edit_->getTempoSequence()) {
    if (auto* tempo = tempoSequence->getTempoAt(0.0)) {
      tempo->setBpm(120.0);
    }
  }

  DBG("FLUX: Edit created successfully");
  return true;
}

tracktion::engine::Edit* EditController::getEdit() {
  return edit_.get();
}

tracktion::engine::TransportControl* EditController::getTransport() {
  if (!edit_) {
    return nullptr;
  }
  return &edit_->getTransport();
}

tracktion::engine::AudioTrack* EditController::createAudioTrack() {
  if (!edit_) {
    return nullptr;
  }

  // Insert an audio track at the end
  auto& trackList = edit_->getTrackList();
  auto* track = edit_->insertNewAudioTrack(trackList.size(), nullptr);

  return dynamic_cast<tracktion::engine::AudioTrack*>(track);
}

tracktion::engine::TempoSequence* EditController::getTempoSequence() {
  if (!edit_) {
    return nullptr;
  }
  return &edit_->getTempoSequence();
}

bool EditController::renderToFile(const juce::File& outputFile,
                                 double durationSeconds,
                                 juce::String& error) {
  if (!edit_) {
    error = "No edit available";
    return false;
  }

  // Set up render parameters
  tracktion::engine::EditTimeRange range(0.0, durationSeconds);

  // Create render parameters
  juce::StringPairArray metadata;
  tracktion::engine::Renderer::Parameters params(
      *edit_, tracktion::engine::TrackMuteState(*edit_, false));

  // Set output file
  juce::FileOutputStream* stream = new juce::FileOutputStream(outputFile);
  if (stream->failedToOpen()) {
    error = "Failed to open output file: " + outputFile.getFullPathName();
    delete stream;
    return false;
  }

  // Configure the render
  params.destFile = outputFile;
  params.bitDepth = 16;
  params.sampleRate = 44100;
  params.time = range;
  params.audioFormat = juce::WavAudioFormat().getFormatName();
  params.createMidiFile = false;

  // Perform the render
  juce::String renderError;
  bool success = tracktion::engine::Renderer::renderToFile(
      "Render", params, 0.0, true, true, &renderError, nullptr);

  if (!success) {
    error = renderError.isEmpty() ? "Render failed" : renderError;
    return false;
  }

  return true;
}

}  // namespace flux
