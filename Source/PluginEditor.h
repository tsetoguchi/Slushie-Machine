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

    // knob 1 (image and instance)
    juce::Image knob1Image = juce::ImageFileFormat::loadFrom(BinaryData::Knob_20_png, BinaryData::Knob_20_pngSize);
    FilmStripKnob knob1 = FilmStripKnob(knob1Image, 61, false, 0);

    juce::Image knob2Image = juce::ImageFileFormat::loadFrom(BinaryData::Knob_20_png, BinaryData::Knob_20_pngSize);
    FilmStripKnob knob2 = FilmStripKnob(knob2Image, 61, false, 0);

    juce::Image knob3Image = juce::ImageFileFormat::loadFrom(BinaryData::Knob_20_png, BinaryData::Knob_20_pngSize);
    FilmStripKnob knob3 = FilmStripKnob(knob3Image, 61, false, 0);


    // background image
    juce::Image background; 


    std::vector<juce::Component*> getComps(); 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiLowCutPluginAudioProcessorEditor)
};
