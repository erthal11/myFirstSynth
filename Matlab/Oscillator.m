classdef Oscillator
    %OSCILLATOR Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        waveType
        t=0;
        sampleRate
        frequency
        sample
        pitchShift =1;
    end
    
    methods
        
        function obj = Oscillator(waveType, sampleRate)
            obj.waveType = waveType;
            obj.sampleRate = sampleRate;
        end
        
        function obj = setFrequency(obj, frequency)
            obj.frequency = frequency;
        end
        
        function obj = setPitchShift(obj, pitchShift)
            obj.pitchShift = pitchShift;
        end
        
        function obj = updateOsc(obj)
            if (obj.t >= 1.0)
                obj.t = obj.t - 1.0;
            end
            
            if obj.waveType=="sine"
                obj.sample = sin(2*pi*obj.t);
            end
            
            if obj.waveType=="triangle"
                obj.sample = (2/pi)*asin(sin(2*pi*obj.t));
            end
            
            if obj.waveType=="sawtooth"
                obj.sample = 2.0 * obj.t - 1.0;
            end
            
            if obj.waveType=="square"
                obj.sample = sign(sin(2*pi*obj.t));
            end
            
            if obj.waveType=="white"
                obj.sample = rand();
            end
            
            obj.t = obj.t + (obj.frequency * obj.pitchShift) / obj.sampleRate;
        end
        
        function sample = getSample(obj)
            sample = obj.sample;
        end
        
        
        
    end
end

