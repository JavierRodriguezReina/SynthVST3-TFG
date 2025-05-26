/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessor::SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

SynthAudioProcessor::~SynthAudioProcessor()
{
}

//==============================================================================
const juce::String SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SynthAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SynthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SynthAudioProcessor::getProgramName(int index)
{
    return {};
}

void SynthAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void SynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void SynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

void SynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);


    }

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(synth.getVoice(i)))
        {

        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
    return new SynthAudioProcessorEditor(*this);
}

//==============================================================================
void SynthAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
        // Crear un objeto ValueTree para almacenar el estado
    juce::ValueTree state("SynthState");

    // Guardar el valor del volumen en el estado
    state.setProperty("volume", currentVolume, nullptr);
	// Guardar el tipo de onda actual en el estado
    state.setProperty("waveform", currentWaveform, nullptr);

	// Guardar los parámetros ADSR en el estado
	state.setProperty("attack", currentAttack, nullptr);
	state.setProperty("decay", currentDecay, nullptr);
	state.setProperty("sustain", currentSustain, nullptr);
	state.setProperty("release", currentRelease, nullptr);

    // Serializar el ValueTree a un MemoryBlock
    juce::MemoryOutputStream stream(destData, true);
    state.writeToStream(stream);

}

void SynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    juce::ValueTree state = juce::ValueTree::readFromStream(stream);

    if (state.hasProperty("volume"))
    {
        float volume = (float)state["volume"];
        setCurrentVolume(volume);  // Usa el setter para propagar el cambio
    }

    if (state.hasProperty("waveform"))
    {
        int waveform = (int)state["waveform"];
        setCurrentWaveform(waveform);
    }
    if (state.hasProperty("attack") && state.hasProperty("decay") &&
        state.hasProperty("sustain") && state.hasProperty("release"))
    {
		float attack = (float)state["attack"];
		float decay = (float)state["decay"];
		float sustain = (float)state["sustain"];
		float release = (float)state["release"];
		setADSRParameters(attack, decay, sustain, release);  // Actualizar ADSR
    }

}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}

void SynthAudioProcessor::setCurrentVolume(float volume)
{
    currentVolume = volume;  // Establecer el valor de currentVolume
    setVolume(currentVolume);  // Propagar el cambio a las voces del sintetizador
}

void SynthAudioProcessor::setVolume(float volume)
{
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->setGain(volume);  
        }
    }
}

void SynthAudioProcessor::setWaveformType(int type)
{
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->setOscillatorWaveform(type);
        }
    }
}

void SynthAudioProcessor::setCurrentWaveform(int waveformType)
{
    currentWaveform = waveformType;
    setWaveformType(waveformType); // Propagar a voces
}

void SynthAudioProcessor::setADSRParameters(float attack, float decay, float sustain, float release) {

    currentAttack = attack;
    currentDecay = decay;
    currentSustain = sustain;
    currentRelease = release;

	updateADSR(attack, decay, sustain, release); // Actualizar ADSR en todas las voces

}
void SynthAudioProcessor::updateADSR(float attack, float decay, float sustain, float release)
{
	// Actualizar los parámetros ADSR de todas las voces del sintetizador

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            juce::ADSR::Parameters adsrParams;
            adsrParams.attack = attack;
            adsrParams.decay = decay;
            adsrParams.sustain = sustain;
            adsrParams.release = release;
            voice->getADSR().setParameters(adsrParams);
        }
    }
}

void SynthAudioProcessor::updateReverb(float roomSize, float damping, float wet, float dry, float width, float freeze)
{
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->setReverbParams(roomSize, damping, wet, dry, width, freeze);
        }
    }
}
