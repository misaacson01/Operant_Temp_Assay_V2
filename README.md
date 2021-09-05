# Operant_Temp_Assay_V2

This repo contains code and instructions for installing, updating, and operating the Hoon lab operant temperature apparatus (V2).

## Getting Started

### Requirements
- Matlab (created using 2020b, no toolboxes necessary). Download here: https://www.mathworks.com/products/matlab.html
- Arduino IDE (for uploading/updating arduino code). Download here: https://www.arduino.cc/en/software
- Arduino libraries: Adafruit MAX31856; Adafruit GFX; Adafruit Seesaw; Adafuit ST7735 and ST7789; Adafruit BUSIO
- To install Arduino libraries: Open the Arduino IDE, Click "Tools" then "Manage libraries". Search for and install the 5 libraries listed above.

### Assembly
1. Hook up water baths according to the tubing diagram locating in /docs/Thermo Assay V2 Diagrams.pptx
2. Hook up the water reward reservoir to the tubing coming from the 3 reward valves (R1, R2, R3). The water rewards are gravity-fed, so the water reward reservoir should be mounted at a height somewhat above the height of the reward spouts. 6-12 inches above the reward spout is typically all that's needed to provide adequate water pressure.
3. Plug in a 12 V (1+ A) power cord to the "12 V" labelled power jack (The 5 V power cord is not necessary).
4. Plug in the USB hub to the computer with Matlab and Arduino software installed.

### Custom Software Installation
1) Download this repository's code (with Github Desktop or your preferred SVN client)
2) Open Matlab and add this repository to the list of paths:
    a) In Matlab's "Home" tab, click "Set Path"
    b) Click "Add with Subfolders"
    c) Navigate to the file location where this repository was downloaded and click on the folder name
    d) "Save" and "Close"
3) In the Matlab command window, type "edit TV2_Test_Experiment"
4) In Matlab's "Editor" tab, click "Run"
5) Watch the command window and the apparatus to check that everything is working correctly

## Running an experiment
To be added

## Troubleshooting
To be added

## Updating Arduino code
To be added
