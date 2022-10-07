function ThermoV2_send_command(tmp, command, param)

if nargin<3
    param = [];
end
if ischar(param)
    param = ThermoV2_common_patterns(param);
end

switch command
    case 'Get-version'
        write(tmp.serial,0,'uint8');
        
    case 'Get-timestamp'
        write(tmp.serial,1,'uint8');
        
    case 'Backlight-On'
        write(tmp.serial,101,'uint8');
        
    case 'Backlight-Off'
        write(tmp.serial,102,'uint8');
        
    case 'Display-rectangle'
        write(tmp.serial,103,'uint8') 
        write(tmp.serial,param.color1(1),'single');
        write(tmp.serial,param.color1(2),'single');
        write(tmp.serial,param.color1(3),'single');
        write(tmp.serial,param.backgroundcolor(1),'single');
        write(tmp.serial,param.backgroundcolor(2),'single');
        write(tmp.serial,param.backgroundcolor(3),'single');
        write(tmp.serial,param.position(1),'int16');
        write(tmp.serial,param.position(2),'int16');
        write(tmp.serial,param.position(3),'int16');
        write(tmp.serial,param.position(4),'int16');
        write(tmp.serial,param.frequency,'single');
        write(tmp.serial,param.duration,'single');
        write(tmp.serial,param.predelay,'single');
        write(tmp.serial,param.trigger,'uint8');
        
    case 'Toggle-relay'
        write(tmp.serial,120,'uint8');
        write(tmp.serial,param.duration,'single');
        write(tmp.serial,param.relay,'uint8');
        
    case 'Read-temperature'
        write(tmp.serial,121,'uint8');
        write(tmp.serial,param.duration,'single');
        
    case 'Read-sensors'
        write(tmp.serial,122,'uint8');
        write(tmp.serial,param.duration,'single');
        
    case 'Test-sensors'
        write(tmp.serial,123,'uint8');
        
    case 'Stop'
        write(tmp.serial,200,'uint8');
        
    otherwise
        error(['Unknown command: ' command]);
        
end