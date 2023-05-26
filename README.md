# The-Slushie-Machine
A multicomponent audio processing application that focuses on maintaining a simplistic UI/UX workflow.

**Goal**

Our goal as producers and developers has been to introduce simplicity and creativity in our plugins. The primary purpose of our project is to create an effects plugin that appeals to a wide demographic of users. We believe that a simple, easy to use plugin will naturally appeal to a wider demo-
graphic. We developed a plugin called The Slushie Machine capable of modifying sound in various ways through the use of multiple effects combined in series. By carefully customizing the parameters, orderings, and algorithms of these effects, we have created our own recipe that is unique to our plugin.

**Background**

Our project was implemented using the JUCE, an open-source cross-platform C++ application framework for creating high quality desktop and mobile application, more specifically, we developed an audio plugin in VST3 format. We worked with JUCE on VisualStudio and considered it our main software component for our project development since we would develop, build and test on it. In addition, we used Github  to share and organize all the files. Some other software tools were used such as JKnobMan and Photoshop in order to create the components for our UI. And finally we used FL Studio and Ableton Live for live production testing of the plugin. 

**Developing Our Recipe**

The process of developing any audio plugin begins by sound experimentation. We knew the importance of chasing a target sound that we might want our plugin to achieve. We decided that The Slushie Machine would not be creating any sound independently, instead it would modify incoming signals. Many hours were spent experimenting with various mixing plugins in FL Studio and Ableton. Eventually, we decided that it would be nice to incorporate multiple effects and chain them in series as the basis of our plugin. The goal was to create different permutations of the order of effects in the chain. The same effects would be used however, different permutations would have the same number of effects. We also wanted to provide our users with parameters the effects. The effects chosen to be in our chain were: Delay, IIR Filter, Chorus, Limiter, Imager, and Waveshaper.


**Filter Design**

Our implementation utilizes two filters both of which are IIR (infinite impulse response) filters. An IIR filter follows a recursive design where the output from the filter is computed from the current and the previous inputs and outputs. IIR filters hold two characteristics that determine its class, the family type, and the order. 

The family type we opted to proceed with was Butterworth. Butterworth filters are known for maintaining a flat response in the passband. Some sacrifices that are made by using a Butterworth filter are phase linearity and steepness of the attenuation slope. However, for the purpose of our plugin, phase linearity was not a priority, whereas reducing anomalies in the passband was. We felt that ensuring that the filters introduce minimal artifacts into the signal would be optimal. This would allow us to experiment with other effects introduced later within the signal processing stage, without the worry of the filters interfering.

The order of the filter determines the roll off rate and the width of the transition band. Thus, an nth order filter will have a roll off rate of 6n dB/octave. For example, a first order filter will have a roll off rate of 6dB/octave, and a third order filter will have a roll off rate of 18dB/octave. Higher order filters are produced by sequencing first and second order filters.

Main Filter

The main filter used in our plugin is a 16th order low pass Butterworth filter,  which would result in a roll off rate of 96dB/octave. We wanted this filter to replicate the behavior of a  brickwall filter as much as possible. We initially had an order of 100, but discovered that higher order filters have a heavier cost and become more inaccurate. Thus, we decided to decrease the filter order from 100 to 16.

High Pass Filter

Our plugin also positions a 20Hz high pass filter at the end of the processing chains. The first purpose of this filter is to ensure that any possible DC offset that occurred during the processing stage is removed. The second purpose of this filter is to remove additional infrasonic frequencies that could have been added to the signal due to processing artifacting. For these reasons, this filter is always positioned at the end of the processing chain.

Delay

An audio delay effect can be achieved by recording the audio signal as it is being played. Each sample of the audio signal will be stored on a buffer, this buffer will be updated in real-time as the signal plays. Due to memory constraints, the buffer has a finite size, which is why we chose a circular buffer for our delay. Circular buffers are arrays that will wrap back to the beginning and keep recording samples once they have reached the size limit. Samples that are recorded in the buffer will be used again and be mixed in with the dry signal in order to create the output signal. 

JUCE DSP Delay Line

The JUCE DSP Module comes with an object known as the Delay Line. The Delay Line acts as a circular buffer and provides functions in order to add or remove samples from the buffer. Since it is a circular buffer, samples can only be removed from the last position of the array and can only be added to the beginning of the array. 


Sample Calculation and Feedback

We used a for loop in order to read and write each individual sample coming from each input channel. At each iteration of our loop, a sample is read and recorded into the circular buffer. At first, before a sample was added to the buffer, we summed the sample with the current last sample of the buffer. This was our  original implementation but we soon realized that we weren't able to control the strength of the samples being added back into the buffer. Because of this, we would get a consistent echoing effect that could not be controlled.


In order to solve this, we added a new parameter that would represent the feedback strength. The value of this parameter would be multiplied to new samples going into the buffer. If the feedback value was at zero, then samples going into the buffer would be multiplied with zero and the buffer would eventually be \textit{empty}, meaning that the delay effect would not be in effect because the original samples would be not be modified. In contrast, a strong echoing effect would quickly be noticeable if the feedback value was higher than 1. Therefore, we chose a balance and thought it would be best to have a feedback ranging from 0.0 - 0.8. 
