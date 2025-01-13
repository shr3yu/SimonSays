# SimonSays

## Overview
This project provides a twist on the classic game- Simon Says. With the main goal of familiarizing myself with optimizing circuit design, I2C communication and debugging. It is build using an ESP-WROOM-32 microcontroller, I2C display, LEDs, and pushbuttons. Players must repeat increasingly complex light sequences to progress through levels. 
![IMG_2232](https://github.com/user-attachments/assets/6c8b3e12-dbc4-4344-92e2-3ac91aa60bb0)

## Circuit Design

- **Charieplexing** : While building the circuit, I initially multiplexed LEDs to reduce the number of GPIO pins used. However, upon further research, I discovered the Charlieplexing technique and adopted it for my circuit, significantly reducing hardware complexity.
- **Register ladder**: Mapping 6 pushbuttons to 6 GPIO pins would be inefficient and non-scalable. Instead, I utilized a resistor ladder configuration. The pushbuttons were connected in a parallel arrangement, each with a uniquely valued resistor. This setup allowed me to use a single analog pin to read varying voltage levels and accurately determine which button was pressed based on the measured value.
- **Polling**: To detect button presses, I implemented a polling mechanism. The analog pin continuously monitored the voltage changes, ensuring real-time detection of button inputs.

## Takeaways

* **Debugging using a multimeter** : Learned to verify voltage levels and connections in the resistor ladder configuration, ensuring accurate button press detection.
* **Reading datasheets and pinout diagrams**
* **Optimization methods**
* **Familarity with Electrical Schematic tools**: Became comfortable using tools like KiCad for designing and simulating circuit schematics.
