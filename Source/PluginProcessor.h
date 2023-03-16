/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum Slope {
    Slope_12, 
    Slope_24, 
    Slope_36,
    Slope_48
};

struct ChainSettings
{
    float lowCutFreq{ 0 }, highCutFreq{ 0 };

    Slope lowCutSlope{ Slope::Slope_12 }, highCutSlope{ Slope::Slope_12 };
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);

//========hello======================================================================
/**
*/
class HiLowCutPluginAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    HiLowCutPluginAudioProcessor();
    ~HiLowCutPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

private:

    using Filter = juce::dsp::IIR::Filter<float>;

    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;

    using MonoChain = juce::dsp::ProcessorChain<CutFilter, CutFilter>; //careful here (34 min)

    MonoChain leftChain, rightChain;

    enum chainPositions {
        LowCut, HighCut
    };

    using Coefficients = Filter::CoefficientsPtr;
    static void updateCoefficients(Coefficients& old, const Coefficients& replacements);

    template<int index, typename ChainType, typename CoefficientType>
    void update(ChainType& chain, const CoefficientType& cutCoefficients) {
        updateCoefficients(chain.template get<index>().coefficients, cutCoefficients[index]);
        chain.template setBypassed<index>(false);
    }
    
    template<typename ChainType, typename CoefficientType>
    void updateCutFilter(ChainType& leftLowCut, const CoefficientType& cutCoefficients, 
        const Slope& lowCutSlope) {

     

        leftLowCut.setBypassed<0>(true);
        leftLowCut.setBypassed<1>(true);
        leftLowCut.setBypassed<2>(true);
        leftLowCut.setBypassed<3>(true);

        switch (lowCutSlope)
        {

        case Slope_48:
        {
            update<3>(leftLowCut, cutCoefficients);

        }
        case Slope_36:
        {
            update<2>(leftLowCut, cutCoefficients);
        }
        case Slope_24:
        {
            update<1>(leftLowCut, cutCoefficients);
        }
        case Slope_12:
        {
            update<0>(leftLowCut, cutCoefficients);
        }
            



        /*case Slope_12: {
            *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            break;
        }
        case Slope_12: {
            *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            break;
        }
        case Slope_36: {
            *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            *leftLowCut.template get<2>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<2>(false);
            break;
        }
        case Slope_48: {
            *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            *leftLowCut.template get<2>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<2>(false);
            *leftLowCut.template get<3>().coefficients = *cutCoefficients[1];
            leftLowCut.template setBypassed<3>(false);
            break;
        }*/
       }
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HiLowCutPluginAudioProcessor)
};
