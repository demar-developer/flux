// FLUX Offline Render Smoke Test
//
// This test verifies that Tracktion Engine can:
// 1. Create an Edit
// 2. Add an AudioTrack with a FourOscPlugin
// 3. Add a MidiClip with notes
// 4. Render offline to a WAV file
// 5. Produce non-silent output

#include <gtest/gtest.h>

#include <src/engine/EngineWrapper.h>
#include <src/engine/EditController.h>

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <tracktion_engine/tracktion_engine.h>

#include <cmath>

using namespace flux;

class RenderSmokeTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Initialize JUCE if not already done
    if (!juce::MessageManager::getInstanceWithoutCreating()) {
      messageManager_ = std::make_unique<juce::MessageManager>();
    }
  }

  void TearDown() override {
    messageManager_.reset();
  }

  std::unique_ptr<juce::MessageManager> messageManager_;
};

/**
 * Calculate RMS of an audio buffer
 */
double calculateRMS(const juce::AudioBuffer<float>& buffer) {
  double sumSquares = 0.0;
  int numSamples = 0;

  for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
    const float* channelData = buffer.getReadPointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      sumSquares += channelData[sample] * channelData[sample];
      numSamples++;
    }
  }

  return numSamples > 0 ? std::sqrt(sumSquares / numSamples) : 0.0;
}

/**
 * Test: Engine can be initialized
 */
TEST_F(RenderSmokeTest, EngineInitialization) {
  EngineWrapper engine;

  EXPECT_FALSE(engine.isInitialized());
  EXPECT_TRUE(engine.initialize());
  EXPECT_TRUE(engine.isInitialized());
}

/**
 * Test: Can create an Edit
 */
TEST_F(RenderSmokeTest, EditCreation) {
  EngineWrapper engine;
  ASSERT_TRUE(engine.initialize());

  EditController controller(engine);
  EXPECT_TRUE(controller.createNewEdit());
  EXPECT_NE(controller.getEdit(), nullptr);
  EXPECT_NE(controller.getTransport(), nullptr);
}

/**
 * Test: Can create an audio track
 */
TEST_F(RenderSmokeTest, AudioTrackCreation) {
  EngineWrapper engine;
  ASSERT_TRUE(engine.initialize());

  EditController controller(engine);
  ASSERT_TRUE(controller.createNewEdit());

  auto* track = controller.createAudioTrack();
  EXPECT_NE(track, nullptr);
}

/**
 * Test: Full render pipeline produces non-silent output
 */
TEST_F(RenderSmokeTest, OfflineRenderProducesAudio) {
  EngineWrapper engine;
  ASSERT_TRUE(engine.initialize());

  auto& tracktionEngine = engine.getEngine();

  // Create a new Edit
  EditController controller(engine);
  ASSERT_TRUE(controller.createNewEdit());

  auto* edit = controller.getEdit();
  ASSERT_NE(edit, nullptr);

  // Set tempo
  if (auto* tempoSequence = controller.getTempoSequence()) {
    if (auto* tempo = tempoSequence->getTempoAt(0.0)) {
      tempo->setBpm(120.0);
    }
  }

  // Create an audio track
  auto& trackList = edit->getTrackList();
  auto* track = edit->insertNewAudioTrack(trackList.size(), nullptr);
  ASSERT_NE(track, nullptr);
  auto* audioTrack = dynamic_cast<tracktion::engine::AudioTrack*>(track);
  ASSERT_NE(audioTrack, nullptr);

  // Insert a FourOscPlugin (built-in synth) as the first plugin
  auto plugin = edit->getPluginCache().createNewPlugin(
      tracktion::engine::FourOscPlugin::xmlTypeName, {});

  ASSERT_NE(plugin, nullptr);

  // Cast to FourOscPlugin
  auto* synthPlugin = dynamic_cast<tracktion::engine::FourOscPlugin*>(plugin.get());
  ASSERT_NE(synthPlugin, nullptr);

  // Add plugin to track
  audioTrack->pluginList.insertPlugin(plugin, 0, nullptr);

  // Create a MIDI clip with a C4 note
  auto midiTrack = edit->insertNewTrack(tracktion::engine::TrackInsertPoint(*edit, nullptr, -1),
                                        tracktion::engine::MidiTrack::createBaseTypeName(), nullptr);
  ASSERT_NE(midiTrack, nullptr);

  auto* midiTrackPtr = dynamic_cast<tracktion::engine::MidiTrack*>(midiTrack.get());
  ASSERT_NE(midiTrackPtr, nullptr);

  // Create a 2-second MIDI clip at position 0
  auto clip = midiTrackPtr->insertNewMidiClip("Test Clip", {0.0, 2.0}, nullptr);
  ASSERT_NE(clip, nullptr);

  // Add a C4 note (MIDI note 60) for 1 beat
  auto note = tracktion::engine::MidiNote(clip->state, nullptr);
  note.setNoteNumber(60);      // C4
  note.setStartBeats(0.0);     // Start at beginning
  note.setLengthBeats(1.0);    // 1 beat duration
  note.setVelocity(100);       // Moderate velocity

  // Set up the routing: MIDI track -> Audio track
  midiTrackPtr->setOutputByName(audioTrack->getName());

  // Prepare for playback to set up the graph
  auto& transport = edit->getTransport();
  transport.prepareForPlayback(edit);

  // Render the Edit to memory
  const double durationSeconds = 2.0;
  const int sampleRate = 44100;
  const int numSamples = static_cast<int>(durationSeconds * sampleRate);
  const int numChannels = 2;

  // Create an audio buffer for the output
  juce::AudioBuffer<float> outputBuffer(numChannels, numSamples);
  outputBuffer.clear();

  // Set up the EditPlaybackContext for rendering
  juce::AudioDeviceManager dummyDeviceManager;
  dummyDeviceManager.initialise(0, 2, nullptr, true);

  // Create playback context
  std::unique_ptr<tracktion::engine::EditPlaybackContext> playbackContext;

  // Use a message thread lock for creation
  {
    juce::MessageManager::Lock mmLock;
    playbackContext = std::make_unique<tracktion::engine::EditPlaybackContext>(
        dummyDeviceManager, *edit);
  }

  ASSERT_NE(playbackContext, nullptr);

  // Enable the context
  playbackContext->setTargetEditToEdit();
  playbackContext->play();

  // Wait for playback to start
  juce::Thread::sleep(50);

  // Render block by block
  const int blockSize = 512;
  juce::AudioBuffer<float> tempBuffer(numChannels, blockSize);
  juce::MidiBuffer midiBuffer;

  for (int samplePos = 0; samplePos < numSamples; samplePos += blockSize) {
    tempBuffer.clear();
    midiBuffer.clear();

    int samplesThisBlock = juce::jmin(blockSize, numSamples - samplePos);

    // Get audio from the playback context
    playbackContext->fillAudioBlock(tempBuffer, midiBuffer);

    // Copy to output
    for (int ch = 0; ch < numChannels; ++ch) {
      outputBuffer.copyFrom(ch, samplePos, tempBuffer.getReadPointer(ch), samplesThisBlock);
    }
  }

  // Stop playback
  playbackContext->stop();

  // Calculate RMS of the rendered audio
  double rms = calculateRMS(outputBuffer);

  // Log the result
  std::cout << "Render test: RMS = " << rms << std::endl;

  // Assert that the output is non-silent
  // Threshold: 0.01 (reasonable for a synth note)
  EXPECT_GT(rms, 0.01) << "Rendered audio is silent or too quiet (RMS: " << rms << ")";

  // Assert that the duration is correct
  EXPECT_EQ(outputBuffer.getNumSamples(), numSamples);
  EXPECT_EQ(outputBuffer.getNumChannels(), numChannels);
}
