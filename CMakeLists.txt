cmake_minimum_required(VERSION 3.15)
project(PedalPower VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE as a subdirectory
add_subdirectory(JUCE)

# Initialize JUCE
juce_add_plugin(PedalPower
    VERSION 1.0.0
    COMPANY_NAME "YourCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Ycom
    PLUGIN_CODE Pdp1
    FORMATS VST3
    PRODUCT_NAME "Pedal Power")

# Add source files
target_sources(PedalPower
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp)

# Set include directories
target_include_directories(PedalPower
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Link JUCE modules
target_link_libraries(PedalPower
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Add JUCE module headers
juce_generate_juce_header(PedalPower) 