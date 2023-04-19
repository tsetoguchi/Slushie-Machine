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
    knob4Attachment(audioProcessor.apvts, "HiCut Freq", knob4), knob5Attachment(audioProcessor.apvts, "Dist Drive", knob5),
    toggleAttachment(audioProcessor.apvts, "ToggleAB", toggleSwitch)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.


    addAndMakeVisible(knob1);

    for (auto* comp : getComps())
    {
        addAndMakeVisible(comp);
    }



    setSize (539, 238); //size of box
}

HiLowCutPluginAudioProcessorEditor::~HiLowCutPluginAudioProcessorEditor()
{
}

//==============================================================================
void HiLowCutPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
 
    background = juce::ImageCache::getFromMemory(BinaryData::UIv2_png, BinaryData::UIv2_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);


    // this for loop[ is simply to visualize each bound that surrounds each knob 
 /*  for (juce::Rectangle bound : getKnobRectangleBounds()) 
    {   

       juce::Colour white = juce::Colour(0, 0, 0);
       g.setColour(white);
       g.drawRect(bound); 
    }*/

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
    //toggle switch 
    toggleSwitch.setBounds(allBoundsVector[5]);

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
        &knob5,
        &toggleSwitch
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
    juce::Point<int> knob1center;
    knob1center.setX(172);
    knob1center.setY(113);
    juce::Rectangle<int> rectangleKnob1(91, 91);
    rectangleKnob1.setCentre(knob1center);
    

    //knob 2 
    juce::Point<int> middlePoint; 
    middlePoint.setX(270);
    middlePoint.setY(113);
    juce::Rectangle<int> rectangleKnob2(91, 91);
    rectangleKnob2.setCentre(middlePoint);  
   

    //knob 3
    juce::Point<int> knob3center; 
    knob3center.setX(366);
    knob3center.setY(113);
    juce::Rectangle<int> rectangleKnob3(91, 91);
    rectangleKnob3.setCentre(knob3center);

    //knob 4 (lowpass)
    juce::Point<int> knobLowpassCenter; 
    knobLowpassCenter.setX(62);
    knobLowpassCenter.setY(145);
    juce::Rectangle<int> rectangleKnob4(60, 60);
    rectangleKnob4.setCentre(knobLowpassCenter);

    // knob 5 (drive)
    juce::Point<int> knobDriveCenter;
    knobDriveCenter.setX(62);
    knobDriveCenter.setY(70);
    juce::Rectangle<int> rectangleKnob5(80, 80);
    rectangleKnob5.setCentre(knobDriveCenter); 

    //toggle Switch 
    juce::Point<int> toggleSwitchCenter;
    toggleSwitchCenter.setX(475);
    toggleSwitchCenter.setY(108);
    juce::Rectangle<int> rectangleToggleSwitch(50, 50);
    rectangleToggleSwitch.setCentre(toggleSwitchCenter);  



    toReturn.push_back(rectangleKnob1);
    toReturn.push_back(rectangleKnob2);
    toReturn.push_back(rectangleKnob3);
    toReturn.push_back(rectangleKnob4);
    toReturn.push_back(rectangleKnob5);
    toReturn.push_back(rectangleToggleSwitch);
    
    return toReturn;
}
