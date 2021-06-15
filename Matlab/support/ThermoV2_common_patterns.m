function pattern = ThermoV2_common_patterns(name)
% FUNCTION pattern = V2patterns(name)
%
% creates commonly used patterns and outputs the "pattern" structure
% containing the following parameters:
% color: [R G B] 0-1 values
% position: [x0 x1 y0 y1] coordinates, in pixels, of rectangle
% frequency: blacklight flicker frequency, in Hz
% duration: how long to display the pattern, in seconds (0 = ON indefinitely)

switch lower(name)
    case 'left rectangle'
        pattern.color1 = [0 0 1]; %[R G B] color of rectangle, 0-1 values
        pattern.backgroundcolor = [0 0 0]; %[R G B] color of background, 0-1 values
        pattern.position = [32 120 64 240]; %[xcenter ycenter width height], in pixels
        pattern.frequency = 0; %flicker frequency, in Hz
        pattern.duration = 0; %display duration, in seconds
        pattern.predelay = 0; %pre-display wait duration, in seconds
        pattern.trigger = 0; %wait for input trigger (1=wait, 0=don't wait)
        
    case 'middle rectangle'
        pattern.color1 = [0 0 1]; 
        pattern.backgroundcolor = [0 0 0];
        pattern.position = [160 120 64 240]; 
        pattern.frequency = 0; 
        pattern.duration = 0; 
        pattern.predelay = 0;
        pattern.trigger = 0;
        
    case 'right rectangle'
        pattern.color1 = [0 0 1];
        pattern.backgroundcolor = [0 0 0];
        pattern.position = [288 120 64 240];
        pattern.frequency = 0; 
        pattern.duration = 0;
        pattern.predelay = 0;
        pattern.trigger = 0;
        
    case 'right grating'
        pattern.color1 = [0 0 1];
        pattern.color2 = [0 0 0];
        pattern.backgroundcolor = [0 0 0.5];
        pattern.position = [160 120 320 240];
        pattern.barwidth = 40;
        pattern.angle = 0;
        pattern.frequency = 2; 
        pattern.duration = 0;
        pattern.predelay = 0;
        pattern.trigger = 0;
        
    otherwise
        error('pattern name not recognized')
end