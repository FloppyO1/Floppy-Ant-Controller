# Floppy Ant Controller

The Floppy Ant Controller is a control board for Antweight (150g) fighting robots. It implements many features and thanks to the ability to change settings without having to modify the code it makes it perfect for less experienced people.

Taking inspiration from the now well-known Malenki nano (https://github.com/MarkR42/malenki-nano.git), other features have been added, maintaining a small size and an affordable price.

<img src= "https://github.com/FloppyO1/Floppy-Ant-Controller/blob/main/images/render%201.png" width="500">

## Features

- 3x DC motors driver (200 speed steps);
- 2x servo outputs (one of which can be selected with a soldering jumper if 5V or HV from the battery);
- 200Hz servo frequency;
- BEC 5V 3A max;
- 2x expansion ports with possible digital or analog readout (e.g. potentiometers, switches etc.);
- Support for 1 and 2s batteries;
- Batter voltage readout, with settable weapon speed limitation threshold & cutoff threshold (indipendent);
- Settings can be set via USB with dedicated software or serial terminal;
- 4 channels PWM, and 8 channels PPM compatibility;
- Dimensions of 23mm*33mm for a weight of 2.9g.
<img src="https://github.com/FloppyO1/Floppy-Ant-Controller/blob/main/images/FAC1.3%20weight%20(no%20reciever).jpg" width="500">

## Last hardware version notes (V1.3)
In the latest version (V1.3), problems with errors in the schematic were fixed, specifically a capacitor in the Buck Converter that prevented it from functioning correctly.

Compared to version 1.2, the battery and switch connections have been moved, because in their previous position they were too close to the receiver connections, so there was a risk of short circuits and burning out the entire 3.3V part (which happened to me😢).

## Software notes:

The FAC settings tool has been completely rewritten, this time not using phython and pysimplegui (which I discovered ask for a monthly fee to be distributed).

This new version, signed with the 2.0 signature, is much more stable and looks better in my opinion. Like the previous version, it allows you to export and import all your settings in a .csv file for easy sharing.

## Support this project with a donation

To support this project you can make a donation via PayPal or buy a Floppy Ant Controller:

- https://www.paypal.com/paypalme/floppylab

- https://ko-fi.com/floppylab


<img src="https://floppylab.altervista.org/wp-content/uploads/2023/02/cropped-Stemma-scritta-tonda.png" width="300">

## License
[CERN Open Hardware Licence Version 2 - Strongly Reciprocal](https://choosealicense.com/licenses/cern-ohl-s-2.0/#)
