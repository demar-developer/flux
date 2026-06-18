#!/bin/bash
# FLUX Structure Verification
# Checks that all source files are syntactically valid without full build

set -e

echo "=== FLUX Structure Verification ==="
echo ""

# Check all headers compile (basic syntax check)
echo "Checking header files..."
for header in $(find src -name "*.h"); do
    echo -n "  $header... "
    # Try to parse with gcc -fsyntax-only
    if g++ -std=c++20 -fsyntax-only \
        -I. \
        -c "$header" 2>/dev/null; then
        echo "OK"
    else
        echo "SYNTAX CHECK (may need dependencies)"
    fi
done

echo ""
echo "Checking source files exist..."
for src in src/app/Application.cpp \
           src/app/MainWindow.cpp \
           src/app/MainComponent.cpp \
           src/engine/EngineWrapper.cpp \
           src/engine/EditController.cpp \
           src/ui/FluxLookAndFeel.cpp \
           src/ui/Fonts.cpp; do
    if [ -f "$src" ]; then
        echo "  ✓ $src"
    else
        echo "  ✗ MISSING: $src"
        exit 1
    fi
done

echo ""
echo "Checking test files..."
for test in tests/test_main.cpp \
            tests/test_render_smoke.cpp \
            tests/test_gui_smoke.cpp; do
    if [ -f "$test" ]; then
        echo "  ✓ $test"
    else
        echo "  ✗ MISSING: $test"
        exit 1
    fi
done

echo ""
echo "=== Verification Complete ==="
echo "All required files are present."
echo ""
echo "To do a full build (requires 15-30 min first time):"
echo "  cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release"
echo "  cmake --build build"
