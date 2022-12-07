/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyFirstSynthAudioProcessor::MyFirstSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
treeState (*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
}

MyFirstSynthAudioProcessor::~MyFirstSynthAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout MyFirstSynthAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto normRangeModFreq = juce::NormalisableRange<float>(0,10);
    normRangeModFreq.setSkewForCentre(3);
    
    auto normRangeModDepth = juce::NormalisableRange<float>(0,1);
    normRangeModDepth.setSkewForCentre(.1);
    
    auto normRangeVolume = juce::NormalisableRange<float>(-120,0);
    normRangeVolume.setSkewForCentre(-18);
    
    auto normRangeDrive = juce::NormalisableRange<float>(1,10);
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"modFreq",P_ID_NUM}, "ModFreq",  normRangeModFreq, 1.21));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"modDepth",P_ID_NUM}, "ModDepth",  normRangeModDepth, 0.007));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"master",P_ID_NUM}, "Master",  normRangeVolume, -18));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"drive",P_ID_NUM}, "Drive",  normRangeDrive, 1));
    
    juce::StringArray oscTypeList = {"Sine","Triangle","Sawtooth","Square", "New"};
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"oscillatorVolume0",P_ID_NUM}, "OscillatorVolume0",  normRangeVolume, -20));
    params.push_back (std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"oscillatorType0",P_ID_NUM}, "OscillatorType0", oscTypeList, 3));
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"oscillatorPitchShift0",P_ID_NUM}, "OscillatorPitchShift0",-24,24,0));
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"oscillatorVolume1",P_ID_NUM}, "OscillatorVolume1",  normRangeVolume, -10));
    params.push_back (std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"oscillatorType1",P_ID_NUM}, "OscillatorType1", oscTypeList, 2));
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"oscillatorPitchShift1",P_ID_NUM}, "OscillatorPitchShift1",-24,24,12));
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"oscillatorVolume2",P_ID_NUM}, "OscillatorVolume2",  normRangeVolume, -8));
    params.push_back (std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"oscillatorType2",P_ID_NUM}, "OscillatorType2", oscTypeList, 0));
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"oscillatorPitchShift2",P_ID_NUM}, "OscillatorPitchShift2",-24,24,-12));
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"oscillatorVolume3",P_ID_NUM}, "OscillatorVolume3",  normRangeVolume, -5));
    params.push_back (std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"oscillatorType3",P_ID_NUM}, "OscillatorType3", oscTypeList, 0));
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"oscillatorPitchShift3",P_ID_NUM}, "OscillatorPitchShift3",-24,24,12));
    
    
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"attack",P_ID_NUM}, "Attack",0,1,0));
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"decay",P_ID_NUM}, "Decay",0,1,0));
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"sustain",P_ID_NUM}, "Sustain",0,1,0));
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"release",P_ID_NUM}, "Release",0,1,0));
    
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"monoPoly",P_ID_NUM}, "MonoPoly", false));
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"glideTime",P_ID_NUM}, "GlideTime",  0, 2, 0));


    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String MyFirstSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyFirstSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyFirstSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyFirstSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyFirstSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyFirstSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyFirstSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyFirstSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyFirstSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyFirstSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyFirstSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.prepareToPlay(sampleRate);
}

void MyFirstSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyFirstSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MyFirstSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    
    for (int osc=0;osc<4;osc++)
    {
        oscillatorVolumes[osc] = dbToLinear(treeState.getRawParameterValue("oscillatorVolume"+to_string(osc))->load());
        oscillatorTypes[osc] = treeState.getRawParameterValue("oscillatorType"+to_string(osc))->load();
        oscillatorPitchShift[osc] = treeState.getRawParameterValue("oscillatorPitchShift"+to_string(osc))->load();
    }
    
    synth.setOscillatorTypes(oscillatorTypes);
    
    synth.setEffectParameters(
                              dbToLinear(treeState.getRawParameterValue("master")->load()),
                              treeState.getRawParameterValue("modFreq")->load(),
                              treeState.getRawParameterValue("modDepth")->load(),
                              oscillatorVolumes,
                              oscillatorPitchShift,
                              treeState.getRawParameterValue("monoPoly")->load(),
                              treeState.getRawParameterValue("glideTime")->load(),
                              treeState.getRawParameterValue("drive")->load()
                              );
    
    
    synth.render(buffer, midiMessages);
    
}

//==============================================================================
bool MyFirstSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyFirstSynthAudioProcessor::createEditor()
{
    return new MyFirstSynthAudioProcessorEditor (*this);
}

//==============================================================================
void MyFirstSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MyFirstSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
         
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (treeState.state.getType()))
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyFirstSynthAudioProcessor();
}
