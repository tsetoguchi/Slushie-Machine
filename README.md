# The Slushie Machine

A multi-component audio processing plugin focused on simplicity and creativity, designed for an intuitive and streamlined user experience.

## Overview

**The Slushie Machine** is an audio effects plugin created to bring creative sound-shaping capabilities to a broad range of users. By integrating a series of customizable effects, this plugin provides a unique "recipe" for sound modification. Our goal has always been to offer a powerful tool that is both easy to use and effective for diverse sound design needs.

## Goal

As both producers and developers, we aimed to make an effects plugin that introduces simplicity and creative freedom into any workflow. We believe that an intuitive, easy-to-use interface will appeal to a wider audience. By combining multiple effects with carefully customized parameters and algorithms, we crafted The Slushie Machine to deliver versatile sound transformations with ease.

## Background

### Technology Stack
The Slushie Machine was developed using:
- **JUCE** – an open-source, cross-platform C++ framework for creating desktop and mobile applications. We specifically used JUCE for VST3 audio plugin development.
- **Visual Studio** – our main environment for development, building, and testing.
- **GitHub** – for file organization, collaboration, and version control.
- **JKnobMan** & **Photoshop** – to create custom UI components.
- **FL Studio** & **Ableton Live** – for extensive testing during live production.

This combination of tools allowed us to develop, design, and refine the plugin effectively.

## Developing the "Recipe"

Creating The Slushie Machine involved extensive sound experimentation to find the perfect target sound profile. The plugin was designed to modify incoming audio rather than generate new sound. Here’s how we approached the process:
1. **Experimentation:** We spent hours testing various effects and mixing setups in FL Studio and Ableton Live to explore different sound possibilities.
2. **Effect Chaining:** We decided to incorporate multiple effects in series to allow unique permutations and chaining. Although each effect remains the same, different orderings yield distinct sonic results.
3. **User Control:** To enhance flexibility, we provide adjustable parameters for each effect, allowing users to fine-tune and explore their own sound transformations.

### Effects Used
The plugin chains the following effects in series:
- **Delay:** Adds depth and spatial layering.
- **IIR Filter:** Adjusts frequencies to shape tonal qualities.
- **Chorus:** Adds richness and movement to sounds.
- **Limiter:** Ensures audio peaks stay within a controlled range.
- **Imager:** Enhances stereo width and spatial perception.
- **Waveshaper:** Adds harmonic distortion and texture.

Each effect has adjustable parameters, empowering users to explore limitless creative combinations.
