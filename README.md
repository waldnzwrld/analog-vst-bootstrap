# Pedal Power - Guitar Pedal VST

A VST plugin that emulates guitar pedal effects. Built with JUCE framework.

## Prerequisites

- CMake (version 3.15 or higher)
- C++17 compatible compiler
- JUCE framework
- VST3 SDK

## Building the Project

1. Clone this repository
2. Install JUCE framework
3. Run the following commands:
```bash
mkdir build
cd build
cmake ..
cmake --build .
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