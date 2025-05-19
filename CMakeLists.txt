cmake_minimum_required(VERSION 3.15)
project(ArchitextureStudiosAnalogCore VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# use build caching
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Add JUCE as a subdirectory
add_subdirectory(JUCE)

# Cache the JUCE build so that compilation is faster
set(JUCE_BUILD_DIR "${CMAKE_BINARY_DIR}/juce_build" CACHE PATH "JUCE build directory") 

# Initialize JUCE
juce_add_plugin(ArchitextureStudiosAnalogCore
    VERSION 1.0.0
    COMPANY_NAME "Architexture Studios"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Ycom
    PLUGIN_CODE Pdp1
    FORMATS VST3
    PRODUCT_NAME "Bass Fukz")

# Add source files
target_sources(ArchitextureStudiosAnalogCore
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/Components/Analog/Capacitor.cpp
        Source/Components/Analog/Transistor.cpp
        Source/Components/Analog/Diode.cpp
        Source/Components/Analog/Resistor.cpp
        Source/Components/Analog/Potentiometer.cpp
        Source/Circuits/TransistorClipper.cpp)

# Set include directories
target_include_directories(ArchitextureStudiosAnalogCore
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Add JUCE module headers
juce_generate_juce_header(ArchitextureStudiosAnalogCore)

# Disable precompiled headers for now as they're causing issues
set_target_properties(ArchitextureStudiosAnalogCore PROPERTIES
    DISABLE_PRECOMPILE_HEADERS ON)

# Link JUCE modules
target_link_libraries(ArchitextureStudiosAnalogCore
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_audio_basics
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)


# Add custom test target
add_custom_target(test
    COMMAND ${CMAKE_BINARY_DIR}/Source/Tests/TransistorClipperTest
    DEPENDS TransistorClipperTest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running TransistorClipper tests..."
)

