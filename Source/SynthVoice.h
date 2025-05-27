/*
  ==============================================================================

	SynthVoice.h
	Created: 22 Mar 2025 4:20:59pm
	Author:  jrrro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"


class SynthVoice : public juce::SynthesiserVoice {

public:
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
	void setGain(float newGain);
	void setOscillatorWaveform(int type);
	juce::ADSR& getADSR() { return adsr; }
	void setReverbParams(float roomSize, float damping, float wetLevel, float dryLevel, float width, float freeze);
	void setReverbEnabled(bool shouldEnable);

private:
	juce::ADSR adsr;
	juce::ADSR::Parameters adsrParams;
	enum WaveformType {
		Sine = 0,
		Square,
		Saw,
		Triangle
	};
	juce::dsp::Oscillator<float> osc;
	juce::dsp::Gain<float> gain;

	juce::dsp::Reverb reverb;
	juce::dsp::Reverb::Parameters reverbParams;
	bool reverbEnabled = true;



	bool isPrepared{ false };
};