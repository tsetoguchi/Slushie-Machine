/*
  ==============================================================================

    StereoImager.h
    Created: 11 Apr 2023 3:09:23am
    Author:  tao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class StereoImager {

public:

	juce::AudioBuffer<float> processedBuffer;

    void process(juce::AudioBuffer<float>& buffer, int numInputChannels, int width) {

			juce::dsp::AudioBlock<float> block(buffer);
			float* leftChannel = buffer.getWritePointer(0);
			float* rightChannel = buffer.getWritePointer(1);


			for (int i = 0; i < buffer.getNumSamples(); ++i)
			{
				float widthVal = width * 0.5;
				float tmp = 1 / fmax(1 + width, 2);
				float coef_m = 1 * tmp;
				float coef_s = width * tmp;

				float mid = (leftChannel[i] + rightChannel[i]) * coef_m;
				float sides = (rightChannel[i] - leftChannel[i]) * coef_s;

				leftChannel[i] = mid - sides;
				rightChannel[i] = mid + sides;
			}
        }

};