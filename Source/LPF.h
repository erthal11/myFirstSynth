/*
  ==============================================================================

    LPF.h
    Created: 28 Sep 2022 10:40:21pm
    Author:  Nathan Erthal

  ==============================================================================
*/

// pg 54 DAFX

#pragma once
 
#include <iostream>
#include <cmath>
 
using namespace std;
 
class LPF {
public:
 
      LPF() = default;
      ~LPF() = default;
 
      LPF(const LPF& lpf) = default;
    
    void setSampleRate(double sampleRate){
        fs = float(sampleRate);
    }
    
    void updateCoeffs(float fc){
        wc = 2*fc/fs;
        c = (tan(M_PI*wc/2)-1)/(tan(M_PI*wc/2)+1);
    }
 
    float processAudioSample(float x)
      {
          xh_new = x - c*xh;
          ap_y = c* xh_new + xh;
          xh = xh_new;
          return 0.5*(x+ap_y);
      }
 
private:
    
    float xh_new;
    float ap_y;
    float xh=0;
    float c;
    float wc;
    float fs;
};

