/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FilmStripKnob.h"
#include "FilmStripButton.h"


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

    // all knobs (their images are loaded in here)
    juce::Image knob1Image = juce::ImageFileFormat::loadFrom(BinaryData::knob1slushie_png, BinaryData::knob1slushie_pngSize);
    FilmStripKnob knob1 = FilmStripKnob(knob1Image, 33, false, 0);

    juce::Image knob2Image = juce::ImageFileFormat::loadFrom(BinaryData::Knob_20_png, BinaryData::Knob_20_pngSize);
    FilmStripKnob knob2 = FilmStripKnob(knob2Image, 61, false, 0);

    juce::Image knob3Image = juce::ImageFileFormat::loadFrom(BinaryData::Knob_20_png, BinaryData::Knob_20_pngSize);
    FilmStripKnob knob3 = FilmStripKnob(knob3Image, 61, false, 0);

    juce::Image knob4Image = juce::ImageFileFormat::loadFrom(BinaryData::resizedTestKnob_png, BinaryData::resizedTestKnob_pngSize);
    FilmStripKnob knob4 = FilmStripKnob(knob4Image, 31, false, 0);

    juce::Image knob5Image = juce::ImageFileFormat::loadFrom(BinaryData::customDriveKnob_png, BinaryData::customDriveKnob_pngSize);
    FilmStripKnob knob5 = FilmStripKnob(knob5Image, 31, false, 0);

    // the image for the flip switch goes here and the button as instance as well

    //APVTS and attachments 
    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment knob1Attachment, knob2Attachment, knob3Attachment, knob4Attachment, knob5Attachment;  

    // background image
    juce::Image background;  


    std::vector<juce::Component*> getComps(); 
    std::vector<juce::Rectangle<int>> getKnobRectangleBounds();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiLowCutPluginAudioProcessorEditor)
};
