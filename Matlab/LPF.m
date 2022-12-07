classdef LPF
    %LPF Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        xh_new;
    	ap_y;
    	xh=0;
    	c;
    	fs;
        sample;
    end
    
    methods
        function obj = LPF(sampleRate)
            obj.fs = sampleRate;
        end
        
        function obj = updateCoeffs(obj,f)
            wc = 2*f/obj.fs;
            obj.c = (tan(pi*wc/2)-1)/(tan(pi*wc/2)+1);
        end
        
        function obj = processAudioSample(obj,x)
            obj.xh_new = x - obj.c * obj.xh;
            obj.ap_y = obj.c * obj.xh_new + obj.xh;
            obj.xh = obj.xh_new;
            obj.sample = 0.5*(x + obj.ap_y);
        end
        
        function sample = getProcessedSample(obj)
            sample = obj.sample;
        end
        
        
    end
end

