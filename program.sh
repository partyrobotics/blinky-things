#!/bin/sh
# This script programs the chandelier. Could be called from the makefile..

dfu-programmer at90usb162 erase
dfu-programmer at90usb162 flash xmas_led.hex
