/*
  ==============================================================================

    LFO.h
    Created: 13 Oct 2022 7:36:53pm
    Author:  Nathan Erthal

  ==============================================================================
*/

#pragma once

#include <cmath>

 
using namespace std;
 
class LFO {
public:
 
    LFO() = default;
      ~LFO() = default;
 
    LFO(const LFO& lfo) = default;
    
    void prepare(double sampleRate, float minVal, float maxVal){
        this->sampleRate = float(sampleRate);
        this->minVal = minVal;
        this->maxVal = maxVal;
    }
    
    void reset(){
        t=0;
    }
    
    void setParameters(float frequency,float amplitude){
        f = frequency;
        a = amplitude;
    }
    
    void modulate(){
        
        //multiplyer
        sineWave = a*sin(2*M_PI*f*(t++)/sampleRate);
        if (sineWave<0) sineWave/=2.0;
        sineWave+=1;
        
//      //  adder
//        sineWave = a*sin(2*M_PI*f*(t++)/sampleRate);
    }
    
    float getNextVal(){
        return sineWave;
    }
 
 
 
private:
    int t=0;
    float sampleRate;
    float minVal;
    float maxVal;
    float sineWave;
    float integral = 0;
    float f;
    float a;
};

