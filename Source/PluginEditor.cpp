#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(500, 250);

    // ==== VOLUMEN ====
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(audioProcessor.getCurrentVolume());
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // ==== WAVEFORM SELECTOR ====
    waveformSelector.addItem("Sine", 1);
    waveformSelector.addItem("Square", 2);
    waveformSelector.addItem("Saw", 3);
    waveformSelector.addItem("Triangle", 4);
    waveformSelector.setSelectedId(audioProcessor.getCurrentWaveform() + 1);
    waveformSelector.onChange = [this]() {
        audioProcessor.setCurrentWaveform(waveformSelector.getSelectedId() - 1);
        };
    addAndMakeVisible(waveformSelector);

    // ==== ADSR Sliders ====
    auto configureSlider = [](juce::Slider& slider, juce::Label& label, const juce::String& name) {
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        };

    configureSlider(attackSlider, attackLabel, "Attack");
    attackSlider.setRange(0.01, 5.0, 0.01);
    attackSlider.setValue(0.1);
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(attackLabel);

    configureSlider(decaySlider, decayLabel, "Decay");
    decaySlider.setRange(0.01, 5.0, 0.01);
    decaySlider.setValue(0.1);
    decaySlider.addListener(this);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(decayLabel);

    configureSlider(sustainSlider, sustainLabel, "Sustain");
    sustainSlider.setRange(0.0, 1.0, 0.01);
    sustainSlider.setValue(1.0);
    sustainSlider.addListener(this);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(sustainLabel);

    configureSlider(releaseSlider, releaseLabel, "Release");
    releaseSlider.setRange(0.01, 5.0, 0.01);
    releaseSlider.setValue(0.4);
    releaseSlider.addListener(this);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(releaseLabel);
}

//==============================================================================
SynthAudioProcessorEditor::~SynthAudioProcessorEditor() {}

void SynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    g.drawFittedText("TFG Synth Plugin", getLocalBounds().removeFromTop(30), juce::Justification::centredTop, 1);
}

//==============================================================================
void SynthAudioProcessorEditor::resized()
{
    const int margin = 10;
    const int labelWidth = 70;
    const int controlHeight = 25;

    // Volumen
    volumeSlider.setBounds(labelWidth, 40, getWidth() - labelWidth - margin, controlHeight);

    // Waveform selector
    waveformSelector.setBounds(labelWidth, 70, 150, controlHeight);

    // ADSR
    int adsrTop = 120;
    int sliderSize = 80;
    int spacing = 20;
    int totalWidth = 4 * sliderSize + 3 * spacing;
    int startX = (getWidth() - totalWidth) / 2;

    auto placeSlider = [adsrTop, sliderSize](juce::Slider& s, juce::Label& l, int x) {
        s.setBounds(x, adsrTop, sliderSize, sliderSize);
        l.setBounds(s.getX(), s.getBottom(), sliderSize, 20);
        };

    placeSlider(attackSlider, attackLabel, startX);
    placeSlider(decaySlider, decayLabel, startX + sliderSize + spacing);
    placeSlider(sustainSlider, sustainLabel, startX + 2 * (sliderSize + spacing));
    placeSlider(releaseSlider, releaseLabel, startX + 3 * (sliderSize + spacing));
}

//==============================================================================
void SynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioProcessor.setCurrentVolume(volumeSlider.getValue());
    }

    audioProcessor.updateADSR(
        attackSlider.getValue(),
        decaySlider.getValue(),
        sustainSlider.getValue(),
        releaseSlider.getValue()
    );
}
