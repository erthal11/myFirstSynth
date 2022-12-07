/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyFirstSynthAudioProcessorEditor::MyFirstSynthAudioProcessorEditor (MyFirstSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 500);
    
    modFreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    modFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    modFreqSlider.addListener(this);
    addAndMakeVisible(&modFreqSlider);
       
    modFreqValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "modFreq", modFreqSlider);
       
    modFreqLabel.attachToComponent(&modFreqSlider, false);
    modFreqLabel.setText("Frequency", juce::dontSendNotification);
    modFreqLabel.setJustificationType(juce::Justification::centred);
    
    
    
    
    
    modDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    modDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    modDepthSlider.addListener(this);
    addAndMakeVisible(&modDepthSlider);
       
    modDepthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "modDepth", modDepthSlider);
       
    modDepthLabel.attachToComponent(&modDepthSlider, false);
    modDepthLabel.setText("Depth", juce::dontSendNotification);
    modDepthLabel.setJustificationType(juce::Justification::centred);
    
    
 

    
    
    masterVolumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    masterVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    masterVolumeSlider.addListener(this);
    addAndMakeVisible(&masterVolumeSlider);
       
    masterVolumeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "master", masterVolumeSlider);
       
    masterVolumeLabel.attachToComponent(&masterVolumeSlider, false);
    masterVolumeLabel.setText("Master", juce::dontSendNotification);
    masterVolumeLabel.setJustificationType(juce::Justification::centred);
    
    
    
    
    for (int osc=0; osc<4; osc++)
    {
        oscVolumeSlider[osc].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        oscVolumeSlider[osc].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
        oscVolumeSlider[osc].addListener(this);
        addAndMakeVisible(&oscVolumeSlider[osc]);
        
        oscVolumeValue[osc] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, ("oscillatorVolume"+to_string(osc)), oscVolumeSlider[osc]);
        
        oscVolumeLabel[osc].attachToComponent(&oscVolumeSlider[osc], false);
        oscVolumeLabel[osc].setText("volume", juce::dontSendNotification);
        oscVolumeLabel[osc].setJustificationType(juce::Justification::centred);
        
        
        
        
        addAndMakeVisible (oscTypeDropdown[osc]);
        oscTypeDropdown[osc].addItem ("Sine",  1);
        oscTypeDropdown[osc].addItem ("Triangle",   2);
        oscTypeDropdown[osc].addItem ("Sawtooth", 3);
        oscTypeDropdown[osc].addItem ("Square", 4);
        oscTypeDropdown[osc].addItem ("White", 5);
        
        oscTypeValue[osc] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, ("oscillatorType"+to_string(osc)), oscTypeDropdown[osc]);
        
        
        oscPitchShiftSlider[osc].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        oscPitchShiftSlider[osc].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
        oscPitchShiftSlider[osc].addListener(this);
        addAndMakeVisible(&oscPitchShiftSlider[osc]);
        
        oscPitchShiftValue[osc] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, ("oscillatorPitchShift"+to_string(osc)), oscPitchShiftSlider[osc]);
        
        oscPitchShiftLabel[osc].attachToComponent(&oscPitchShiftSlider[osc], false);
        oscPitchShiftLabel[osc].setText("pitch shift", juce::dontSendNotification);
        oscPitchShiftLabel[osc].setJustificationType(juce::Justification::centred);
        
    }
    
    
    monoPolyButton.setClickingTogglesState (true);
    monoPolyButton.onClick = [this]() {
        if (monoPolyButton.getToggleState()==true)
            monoPolyButton.setButtonText("Monophonic");
        else
            monoPolyButton.setButtonText("Polyphonic");
    };
    addAndMakeVisible (&monoPolyButton);
        
    monoPolyValue = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "monoPoly", monoPolyButton);
    
    
    
    
    glideTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    glideTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    glideTimeSlider.addListener(this);
    addAndMakeVisible(&glideTimeSlider);
       
    glideTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "glideTime", glideTimeSlider);
       
    glideTimeLabel.attachToComponent(&glideTimeSlider, false);
    glideTimeLabel.setText("Glide Time", juce::dontSendNotification);
    glideTimeLabel.setJustificationType(juce::Justification::centred);
    
    
    
    
    driveSlider.setSliderStyle(juce::Slider::LinearVertical);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    driveSlider.addListener(this);
    addAndMakeVisible(&driveSlider);
       
    driveValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "drive", driveSlider);
       
    driveLabel.attachToComponent(&driveSlider, false);
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);
    
    
    
    
//    attackSlider.setSliderStyle(juce::Slider::LinearVertical);
//    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
//    attackSlider.addListener(this);
//    addAndMakeVisible(&attackSlider);
//
//    attackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "attack", attackSlider);
//
//    attackLabel.attachToComponent(&attackSlider, false);
//    attackLabel.setText("Attack", juce::dontSendNotification);
//    attackLabel.setJustificationType(juce::Justification::centred);
//
//
//
//
//    decaySlider.setSliderStyle(juce::Slider::LinearVertical);
//    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
//    decaySlider.addListener(this);
//    addAndMakeVisible(&decaySlider);
//
//    decayValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "decay", decaySlider);
//
//    decayLabel.attachToComponent(&decaySlider, false);
//    decayLabel.setText("Decay", juce::dontSendNotification);
//    decayLabel.setJustificationType(juce::Justification::centred);
//
//
//
//
//    sustainSlider.setSliderStyle(juce::Slider::LinearVertical);
//    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
//    sustainSlider.addListener(this);
//    addAndMakeVisible(&sustainSlider);
//
//    sustainValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "sustain", sustainSlider);
//    sustainLabel.attachToComponent(&sustainSlider, false);
//    sustainLabel.setText("Sustain", juce::dontSendNotification);
//    sustainLabel.setJustificationType(juce::Justification::centred);
//
//
//
//
//    releaseSlider.setSliderStyle(juce::Slider::LinearVertical);
//    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
//    releaseSlider.addListener(this);
//    addAndMakeVisible(&releaseSlider);
//
//    releaseValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "release", releaseSlider);
//    releaseLabel.attachToComponent(&releaseSlider, false);
//    releaseLabel.setText("Release", juce::dontSendNotification);
//    releaseLabel.setJustificationType(juce::Justification::centred);

    
        
    
    
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::purple);
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::grey);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::magenta);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::grey);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::magenta);
    
}

MyFirstSynthAudioProcessorEditor::~MyFirstSynthAudioProcessorEditor()
{
}

//==============================================================================
void MyFirstSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    auto purpleHue = juce::Colours::purple.getHue();
//    g.fillAll (juce::Colour::fromHSV (purpleHue, 0.5f, 0.1f, 1.0f));
    
    auto backgroundImage = juce::ImageCache::getFromMemory (BinaryData::SynthBackground1_png, BinaryData::SynthBackground1_pngSize);
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, 1234, 597);
    
    
    for (int osc = 0; osc<4; osc++)
    {
        g.setColour(juce::Colour::fromHSV (purpleHue, 0.5f, 0.5f, 1.0f));
        g.drawRoundedRectangle(oscTypeDropdown[osc].getX(), oscTypeDropdown[osc].getY(), oscTypeDropdown[osc].getWidth(), (oscPitchShiftSlider[osc].getY() + oscPitchShiftSlider[osc].getHeight() - oscTypeDropdown[osc].getY()), 5, 3);
        g.setColour (juce::Colours::white);
        g.setFont (18.0f);
        g.drawFittedText("Oscillator"+to_string(osc+1), oscTypeDropdown[osc].getX(), oscTypeDropdown[osc].getY()-12, oscTypeDropdown[osc].getWidth(), 1, juce::Justification::centred, 1);
    }
    
    g.setColour(juce::Colour::fromHSV (purpleHue, 0.5f, 0.5f, 1.0f));
    g.drawRoundedRectangle(modFreqLabel.getX(), modFreqLabel.getY(), modFreqSlider.getWidth() *2 +2, modFreqSlider.getHeight()+modFreqLabel.getHeight(), 5, 3);
    g.setColour (juce::Colours::white);
    g.setFont (18.0f);
    g.drawFittedText("LFO", modDepthLabel.getX()-40, modFreqLabel.getY() - 12, modFreqSlider.getWidth(), 1, juce::Justification::centred, 1);

    
}

void MyFirstSynthAudioProcessorEditor::resized()
{
    //    Dividing the GUI window into a grid of 10x10
        auto x = getWidth()/15;
        auto y = getHeight()/20;
    
        driveSlider.setBounds(0,     4.5*y,    2.5*x,    11*y);
    
        oscVolumeSlider[0].setBounds(2.5*x,     3.5*y,    2*x,    4*y);
        oscVolumeSlider[1].setBounds(5*x,   3.5*y,    2*x,    4*y);
        oscVolumeSlider[2].setBounds(7.5*x,   3.5*y,    2*x,    4*y);
        oscVolumeSlider[3].setBounds(10*x,  3.5*y,    2*x,    4*y);
    
    oscPitchShiftSlider[0].setBounds(2.5*x,      8.5*y,     2*x,    4*y);
    oscPitchShiftSlider[1].setBounds(5*x,    8.5*y,     2*x,    4*y);
    oscPitchShiftSlider[2].setBounds(7.5*x,    8.5*y,     2*x,    4*y);
    oscPitchShiftSlider[3].setBounds(10*x,   8.5*y,     2*x,    4*y);
    
        oscTypeDropdown[0].setBounds(2.5*x,    1.5*y,      2*x,      y);
        oscTypeDropdown[1].setBounds(5*x, 1.5*y,      2*x,      y);
        oscTypeDropdown[2].setBounds(7.5*x,  1.5*y,      2*x,      y);
        oscTypeDropdown[3].setBounds(10*x,  1.5*y,      2*x,      y);
    
    modFreqSlider.setBounds(3.5*x,     15*y,    2*x,    4*y);
   modDepthSlider.setBounds(5.5*x,     15*y,    2*x,    4*y);
    
    attackSlider.setBounds(6*x,     15*y,    2*x,    4*y);
    decaySlider.setBounds(7.1*x,     15*y,    2*x,    4*y);
    sustainSlider.setBounds(8.2*x,     15*y,    2*x,    4*y);
    releaseSlider.setBounds(9.3*x,     15*y,    2*x,    4*y);
    
 masterVolumeSlider.setBounds(12*x,     4.5*y,    3*x,    11*y);
    
           glideTimeSlider.setBounds(9*x,   14*y,      2*x,   4*y);
            monoPolyButton.setBounds(9*x,   18*y,      2*x,      y);
}

void MyFirstSynthAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
}

void oscTypeChanged()
{
        
}
