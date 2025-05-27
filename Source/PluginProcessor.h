/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
/**
*/
class SynthAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    SynthAudioProcessor();
    ~SynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void setVolume(float newVolume);
    float getCurrentVolume() const { return currentVolume; }
    void setCurrentVolume(float volume);
	//Esto es para cambiar el tipo de onda del oscilador
    void setWaveformType(int type);
	int getCurrentWaveform() const { return currentWaveform; }
    void setCurrentWaveform(int waveformType);
	// Métodos para ADSR
    void setADSRParameters(float attack, float decay, float sustain, float release);
    void updateADSR(float attack, float decay, float sustain, float release);
	float getCurrentAttack() const { return currentAttack; }
	float getCurrentDecay() const { return currentDecay; }
	float getCurrentSustain() const { return currentSustain; }
	float getCurrentRelease() const { return currentRelease; }
    // Metodos para Reverb
    void updateReverb(float roomSize, float damping, float wet, float dry, float width, float freeze);
    void setReverbParameters(float roomSize, float damping, float wet, float dry, float width, float freeze);
	void setReverbEnabled(bool shouldEnable);
    void setReverbEnabledForAllVoices(bool shouldEnable);
    float getCurrentRoomSize() { return currentRoomSize;}
    float getCurrentDamping() { return currentDamping;}
    float getCurrentWetLevel() { return currentWetLevel;}
    float getCurrentDryLevel() { return currentDryLevel;}
    float getCurrentWidth() { return currentWidth;}
    float getCurrentFreeze() { return currentFreeze;}
	bool getReverbEnabled() const { return reverbEnabled; }
    


private:
    juce::Synthesiser synth;
    float currentVolume = 0.5f;
    int currentWaveform = 0;

	float currentAttack = 0.1f;
	float currentDecay = 0.1f;
	float currentSustain = 1.0f;
	float currentRelease = 0.4f;

	float currentRoomSize = 0.5f;
	float currentDamping = 0.5f;
	float currentWetLevel = 0.3f;
	float currentDryLevel = 0.7f;
	float currentWidth = 1.0f;
	float currentFreeze = 0.0f;
	bool reverbEnabled = true;

    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessor)
};