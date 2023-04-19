/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HiLowCutPluginAudioProcessor::HiLowCutPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

HiLowCutPluginAudioProcessor::~HiLowCutPluginAudioProcessor()
{
}

//==============================================================================
const juce::String HiLowCutPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HiLowCutPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HiLowCutPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HiLowCutPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HiLowCutPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HiLowCutPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HiLowCutPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HiLowCutPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HiLowCutPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void HiLowCutPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void setSafetyLimiter(juce::dsp::Limiter<float> limiter, juce::dsp::ProcessSpec spec) {
// nothing atm
}

//==============================================================================
void HiLowCutPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate; 
    
    // here we pass in the spec to both left and right chains
    leftChain.prepare(spec);
    rightChain.prepare(spec);

    updateFilters();


    // this is a second spec for delayLine from dsp_module (they all need to be processed with a spec)
    juce::dsp::ProcessSpec spec2;

    spec2.maximumBlockSize = samplesPerBlock;
    spec2.numChannels = getTotalNumInputChannels();
    spec2.sampleRate = sampleRate;


    // DELAY SETTERS
    float initialDelay = 24000;
    delayLine.prepare(spec2, initialDelay);


    // CHORUS SETTERS
    float chorusCentreDelay = 15;
    float chorusRate = 0.5;
    float chorusMix = 1;
    chorus.prepare(spec2, chorusCentreDelay, chorusRate, chorusMix);


    mySampleRate = sampleRate;


    // SAFETY LIMITER
    limiter.reset();
    limiter.setThreshold(0);
    limiter.setRelease(50);
    limiter.prepare(spec2);

    waveshaper.functionToUse = [](float x)
    {
        float out = x;

        if (x <= -1.7f)
            out = -1.0f;
        else if ((x > -1.7f) && (x < -0.3f))
        {
            x += 0.3f;
            out = x + (x * x) / (4 * (1 - 0.3f)) - 0.3f;
        }
        else if ((x > 0.9f) && (x < 1.1f))
        {
            x -= 0.9f;
            out = x - (x * x) / (4 * (1 - 0.9f)) + 0.9f;
        }
        else if (x > 1.1f)
            out = 1.0f;

        return out;

    };

    updateKnobsAorB(true);
    
}

float HiLowCutPluginAudioProcessor::sgnDist(float x) {
        float y = 0;
        if (x > 0) {
            y = 1 - exp(-x);
        }
        else {
            y = -1 + exp(x);
        }
        return y;
}

float HiLowCutPluginAudioProcessor::janosDist(float x) {
    float out = x;

    if (x <= -1.7f)
        out = -1.0f;
    else if ((x > -1.7f) && (x < -0.3f))
    {
        x += 0.3f;
        out = x + (x * x) / (4 * (1 - 0.3f)) - 0.3f;
    }
    else if ((x > 0.9f) && (x < 1.1f))
    {
        x -= 0.9f;
        out = x - (x * x) / (4 * (1 - 0.9f)) + 0.9f;
    }
    else if (x > 1.1f)
        out = 1.0f;

    return out;
};

void HiLowCutPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HiLowCutPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HiLowCutPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    juce::dsp::AudioBlock<float> sampleBlock(buffer);

    ChainSettings settingsOfParameters = getChainSettings(apvts);
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Drive
    buffer.applyGain(settingsOfParameters.distDrive);

    juce::dsp::AudioBlock<float> block(buffer);
    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    updateKnobsAorB(settingsOfParameters.toggleAB); 

    if (settingsOfParameters.toggleAB) {

        // Chorus
        chorus.process(sampleBlock);

        // Imager
        imager.process(buffer, totalNumInputChannels, settingsOfParameters.knob3 + 1);

        // Delay
        delayLine.process(totalNumInputChannels, buffer, settingsOfParameters.knob1);

        // Waveshaper
        waveshaper.functionToUse = [](float x)
            {
                float out = x;

                if (x <= -1.7f)
                    out = -1.0f;
                else if ((x > -1.7f) && (x < -0.3f))
                {
                    x += 0.3f;
                    out = x + (x * x) / (4 * (1 - 0.3f)) - 0.3f;
                }
                else if ((x > 0.9f) && (x < 1.1f))
                {
                    x -= 0.9f;
                    out = x - (x * x) / (4 * (1 - 0.9f)) + 0.9f;
                }
                else if (x > 1.1f)
                    out = 1.0f;

                return out;

            };
        waveshaper.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));
        
        // Filter
        juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
        juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
        leftChain.process(leftContext);
        rightChain.process(rightContext);
        updateHighCutFilters(settingsOfParameters);
    }
    else {

        // Filter
        juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
        juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
        leftChain.process(leftContext);
        rightChain.process(rightContext);
        updateHighCutFilters(settingsOfParameters);

        // Delay
        delayLine.process(totalNumInputChannels, buffer, settingsOfParameters.knob1);

        // Chorus
        chorus.process(sampleBlock);

        // Waveshaper
        waveshaper.functionToUse = [](float x)
        {
            float out = x;

            if (x <= -1.7f)
                out = -1.0f;
            else if ((x > -1.7f) && (x < -0.3f))
            {
                x += 0.3f;
                out = x + (x * x) / (4 * (1 - 0.3f)) - 0.3f;
            }
            else if ((x > 0.9f) && (x < 1.1f))
            {
                x -= 0.9f;
                out = x - (x * x) / (4 * (1 - 0.9f)) + 0.9f;
            }
            else if (x > 1.1f)
                out = 1.0f;

            return out;

        };
        waveshaper.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));

        // Imager
        imager.process(buffer, totalNumInputChannels, settingsOfParameters.knob3 + 1);

    }

    

    limiter.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));

    // Anti-aliasing Filter
    updateAntiAliasingFilter(settingsOfParameters);
}

//==============================================================================
bool HiLowCutPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HiLowCutPluginAudioProcessor::createEditor()
{
    return new HiLowCutPluginAudioProcessorEditor(*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void HiLowCutPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream mos(destData, true); 
    apvts.state.writeToStream(mos);
}

void HiLowCutPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        apvts.replaceState(tree);
        updateFilters();
    }
}


ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts) {

    ChainSettings settings; 

    settings.lowCutFreq = apvts.getRawParameterValue("LowCut Freq")->load();
    settings.highCutFreq = apvts.getRawParameterValue("HiCut Freq")->load();

    //settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCut Slope")->load());
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HiCut Slope")->load());

    settings.distDrive = apvts.getRawParameterValue("Dist Drive")->load();

    settings.distBypass = apvts.getRawParameterValue("Dist Bypass")->load();

    settings.compressorThreshold = apvts.getRawParameterValue("Compressor Threshold")->load();

    settings.knob1 = apvts.getRawParameterValue("Knob 1")->load();

    settings.knob2 = apvts.getRawParameterValue("Knob 2")->load();

    settings.knob3 = apvts.getRawParameterValue("Knob 3")->load();

    settings.width = apvts.getRawParameterValue("width")->load();

    settings.toggleAB = apvts.getRawParameterValue("ToggleAB")->load(); 

    return settings; 
}

    // this function below sets up the parameter layout for all the parameters that will be available to be changed in the gui

juce::AudioProcessorValueTreeState::ParameterLayout
HiLowCutPluginAudioProcessor::createParameterLayout() {

    juce::AudioProcessorValueTreeState::ParameterLayout layout; // we add any parameter to be created to this


    // Parameters for HI/LOW Cut

    layout.add(std::make_unique<juce::AudioParameterFloat>("LowCut Freq",
        "LowCut Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
        20.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("HiCut Freq",
        "HiCut Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
        20000.f));

    juce::StringArray stringArray;

    for (int i = 0; i < 4; i++) {
        juce::String str;
        str << (12 + i * 12);
        str << "db/Oct";
        stringArray.add(str);
    }

    //layout.add(std::make_unique<juce::AudioParameterChoice>("LowCut Slope", "LowCut Slope", stringArray, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("HiCut Slope", "HiCut Slope", stringArray, 0));


    // Parameters for Gain


    // Parameters for Delay 


    // Parameters for Chorus

    layout.add(std::make_unique<juce::AudioParameterBool>("Dist Bypass", "Dist Bypass", true));

    // Parameter for Distortion Drive
    layout.add(std::make_unique<juce::AudioParameterFloat>("Dist Drive", "Dist Drive", 1.0f, 12.0f, 0.0f));  

    // Parameter for SAFETY Compressor
    layout.add(std::make_unique<juce::AudioParameterFloat>("Compressor Threshold", "Compressor Threshold", -100.0f, 0.0f, -6.0f));



    // Knob 1
    //ChorusDepth(0 - 0.5) -  ChorusFeedback(0 - 0.1) - DelayFeedback(0 - 0.1)
    layout.add(std::make_unique<juce::AudioParameterFloat>("Knob 1", "Knob 1", 0.0f, 1.0f, 0.0f));
    
    // Knob 2
    // ChorusCentreDelay - DelayDelayTime
    layout.add(std::make_unique<juce::AudioParameterFloat>("Knob 2", "Knob 2", 0.0f, 1.0f, 0.0f));

    // Knob 3
    // ChorusMix - SaturationDrive
    layout.add(std::make_unique<juce::AudioParameterFloat>("Knob 3", "Knob 3", 0.0f, 1.0f, 0.0f));

    // width
    layout.add(std::make_unique<juce::AudioParameterFloat>("width", "width", 1.0f, 5.0f, 1.0f));

    // Toggle A and B switch 
    layout.add(std::make_unique<juce::AudioParameterBool>("ToggleAB", "ToggleAB", true));

    return layout;
}

void HiLowCutPluginAudioProcessor::updateCoefficients(Coefficients& old, const Coefficients& replacements)
{
    *old = *replacements;
}

void HiLowCutPluginAudioProcessor::updateAntiAliasingFilter(const ChainSettings& chainSettings)
{

    float antiAliasingFreq = 20;

    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
        antiAliasingFreq, getSampleRate(), 2);

    auto& leftLowCut = leftChain.get<chainPositions::LowCut>();
    updateCutFilter(leftLowCut, lowCutCoefficients, chainSettings.lowCutSlope);

    auto& rightLowCut = rightChain.get<chainPositions::LowCut>();
    updateCutFilter(rightLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
}

void HiLowCutPluginAudioProcessor::updateHighCutFilters(const ChainSettings& chainSettings)
{
    auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.highCutFreq, getSampleRate(), 100 * (chainSettings.highCutSlope + 1));

    auto& leftHighCut = leftChain.get<chainPositions::HighCut>();
    updateCutFilter(leftHighCut, highCutCoefficients, chainSettings.highCutSlope);

    auto& rightHighCut = rightChain.get<chainPositions::HighCut>();
    updateCutFilter(rightHighCut, highCutCoefficients, chainSettings.highCutSlope);
}

//void HiLowCutPluginAudioProcessor::setAntiAliasFilter(const ChainSettings& chainSettings) {
//    auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.antiAliasFilterFreq, getSampleRate(), 6);
//    auto& leftHighCut = leftChain.get<chainPositions::HighCut>();
//    auto& rightHighCut = rightChain.get<chainPositions::HighCut>();
//}

void HiLowCutPluginAudioProcessor::updateFilters() {
    auto chainSettings = getChainSettings(apvts);
    updateHighCutFilters(chainSettings);
    updateAntiAliasingFilter(chainSettings);
}

void HiLowCutPluginAudioProcessor::updateKnobsAorB(bool mode) {
    updateKnob1(mode);
    updateKnob2(mode);
    updateKnob3(mode);
}

void HiLowCutPluginAudioProcessor::updateKnob1(bool mode) {
    auto chainSettings = getChainSettings(apvts);

    auto coefficient = chainSettings.knob1;

    float maxChorusDepth = 1.0;
    float maxChorusFeedback = 0.1;
    float maxDelayFeedback = 0.1;

    chorus.setDepth(coefficient * maxChorusDepth);
    chorus.setFeedback(coefficient * maxChorusFeedback);
}

void HiLowCutPluginAudioProcessor::updateKnob2(bool mode) {
    auto chainSettings = getChainSettings(apvts);
    auto coefficient = chainSettings.knob2;

    // 15 -> 5
    float maxChorusDelay = 5;
    float maxDelayDelayTime = 0.05;
    chorus.setCentreDelay(coefficient * maxChorusDelay);

    // reset the delayLine if the delayTime is 0
    if (coefficient == 0) {
        delayLine.setDelay(0);
    }
    else {
        int delayTimeInSamples = coefficient * mySampleRate * maxDelayDelayTime;
        delayLine.setDelay(delayTimeInSamples);
    }
}

void HiLowCutPluginAudioProcessor::updateKnob3(bool mode) {
    auto chainSettings = getChainSettings(apvts);
    auto coefficient = chainSettings.knob3;
   
    float maxChorusMix = 1;
    chorus.setMix(coefficient * maxChorusMix);
}








//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HiLowCutPluginAudioProcessor();
}
