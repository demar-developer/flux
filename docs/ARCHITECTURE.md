# FLUX Architecture

> Master project brief — the canonical reference for FLUX design and implementation.

## 1. Vision

FLUX is a desktop DAW modeled on FL Studio's pattern-based workflow: a Channel Rack step sequencer feeds named Patterns, and a Playlist arranges instances of those patterns on a song timeline. It also supports a conventional linear arrangement for audio recording. Four primary views: Channel Rack, Piano Roll, Playlist, Mixer. Modern 2026 aesthetic, professional-grade UX.

A validated browser prototype of these four views and their interactions already exists and is the canonical interaction + visual reference (commit it to `design/prototype/` and match its layout, interaction logic, and design tokens).

## 2. Tech Stack & Non-Negotiables

| Aspect | Choice |
|--------|--------|
| Language | C++20 |
| Build | CMake ≥ 3.22 |
| Framework | JUCE 8 |
| Engine | Tracktion Engine v3.2.0 (commit 0a5f4e6) |
| Testing | GoogleTest + CTest |
| Targets | Windows, macOS, Linux (dev/CI on Linux) |

Dependencies are **pinned to explicit commits/tags** via FetchContent. Reproducible from a clean clone.

## 3. Repository Structure

```
flux/
├── CMakeLists.txt
├── AGENTS.md                 # Condensed brief for agents
├── README.md                 # Build/run, pinned dependency commits
├── setup.sh                  # Installs Linux build deps
├── scripts/{build.sh,test.sh,run.sh}
├── cmake/                    # JUCE/Tracktion fetch + helpers
├── docs/ARCHITECTURE.md      # Full master brief (this file)
├── design/prototype/         # HTML prototype (visual reference)
├── src/
│   ├── app/                  # Application, MainWindow, MainComponent
│   ├── engine/               # Tracktion wrapper: EngineWrapper, EditController
│   ├── model/                # FLUX-level abstractions: Pattern, PatternBank, etc.
│   ├── views/
│   │   ├── channelrack/      # Step sequencer
│   │   ├── pianoroll/        # MIDI editor
│   │   ├── playlist/         # Pattern arrangement
│   │   └── mixer/            # Channel strips
│   ├── ui/                   # FluxLookAndFeel, shared widgets
│   └── plugins/              # Plugin hosting glue
└── tests/                    # GoogleTest: engine, model, render-smoke, gui-smoke
```

## 4. Architecture

### Three Layers

1. **Engine Layer** (`src/engine/`) — thin wrapper over Tracktion. Owns one `tracktion::Engine`, active `tracktion::Edit`, `tracktion::TransportControl`, and `tracktion::DeviceManager`. Exposes intent-level operations (create track, add clip, arm/record, etc.). The UI never touches Tracktion internals directly.

2. **FLUX Model Layer** (`src/model/`) — the pattern abstraction FL Studio has and Tracktion does not. See Section 5. Owns `Pattern`, `PatternBank` (A/B/C/D), `Channel`, and `Project`. Persists alongside Tracktion Edit state.

3. **UI Layer** (`src/views/`, `src/ui/`) — JUCE Components for the four views, driven by custom `FluxLookAndFeel`. Reflects engine/model state; sends user intents back down.

### Key Tracktion Classes

- Core: `Engine`, `Edit`, `TransportControl`, `EditPlaybackContext`, `DeviceManager`
- Devices: `WaveInputDevice`, `WaveOutputDevice`, `MidiInputDevice`
- Tracks: `AudioTrack`, `FolderTrack`, `TempoTrack`, `MarkerTrack`, `AutomationTrack`
- Clips: `WaveAudioClip`, `MidiClip`, `StepClip`, `EditClip`, `ContainerClip`
- Plugins: `ExternalPlugin`, `VolumeAndPanPlugin`, `LevelMeterPlugin`, `AuxSendPlugin`, `AuxReturnPlugin`, `RackType`, `SamplerPlugin`, `FourOscPlugin`
- Automation: `AutomatableParameter`, `AutomationCurve`, `AutomationRecordManager`
- Hosting: `PluginManager`, `knownPluginList`
- Rendering: `Renderer`, `EditRenderer`

## 5. The Pattern/Playlist Model

FL Studio's workflow: **pattern → playlist**. Build a Pattern in Channel Rack, then drop instances onto Playlist timeline. Tracktion's native model is linear arrangement of clips.

### Recommended Approach

A **Pattern** is a FLUX-level object owning a collection of clips per channel:
- Drum content: `StepClip` (channels map to kick/snare/hat)
- Melodic content: `MidiClip`

Patterns are stored as **reusable content**, not glued to timeline position.

- Channel Rack edits the currently selected Pattern
- PAT mode loops that Pattern
- Playlist places pattern instances on song tracks

Two mechanisms for Playlist:
- (a) Composite Pattern content onto timeline as clips at each placement (simplest, FL-faithful)
- (b) Use Tracktion's clip launcher/scene support if present

**Prefer (a)** for FL fidelity. Stop and discuss after a spike before building full Playlist.

## 6. Feature → Engine Mapping

| Feature | Tracktion Implementation |
|---------|------------------------|
| Audio recording | `AudioTrack` + `WaveInputDevice`, record-arm, punch/loop → `WaveAudioClip` |
| MIDI/instrument tracks | `AudioTrack` with instrument plugin first, fed by `MidiClip` |
| Drum/step sequencing | `StepClip` |
| Mixer (vol/pan/sends) | `VolumeAndPanPlugin`, `AuxSendPlugin`, `AuxReturnPlugin` |
| Buses/submixes | `FolderTrack` or track-to-track routing |
| VST3 hosting | `ExternalPlugin` with VST3 format enabled |
| Automation | `AutomatableParameter` + `AutomationCurve` + `AutomationRecordManager` |

## 7. Design System

From the validated FLUX prototype (in `design/prototype/`):

### Colors
- **Hero accent**: `#FF8A4C` (warm amber) — active states, playhead, selection, transport-armed

### Typography
- **Space Grotesk**: display/headers
- **Inter**: UI labels/controls
- **JetBrains Mono**: numeric readouts (BPM, dB, time, step counters)

### Widgets
Implement in `FluxLookAndFeel` (subclass `juce::LookAndFeel_V4`):
- Rotary knob
- Channel fader
- Level meter
- Transport bar
- Step button
- Timeline ruler
- Automation lane

## 8. Plugin Hosting

- **Enable**: VST3 + AU (macOS) + LV2
- **Scan**: Out-of-process via `PluginManager` child-process scanner
- **Persist**: `knownPluginList` with blocklist for crashers
- **Sandbox**: Defensive instantiation, surface scan failures in UI

### Licensing Note
JUCE and Tracktion Engine are dual-licensed GPLv3 or commercial. VST3 SDK is dual-licensed GPLv3 or Steinberg proprietary. Shipping closed-source requires appropriate licenses.

## 9. Real-Time Safety

- Never allocate, lock, log, or block on audio thread
- No `new`/`delete`, no `std::mutex`, no file I/O in audio callback
- UI ↔ audio via lock-free FIFOs / atomics / Tracktion's parameter system
- All UI work on message thread via `juce::MessageManager`

## 10. Persistence

- Tracktion Edit: `juce::ValueTree`, serialize from `Edit::state`
- FLUX model: patterns, bank, channel metadata, view state — sidecar or embedded in project
- Undo: Tracktion's built-in `ValueTree` undo
- Format: `.flux` project bundle (versioned)

## 11. Environment (Linux)

Dependencies (Ubuntu):
```bash
sudo apt-get install -y \
  build-essential cmake ninja-build pkg-config \
  libasound2-dev libjack-jackd2-dev \
  libcurl4-openssl-dev libfreetype-dev libfontconfig1-dev \
  libx11-dev libxcomposite-dev libxcursor-dev libxext-dev \
  libxinerama-dev libxrandr-dev libxrender-dev \
  libglu1-mesa-dev mesa-common-dev libgtk-3-dev \
  libwebkit2gtk-4.1-dev ladspa-sdk xvfb
```

Headless runs via `xvfb-run`. Audio verification is offline rendering only.

## 12. Conventions

- Modern C++20, RAII, `std::unique_ptr` ownership
- No raw owning pointers
- Follow JUCE/Tracktion idioms
- clang-format (`.clang-format` committed)
- clang-tidy where practical
- Small, reviewable PRs
- Conventional commit messages
- Every PR keeps CI green

## 13. Non-Goals

Do NOT build:
- Cloud sync, accounts, telemetry/analytics
- Mobile or web versions
- AI/generative features
- Marketplace
- DRM or bundled third-party content

Keep scope to a local, professional DAW core.

## 14. Escalation Points

Stop and ask the user when:
1. A licensing gate appears (JUCE / Tracktion / VST3 SDK)
2. Pattern/Playlist design needs decision (after spike)
3. Pinned Tracktion API differs materially from this brief
4. Slice would exceed reasonable ACU/time budget
5. Any irreversible architecture choice (project format, plugin sandbox, undo strategy)

## 15. Dependency Versions

| Dependency | Version | Commit/Tag |
|------------|---------|------------|
| JUCE | 8.0.13 | `7c9d378` |
| Tracktion Engine | 3.2.0 | `0a5f4e6` |

---

*Generated for FLUX Session 1 — Foundation Slice*
