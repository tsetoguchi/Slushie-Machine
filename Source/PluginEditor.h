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

    // all knobs (their images are loaded in here) green, blue, pink 
    juce::Image knob1Image = juce::ImageFileFormat::loadFrom(BinaryData::knob2fixed_png, BinaryData::knob2fixed_pngSize);
    FilmStripKnob knob1 = FilmStripKnob(knob1Image, 33, false, 0);

    juce::Image knob2Image = juce::ImageFileFormat::loadFrom(BinaryData::knob3fixed_png, BinaryData::knob3fixed_pngSize);
    FilmStripKnob knob2 = FilmStripKnob(knob2Image, 33, false, 0);

    juce::Image knob3Image = juce::ImageFileFormat::loadFrom(BinaryData::knob160_png, BinaryData::knob160_pngSize);
    FilmStripKnob knob3 = FilmStripKnob(knob3Image, 33, false, 0);

    juce::Image knob4Image = juce::ImageFileFormat::loadFrom(BinaryData::driveKnobdoodle_png, BinaryData::driveKnobdoodle_pngSize);
    FilmStripKnob knob4 = FilmStripKnob(knob4Image, 33, false, 0);

    juce::Image knob5Image = juce::ImageFileFormat::loadFrom(BinaryData::driveKnobdoodle_png, BinaryData::driveKnobdoodle_pngSize);
    FilmStripKnob knob5 = FilmStripKnob(knob5Image, 33, false, 0);

    // the image for the flip switch goes here and the button as instance as well
    juce::Image toggleSwitchImage = juce::ImageFileFormat::loadFrom(BinaryData::switchKnob_png, BinaryData::switchKnob_pngSize);
    FilmStripButton toggleSwitch = FilmStripButton(toggleSwitchImage, false, 0);

    //APVTS and attachments 
    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;
    using ButtonAttachment = APVTS::ButtonAttachment;
    
    Attachment knob1Attachment, knob2Attachment, knob3Attachment, knob4Attachment, knob5Attachment; 
    ButtonAttachment toggleAttachment; 

    // background image
    juce::Image background;  


    std::vector<juce::Component*> getComps(); 
    std::vector<juce::Rectangle<int>> getKnobRectangleBounds();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiLowCutPluginAudioProcessorEditor)
};
