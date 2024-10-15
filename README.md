# SimpleVST Plugin

## Overview
This is a simple synthesizer VST plugin built using JUCE. It includes a basic waveform generation (sine, triangle, sawtooth), a reverb effect and a piano keyboard. More oscillators, effects and controls could be added in the future.

## Features
- Oscillator with sine, triangle and sawtooth waveforms.
- Reverb effect with adjustable room size, damping, wet/dry mix, and width.
- MIDI keyboard integration for triggering sounds.

## Screenshots
<figure>
      <img src="assets/screenshots/SimpleVST.png" alt="SimpleVST image">
</figure>

## Requirements
- [JUCE Framework](https://juce.com/get-juce)
To install it from the terminal:
    git clone https://github.com/juce-framework/JUCE.git
    in JUCE/extras/Projucer/Builds/MacOSX open __Projucer.xcodeproj__
    Build and run it 
    From the __New Project__ wizard, in GlobalPaths set __Path to JUCE__ and __JUCE Modules_ to the right path if not already set
- Xcode (for macOS)
- Visual Studio (for Windows)

## Building the Plugin

### macOS (with Xcode)
1. Clone the repository:
    git clone https://github.com/LudoLozza/SimpleVST.git
    cd SimpleVST
2. Open the project:
    Open __SimpleVST.jucer__
3. Configure Xcode:
    In Projucer, set __Selected exporter__ to Xcode (MacOS)
    Open it on Xcode
4. Compile the plugin:
    In Xcode, select __Simple VST - Standalone__ plugin and run the project
    
## Contributing
### To contribute to my project:
1. Click the “Fork” button at the top-right corner to create your own copy of the repository
2. From the terminal:
    git clone https://github.com/LudoLozza/SimpleVST.git
    cd SimpleVST
3. Create your branch:
    git checkout -b my-branch
4. Make your changes:
    Add new features or fix bugs in the code, make sure they are well-documented
5. Commit:
    git add .
    git commit -m "Describe your changes"
6. Push:
    git push origin my-branch
7. Submit Pull Request:
    Go to your fork on GitHub and click the “New Pull Request” button, clearly describing the changes you’ve made.
    
    
    
