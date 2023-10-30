# mtb_suspension_analyzer

## Description

This is a prototype of a suspension analyzer system.

The project will be initially developed for a hardtail mtb. A prototype for full suspended mtb will probably be developed in the future with the help of some friends who own a full suspended mtb.

The code is mainly written in C. 

At the moment the prototype will be developed using a Raspberry Pi Pico as MCU with 16MB of RAM.

Probably one of the next step will be to add a (third) LSM6DSOX sensor in crankset zone to get better data about bike stability.

## Dev stages

1. Develop code and circuit diagram for the first version of the project: an easy data collector
2. Understand how to elaborate collected data
3. Write UML and LaTeX documentation for a more advanced version of the project
4. Continue...

## Used Hardware

- 1 x Raspberry Pi Pico
- 2 x LSM6DSOX
- 1 x SSD1306 OLED display (0.91", (128x32)px)
- 1 x micro sd storage expansion board
- 2 x 18650 batteries in series
