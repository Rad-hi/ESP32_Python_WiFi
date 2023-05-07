# ESP32 and Python two-way communication made easy

This a demo on how to establish a two-way communication between an ESP32 and a laptop/SBC (like a raspberry pi) running Python using WiFi (sockets).

## Quickstart

### 1- Arduino libraries to install:

- MacroDebugger (mine): [https://github.com/Rad-hi/MacroDebugger](https://github.com/Rad-hi/MacroDebugger)
- MyButton (mine): [https://github.com/Rad-hi/MyButton_Arduino](https://github.com/Rad-hi/MyButton_Arduino)

### 2- Firmware configuration

Go to ```src/config.h``` and put in your wifi credentials, as well as you laptops' IP address. Now Upload the code to your ESP32 board (hook an LED and a push-button to pins 33, and 25, or any couple you want).

### 3- Python dependencies

Install the Sun-Valley theme of Tkinter using ```python3 -m pip install sv-ttk``` (on some Linux distros, you might need to install tkinter!)

### 4- Show time

run ```python3 scripts/main.py```

Enjoy!
