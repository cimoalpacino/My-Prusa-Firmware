# Prusa i3 MK2/2.5 Firmware for Arduino RAMPS 1.4 electronics

For when you want the best for less!

This is the Prusa i3 MK2/2.5 Firmware v3.6.0 (#2069) modified to run on RAMPS. The main reason to run this firmware would be if you have a MK42 heatbed and PINDA sensor and use all the great features of the Original Prusa i3 with RAMPS. 

THIS IS STIL A FW IN DEVELOPMENT, SO IT MAY HAVE SOME BUGS (more info on last commit).

## Added/Backported from Marlin 1.1.x
* RAMPS 1.4 support.
* Changed bootscreen to say "Unoriginal" in place of "Original". If you want to use the original firmware, buy a Mini-Rambo!

## Removed
* Farm mode.
* Silent/High power/Normal mode have no effect on drivers that are not TMC. If you're using Allegro drivers you can't set the motor current via software so it doesn't do anything anyway.

## Added
* new xyz auto bed leveling calibration from MK2.5/MK3

## Needs to be added
* find and remove all bugs (display settings, temperature readings, wizard steps....)
* filament sensor (when most bugs will be resolved)
* Power panic (UVLO) feature

THIS IS STIL A FW IN DEVELOPMENT, SO IT MAY HAVE SOME BUGS (more info on last commit).

The config file has  been modified for my printer (Prusa i3 Bear Upgrade with MK42 heatbed and PINDA), so before running please set the steps/mm and motor/endstop inverting as necessary. 
Happy printing!
