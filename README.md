# Operant_Temp_Assay_V2

This repo contains code and instructions for assembling, installing, and operating the Hoon lab operant temperature apparatus (V2). This is an updated and modified version of the assay originally described by [Isaacson and Hoon, 2021](https://doi.org/10.1177/17448069211013633).

## Hardware installation
1) Order two recirculating water baths with external recirculation ports, such as []().
2) Order all tubing, parts, and electronics according to the [parts list](https://github.com/misaacson01/Operant_Temp_Assay_V2/blob/main/Parts%20List.xlsx).
3) Order all custom 3D printable parts listed in the [parts list](https://github.com/misaacson01/Operant_Temp_Assay_V2/blob/main/Parts%20List.xlsx). Stl files needed to print the parts can be found in the [3D Prints](https://github.com/misaacson01/Operant_Temp_Assay_V2/tree/main/3D%20prints) folder.
    * To order custom prints online, use a 3D print service such as [Craftcloud](https://craftcloud3d.com/) by uploading the .stl files located in the [3D Prints folder](https://github.com/misaacson01/Operant_Temp_Assay_V2/tree/main/3D%20prints).
    * To print the parts yourself, we recommended using an FDM printer such as the [Prusa MK4S](https://www.prusa3d.com/product/original-prusa-mk4s-3d-printer-5/) using PLA filament.
    * Whether going through a service or printing yourself, these are our suggested print parameters:
        * File units: millimeters
        * Printing process type: FDM
        * Material: PLA
        * Infill: 20%
        * Support: none
        * Layer Height: 300 micrometers (or less)
        * Finish: Standard/As printed/No finish
        * Print orientation: default
        * Tolerance: 500 micrometers (or less)
4) Assemble the nose poke detectors
    * Solder the photo interrupter, a 200 Ohm resistor, and ~0.5 m each of 3 hookup wires (red=power, black=ground, and yellow=signal) to the photo interrupter breakout PCB. Repeat this step to create 3 assembled photointerrupter-based nose poke detectors
    * Place each nose poke detector into the matching slots of the Thermobox 3D print
5) Assemble the liquid reward delivery valves
    * Using black hookup wire, connect the negative (ground) screw terminal of the 2.1 mm power jack to the center screw terminal of a relay in the relay module
    * Using another black wire, connect the "normally open" screw terminal of the relay to one of the two pins of a small solenoid valve (the wire may need to be soldered to the solenoid valve pin)
    * Using a red wire, connect the other pin of the solenoid valve to the positive (power) screw terminal of the 2.1 mm power jack
    * Solder a diode between the two pins of the small solenoid valve, orienting the diode so that current can flow from the negative pin to the positive pin (the positive pin is the one which is wired to the positive terminal of the power jack)
    * Repeat these steps for 3 water reward valves
    * Plug in a 12 V (1+ A) power cord to the 2.1 mm power jack
6) Assemble the liquid reward delivery tubing
    * Connect a water reservoir (e.g. a large syringe tube) to the top ports of each of the liquid reward solenoid valves using masteflex tubing. Use Tee connectors to split the tubing from the reservoir so that all valves are connected
    * Connect the lower ports of each liquid reward solenoid valve to the reward spounts on the Thermobox (the holes above the nose poke detectors)
    * Mounted the water reservoir 6-12" above the height of the reward spouts (the water rewards are gravity-fed)
8) Assemble the LED display boards
    * [displays are deprecated, to be replaced with newer I2C displays such as [this one](https://www.adafruit.com/product/4741)]
9) Assemble the water bath valves and large tubing
    * Hook up water baths and valves according to the [tubing diagram](https://github.com/misaacson01/Operant_Temp_Assay_V2/blob/main/docs/Thermo%20Assay%20V2%20Diagrams.pptx)
    * Connect the large ans small solenoid valves to power similarly as was done fot the liquid reward small solenoid valves
10) Assemble the water bath small tubing
    * Using the ThermoTemplate 3D print, wind the copper tubing into the slots of the template to bend the copper tubing into a shape of a regularly-spaced grid. Remove the copper tubing from the template when finished
    * Connect the copper tubing to the small valve tubing according to the [tubing diagram](https://github.com/misaacson01/Operant_Temp_Assay_V2/blob/main/docs/Thermo%20Assay%20V2%20Diagrams.pptx)
    * Slide the copper tubing grid into the floor slot of the Thermobox

## Software Installation

##### Install software on the host PC
1) Install [Matlab](https://www.mathworks.com/products/matlab.html)
2) Download this repository's code (with Github Desktop or your preferred SVN client)
3) Open Matlab and add this repository to the list of paths:
    * In Matlab's "Home" tab, click "Set Path"
    * Click "Add with Subfolders"
    * Navigate to the file location where this repository was downloaded and click on the folder name
    * "Save" and "Close"
4) Install the [Arduino IDE](https://www.arduino.cc/en/software).
5) Open the Arduino IDE, click "Tools", then "Manage libraries". Install the following libraries: Adafruit MAX31856; Adafruit GFX; Adafruit Seesaw; Adafuit ST7735 and ST7789; Adafruit BUSIO

##### Install the Arduino controller firmware
1) Plug in the Arduino microcontroller connected to the apparatus to the host PC.
2) Open the Arduino IDE
3) Select "Arduino Uno" from the microcontroller dropdown menu, and select the port which your Arduino is connected to
    * if you don't know which port the Arduino is on, try disconnecting the Arduino to see which port disappears
4) Download the controller.ino file from the [Arduino](https://github.com/misaacson01/Operant_Temp_Assay_V2/tree/main/Arduino) folder and load it in the Arduino IDE
5) Upload the code to the Arduino

##### Test the software installation
1) In the Matlab command window, type "edit TV2_Test_Experiment"
2) In Matlab's "Editor" tab, click "Run"
3) Watch the command window and the apparatus to check that everything is working correctly

## Running an experiment
To be added

## Troubleshooting
To be added

## Updating Arduino code
To be added
