#include "MainWindow.h"

#include "../ui/FluxLookAndFeel.h"

namespace flux {

MainWindow::MainWindow(const juce::String& name, EngineWrapper& engine)
    : DocumentWindow(name,
                    FluxLookAndFeel::getBackgroundColour(),
                    DocumentWindow::allButtons,
                    true) {
  // Create and set the main component
  mainComponent_ = std::make_unique<MainComponent>(engine);
  setContentOwned(mainComponent_.release(), true);

  // Window settings
  setResizable(true, true);
  setUsingNativeTitleBar(true);
  setTitleBarTextCentred(false);

  // Default size
  setSize(1280, 800);
  centreWithSize(getWidth(), getHeight());

  // Show the window
  setVisible(true);

  DBG("FLUX: Main window created");
}

MainWindow::~MainWindow() = default;

void MainWindow::closeButtonPressed() {
  DBG("FLUX: Close button pressed");

  // Clean exit through the application
  JUCEApplication::getInstance()->systemRequestedQuit();
}

}  // namespace flux
