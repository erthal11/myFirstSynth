/*
  ==============================================================================

    Synth.h
    Created: 13 Oct 2022 3:31:16pm
    Author:  Nathan Erthal

  ==============================================================================
*/

#pragma once

#include "Oscillator.h"
#include "LFO.h"
//#include "LPF.h"
#include "Interpolator.h"

#define TOTAL_NOTES 127
#define TOTAL_OSCILLATORS 4

class Synth
{
public:
    Synth() = default;
    ~Synth() = default;
    Synth(const Synth& synth) = default;

    void prepareToPlay(const double& sampleRate)
    {
        this->sampleRate = sampleRate;
        
        for (int osc=0; osc<TOTAL_OSCILLATORS; osc++)
            for (int note=0; note<TOTAL_NOTES; note++)
                oscillators[osc][note].setSampleRate(sampleRate);
        
        lfo.prepare(sampleRate,0,20000);
//        lpf.setSampleRate(sampleRate);
        interpolator.setSampleRate(sampleRate);
        interpolator.reset(200.0);
    }
    
    void setOscillatorTypes(int oscTypes[]){
        for (int osc=0; osc<TOTAL_OSCILLATORS; osc++)
            for (int note=0; note<TOTAL_NOTES; note++)
                oscillators[osc][note].setWaveType(oscTypes[osc]);
    }
    
    void setEffectParameters(float masterVolume, float lfoFreq, float lfoDepth, float oscVolumes[TOTAL_OSCILLATORS], int oscPitchShift[TOTAL_OSCILLATORS], bool monophonic, float glideTime, float drive){
        this->masterVolume = masterVolume;
        this->drive=drive;
        lfo.setParameters(lfoFreq,lfoDepth);
//        lpf.updateCoeffs(lpfFreq);
        
        for (int osc=0; osc<TOTAL_OSCILLATORS; osc++)
        {
            this->oscVolumes[osc] = oscVolumes[osc];
            this->oscPitchShift[osc] = oscPitchShift[osc];
        }
        
        this->monophonic = monophonic;
        interpolator.setRampLength(glideTime);
    }

    void render(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& messages)
    {
        auto currentSample = 0;
        for (const auto midi : messages)
        {
            auto message = midi.getMessage();
            auto messagePosition = static_cast<int> (message.getTimeStamp());
            process(buffer, currentSample, messagePosition);
            currentSample = messagePosition;
            handleMidiEvent(message);
        }
        process(buffer, currentSample, buffer.getNumSamples());
    }
    
private:
    
    float midiToFreq(const uint32& midiNote)
    {
        constexpr float A4_FREQ = 440;
        constexpr float A4_MIDINOTE = 81;
        constexpr float NOTES_IN_OCTAVE = 12.f;
        return A4_FREQ * std::powf(2, (static_cast<float>(midiNote) - A4_MIDINOTE) / NOTES_IN_OCTAVE);
    }
    
    void handleMidiEvent(const juce::MidiMessage& message)
    {
        if (message.isNoteOn())
        {
            frequency = midiToFreq(message.getNoteNumber());
            if (monophonic)
                for (int osc=0; osc<TOTAL_OSCILLATORS; osc++)
                    for (uint32_t i = 0; i < TOTAL_NOTES; i++)
                        oscillators[osc][i].stop();
            
            interpolator.setTargetValue(frequency);
            for (int osc=0; osc<TOTAL_OSCILLATORS; osc++)
                oscillators[osc][message.getNoteNumber()].setFrequency(frequency);
        }
        
        if (message.isNoteOff())
        {
            for (int osc=0; osc<TOTAL_OSCILLATORS; osc++)
                oscillators[osc][message.getNoteNumber()].stop();
        }

        if (message.isAllNotesOff())
        {
            for (int osc=0; osc<TOTAL_OSCILLATORS; osc++)
                for (uint32_t i = 0; i < TOTAL_NOTES; i++)
                    oscillators[osc][message.getNoteNumber()].stop();
        }
    }

    void process(juce::AudioBuffer<float>& buffer, const uint32& startSample, const uint32& endSample)
    {
        auto* firstChannel = buffer.getWritePointer(0);
        for (auto sample = startSample; sample < endSample; sample++)
        {
            lfo.modulate();
            for (int osc=0; osc<TOTAL_OSCILLATORS; osc++)
                for (uint32 note = 0; note < TOTAL_NOTES; note++)
                {
                    if (oscillators[0][note].isPlaying())
                    {
                        
                        float freqMultiplyer = halfStepToFreqMultiplyer(oscPitchShift[osc]);
                        oscillators[osc][note].setpitchShift(freqMultiplyer * lfo.getNextVal());
                        if (monophonic){    // for now glide only works in monophonic
                            float newFreq = interpolator.getNextValue();
                            oscillators[osc][note].setFrequency(newFreq);
                        }
                        firstChannel[sample] += tanh(drive* oscillators[osc][note].getSample() * oscVolumes[osc] );
                    }
                }
//            firstChannel[sample] = lpf.processAudioSample(firstChannel[sample]);
            firstChannel[sample] = firstChannel[sample] * masterVolume;
        }

        for (uint32 channel = 1; channel < buffer.getNumChannels(); channel++)
        {
            auto channelData = buffer.getWritePointer(channel);
            std::copy(firstChannel + startSample, firstChannel + endSample, channelData + startSample);
        }
    }
    
    float halfStepToFreqMultiplyer(int halfSteps)
    {
        if (halfSteps>0)
            return 1+(1.f/12.f)*halfSteps;
        else
            return 1+(0.04166666667*halfSteps);
    }

    Oscillator oscillators[TOTAL_OSCILLATORS][TOTAL_NOTES];
    double sampleRate;
    LFO lfo;
//    LPF lpf;
    float frequency;
    float masterVolume;
    float oscVolumes[TOTAL_OSCILLATORS];
    int oscPitchShift[TOTAL_OSCILLATORS];   //value is in halfSteps
    bool monophonic = false;
    Interpolator interpolator;
    float drive;
};
