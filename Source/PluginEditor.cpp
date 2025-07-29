#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor(SynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(620, 500); 
    
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::TrashHand_TTF, BinaryData::TrashHand_TTFSize);
    customFont = juce::Font(typeface).withHeight(24.0f);
    customFont.setHeight(24.0f);
    // Tamaño aumentado para caber todos los controles cómodamente

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
    auto configureADSRSlider = [](juce::Slider& slider, juce::Label& label, const juce::String& name, float min, float max, float init) {
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        slider.setRange(min, max, 0.01f);
        slider.setValue(init);
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        };

    configureADSRSlider(attackSlider, attackLabel, "Attack", 0.01f, 5.0f, audioProcessor.getCurrentAttack());
    configureADSRSlider(decaySlider, decayLabel, "Decay", 0.01f, 5.0f, audioProcessor.getCurrentDecay());
    configureADSRSlider(sustainSlider, sustainLabel, "Sustain", 0.0f, 1.0f, audioProcessor.getCurrentSustain());
    configureADSRSlider(releaseSlider, releaseLabel, "Release", 0.01f, 5.0f, audioProcessor.getCurrentRelease());

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

    // === ESTILO VERDE CHILLÓN ===
    juce::Colour neonGreen = juce::Colours::limegreen;

    auto setSliderGreenStyle = [neonGreen](juce::Slider& slider) {
        slider.setColour(juce::Slider::thumbColourId, neonGreen);
        slider.setColour(juce::Slider::trackColourId, neonGreen);
        slider.setColour(juce::Slider::rotarySliderFillColourId, neonGreen);
        };

    auto setLabelGreenStyle = [neonGreen](juce::Label& label) {
        label.setColour(juce::Label::textColourId, neonGreen);
        };

    reverbToggleButton.setColour(juce::ToggleButton::textColourId, neonGreen);
    waveformSelector.setColour(juce::ComboBox::textColourId, neonGreen);
    waveformSelector.setColour(juce::ComboBox::outlineColourId, neonGreen);

    for (auto* s : { &attackSlider, &decaySlider, &sustainSlider, &releaseSlider,
                     &reverbRoomSlider, &reverbDampingSlider, &reverbWetSlider,
                     &reverbDrySlider, &reverbWidthSlider, &reverbFreezeSlider, &volumeSlider }) {
        setSliderGreenStyle(*s);
    }

    for (auto* l : { &attackLabel, &decayLabel, &sustainLabel, &releaseLabel,
                     &reverbRoomLabel, &reverbDampingLabel, &reverbWetLabel,
                     &reverbDryLabel, &reverbWidthLabel, &reverbFreezeLabel }) {
        setLabelGreenStyle(*l);
    }
    addAndMakeVisible(waveformTitleLabel);
    addAndMakeVisible(adsrTitleLabel);
    addAndMakeVisible(reverbTitleLabel);

}

//==============================================================================
SynthAudioProcessorEditor::~SynthAudioProcessorEditor() {}

void SynthAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font("Arial", 24.0f, juce::Font::bold));  // Más grande y con estilo

    juce::Rectangle<int> titleArea = getLocalBounds().withTop(15).withHeight(40); // Margen superior de 15
    g.drawFittedText("TFG Synth Plugin", titleArea, juce::Justification::centredTop, 1);
	g.setFont(customFont);
}

void SynthAudioProcessorEditor::resized()
{

    const int margin = 20;
    const int controlHeight = 30;
    const int titleHeight = 25;

    // Títulos
    waveformTitleLabel.setText("Controles de Volumen y Forma de Onda", juce::dontSendNotification);
	waveformTitleLabel.setFont(customFont);
    adsrTitleLabel.setText("Controles ADSR", juce::dontSendNotification);
    reverbTitleLabel.setText("Controles de Reverb", juce::dontSendNotification);

    waveformTitleLabel.setJustificationType(juce::Justification::centred);
    adsrTitleLabel.setJustificationType(juce::Justification::centred);
    reverbTitleLabel.setJustificationType(juce::Justification::centred);

    int y = 50;

    setSize(800, y + controlHeight + 500);

    // Waveform y volumen
    waveformTitleLabel.setBounds(0, y, getWidth(), titleHeight);
    y += titleHeight + 10;

    int selectorWidth = 180;
    waveformSelector.setBounds((getWidth() - selectorWidth) / 2, y, selectorWidth, controlHeight);
    y += controlHeight + 10;

    int volumeSliderWidth = 300;
    volumeSlider.setBounds((getWidth() - volumeSliderWidth) / 2, y, volumeSliderWidth, controlHeight);
    y += controlHeight + 30;

    // ADSR
    adsrTitleLabel.setBounds(0, y, getWidth(), titleHeight);
    y += titleHeight + 10;

    int adsrSliderSize = 100;
    int adsrSpacing = 30;
    int adsrTotalWidth = 4 * adsrSliderSize + 3 * adsrSpacing;
    int adsrStartX = (getWidth() - adsrTotalWidth) / 2;
    int adsrTop = y;

    auto placeADSRSlider = [adsrTop, adsrSliderSize](juce::Slider& s, juce::Label& l, int x) {
        s.setBounds(x, adsrTop, adsrSliderSize, adsrSliderSize);
        l.setBounds(x, adsrTop + adsrSliderSize, adsrSliderSize, 20);
        };

    placeADSRSlider(attackSlider, attackLabel, adsrStartX);
    placeADSRSlider(decaySlider, decayLabel, adsrStartX + (adsrSliderSize + adsrSpacing));
    placeADSRSlider(sustainSlider, sustainLabel, adsrStartX + 2 * (adsrSliderSize + adsrSpacing));
    placeADSRSlider(releaseSlider, releaseLabel, adsrStartX + 3 * (adsrSliderSize + adsrSpacing));

    y = adsrTop + adsrSliderSize + 40;

    // Reverb
    reverbTitleLabel.setBounds(0, y, getWidth(), titleHeight);
    y += titleHeight + 10;

    int numReverbSliders = 6;
    int reverbSliderSize = 90;
    int reverbSpacing = (getWidth() - 2 * margin - numReverbSliders * reverbSliderSize) / (numReverbSliders - 1);
    int reverbTop = y;
    int reverbX = margin;

    auto placeReverbSlider = [reverbSliderSize, reverbTop](juce::Slider& s, juce::Label& l, int x) {
        s.setBounds(x, reverbTop, reverbSliderSize, reverbSliderSize);
        l.setBounds(x, reverbTop + reverbSliderSize, reverbSliderSize, 20);
        };

    placeReverbSlider(reverbRoomSlider, reverbRoomLabel, reverbX); reverbX += reverbSliderSize + reverbSpacing;
    placeReverbSlider(reverbDampingSlider, reverbDampingLabel, reverbX); reverbX += reverbSliderSize + reverbSpacing;
    placeReverbSlider(reverbWetSlider, reverbWetLabel, reverbX); reverbX += reverbSliderSize + reverbSpacing;
    placeReverbSlider(reverbDrySlider, reverbDryLabel, reverbX); reverbX += reverbSliderSize + reverbSpacing;
    placeReverbSlider(reverbWidthSlider, reverbWidthLabel, reverbX); reverbX += reverbSliderSize + reverbSpacing;
    placeReverbSlider(reverbFreezeSlider, reverbFreezeLabel, reverbX);

    reverbToggleButton.setBounds(getWidth() - margin - 150, reverbTop + reverbSliderSize + 30, 150, controlHeight);


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
        audioProcessor.setCurrentADSRParameters(
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
        audioProcessor.setCurrentReverbParameters(
            reverbRoomSlider.getValue(),
            reverbDampingSlider.getValue(),
            reverbWetSlider.getValue(),
            reverbDrySlider.getValue(),
            reverbWidthSlider.getValue(),
            reverbFreezeSlider.getValue()
        );
	}
   
}
