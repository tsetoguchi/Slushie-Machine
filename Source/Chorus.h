/*
  ==============================================================================

    Chorus.h
    Created: 19 Apr 2023 4:50:20pm
    Author:  tao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Chorus {

public:
    void process(juce::dsp::ProcessContextReplacing<float> context) {

        // chorus processing

        //chorus.setMix(settingsOfParameters.chorusMix);
        chorus.process(context);
    }

    void prepare(juce::dsp::ProcessSpec spec, float centreDelay, float rate, float mix) {
        chorus.reset();
        chorus.prepare(spec);
        chorus.setCentreDelay(centreDelay);
        chorus.setRate(rate);
        chorus.setMix(mix);
    }

    void setMix(float mix) {
        chorus.setMix(mix);
    }

    void setDepth(float depth) {
        chorus.setDepth(depth);
    }

    void setRate(float rate) {
        chorus.setRate(rate);
    }

    void setCentreDelay(float centreDelay) {
        chorus.setCentreDelay(centreDelay);
    }

    void setFeedback(float feedback) {
        chorus.setFeedback(feedback);
    }


private:
    juce::dsp::Chorus<float> chorus;
    

};