#!/bin/bash

# S41 4G POWER (GPIO4_IO02, ((4-1)*32)+2)=98)
echo 98 > /sys/class/gpio/export
sleep 1
echo out > /sys/class/gpio/gpio1/direction
sleep 1
echo 1 > /sys/class/gpio/gpio1/value
sleep 2

# P108 Camera POWER
echo 504 > /sys/class/gpio/export
sleep 1
echo out > /sys/class/gpio/gpio2/direction
sleep 1
echo 1 > /sys/class/gpio/gpio2/value
sleep 2

# P117 USB HUB Power 
echo 510 > /sys/class/gpio/export
sleep 1
echo out > /sys/class/gpio/gpio3/direction
sleep 1
echo 1 > /sys/class/gpio/gpio3/value
sleep 2

# P126 RESET USB HUB (GPIO4_IO28, ((4-1)*32)+28)=124)
echo 124 > /sys/class/gpio/export
sleep 1
echo out > /sys/class/gpio/gpio4/direction
sleep 1
echo 1 > /sys/class/gpio/gpio4/value
sleep 2

# S107 EPD POWER (GPIO4_IO27)
echo 123 > /sys/class/gpio/export
sleep 1
echo out > /sys/class/gpio/gpio5/direction
sleep 1
echo 1 > /sys/class/gpio/gpio5/value




