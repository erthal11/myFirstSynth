/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MyFirstSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    MyFirstSynthAudioProcessorEditor (MyFirstSynthAudioProcessor&);
    ~MyFirstSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider* sliderGain) override;
    
    juce::Slider modFreqSlider;
    juce::Label modFreqLabel;
    
    juce::Slider modDepthSlider;
    juce::Label modDepthLabel;
    
    juce::Slider masterVolumeSlider;
    juce::Label masterVolumeLabel;
    
    juce::Slider lpfSlider;
    juce::Label lpfLabel;
    
    juce::Slider oscVolumeSlider[4];
    juce::Label oscVolumeLabel[4];
    
    juce::ComboBox oscTypeDropdown[4];
    
    juce::Slider oscPitchShiftSlider[4];
    juce::Label oscPitchShiftLabel[4];
    
    juce::TextButton monoPolyButton {"Polyphonic"};
    
    juce::Slider glideTimeSlider;
    juce::Label glideTimeLabel;
    
    juce::Slider attackSlider;
    juce::Label attackLabel;
    
    juce::Slider decaySlider;
    juce::Label decayLabel;
    
    juce::Slider sustainSlider;
    juce::Label sustainLabel;
    
    juce::Slider releaseSlider;
    juce::Label releaseLabel;
    
    juce::Slider driveSlider;
    juce::Label driveLabel;
    
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> modFreqValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> modDepthValue;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> masterVolumeValue;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> oscVolumeValue[4];
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscTypeValue[4];
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> oscPitchShiftValue[4];
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> monoPolyValue;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> glideTimeValue;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> attackValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> decayValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sustainValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> releaseValue;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> driveValue;
    
    void oscTypeChanged();
    
    juce::ImageComponent imageComponent;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyFirstSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyFirstSynthAudioProcessorEditor)
};
