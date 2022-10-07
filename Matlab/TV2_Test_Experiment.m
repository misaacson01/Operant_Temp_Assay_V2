%% connect to displays and controller
[controller, display1, display2] = ThermoV2_connect_mac();


%% show the "left rectangle" pattern on display 2
ThermoV2_send_command(display2, 'Display-rectangle', 'left rectangle');
pause(3); %wait 3 seconds
ThermoV2_send_command(display2, 'Stop');
pause(1);
display2 = ThermoV2_read_serial(display2);


%% show a custom pattern (flickering square) on display 1
pattern.color1 = [1 1 1];
pattern.backgroundcolor = [0 0 0];
pattern.position = [160 120 120 120];
pattern.frequency = 1; 
pattern.duration = 0;
pattern.predelay = 0;
pattern.trigger = 0;

ThermoV2_send_command(display1, 'Display-rectangle', pattern);
pause(3); %wait 3 seconds
ThermoV2_send_command(display1, 'Stop');
pause(1); %wait 5 seconds
display1 = ThermoV2_read_serial(display1);


%% toggle all relays, 1 at a time
for i = 0:7
    param.duration = 1;
    param.relay = i;
    ThermoV2_send_command(controller, 'Toggle-relay', param);
    pause(2);
end


%% read temperature for 5 s
param.duration = 5;
ThermoV2_send_command(controller, 'Read-temperature', param);
pause(6)
controller = ThermoV2_read_serial(controller);


%% read sensors for 5 s
param.duration = 5;
ThermoV2_send_command(controller, 'Read-sensors', param);
pause(6)
controller = ThermoV2_read_serial(controller);


%% test sensors for 5 s
ThermoV2_send_command(controller, 'Test-sensors');
pause(6)
controller = ThermoV2_read_serial(controller);
