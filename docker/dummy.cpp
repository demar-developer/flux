// Dummy file for pre-building dependencies
#include <juce_core/juce_core.h>
#include <tracktion_engine/tracktion_engine.h>

int main() {
    // Just reference some symbols to force linking
    juce::ignoreUnused(tracktion::engine::Engine::xmlTypeName);
    return 0;
}
