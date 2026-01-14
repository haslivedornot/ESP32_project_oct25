# metropolia-oct25-esp

## Preliminary design
<img width="812" height="715" alt="image" src="https://github.com/user-attachments/assets/4564f8ad-db44-460a-8e99-506fba639e6e" />

### About
This project is mandatory part of our degree programme in Electronics in the first period of our studies in Metropolia UAS, October 2025.

An embedded system project where an ESP32 was connected to a humidity and temperature sensor, as well as an LED bar and LCD screen that provides visual reading on the current temperature and humidity. When those two variables exceeded the set limit, the program will trigger a warning and turns on a fan connected to the board to lower the temperature. There is also an audible warning from the hardware, which can be turned off by pressing a button.
The readings will be sent to a web server that keeps track of recent readings and provide warnings to the end-users as shown below.

Our team earned full points as our final grade.

The code for this project was written in rthe Arduino IDE, while the web app was written in Express.js and React.js. The .ino file can be found in root.

<img width="2127" height="1187" alt="image" src="https://github.com/user-attachments/assets/f8637d44-e3b6-4196-8864-a1f1a9314eeb" />

Personal contribution:

Svetlana Khodos:

My main role on the project was designing the circuit and programming the hardware to perform the functions.
