ReadMe is from user sakibc with my addition at the end

# Prusa i3 MK2 Firmware for RAMPS 1.4

For when you want the best for less!

This is the Prusa i3 MK2 Firmware v3.1.0 modified to run on RAMPS. The main reason to run this firmware would be if you have a MK42 heatbed you would like to use with RAMPS. Everything hopefully works as you'd expect, but please configure/calibrate your printer before using this!

## Added/Backported from Marlin 1.1.x
* RAMPS 1.4 support, with potential for more boards to be easily added.
* Support for TMC2130 SilentStepStick stepper motor drivers. May be buggy!
* Configuration toggle for the E3D Volcano hotend, so that the build volume is adjusted to account for its longer heatblock.
* Changed bootscreen to say "Unoriginal" in place of "Original". If you want to use the original firmware, buy a Mini-Rambo!

## Removed
* Farm mode.
* Silent/loud mode. If you're using Allegro drivers you can't set the motor current via software so it didn't do anything anyway.

The config file has also been modified for my printer, so before running please set the steps/mm and motor/endstop inverting as necessary. Happy printing!

## Added
* filament sensor
