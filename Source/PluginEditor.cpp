/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Configurar el slider
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(audioProcessor.getCurrentVolume()); // <-- Obtener el volumen actual del procesador
    volumeSlider.addListener(this); // <-- Permite detectar cambios en el slider
    addAndMakeVisible(volumeSlider);

    setSize(400, 300);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
}

//==============================================================================
void SynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::green);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Plugin TFG", getLocalBounds(), juce::Justification::centred, 1);
}

void SynthAudioProcessorEditor::resized()
{
    volumeSlider.setBounds(10, 10, getWidth() - 20, 20);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void SynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioProcessor.setCurrentVolume(volumeSlider.getValue()); // <-- Actualiza el volumen en el procesador
    }
}
