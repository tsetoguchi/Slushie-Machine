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

    setSize (600, 400);
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

    auto bounds = getLocalBounds();
    auto boundsTopHalf = bounds.removeFromTop(bounds.getHeight() * 0.66);
    auto reducedRect = boundsTopHalf.reduced(50, 50);
    reducedRect.setSize(120, 120);



    

    g.drawRect(reducedRect);
}

void HiLowCutPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    auto boundsTopHalf = bounds.removeFromTop(bounds.getHeight() * 0.66);
    auto reducedRect = boundsTopHalf.reduced(50, 50);
    reducedRect.setSize(120, 120);





    knob1.setBounds(reducedRect);

}

// add all knobs here in the future (rn im just testing 1 knob)
std::vector<juce::Component*> HiLowCutPluginAudioProcessorEditor::getComps()
{
    return
    {
        &knob1
    };
}
