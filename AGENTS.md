# FLUX — Agent Brief

> Condensed reference for AI coding agents. Full spec in `docs/ARCHITECTURE.md`.

## Mission

Pattern-based DAW (FL Studio-style) using C++20, JUCE 8, and Tracktion Engine v3.2.0.

## Stack

- **C++20**, CMake ≥ 3.22
- **JUCE 8.0.13** (commit `7c9d378`)
- **Tracktion Engine v3.2.0** (commit `0a5f4e6`)
- **GoogleTest** + CTest
- Targets: Windows, macOS, Linux (dev on Linux)

## Architecture (3 Layers)

```
UI Layer (src/views/, src/ui/)
  ↓ intents
FLUX Model Layer (src/model/) — Pattern, PatternBank, Channel, Project
  ↓
Engine Layer (src/engine/) — wrapper over tracktion::Engine, Edit, TransportControl
```

**UI never touches Tracktion directly** — always through Engine layer.

## Key Classes

- **Engine**: `tracktion::Engine`, `Edit`, `TransportControl`, `DeviceManager`
- **Tracks**: `AudioTrack`, `FolderTrack`, `TempoTrack`
- **Clips**: `WaveAudioClip`, `MidiClip`, `StepClip`
- **Plugins**: `ExternalPlugin`, `VolumeAndPanPlugin`, `LevelMeterPlugin`, `AuxSendPlugin`, `RackType`, `FourOscPlugin`
- **Automation**: `AutomatableParameter`, `AutomationCurve`

## Design System

- **Accent**: `#FF8A4C` (warm amber)
- **Fonts**: Space Grotesk (headers), Inter (UI), JetBrains Mono (numeric)
- **LookAndFeel**: `FluxLookAndFeel` extends `juce::LookAndFeel_V4`

## Pattern/Playlist Model

Pattern = reusable content (StepClip for drums, MidiClip for melody). Channel Rack edits selected Pattern. Playlist places pattern instances on timeline as clips.

## Non-Goals

NO cloud, mobile, web, AI, telemetry, marketplace, DRM. Local DAW core only.

## Escalation

STOP and ask when:
1. Licensing gate (JUCE/Tracktion/VST3)
2. Pattern/Playlist design needs decision
3. Tracktion API differs from brief
4. Budget exceeded — propose caching/Docker
5. Irreversible architecture choice

## Build Commands

### Docker (Fast — Pre-cached JUCE/Tracktion)
```bash
docker-compose run --rm builder  # Build FLUX
docker-compose run --rm test     # Run tests
docker-compose run --rm shell    # Interactive dev shell
```

### Native (Slow — Builds JUCE/Tracktion from source)
```bash
./setup.sh                    # Install deps (~5 min)
./scripts/build.sh            # Build Release (~30 min first time)
./scripts/test.sh             # Run tests
```

## Testing

- **Offline render test**: Create Edit → add track → FourOscPlugin → render → assert non-silent
- **GUI smoke test**: `xvfb-run` app, verify window opens

## CI

GitHub Actions: Ubuntu, setup → build → ctest (includes both smoke tests)

---

*Session 1: Foundation — Buildable app with Engine + Edit + window + tests.*
