classdef Interpolator
    %INTERPOLATOR Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        targetValue;
        currentValue;
        sampleRate;
        step;
        timeInSamples;
    end
    
    methods
        
        function obj = Interpolator(sampleRate)
            obj.sampleRate = sampleRate;
        end
        
        function obj = resetInterpolator(obj, currentValue)
            obj.currentValue = currentValue;
        end
        
        function obj = setRampLength(obj, timeInSeconds)
            obj.timeInSamples = timeInSeconds * obj.sampleRate;
        end
        
        function obj = setTargetValue(obj, targetValue)
            obj.targetValue = targetValue;
            
            if (obj.timeInSamples==0)
                obj.step = 0;
                obj.currentValue=targetValue;
            else
                obj.step = (targetValue - obj.currentValue) ./ (obj.timeInSamples);
            end
            
        end
        
        function obj = updateInterpolator(obj)
            if (abs(obj.currentValue - obj.targetValue)>0.01)
                obj.currentValue = obj.currentValue + obj.step;
            else
                obj.currentValue = obj.targetValue;
            end
        end
        
        function newValue = getNextValue(obj)
            newValue = obj.currentValue;
        end
        
    end
end

