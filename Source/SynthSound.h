/*
  ==============================================================================

    SynthSound.h
    Created: 22 Mar 2025 4:21:31pm
    Author:  jrrro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound {

public:
	bool appliesToNote(int midiNoteNumber) override {
		return true;
	}
	bool appliesToChannel(int midiChannel) override {
		return true;
	}

};

