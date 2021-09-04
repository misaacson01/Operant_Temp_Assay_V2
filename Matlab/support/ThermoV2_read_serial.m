function tmp = ThermoV2_read_serial(tmp)
%
%

    while tmp.serial.NumBytesAvailable>0
        command = read(tmp.serial,1,'uint8');
        if isfield(tmp,'log')
            N = length(tmp.log)+1-isempty(tmp.log(1).prog_millis);
        else
            N=1;
        end

        switch command
            case 0 %get program and version
                bytes = read(tmp.serial,2,'uint8')';
                tmp.program = typecast(uint8(bytes),'int16');
                tmp.programnum = read(tmp.serial,1,'uint8');
                tmp.version = read(tmp.serial,1,'uint8');

            case 1 %get timestamp for clock synchronization
                bytes = read(tmp.serial,4,'uint8')';
                t = now;
                millis = typecast(uint8(bytes),'int32');
                if ~isfield(tmp,'pc_start_datestr')
                    tmp.pc_datenum = t;
                    tmp.prog_millis = millis;
                    tmp.pc_start_datestr = datestr(t,'yy/mm/dd HH:MM:SS.fff');
                else
                    tmp(1).pc_datenum = [tmp.pc_datenum t];
                    tmp(1).prog_millis = [tmp.prog_millis millis];
                end

            case 101 %turn on backlight
                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'backlight on';

            case 102 %turn off backlight
                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'backlight off';

            case 103 %display rectangle
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color1(1) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color1(2) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color1(3) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).backgroundcolor(1) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).backgroundcolor(2) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).backgroundcolor(3) = typecast(uint8(bytes),'single');

                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).position(1) = typecast(uint8(bytes),'int16');
                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).position(2) = typecast(uint8(bytes),'int16');
                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).position(3) = typecast(uint8(bytes),'int16');
                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).position(4) = typecast(uint8(bytes),'int16');

                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).frequency = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).duration = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).predelay = typecast(uint8(bytes),'single');
                tmp.log(N).trigger = read(tmp.serial,1,'uint8');

                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'display rectangle';

            case 104 %tmp grating
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color1(1) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color1(2) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color1(3) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color2(1) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color2(2) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).color2(3) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).backgroundcolor(1) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).backgroundcolor(2) = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).backgroundcolor(3) = typecast(uint8(bytes),'single');

                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).position(1) = typecast(uint8(bytes),'int16');
                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).position(2) = typecast(uint8(bytes),'int16');
                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).position(3) = typecast(uint8(bytes),'int16');
                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).position(4) = typecast(uint8(bytes),'int16');

                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).barwidth = typecast(uint8(bytes),'int16');
                bytes = read(tmp.serial,2,'uint8');
                tmp.log(N).angle = typecast(uint8(bytes),'int16');

                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).frequency = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).duration = typecast(uint8(bytes),'single');
                bytes = read(tmp.serial,4,'uint8');
                tmp.log(N).predelay = typecast(uint8(bytes),'single');
                tmp.log(N).gratingtype = read(tmp.serial,1,'uint8');
                tmp.log(N).trigger = read(tmp.serial,1,'uint8');

                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'display grating';

            case 120 %toggle relay
                tmp = log_timestamp(tmp,N);
                bytes = read(tmp.serial,4,'uint8')';
                tmp.log(N).duration = typecast(uint8(bytes),'single');
                tmp.log(N).relay = read(tmp.serial,1,'uint8');
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'toggle relay';
                
            case 121 %read temperature
                tmp = log_timestamp(tmp,N);
                bytes = read(tmp.serial,4,'uint8')';
                tmp.log(N).duration = typecast(uint8(bytes),'single');
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'read temperature';
                
            case 122 %read sensors
                tmp = log_timestamp(tmp,N);
                bytes = read(tmp.serial,4,'uint8')';
                tmp.log(N).duration = typecast(uint8(bytes),'single');
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'read sensors';
                
            case 150 %input trigger
                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'input trigger';

            case 151 %temperature reading
                bytes = read(tmp.serial,4,'uint8')';
                millis = typecast(uint8(bytes),'int32');
                bytes = read(tmp.serial,4,'uint8');
                temperature = typecast(uint8(bytes),'single');
                if ~isfield(tmp,'temperature_time')
                    tmp.temperature_time = millis;
                    tmp.temperature_read = temperature;
                else
                    tmp.temperature_time = [tmp.temperature_time millis];
                    tmp.temperature_read = [tmp.temperature_read temperature];
                end
                
            case 152 %sensor reading
                bytes = read(tmp.serial,4,'uint8')';
                millis = typecast(uint8(bytes),'int32');
                sensor = read(tmp.serial,1,'uint8');
                if ~isfield(tmp,'sensor_time')
                    tmp.sensor_time = millis;
                    tmp.sensor_read = sensor;
                else
                    tmp.sensor_time = [tmp.sensor_time millis];
                    tmp.sensor_read = [tmp.sensor_read sensor];
                end
                
            case 200 %stop command
                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'stop';

            otherwise %get message
                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'message';
                tmp.log(N).message = readline(tmp.serial);
                disp('message recieved:');
                disp(tmp.log(N).message);
        end
    end
end

function tmp = log_timestamp(tmp,N)
    bytes = read(tmp.serial,4,'uint8')';
    millis = typecast(uint8(bytes),'int32');
    tmp.log(N).prog_millis = millis;
end