# analog-vst-bootstrap

This is a base level framework for recreating analog style functionality within a VST using modeled variants of electronic components. The project focuses on accurate modeling of analog circuits and their components, with particular attention to the non-linear behaviors that give analog circuits their characteristic sound.

## Prerequisites

- CMake (version 3.15 or higher)
- C++17 compatible compiler
- Git
- VST3 SDK

## Building the Project

1. Clone this repository with submodules:
```bash
git clone --recursive https://github.com/waldnzwrld/analog-vst-bootstrap.git
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

- Accurate analog component modeling:
  - Capacitors with ESR, ESL, and dielectric absorption
  - Diodes with temperature-dependent behavior
  - Transistors with full Ebers-Moll model
  - Resistors with temperature coefficients
  - Potentiometers with various taper types

- Circuit Implementations:
  - Transistor-based clipper with Darlington pair
  - Input/output capacitor coupling
  - Bias networks
  - Drive control via potentiometer

- Real-time audio processing
- VST3 plugin format

## Project Structure

The project is organized into several key components:

### Core Components
- `Source/Components/Analog/` - Analog component models
  - `Capacitor.h/cpp` - Capacitor model with ESR, ESL, and dielectric effects
  - `Diode.h/cpp` - Diode model with temperature effects
  - `Transistor.h/cpp` - Transistor model using Ebers-Moll equations
  - `Resistor.h/cpp` - Resistor model with temperature effects
  - `Potentiometer.h/cpp` - Potentiometer model with various tapers

### Circuit Implementations
- `Source/Circuits/` - Circuit implementations
  - `TransistorClipper.h/cpp` - Transistor-based clipper circuit

### Plugin Interface
- `Source/PluginProcessor.cpp` - Audio processing logic
- `Source/PluginEditor.cpp` - User interface
- `Source/PluginProcessor.h` - Plugin processor header
- `Source/PluginEditor.h` - Plugin editor header

## Development

This project uses the JUCE framework for audio processing and VST development. The focus is on accurate modeling of analog components and their interactions within circuits.

Key development areas:
1. Component Modeling - Adding new components or improving existing models
2. Circuit Implementation - Creating new circuits or refining existing ones
3. Plugin Interface - Enhancing the user interface and controls
