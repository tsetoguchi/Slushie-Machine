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
