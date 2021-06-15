function tmp = ThermoV2_read_serial(tmp)
%
%

    while tmp.serial.NumBytesAvailable>0
        command = read(tmp.serial,1,'uint8');
        if isfield(tmp,'log')
            N = length(tmp.log)+1-isempty(tmp.log(1).datenum);
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
                tmp.prog_start_datenum = addtodate(t,-millis,'millisecond');
                tmp.prog_start_datestr = datestr(tmp.prog_start_datenum,'yy/mm/dd HH:MM:SS.fff');

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
                tmp.log(N).relay = read(tmp.serial,1,'uint8');
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'toggle relay';
                
            case 121 %read temperature
                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'read temperature';
                
            case 122 %read sensors
                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'read sensors';
                
            case 150 %input trigger
                tmp = log_timestamp(tmp,N);
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'input trigger';

            case 151 %temperature reading
                bytes = read(tmp.serial,4,'uint8')';
                millis = typecast(uint8(bytes),'int32');
                timestamp_num = addtodate(tmp.prog_start_datenum,millis,'millisecond');
                tmp.log(1).temperature_time = [tmp.log(1).temperature_time timestamp_num];
                
                bytes = read(tmp.serial,4,'uint8');
                temperature = typecast(uint8(bytes),'single');
                tmp.log(1).temperature_read = [tmp.log(1).temperature_read temperature];
                
            case 152 %sensor reading
                tmp = log_timestamp(tmp,N);
                tmp.log(N).sensor = read(tmp.serial,1,'uint8');
                tmp.log(N).commandnum = command;
                tmp.log(N).commandname = 'sensor read';
                
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
    tmp.log(N).datenum = addtodate(tmp.prog_start_datenum,millis,'millisecond');
    tmp.log(N).datestr = datestr(tmp.log(N).datenum,'HH:MM:SS.fff');
end