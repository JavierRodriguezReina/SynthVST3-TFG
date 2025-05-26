/*
  ==============================================================================

    SynthVoice.cpp
    Created: 22 Mar 2025 4:20:59pm
    Author:  jrrro

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}
void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();

}
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {}
void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {}
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputCannels)
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputCannels;

    osc.prepare(spec);
    gain.prepare(spec);

    gain.setGainLinear(0.01f);
    setOscillatorWaveform(0);

    reverb.prepare(spec);

    isPrepared = true;
}
void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
    reverb.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

void SynthVoice::setGain(float newGain)
{
    gain.setGainLinear(newGain);
}

void SynthVoice::setOscillatorWaveform(int type)
{
    switch (type)
    {
    case Sine:
        osc.initialise([](float x) { return std::sin(x); }, 128);
        break;
    case Square:
        osc.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 128);
        break;
    case Saw:
        osc.initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 128);
        break;
    case Triangle:
        osc.initialise([](float x) {
            return std::asin(std::sin(x)) * (2.0f / juce::MathConstants<float>::pi);
            }, 128);
        break;
    default:
        break;
    }
}
void SynthVoice::setReverbParams(float roomSize, float damping, float wetLevel, float dryLevel, float width, float freeze)
{
    reverbParams.roomSize = roomSize;
    reverbParams.damping = damping;
    reverbParams.wetLevel = wetLevel;
    reverbParams.dryLevel = dryLevel;
    reverbParams.width = width;
    reverbParams.freezeMode = freeze;
    reverb.setParameters(reverbParams);
}
