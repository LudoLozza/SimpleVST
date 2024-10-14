/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleVSTAudioProcessorEditor::SimpleVSTAudioProcessorEditor (SimpleVSTAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), oscillatorsGroup("Oscillators Group", "Oscillator 1"), reverbGroup ("Reverb Group", "Reverb"), midiKeyboard (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Add Oscillator 1
    addAndMakeVisible(oscillatorsGroup);
    //oscillatorLabel.setText ("Oscillator 1", juce::dontSendNotification);
    //oscillatorLabel.setJustificationType (juce::Justification::centredLeft);
    
    // Add waveform selector ComboBox
    addAndMakeVisible(waveSelector);
    waveSelector.addItem("Sine", 1);
    waveSelector.addItem("Triangle", 2);
    waveSelector.addItem("Sawtooth", 3);
    waveSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colours::black);
    waveSelector.setColour(juce::ComboBox::textColourId, juce::Colours::orangered);
    waveSelector.setColour(juce::ComboBox::outlineColourId, juce::Colours::darkgrey);
    waveSelector.setColour(juce::ComboBox::arrowColourId, juce::Colours::darkgrey);
    waveSelector.onChange = [this] { audioProcessor.setWaveform(waveSelector.getSelectedId() - 1); };
    waveSelector.setSelectedId(1);
    
    // Reverb group
    addAndMakeVisible(reverbGroup);

    // Room Size slider
    addAndMakeVisible(roomSizeSlider);
    roomSizeSlider.setRange(0.0, 1.0);
    roomSizeSlider.setSliderStyle(juce::Slider::Rotary);
    roomSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    roomSizeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::orangered);   // Knob color
    roomSizeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange); // Fill color
    roomSizeSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey); // Background color
    roomSizeSlider.onValueChange = [this] { audioProcessor.reverbParams.roomSize = roomSizeSlider.getValue(); audioProcessor.rev.setParameters(audioProcessor.reverbParams); };
    
    // Room Size label
    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(roomSizeLabel);

    // Damping slider
    addAndMakeVisible(dampingSlider);
    dampingSlider.setRange(0.0, 1.0);
    dampingSlider.setSliderStyle(juce::Slider::Rotary);
    dampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    dampingSlider.setColour(juce::Slider::thumbColourId, juce::Colours::orangered);   // Knob color
    dampingSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange); // Fill color
    dampingSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey); // Background color
    dampingSlider.onValueChange = [this] { audioProcessor.reverbParams.damping = dampingSlider.getValue(); audioProcessor.rev.setParameters(audioProcessor.reverbParams); };
    
    // Damping label
    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(dampingLabel);

    // Wet/Dry slider
    addAndMakeVisible(wetDrySlider);
    wetDrySlider.setRange(0.0, 1.0);
    wetDrySlider.setSliderStyle(juce::Slider::Rotary);
    wetDrySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    wetDrySlider.setColour(juce::Slider::thumbColourId, juce::Colours::orangered);   // Knob color
    wetDrySlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange); // Fill color
    wetDrySlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey); // Background color
    wetDrySlider.onValueChange = [this] { audioProcessor.reverbParams.wetLevel = wetDrySlider.getValue(); audioProcessor.rev.setParameters(audioProcessor.reverbParams); };
    
    // Wet/Dry label
    wetDryLabel.setText("Wet/Dry", juce::dontSendNotification);
    wetDrySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(wetDryLabel);
    
    // Width slider
    addAndMakeVisible(widthSlider);
    widthSlider.setRange(0.0, 1.0);
    widthSlider.setSliderStyle(juce::Slider::Rotary);
    widthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    widthSlider.setColour(juce::Slider::thumbColourId, juce::Colours::orangered);   // Knob color
    widthSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange); // Fill color
    widthSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey); // Background color
    widthSlider.onValueChange = [this] { audioProcessor.reverbParams.width = widthSlider.getValue(); audioProcessor.rev.setParameters(audioProcessor.reverbParams); };
    
    // Width label
    widthLabel.setText("Width", juce::dontSendNotification);
    widthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(widthLabel);
    
    // Add the MIDI keyboard
    addAndMakeVisible(midiKeyboard);
 
    // Set up the keyboard listener
    keyboardState.addListener(this);

    setSize (400, 400);
}

SimpleVSTAudioProcessorEditor::~SimpleVSTAudioProcessorEditor()
{
    // Remove the listener when the editor is destroyed
    keyboardState.removeListener(this);
}

//==============================================================================
void SimpleVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void SimpleVSTAudioProcessorEditor::resized()
{
    oscillatorsGroup.setBounds (10, 10, 380, 60);
    waveSelector.setBounds (115, 28, 170, 30);
    
    reverbGroup.setBounds(10, 75, 380, 195);
    roomSizeLabel.setBounds(87, 140, 150, 30);
    roomSizeSlider.setBounds(50, 85, 150, 70);
    dampingLabel.setBounds(241, 140, 150, 30);
    dampingSlider.setBounds (200, 85, 150, 70);
    wetDryLabel.setBounds(93, 235, 150, 30);
    wetDrySlider.setBounds (50, 180, 150, 70);
    widthLabel.setBounds(252, 235, 150, 30);
    widthSlider.setBounds(200, 180, 150, 70);
    
    // Position the keyboard in the bottom section
    midiKeyboard.setBounds (10, 280, getWidth() - 20, 100);
}

void SimpleVSTAudioProcessorEditor::handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    audioProcessor.noteOn(midiNoteNumber, velocity);
}

void SimpleVSTAudioProcessorEditor::handleNoteOff (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    audioProcessor.noteOff(midiNoteNumber);
}
