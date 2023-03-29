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
    delayLine.reset();
    delayLine.prepare(spec2);
    delayLine.setDelay(24000);


    // CHORUS SETTERS
    chorus.reset();
    chorus.prepare(spec2);
    chorus.setCentreDelay(15);
    chorus.setDepth(0.5);
    chorus.setRate(8);
    chorus.setFeedback(0.5);
    mySampleRate = sampleRate;

    // SAFETY LIMITER
    limiter.reset();
    limiter.prepare(spec2);
}

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

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateFilters();
    ChainSettings settingsOfParameters = getChainSettings(apvts);

    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    // chorus processing

    juce::dsp::AudioBlock<float> sampleBlock(buffer);
    chorus.setMix(settingsOfParameters.chorusMix);
    chorus.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));

    limiter.setRelease(40.0f);
    limiter.setThreshold(-10.0f);
    limiter.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));
    

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);

    buffer.applyGain(settingsOfParameters.gainSetting); // this line solely for volume 

    // reset the delayLine if the delayTime is 0
    if (settingsOfParameters.delayTime == 0) {
        delayLine.reset();
    }
    else {
        int delayTimeInSamples = settingsOfParameters.delayTime * mySampleRate;
        delayLine.setDelay(delayTimeInSamples);
    }
    


   /* delayLine.process(leftContext);
    delayLine.process(rightContext);*/
    //we would use the process function above if we wanted to have a delay
    //that does not change and cant be changed by sliders so like just constant  

    for (int channel = 0; channel < totalNumInputChannels; channel++) {

        auto* inSamples = buffer.getReadPointer(channel); 
        auto* outSamples = buffer.getWritePointer(channel);


        for (int i = 0; i < buffer.getNumSamples(); i++) {

            float delayedSample = delayLine.popSample(channel);
            float newSampleToPush = inSamples[i] + (delayedSample * settingsOfParameters.feedBack);
            delayLine.pushSample(channel, newSampleToPush);
            outSamples[i] = inSamples[i] + delayedSample; 
        }
    }

    // Distortion Processing
    //for (int channel = 0; channel < totalNumInputChannels; channel++) {

    //    auto* inSamples = buffer.getReadPointer(channel);
    //    auto* outSamples = buffer.getWritePointer(channel);


    //    for (int i = 0; i < buffer.getNumSamples(); i++) {
    //        int y = 0;
    //        if (inSamples[i] > 0) {
    //            y = 1 - exp(-inSamples[i]);
    //        }
    //        else {
    //            y = -1 + exp(inSamples[i]);
    //        }
    //        float distortedSample = inSamples[i] / abs(inSamples[i]) * y;
    //        float newSampleToPush = inSamples[i] + (distortedSample * settingsOfParameters.feedBack);
    //        delayLine.pushSample(channel, newSampleToPush);
    //        outSamples[i] = inSamples[i] + distortedSample;
    //    }
    //}
    
}

//==============================================================================
bool HiLowCutPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HiLowCutPluginAudioProcessor::createEditor()
{
    //return new HiLowCutPluginAudioProcessorEditor(*this);
    return new juce::GenericAudioProcessorEditor(*this);
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

    settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCut Slope")->load());
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HiCut Slope")->load());

    settings.gainSetting = apvts.getRawParameterValue("Gain")->load(); 

    settings.delayTime = apvts.getRawParameterValue("Delay Time")->load();
    settings.delayTime =  apvts.getRawParameterValue("Feedback")->load();

    settings.chorusMix = apvts.getRawParameterValue("Chorus Mix")->load();

    settings.limiterThreshold = apvts.getRawParameterValue("Limiter Threshold")->load();

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

    layout.add(std::make_unique<juce::AudioParameterChoice>("LowCut Slope", "LowCut Slope", stringArray, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("HiCut Slope", "HiCut Slope", stringArray, 0));


    // Parameters for Gain

    layout.add(std::make_unique<juce::AudioParameterFloat>("Gain",
        "Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        0.5f));


    // Parameters for Delay 

    layout.add(std::make_unique<juce::AudioParameterFloat>("Feedback",
        "Feedback",
        0.00f, 0.09f,
        0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Delay Time",
        "Delay Time",
        0.00f, 0.9f,
        0.0f));

    // Parameters for Chorus

    layout.add(std::make_unique<juce::AudioParameterFloat>("Chorus Mix",
        "Chorus Mix",
        0.00f, 1.0f,
        0.0f));

    // Parameter for SAFETY Limiter
    layout.add(std::make_unique<juce::AudioParameterFloat>("Limiter Threshold", "Limiter Threshold", -100.0f, 0.0f, -6.0f));

    return layout;
}

void HiLowCutPluginAudioProcessor::updateCoefficients(Coefficients& old, const Coefficients& replacements)
{
    *old = *replacements;
}

void HiLowCutPluginAudioProcessor::updateLowCutFilters(const ChainSettings& chainSettings)
{
    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCutFreq,
        getSampleRate(),
        2 * (chainSettings.lowCutSlope + 1));

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

void HiLowCutPluginAudioProcessor::updateFilters() {
    auto chainSettings = getChainSettings(apvts);
    updateHighCutFilters(chainSettings);
    updateLowCutFilters(chainSettings);
}




//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HiLowCutPluginAudioProcessor();
}
