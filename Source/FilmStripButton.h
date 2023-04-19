/*
  ==============================================================================

    FilmStripButton.h
    Created: 18 Apr 2023 8:38:38pm
    Author:  gordi

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

        if (isHorizontal_) {
            frameHeight = filmStrip.getHeight();
            frameWidth = filmStrip.getWidth() / 2;
        }
        else {
            frameHeight = filmStrip.getHeight() / 2;
            frameWidth = filmStrip.getWidth();
        }
        
        getProperties().set(juce::Identifier("index"), index);
    }

	void paint(juce::Graphics& g) override
	{
        bool isOn = getToggleState(); 

		if (isOn)
		{
			g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
				frameWidth, 0, frameWidth, frameHeight);
		}
		else
		{
			g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
				0, frameHeight, frameWidth, frameHeight);
		}

	}




private:
    juce::Image filmStrip; 
    const bool isHorizontal_;
    int frameWidth, frameHeight;
};
