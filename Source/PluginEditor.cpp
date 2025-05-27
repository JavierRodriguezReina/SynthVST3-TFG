#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(620, 500);  // Tamaño aumentado para caber todos los controles cómodamente

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

    // ==== ADSR SLIDERS ====
    auto configureSlider = [](juce::Slider& slider, juce::Label& label, const juce::String& name) {
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        };

    configureSlider(attackSlider, attackLabel, "Attack");
    configureSlider(decaySlider, decayLabel, "Decay");
    configureSlider(sustainSlider, sustainLabel, "Sustain");
    configureSlider(releaseSlider, releaseLabel, "Release");

    attackSlider.setRange(0.01, 5.0, 0.01);     attackSlider.setValue(audioProcessor.getCurrentAttack());
    decaySlider.setRange(0.01, 5.0, 0.01);      decaySlider.setValue(audioProcessor.getCurrentDecay());
    sustainSlider.setRange(0.0, 1.0, 0.01);     sustainSlider.setValue(audioProcessor.getCurrentSustain());
    releaseSlider.setRange(0.01, 5.0, 0.01);    releaseSlider.setValue(audioProcessor.getCurrentRelease());

    for (auto* s : { &attackSlider, &decaySlider, &sustainSlider, &releaseSlider }) {
        s->addListener(this);
        addAndMakeVisible(*s);
    }

    for (auto* l : { &attackLabel, &decayLabel, &sustainLabel, &releaseLabel }) {
        addAndMakeVisible(*l);
    }

    // ==== REVERB SLIDERS ====
    auto configureReverbSlider = [](juce::Slider& slider, juce::Label& label, const juce::String& name, float min, float max, float init) {
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        slider.setRange(min, max, 0.01f);
        slider.setValue(init);
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        };

    configureReverbSlider(reverbRoomSlider, reverbRoomLabel, "Room Size", 0.0f, 1.0f, audioProcessor.getCurrentRoomSize());
    configureReverbSlider(reverbDampingSlider, reverbDampingLabel, "Damping", 0.0f, 1.0f, audioProcessor.getCurrentDamping());
    configureReverbSlider(reverbWetSlider, reverbWetLabel, "Wet Level", 0.0f, 1.0f, audioProcessor.getCurrentWetLevel());
    configureReverbSlider(reverbDrySlider, reverbDryLabel, "Dry Level", 0.0f, 1.0f, audioProcessor.getCurrentDryLevel());
    configureReverbSlider(reverbWidthSlider, reverbWidthLabel, "Width", 0.0f, 1.0f, audioProcessor.getCurrentWidth());
    configureReverbSlider(reverbFreezeSlider, reverbFreezeLabel, "Freeze", 0.0f, 1.0f, audioProcessor.getCurrentFreeze());

    for (auto* s : { &reverbRoomSlider, &reverbDampingSlider, &reverbWetSlider, &reverbDrySlider, &reverbWidthSlider, &reverbFreezeSlider }) {
        s->addListener(this);
        addAndMakeVisible(*s);
    }

    for (auto* l : { &reverbRoomLabel, &reverbDampingLabel, &reverbWetLabel, &reverbDryLabel, &reverbWidthLabel, &reverbFreezeLabel }) {
        addAndMakeVisible(*l);
    }

    addAndMakeVisible(reverbToggleButton);
    reverbToggleButton.setToggleState(audioProcessor.getReverbEnabled(), juce::dontSendNotification);
    reverbToggleButton.onClick = [this]() {
        bool isOn = reverbToggleButton.getToggleState();
        audioProcessor.setReverbEnabled(isOn);
        };
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

void SynthAudioProcessorEditor::resized()
{
    const int margin = 10;
    const int labelWidth = 70;
    const int controlHeight = 25;

    // Volumen y forma de onda
    volumeSlider.setBounds(labelWidth, 40, getWidth() - labelWidth - margin, controlHeight);
    waveformSelector.setBounds(labelWidth, 70, 150, controlHeight);

    // ADSR Sliders
    int adsrTop = 120;
    int adsrSliderSize = 80;
    int adsrSpacing = 20;
    int adsrTotalWidth = 4 * adsrSliderSize + 3 * adsrSpacing;
    int adsrStartX = (getWidth() - adsrTotalWidth) / 2;

    auto placeADSRSlider = [adsrTop, adsrSliderSize](juce::Slider& s, juce::Label& l, int x) {
        s.setBounds(x, adsrTop, adsrSliderSize, adsrSliderSize);
        l.setBounds(x, adsrTop + adsrSliderSize, adsrSliderSize, 20);
        };

    placeADSRSlider(attackSlider, attackLabel, adsrStartX);
    placeADSRSlider(decaySlider, decayLabel, adsrStartX + (adsrSliderSize + adsrSpacing));
    placeADSRSlider(sustainSlider, sustainLabel, adsrStartX + 2 * (adsrSliderSize + adsrSpacing));
    placeADSRSlider(releaseSlider, releaseLabel, adsrStartX + 3 * (adsrSliderSize + adsrSpacing));

    // Reverb Sliders

    int reverbSliderSize = 70;
    int reverbSpacing = 10;
    int reverbRow1Y = adsrTop + adsrSliderSize + 50;
    int reverbRow2Y = reverbRow1Y + reverbSliderSize + 30;
    int reverbStartX = 20;

    reverbToggleButton.setBounds(400, 70, 150, 25);

    auto placeReverbSlider = [reverbSliderSize](juce::Slider& s, juce::Label& l, int x, int y) {
        s.setBounds(x, y, reverbSliderSize, reverbSliderSize);
        l.setBounds(x, y + reverbSliderSize, reverbSliderSize, 20);
        };

    placeReverbSlider(reverbRoomSlider, reverbRoomLabel, reverbStartX, reverbRow1Y);
    placeReverbSlider(reverbDampingSlider, reverbDampingLabel, reverbStartX + (reverbSliderSize + reverbSpacing), reverbRow1Y);
    placeReverbSlider(reverbWetSlider, reverbWetLabel, reverbStartX + 2 * (reverbSliderSize + reverbSpacing), reverbRow1Y);

    placeReverbSlider(reverbDrySlider, reverbDryLabel, reverbStartX, reverbRow2Y);
    placeReverbSlider(reverbWidthSlider, reverbWidthLabel, reverbStartX + (reverbSliderSize + reverbSpacing), reverbRow2Y);
    placeReverbSlider(reverbFreezeSlider, reverbFreezeLabel, reverbStartX + 2 * (reverbSliderSize + reverbSpacing), reverbRow2Y);
}

//==============================================================================
void SynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioProcessor.setCurrentVolume(volumeSlider.getValue());
    }

    if (slider == &attackSlider || slider == &decaySlider || slider == &sustainSlider || slider == &releaseSlider)
    {
        audioProcessor.setADSRParameters(
            attackSlider.getValue(),
            decaySlider.getValue(),
            sustainSlider.getValue(),
            releaseSlider.getValue()
        );
    }
	if (slider == &reverbRoomSlider || slider == &reverbDampingSlider ||
		slider == &reverbWetSlider || slider == &reverbDrySlider ||
		slider == &reverbWidthSlider || slider == &reverbFreezeSlider)
	{
        audioProcessor.setReverbParameters(
            reverbRoomSlider.getValue(),
            reverbDampingSlider.getValue(),
            reverbWetSlider.getValue(),
            reverbDrySlider.getValue(),
            reverbWidthSlider.getValue(),
            reverbFreezeSlider.getValue()
        );
	}
   
}
