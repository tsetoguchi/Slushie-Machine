/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HiLowCutPluginAudioProcessorEditor::HiLowCutPluginAudioProcessorEditor(HiLowCutPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), knob1Attachment(audioProcessor.apvts, "Knob 1", knob1), 
    knob2Attachment(audioProcessor.apvts, "Knob 2", knob2), knob3Attachment(audioProcessor.apvts, "Knob 3", knob3), 
    knob4Attachment(audioProcessor.apvts, "HiCut Freq", knob4), knob5Attachment(audioProcessor.apvts, "Dist Drive", knob5)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.


    addAndMakeVisible(knob1);

    for (auto* comp : getComps())
    {
        addAndMakeVisible(comp);
    }



    setSize (580, 282); //size of box
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


    // this for loop[ is simply to visualize each bound that surrounds each knob 
   for (juce::Rectangle bound : getKnobRectangleBounds()) 
    {
        g.drawRect(bound);
    }

}

void HiLowCutPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    std::vector<juce::Rectangle<int>> allBoundsVector = getKnobRectangleBounds();

    //knob 1
    knob1.setBounds(allBoundsVector[0]);
    //knob2
    knob2.setBounds(allBoundsVector[1]);
    //knob 3
    knob3.setBounds(allBoundsVector[2]);
    //knob 4
    knob4.setBounds(allBoundsVector[3]);
    //knob 5
    knob5.setBounds(allBoundsVector[4]);
}

// add all knobs here in the future (rn im just testing 1 knob)
std::vector<juce::Component*> HiLowCutPluginAudioProcessorEditor::getComps()
{
    return
    {
        &knob1,
        &knob2,
        &knob3, 
        &knob4, 
        &knob5
    };
}

// this helper function returns a list of all the rectangles that represents the bounds of each knob
// knob 1 should use the bound of the first element of the vector and knob n should use the bounds of n - 1 element in the vector
std::vector<juce::Rectangle<int>> HiLowCutPluginAudioProcessorEditor::getKnobRectangleBounds()
{
    std::vector<juce::Rectangle<int>> toReturn{};

    auto bounds1 = getLocalBounds(); 
    auto bounds2 = getLocalBounds(); 
    auto bounds3 = getLocalBounds(); 
    auto bounds4 = getLocalBounds();
    auto bounds5 = getLocalBounds();

    //knob 1
    auto boundsHalfLeft = bounds1.removeFromLeft(bounds1.getWidth() * 0.5); 
    juce::Point<int> centerLeftPoint = boundsHalfLeft.getCentre(); 
    juce::Rectangle<int> rectangleKnob1(100, 100); 
    rectangleKnob1.setCentre(centerLeftPoint); 
    rectangleKnob1.setY(130); 

    //knob 2 
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

    //knob 4 (lowpass)
    auto boundsHalfLeft2 = bounds4.removeFromLeft(bounds4.getWidth() * 0.33);
    juce::Point<int> centerLeftPoint2 = boundsHalfLeft2.getCentre(); 
    juce::Rectangle<int> rectangleKnob4(60, 60);
    rectangleKnob4.setCentre(centerLeftPoint2); 
    rectangleKnob4.setY(60);

    // knob 5 (drive)
    auto boundsHalfRight2 = bounds5.removeFromRight(bounds5.getWidth() * 0.33);
    juce::Point<int> centerRightPoint2 = boundsHalfRight2.getCentre();
    juce::Rectangle<int> rectangleKnob5(75, 75);
    rectangleKnob5.setCentre(centerRightPoint2);
    rectangleKnob5.setY(60);




    toReturn.push_back(rectangleKnob1);
    toReturn.push_back(rectangleKnob2);
    toReturn.push_back(rectangleKnob3);
    toReturn.push_back(rectangleKnob4);
    toReturn.push_back(rectangleKnob5);
    
    return toReturn;
}
