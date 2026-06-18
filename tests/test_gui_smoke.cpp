// FLUX GUI Smoke Test
//
// This test verifies that the FLUX application can:
// 1. Launch without crashing
// 2. Initialize the JUCE message manager
// 3. Create the engine and edit
// 4. Open a window with the FLUX theme

#include <gtest/gtest.h>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>

#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

class GUISmokeTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Ensure we have a message manager
    if (!juce::MessageManager::getInstanceWithoutCreating()) {
      messageManager_ = std::make_unique<juce::Initialiser>(nullptr);
    }
  }

  void TearDown() override {
    messageManager_.reset();
  }

  std::unique_ptr<juce::Initialiser> messageManager_;
};

/**
 * Test: JUCE Message Manager can be initialized
 */
TEST_F(GUISmokeTest, MessageManagerInitialization) {
  EXPECT_NE(juce::MessageManager::getInstanceWithoutCreating(), nullptr);
}

/**
 * Test: Can create and show a simple JUCE window
 */
TEST_F(GUISmokeTest, WindowCreation) {
  juce::MessageManager::callAsync([]() {
    // Create a simple test window
    auto* window = new juce::DocumentWindow(
        "FLUX Test",
        juce::Colours::darkgrey,
        juce::DocumentWindow::closeButton);

    // Set size and show
    window->setSize(400, 300);
    window->setVisible(true);

    // Close immediately (this is a smoke test, not a functional test)
    window->closeButtonPressed();

    // Clean up
    delete window;
  });

  // Process messages for a bit
  for (int i = 0; i < 10; ++i) {
    juce::MessageManager::getInstance()->runDispatchLoopUntil(10);
    std::this_thread::sleep_for(10ms);
  }

  // If we got here without crashing, the test passes
  SUCCEED();
}

/**
 * Test: GUI components can be instantiated
 */
TEST_F(GUISmokeTest, ComponentCreation) {
  // Create various UI components
  juce::Component parent;

  juce::TextButton button("Test");
  juce::Label label("label", "Test Label");
  juce::Slider slider;
  juce::ComboBox comboBox;
  juce::TextEditor textEditor;

  // Add to parent
  parent.addAndMakeVisible(button);
  parent.addAndMakeVisible(label);
  parent.addAndMakeVisible(slider);
  parent.addAndMakeVisible(comboBox);
  parent.addAndMakeVisible(textEditor);

  // Set bounds
  parent.setSize(400, 300);
  button.setBounds(10, 10, 100, 30);
  label.setBounds(10, 50, 100, 30);
  slider.setBounds(10, 90, 200, 30);
  comboBox.setBounds(10, 130, 150, 30);
  textEditor.setBounds(10, 170, 200, 30);

  // Trigger a paint (will paint to a dummy context)
  juce::Image dummyImage(juce::Image::RGB, 400, 300, true);
  juce::Graphics g(dummyImage);
  parent.paint(g);

  // If we got here without crashing, the test passes
  SUCCEED();
}

/**
 * Test: Application entry point can be invoked
 *
 * This test checks that the FLUX application binary can be executed
 * with the --smoke-test flag and exits cleanly.
 */
TEST_F(GUISmokeTest, ApplicationSmokeTest) {
  // Get the path to the FLUX binary
  const char* testBinDir = FLUX_TEST_BIN_DIR;

  // Construct the binary path (adjust for build type)
  juce::String binaryPath = juce::String(testBinDir) +
                            "/FLUX_artefacts/Release/FLUX";

  // If Release doesn't exist, try Debug
  if (!juce::File(binaryPath).existsAsFile()) {
    binaryPath = juce::String(testBinDir) +
                 "/FLUX_artefacts/Debug/FLUX";
  }

  // Check if binary exists
  juce::File binaryFile(binaryPath);
  if (!binaryFile.existsAsFile()) {
    std::cout << "FLUX binary not found at: " << binaryPath.toStdString() << std::endl;
    GTEST_SKIP() << "FLUX binary not built yet";
    return;
  }

  // Run the binary with --smoke-test flag
  juce::String command = binaryPath + " --smoke-test";

  std::cout << "Running: " << command.toStdString() << std::endl;

  int exitCode = std::system(command.toStdString().c_str());

  // Check that the application exited cleanly
  // On Linux, exit code 0 means success
  EXPECT_EQ(exitCode, 0) << "FLUX application exited with code: " << exitCode;
}
