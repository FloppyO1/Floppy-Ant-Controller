# F.A.C. V2 - Floppy Ant Controller

**F.A.C. V2** is an advanced control board specifically designed for combat robots (such as the Antweight category). Created by **Floppy Lab™**, it serves as a high-performance, highly customizable alternative to the Malenki-nano, offering professional features at a competitive price point.

## 🚀 Key Features

- **Full Customization**: All parameters are adjustable via USB using the dedicated [FAC Tool](https://factool.floppylab.it/).

- **Advanced Hardware**: Features a built-in **IMU** (accelerometer and gyroscope) for future firmware updates and advanced control algorithms.

- **Open Source**: Built with a community-first approach, allowing for user modifications and contributions.

- **Robust Safety**: Includes integrated Watchdog timers, Fail-Safe mechanisms, and Battery Cut-Off to protect your hardware.

## 📊 Technical Specifications

- **DC Motor Outputs**: 3 channels (1.8A max, 1000-step resolution, adjustable frequency 100Hz-10kHz).

- **Servo Outputs**: 2 HV channels (powered directly by the battery) with adjustable frequency and pulse width.

- **Inputs**: 2 Digital inputs and 1 Analog input (max 3.3V) for sensors or telemetry.

- **Power Supply**: Optimized for 1-2S LiPo batteries (up to 7.4V nominal).

- **Compact Form Factor**: 21mm x 31mm; weighs only 2.4g (bare board).

- **Receiver Compatibility**: Supports PWM (up to 4 channels) and PPM (up to 8 channels).

## 🛠️ Configuration (FAC Tool)

The **FAC Tool** provides a graphical interface for granular control over your robot's behavior:

1. **Mapping**: Freely assign software functions (e.g., Tank Mix) to physical pins on the board.

2. **Mixes & Special Functions**: Easily configure steering mixes or direct links between radio channels and outputs.

3. **Real-time Telemetry**: Monitor battery voltage, radio signal values, and operational status (Armed/Disarmed/Cut-Off).

## ⚠️ Safety Information

- **Reverse Polarity**: The board **does not** have reverse polarity protection. Ensure the battery is connected correctly to avoid permanent damage.

- **Activation**: For safety, the FAC will not activate until it detects a valid signal from your receiver.

- **Battery Management**: Always configure the Battery Cut-Off to prevent over-discharging your LiPo cells.

## 📂 Project Structure

- `/firmware`: Firmware source code and binaries.

- `/docs`: Full English manual and wiring diagrams.

- `/hardware`: Design files and pinout maps.

## 📩 Support & Feedback

Developed by **Floppy Lab™**. For suggestions, bugs, or feedback:

- **Email**: thefloppylab@gmail.com

- **Instagram**: [@the_floppy_lab](https://www.instagram.com/the_floppy_lab)

- **YouTube**: [@floppylab](https://www.youtube.com/@floppylab)

- **Website**: [www.floppylab.it](https://www.floppylab.it)

---

*© 2026 The Floppy Lab™. All rights reserved.*
