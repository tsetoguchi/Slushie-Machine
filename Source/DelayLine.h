/*
  ==============================================================================

    DelayLine.h
    Created: 19 Apr 2023 5:09:29pm
    Author:  tao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename SampleType>
class DelayLine {

public:

    DelayLine() {
        delayLine.setMaximumDelayInSamples(192000);
    }

    void prepare(juce::dsp::ProcessSpec spec, SampleType initialDelay) {
        delayLine.reset();
        delayLine.prepare(spec);
        delayLine.setDelay(initialDelay);
    }

    void process(int totalNumInputChannels, juce::AudioBuffer<float>& buffer, float coefficient) {
        for (int channel = 0; channel < totalNumInputChannels; channel++) {

            if (delayLine.getDelay() == 0) {
                break;
            }

            auto* inSamples = buffer.getReadPointer(channel);
            auto* outSamples = buffer.getWritePointer(channel);



            for (int i = 0; i < buffer.getNumSamples(); i++) {



                float delayedSample = delayLine.popSample(channel);
                float newSampleToPush = inSamples[i] + (delayedSample * (coefficient * 0.8));
                // instead of setting settingsOfParameters.feedBack TRY settingsOfParameters.knob1 * 0.1 aka maxDelayFeedback
                delayLine.pushSample(channel, newSampleToPush);
                outSamples[i] = inSamples[i] + delayedSample;
            }
        }
    }

    void setDelay(SampleType delay) {
        delayLine.setDelay(delay);
    }

    SampleType getDelay() {
        return delayLine.getDelay();
    }

private:
    juce::dsp::DelayLine<SampleType, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine{ 0 };
};