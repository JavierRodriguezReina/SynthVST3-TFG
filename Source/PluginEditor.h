/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SynthAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    SynthAudioProcessorEditor(SynthAudioProcessor&);
    ~SynthAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;


private:
    juce::Slider volumeSlider;
    void sliderValueChanged(juce::Slider* slider);
    SynthAudioProcessor& audioProcessor;

    juce::ComboBox waveformSelector;

    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;


    juce::Slider reverbRoomSlider, reverbDampingSlider, reverbWetSlider, reverbDrySlider, reverbWidthSlider, reverbFreezeSlider;
    juce::Label reverbRoomLabel, reverbDampingLabel, reverbWetLabel, reverbDryLabel, reverbWidthLabel, reverbFreezeLabel;
    juce::ToggleButton reverbToggleButton{ "Enable Reverb" };

    juce::Label waveformTitleLabel;
    juce::Label adsrTitleLabel;
    juce::Label reverbTitleLabel;

    juce::Font customFont;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessorEditor)
};