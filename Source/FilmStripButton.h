/*
  ==============================================================================

    FilmStripButton.h
    Created: 18 Apr 2023 8:38:38pm
    Author:  gordo

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>


class FilmStripButton : public juce::Button
{
public:
    FilmStripButton(juce::Image image, const bool stripIsHorizontal, int index)
        :Button(juce::String(index)),
        filmStrip(image),
        isHorizontal_(stripIsHorizontal)
    {
        setToggleable(true);
        setClickingTogglesState(true);

        frameHeight = filmStrip.getHeight() / 2;
        frameWidth = filmStrip.getWidth();
        
        getProperties().set(juce::Identifier("index"), index);
    }

	void paint(juce::Graphics& g) override
	{
        bool isOn = getToggleState(); 

        if (isHorizontal_)
        {
            if (isOn) {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    0 * frameWidth, 0, frameWidth, frameHeight);
            }
            else {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    1 * frameWidth, 0, frameWidth, frameHeight);
            }
        }
        else
        {
            if (isOn) {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    0, 0 * frameHeight, frameWidth, frameHeight);
            }

            else {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                    0, 1 * frameHeight, frameWidth, frameHeight);
            }
        
        }

	}

    // this one is always requeired and would be better if there was mpre effects on the buttons
    virtual void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
        bool 	shouldDrawButtonAsDown) override {
    }

private:
    juce::Image filmStrip; 
    const bool isHorizontal_;
    int frameWidth, frameHeight;
};
