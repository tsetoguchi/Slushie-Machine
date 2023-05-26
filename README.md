# The-Slushie-Machine
A multicomponent audio processing application that focuses on maintaining a simplistic UI/UX workflow.

**Goal**

Our goal as producers and developers has been to introduce simplicity and creativity in our plugins. The primary purpose of our project is to create an effects plugin that appeals to a wide demographic of users. We believe that a simple, easy to use plugin will naturally appeal to a wider demo-
graphic. We developed a plugin called The Slushie Machine capable of modifying sound in various ways through the use of multiple effects combined in series. By carefully customizing the parameters, orderings, and algorithms of these effects, we have created our own recipe that is unique to our plugin.

**Background**

Our project was implemented using the JUCE, an open-source cross-platform C++ application framework for creating high quality desktop and mobile application, more specifically, we developed an audio plugin in VST3 format. We worked with JUCE on VisualStudio and considered it our main software component for our project development since we would develop, build and test on it. In addition, we used Github  to share and organize all the files. Some other software tools were used such as JKnobMan and Photoshop in order to create the components for our UI. And finally we used FL Studio and Ableton Live for live production testing of the plugin. 

**Developing Our Recipe**

The process of developing any audio plugin begins by sound experimentation. We knew the importance of chasing a target sound that we might want our plugin to achieve. We decided that The Slushie Machine would not be creating any sound independently, instead it would modify incoming signals. Many hours were spent experimenting with various mixing plugins in FL Studio and Ableton. Eventually, we decided that it would be nice to incorporate multiple effects and chain them in series as the basis of our plugin. The goal was to create different permutations of the order of effects in the chain. The same effects would be used however, different permutations would have the same number of effects. We also wanted to provide our users with parameters the effects. The effects chosen to be in our chain were: Delay, IIR Filter, Chorus, Limiter, Imager, and Waveshaper.

