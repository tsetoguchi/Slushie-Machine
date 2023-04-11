/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FilmStripKnob.h"


//==============================================================================
/**
*/
class HiLowCutPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HiLowCutPluginAudioProcessorEditor (HiLowCutPluginAudioProcessor&);
    ~HiLowCutPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HiLowCutPluginAudioProcessor& audioProcessor;

    juce::Image myImage = juce::ImageFileFormat::loadFrom(BinaryData::ImageKnobz_png, BinaryData::ImageKnobz_pngSize);
    
    FilmStripKnob knob1 = FilmStripKnob(myImage, 60, false, 0);



    std::vector<juce::Component*> getComps(); 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiLowCutPluginAudioProcessorEditor)
};
