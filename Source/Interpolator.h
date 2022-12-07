/*
  ==============================================================================

    Interpolator.h
    Created: 19 Oct 2022 7:46:53pm
    Author:  Nathan Erthal

  ==============================================================================
*/

#pragma once

#include<iostream>
#include<algorithm>
using namespace std;

class Interpolator {
public:
    
    void setSampleRate(int sampleRate){
        this->sampleRate = sampleRate;
    }
    
    void reset(float currentValue){
        this->currentValue = currentValue;
        targetValue = currentValue;
    }
    
    void setRampLength(float timeInSeconds){
        this->timeInSamples = timeInSeconds*sampleRate;
    }
    
    void setTargetValue(float targetValue){
        this->targetValue = targetValue;
        if (timeInSamples==0)
        {
            step = 0;
            currentValue=targetValue;
        }
        else
            step = (targetValue - currentValue) / (timeInSamples);
    }
    
    float getNextValue(){
        if (abs(currentValue - targetValue)>0.01)
            currentValue += step;
        else
            reset(currentValue);
        return currentValue;
    }
    
    
private:
    float targetValue;
    float currentValue;
    int sampleRate;
    float step;
    float timeInSamples;
};
