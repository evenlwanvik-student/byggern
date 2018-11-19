# "Byggern" on Linux

[TTK4155 - Embedded and Industrial Computer Systems Design](https://www.ntnu.edu/studies/courses/TTK4155#tab=omEmnet)

**Course content:** Design of embedded computer systems. Computer architectures and system components for embedded and industrial applications. Microcontrollers and specialized microprocessors. Parallel and seriabl bus systems. Datacommunication in industrial environments. Analog/digital interfaces.

In this project, we (a group of two in this case) were to create an electromechanical ping pong game. The main challenge were to assemble the hardware components and develop software for the microcontrollers, making a fully functional embedded computer system that will enable you to play a "game of ping pong". The tern project was a rather comprehensive labratory exercise which required allocation of a substantial amount of time for the lab throughout the semester. We were given one day every week, where we had access to the lab and a student assistant available. An oral presentation and practical demonstration of this labratory project accounts for 50% of the final grade. The project was divided into nine parts, that we did more-or-less on a week-to-week basis:

* **part 1** - Initial assembly of microcontroller and RS-232
* **part 2** - Address decoding and external RAM
* **part 3** - A/D converting and joystick input
* **part 4** - OLED display and user interface
* **part 5** - SPI and CAN controller
* **part 6** - CAN-bus and communication between nodes
* **part 7** - Controlling servo and IR
* **part 8** - Controlling motor and solenoid
* **part 9** - Completion of the project and extras

example hierarchy of each node folder:
```
.
├── build
│   ├── a.out
│   ├── pingpong.o
│   ├── main.hex
│   └── main.o
├── pingpong.c
├── pingpong.h
├── main.c
└── Makefile

```

