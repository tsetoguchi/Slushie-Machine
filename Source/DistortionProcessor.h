#pragma once
#include <JuceHeader.h>

class DistortionProcessor : public juce::AudioProcessor
{
private:
	float sampleValue;


	float DistortionProcessor::sgn(int sample) {
        int y = 0;
        if (sample > 0) {
            y = 1 - exp(-sample);
        }
        else {
            y = -1 + exp(sample);
        }
        return sample / abs(sample) * y;
	}
};




