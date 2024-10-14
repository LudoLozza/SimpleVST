/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleVSTAudioProcessor::SimpleVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Initialize waveform
    osc.initialise([] (float x) { return std::sin(x); }, 128);
    
    // Configure initial reverb parameters
    reverbParams.roomSize = 0.1f;
    reverbParams.damping = 0.1f;
    reverbParams.wetLevel = 0.1f;
    reverbParams.dryLevel = 0.9f;
    reverbParams.width = 0.1f;
    rev.setParameters(reverbParams);
}

SimpleVSTAudioProcessor::~SimpleVSTAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleVSTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleVSTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleVSTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleVSTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleVSTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleVSTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleVSTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleVSTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleVSTAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleVSTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleVSTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    osc.prepare(spec);
    rev.prepare(spec);
}

void SimpleVSTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    
    // If a note is being played, process the oscillator output
    if (isNoteOn)
    {
        // Generate waveform
        osc.process(context);
    }
    
    // Apply reverb
    rev.process(context);
}

void SimpleVSTAudioProcessor::setWaveform (int choice)
{
    switch (choice)
    {
        case 0: // Sine wave
            osc.initialise ([] (float x) { return std::sin(x); }, 128);
            break;
        case 1: // Triangle wave
            osc.initialise([] (float x) {
                float sawtooth = 2.0f * (x / juce::MathConstants<float>::pi - std::floor(x / juce::MathConstants<float>::pi + 0.5f));
                return 1.0f - 2.0f * std::abs(sawtooth);
            }, 128);
            break;
        case 2: // Sawtooth wave
            osc.initialise ([] (float x) { return (2.0f / juce::MathConstants<float>::pi) * (x - juce::MathConstants<float>::pi); }, 128);
            break;
    }
}

void SimpleVSTAudioProcessor::noteOn(int midiNoteNumber, float velocity)
{
    // Convert MIDI note number to frequency
    float frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

    // Set the oscillator's frequency of the note
    osc.setFrequency(frequency);

    // Set the note as pressed
    isNoteOn = true;
}

void SimpleVSTAudioProcessor::noteOff(int midiNoteNumber)
{
    // Stop the oscillator when the note is released
    isNoteOn = false;
}

//==============================================================================
bool SimpleVSTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleVSTAudioProcessor::createEditor()
{
    return new SimpleVSTAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleVSTAudioProcessor();
}
