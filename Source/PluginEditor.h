/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SimpleVSTAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::MidiKeyboardStateListener
{
public:
    SimpleVSTAudioProcessorEditor (SimpleVSTAudioProcessor&);
    ~SimpleVSTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleVSTAudioProcessor& audioProcessor;
    
    juce::GroupComponent oscillatorsGroup; // Ocillators
    juce::ComboBox waveSelector;
    
    juce::GroupComponent reverbGroup;
    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetDrySlider;
    juce::Slider widthSlider;
    juce::Label roomSizeLabel;
    juce::Label dampingLabel;
    juce::Label wetDryLabel;
    juce::Label widthLabel;
    
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent midiKeyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleVSTAudioProcessorEditor)
};
