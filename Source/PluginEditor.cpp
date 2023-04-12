/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HiLowCutPluginAudioProcessorEditor::HiLowCutPluginAudioProcessorEditor(HiLowCutPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(knob1);

    for (auto* comp : getComps())
    {
        addAndMakeVisible(comp);
    }

    setSize (400, 300);
}

HiLowCutPluginAudioProcessorEditor::~HiLowCutPluginAudioProcessorEditor()
{
}

//==============================================================================
void HiLowCutPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
 
    background = juce::ImageCache::getFromMemory(BinaryData::slushie_background_png, BinaryData::slushie_background_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

}

void HiLowCutPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds1 = getLocalBounds();
    auto bounds2 = getLocalBounds();
    auto bounds3 = getLocalBounds();

    //knob 1
    auto boundsHalfLeft = bounds1.removeFromLeft(bounds1.getWidth() * 0.5);
    juce::Point<int> centerLeftPoint = boundsHalfLeft.getCentre();
    juce::Rectangle<int> rectangleKnob1(90, 90);
    rectangleKnob1.setCentre(centerLeftPoint);
    rectangleKnob1.setY(130);

    //knob2
    juce::Point<int> middlePoint = bounds2.getCentre();
    juce::Rectangle<int> rectangleKnob2(90, 90);
    rectangleKnob2.setCentre(middlePoint);
    rectangleKnob2.setY(60);

    //knob 3
    auto boundsHalfRight = bounds3.removeFromRight(bounds3.getWidth() * 0.5);
    juce::Point<int> centerRightPoint = boundsHalfRight.getCentre();
    juce::Rectangle<int> rectangleKnob3(90, 90);
    rectangleKnob3.setCentre(centerRightPoint);
    rectangleKnob3.setY(130);

    // here i set the bounds for each knob:
    knob1.setBounds(rectangleKnob1);
    knob2.setBounds(rectangleKnob2);
    knob3.setBounds(rectangleKnob3);
}

// add all knobs here in the future (rn im just testing 1 knob)
std::vector<juce::Component*> HiLowCutPluginAudioProcessorEditor::getComps()
{
    return
    {
        &knob1,
        &knob2,
        &knob3
    };
}
