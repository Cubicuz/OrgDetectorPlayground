

# Requirements Prio 1
I2C Bus:
ADC, Display, Rotary Encoder

## User Interface
### Display contents
#### Play Menu
closeness over time
maximum closeness
denied counter
toy activation

Maximum closeness can be adjusted by Encoder
Long Press goes back to menu


#### Bluetooth
show all available devices
show supported available devices
show saved devices
save a device
select a device
Autoconnect to saved devices


### Userinput
stop button


### Settings
maximum closeness
calibration

## Signal evaluation
### Baseline detection
lowpass for baseline detection (probably 10s to 30s filtertime) 
store lowest value of last 30s and limit baseline increments to a factor of this value


### Highside detection
highpass integration determines closeness

## Interface to toys
mosfet for boolean interface
mosfet PWM interface?
bluetooth interface

decrease toy intensity as integral aproaches maximum closenes

# Requirements Prio 2
connection to homeassistant
offer certain values like denied counter
