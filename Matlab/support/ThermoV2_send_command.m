function ThermoV2_send_command(display, command, param)

if nargin<3
    param = [];
end
if ischar(param)
    param = ThermoV2_common_patterns(param);
end

switch command
    case 'Get-version'
        write(display.serial,0,'uint8');
        
    case 'Get-timestamp'
        write(display.serial,1,'uint8');
        
    case 'Backlight-On'
        write(display.serial,101,'uint8');
        
    case 'Backlight-Off'
        write(display.serial,102,'uint8');
        
    case 'Display-rectangle'
        write(display.serial,103,'uint8') 
        write(display.serial,param.color1(1),'single');
        write(display.serial,param.color1(2),'single');
        write(display.serial,param.color1(3),'single');
        write(display.serial,param.backgroundcolor(1),'single');
        write(display.serial,param.backgroundcolor(2),'single');
        write(display.serial,param.backgroundcolor(3),'single');
        write(display.serial,param.position(1),'int16');
        write(display.serial,param.position(2),'int16');
        write(display.serial,param.position(3),'int16');
        write(display.serial,param.position(4),'int16');
        write(display.serial,param.frequency,'single');
        write(display.serial,param.duration,'single');
        write(display.serial,param.predelay,'single');
        write(display.serial,param.trigger,'uint8');
        
    case 'Toggle-relay'
        write(display.serial,120,'uint8');
        write(display.serial,param.duration,'single');
        write(display.serial,param.relay,'uint8');
        
    case 'Read-temperature'
        write(display.serial,121,'uint8');
        write(display.serial,param.duration,'single');
        
    case 'Read-sensors'
        write(display.serial,122,'uint8');
        write(display.serial,param.duration,'single');
        
    case 'Stop'
        write(display.serial,200,'uint8');
        
    otherwise
        error(['Unknown command: ' command]);
        
end