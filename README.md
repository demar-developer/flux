# FLUX

A pattern-based DAW for desktop (Windows, macOS, Linux) modeled on FL Studio's workflow. Built with C++20, JUCE 8, and Tracktion Engine.

## Status

**Session 1: Foundation** — Buildable app with Tracktion Engine integration, empty Edit, minimal window, and headless CI tests.

## Quick Start

### Option 1: Docker (Recommended — Fastest)

The Docker builder image has JUCE and Tracktion Engine pre-cached:

```bash
# Build everything (first time builds the image, ~10 min)
docker-compose run --rm builder

# Run tests
docker-compose run --rm test

# Interactive shell
docker-compose run --rm shell
```

### Option 2: Native Build

**Ubuntu/Debian:**
```bash
./setup.sh  # Install dependencies
```

**Then build:**
```bash
./scripts/build.sh        # Release build
./scripts/build.sh Debug  # Debug build
```

### Run

```bash
./scripts/run.sh           # Launch FLUX
xvfb-run ./scripts/run.sh  # Headless (CI)
```

### Test

```bash
./scripts/test.sh          # Run all tests (includes smoke tests)
ctest --test-dir build     # Direct CTest
```

## Development

```bash
# One-time setup
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run specific test
./build/tests/flux_tests --gtest_filter=RenderSmoke*

# With ccache (recommended)
cmake -B build -G Ninja -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
```

## Dependencies

| Dependency | Version | Commit/Tag | URL |
|------------|---------|------------|-----|
| JUCE | 8.0.13 | `7c9d378` | https://github.com/juce-framework/JUCE |
| Tracktion Engine | 3.2.0 | `0a5f4e6` | https://github.com/Tracktion/tracktion_engine |

Dependencies are fetched via CMake FetchContent and cached after first build.

## Architecture

See `docs/ARCHITECTURE.md` for full specification.

Briefly:
- **Engine Layer** (`src/engine/`) — wrapper over Tracktion
- **Model Layer** (`src/model/`) — FLUX-specific abstractions (Pattern, Channel)
- **UI Layer** (`src/views/`, `src/ui/`) — JUCE components with custom LookAndFeel

## License

GPL v3 — JUCE and Tracktion Engine are dual-licensed GPLv3/commercial.

## Contributing

See `AGENTS.md` for agent-facing guidelines.

---

*Generated with [Devin](https://cli.devin.ai/docs)*
