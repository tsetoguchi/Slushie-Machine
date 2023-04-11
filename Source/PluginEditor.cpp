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

    setSize (1600, 1400);
}

HiLowCutPluginAudioProcessorEditor::~HiLowCutPluginAudioProcessorEditor()
{
}

//==============================================================================
void HiLowCutPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::top, 1);
}

void HiLowCutPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    auto boundsTopThird = bounds.removeFromTop(bounds.getHeight() * 0.33);


    auto knob1Area = boundsTopThird.removeFromLeft(bounds.getWidth() * 0.33);

    knob1.setBounds(boundsTopThird);

}

// add all knobs here in the future (rn im just testing 1 knob)
std::vector<juce::Component*> HiLowCutPluginAudioProcessorEditor::getComps()
{
    return
    {
        &knob1
    };
}
