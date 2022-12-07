/*
  ==============================================================================

    Oscillator.h
    Created: 13 Oct 2022 3:31:09pm
    Author:  Nathan Erthal

  ==============================================================================
*/

#pragma once


typedef unsigned int uint32;
typedef int int32;
typedef unsigned long long int uint64;
typedef long long int int64;

class Oscillator
{
public:
    Oscillator() = default;
    ~Oscillator() = default;
    Oscillator(const Oscillator& oscillator) = default;
    
    void setSampleRate(const double& sampleRate)
    {
        this->sampleRate = sampleRate;
        stop();
    }
    
    void setWaveType(int waveType){
        this->waveType = waveType;
//        t=0;
    }
    
    void setFrequency(const float& frequency)
    {
        this->frequency = frequency;
    }
    
    void setpitchShift(float pitchShift)
    {
        this->pitchShift=pitchShift;
    }

    float getSample()
    {
        if (waveType==0)    //Sine
        {
            if (t >= 1.0)
                t = t - 1.0;
            sample = sin(2*M_PI*t);
            t += (frequency*pitchShift) / sampleRate;
            return sample;
//            return sin(2*M_PI*(frequency*pitchShift+mod)*(t++)/sampleRate);
        }
        
        else if (waveType==1)  //triangle
        {
            if (t >= 1.0)
                t = t - 1.0;
            sample = (2/M_PI)*asin(sin(2*M_PI*t));
            t += (frequency*pitchShift) / sampleRate;
            return sample;
//            return (2/M_PI)*asin(sin(2*M_PI*(frequency*pitchShift+mod)*(t++)/sampleRate));
        }
        
        else if (waveType==2)  //Sawtooth
        {
            if (t >= 1.0)
                t = t - 1.0;
            sample = 2.0 * t - 1.0;
            t += (frequency*pitchShift) / sampleRate;
            return sample;
        }
        
        else if (waveType==3)    //square
        {
            if (t >= 1.0)
                t = t - 1.0;
            sample = signbit(sin(2*M_PI*t));
            t += (frequency*pitchShift) / sampleRate;
            return sample;
//            return signbit(sin(2*M_PI*(frequency*pitchShift+mod)*(t++)/sampleRate));
        }
        
        else if (waveType==4)    //white noise
        {
            return (float)rand()/10000000000.0;
        }
        
    }

    void stop()
    {
        frequency = 0;
        t = 0;
    }

    bool isPlaying()
    {
        return frequency != 0;
    }

private:
    float t = 0;
    float sample = 0;
    float frequency;
    float pitchShift = 1;
    double sampleRate;
    int waveType;
    float mod=0;
};
