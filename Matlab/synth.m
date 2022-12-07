fs = 48000;     %sample rate

%% make an object of Oscillator for each different wave shape
sine_osc=Oscillator("sine",fs);
triangle_osc=Oscillator("triangle",fs);
sawtooth_osc=Oscillator("sawtooth",fs);
square_osc=Oscillator("square",fs);
white_osc=Oscillator("white",fs);



%% Run a 1.02 sec buffer just for displaying sinewave at min for 1 sec and mid and max frequencies for .01 seconds each
seconds = 1.02;
buffer= zeros(1,fs*seconds);
buff_size = length(buffer);
time = linspace(0,(buff_size)/fs, (buff_size)); % linspace(start, end, size)

min_f = 20;
mid_f = 10000;
max_f = 20000;

for i = 1:buff_size
    
    % first sec plays min_f, second sec plays mid, 3rd plays max
    if i < fs
        sine_osc = sine_osc.setFrequency(min_f);
    else
        if i < fs*1.01
            sine_osc = sine_osc.setFrequency(mid_f);
        else
            sine_osc = sine_osc.setFrequency(max_f);
        end
    end
    
    %Update oscillator
    sine_osc = sine_osc.updateOsc();
    
    %Get next sample from oscillator
    sine_sample = sine_osc.getSample();
    
    buffer(i) = sine_sample;
    
end

% plots showing sine wave at min mid and max frequencies
figure;
subplot(3,1,1);
plot(time,buffer);
xlim([0 1]);
title(min_f+"Hz throughout 1 sec");
xlabel('time (s)');
ylabel('amplitude');

subplot(3,1,2);
plot(time,buffer);
xlim([1 1.01]);
title(mid_f+"Hz throughout 0.01 sec");
xlabel('time (s)');
ylabel('amplitude');

subplot(3,1,3);
plot(time,buffer);
xlim([1.01 1.02]);
title(max_f+"Hz throughout 0.01 sec");
xlabel('time (s)');
ylabel('amplitude');

sgtitle("Sine wave at min and max frequencies")





%% Create LFO Oscillator
lfo = LFO(fs);


%% Set oscillator Parameters
osc_f = 70;  %oscillator frequency
lfo_f = 4;   %lfo frequency
lfo_amp = 1; %lfo amplitude

sine_osc = sine_osc.setFrequency(osc_f);
triangle_osc = triangle_osc.setFrequency(osc_f);
sawtooth_osc = sawtooth_osc.setFrequency(osc_f);
square_osc = square_osc.setFrequency(osc_f);
white_osc = white_osc.setFrequency(osc_f);

lfo = lfo.setParameters(lfo_f,lfo_amp);


%% Set pitch-shifting parameters
octaveUp = 12; %pitch shift of frequency in half steps
octaveUpMult = halfStepToFreqMultiplyer(octaveUp);

octaveDown = -12; %pitch shift of frequency in half steps (one octave down)
octaveDownMult = halfStepToFreqMultiplyer(octaveDown);


%% Set interpolator (glide effect) parameters
interpolator = Interpolator(fs);
interpolator = interpolator.resetInterpolator(osc_f); %set start value
interpolator = interpolator.setRampLength(1); %in seconds
targetValue = osc_f*7;
interpolator = interpolator.setTargetValue(targetValue);


%% setup variables for a longer process block
LENGTH = 3;     %length of buffer in seconds
out_buffer= zeros(1,fs*LENGTH);
buff_size = length(out_buffer);
time = linspace(0,(buff_size)/fs, (buff_size)); % linspace(start, end, size)
t = 0;


%% make copies of the output buffer for each oscillator
% just for the sake of plotting everything after iterating through buffer once
sine_buffer = out_buffer;
triangle_buffer = out_buffer;
sawtooth_buffer = out_buffer;
square_buffer = out_buffer;
white_buffer = out_buffer;
mixed_buffer = out_buffer;


%% Process block

for i = 1:buff_size
    
    %% seperate block into different sections each using different effects:
    % for plotting purposes, in the first half second play clean tone with
    % no pitch shift (muliplyer of 1)
    if i<fs/2
        pitchShift = 1;
    end
    
    %  then play with pitch shift of helf step multiplyer from second 0.5:
    if i>fs/2
        pitchShift = octaveDownMult;
    end
    
    if i>fs  % from second 1, use lfo as the pitch shift:
       
        % update the LFO and get its next sample to use as the pitchshift multiplyer.
        lfo = lfo.updateOsc();
        lfo_sample = lfo.getSample();
        pitchShift = lfo_sample;
            
        if i>fs*1.5  % from second 1.5 to 2, use lfo and halfstep multiplyer as the pitch shift:
            pitchShift = lfo_sample*octaveUpMult;
        end
        
        if i>fs*2  % from second 2 to 3, interpolate the frequency up, with no extra pitch shifting effects
            pitchShift = 1;
            
            interpolator = interpolator.updateInterpolator();
            new_f = interpolator.getNextValue();
            sine_osc = sine_osc.setFrequency(new_f);
        end
    end
    %%
    
    %Add the multiplyer as the pitch-shift effect in each oscillator
    sine_osc = sine_osc.setPitchShift(pitchShift);
    triangle_osc = triangle_osc.setPitchShift(pitchShift);
    sawtooth_osc = sawtooth_osc.setPitchShift(pitchShift);
    square_osc = square_osc.setPitchShift(pitchShift);
            
    
    %Update each oscillators
    sine_osc = sine_osc.updateOsc();
    triangle_osc = triangle_osc.updateOsc();
    sawtooth_osc = sawtooth_osc.updateOsc();
    square_osc = square_osc.updateOsc();
    white_osc = white_osc.updateOsc();
    
    %Get next sample from each oscillator
    sine_sample = sine_osc.getSample();
    triangle_sample = triangle_osc.getSample();
    sawtooth_sample = sawtooth_osc.getSample();
    square_sample = square_osc.getSample();
    white_sample = white_osc.getSample();
    
    %For plotting purposes, have an output buffer for each different osc
    sine_buffer(i) = sine_sample;
    triangle_buffer(i) = triangle_sample;
    sawtooth_buffer(i) = sawtooth_sample;
    square_buffer(i) = square_sample;
    white_buffer(i) = white_sample;
    
    mixed_buffer(i) = (sine_sample + triangle_sample + sawtooth_sample + square_sample + white_sample)/5;
    
end
%%

tone = audioplayer(sine_buffer, fs);
play(tone);


%% plots showing unaffected sine,triangle,saw,sqare,white wave types
figure;
subplot(6,1,1);
plot(time,sine_buffer);
xlim([0 0.5]);
title("Sine wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(6,1,2);
plot(time,triangle_buffer);
xlim([0 0.5]);
title("Traingle wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(6,1,3);
plot(time,sawtooth_buffer);
xlim([0 0.5]);
title("Saw wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(6,1,4);
plot(time,square_buffer);
xlim([0 0.5]);
title("Square wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(6,1,5);
plot(time,white_buffer);
xlim([0 0.5]);
title("White noise");
xlabel('time (s)');
ylabel('amplitude');

subplot(6,1,6);
plot(time,mixed_buffer);
xlim([0 0.5]);
title("An example combination of sine, triangle, saw, square, and white noise");
xlabel('time (s)');
ylabel('amplitude');

sgtitle("Different wave shapes at "+osc_f+"Hz")



%% plots showing half step multiplyer

figure;
subplot(5,1,1);
plot(time,sine_buffer);
xlim([0 1]);
title("Sine wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(5,1,2);
plot(time,triangle_buffer);
xlim([0 1]);
title("Traingle wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(5,1,3);
plot(time,sawtooth_buffer);
xlim([0 1]);
title("Saw wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(5,1,4);
plot(time,square_buffer);
xlim([0 1]);
title("Square wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(5,1,5);
plot(time,mixed_buffer);
xlim([0 1]);
title("An example combination of sine, triangle, saw, square, and white noise");
xlabel('time (s)');
ylabel('amplitude');

sgtitle(osc_f+"Hz, then pitch shifted at "+octaveDown+" half-steps down")



%% plots showing LFO Multiplyer

figure;
subplot(5,1,1);
plot(time,sine_buffer);
xlim([1 2]);
title("Sine wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(5,1,2);
plot(time,triangle_buffer);
xlim([1 2]);
title("Traingle wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(5,1,3);
plot(time,sawtooth_buffer);
xlim([1 2]);
title("Saw wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(5,1,4);
plot(time,square_buffer);
xlim([1 2]);
title("Square wave");
xlabel('time (s)');
ylabel('amplitude');

subplot(5,1,5);
plot(time,mixed_buffer);
xlim([1 2]);
title("An example combination of sine, triangle, saw, square, and white noise");
xlabel('time (s)');
ylabel('amplitude');

sgtitle(osc_f+"Hz, LFO modulated at "+lfo_f+"Hz, then pitch shifted "+octaveUp+" half-steps up")


%% plots showing Glide effect from interpolator

figure;
subplot(1,1,1);
plot(time,sine_buffer);
xlim([2 3]);
title("Sine wave");
xlabel('time (s)');
ylabel('amplitude');

sgtitle(osc_f+"Hz, interpolated to "+targetValue+"Hz during 1 second")



%%
function mult = halfStepToFreqMultiplyer(halfSteps)
        if (halfSteps>0)
            mult = 1+(1/12)*halfSteps;
        else
            mult = 1+(0.04166666667*halfSteps);
        end
end