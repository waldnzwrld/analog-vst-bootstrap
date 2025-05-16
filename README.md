# Pedal Power - Guitar Pedal VST

A VST plugin that emulates guitar pedal effects. Built with JUCE framework.

## Prerequisites

- CMake (version 3.15 or higher)
- C++17 compatible compiler
- Git
- VST3 SDK

## Building the Project

1. Clone this repository with submodules:
```bash
git clone --recursive https://github.com/waldnzwrld/first-vst.git
# Or if you've already cloned without --recursive:
git submodule update --init --recursive
```

2. Build the project:
```bash
# Build the project
make

# Or for a clean rebuild
make rebuild

# To see all available commands
make help
```

## Features

- Basic guitar pedal emulation
- Real-time audio processing
- VST3 plugin format

## Development

This project uses the JUCE framework for audio processing and VST development. The main components are:

- `Source/PluginProcessor.cpp` - Audio processing logic
- `Source/PluginEditor.cpp` - User interface
- `Source/PluginProcessor.h` - Plugin processor header
- `Source/PluginEditor.h` - Plugin editor header

## License

MIT License 
