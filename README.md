# Elevator Project - TTK4235 Embedded Systems 

The main goal of this project was to design and program a functional elevator that can receive hall orders up, hall orders down, and cab call from within the elevator.
The project was programmed in C utilizing the elevator hardware found in the Real time programming laboratory. The project can also be run on the [`SimElevatorServer`](https://github.com/TTK4145/Simulator-v2) program to test the program. 

With permission from the Lab Instructor Kolbjørn Austreng, we were permitted to communicate with the elevator hardware via the a server used in the course TTK4145 Real-time Programming, [`ElevatorServer`](https://github.com/TTK4145/elevator-server). This means `elevator_hardware.c` and `elevator_hardware.h` were used to communicate with the elevator instead of:

 - `elev.c` 
 - `elev.h` 
 - `io.c`
 - `io.h`

The functions names in `elevator_hardware.c` are the same and behave in the same manner as the files listed above.

--------------------

### Documentation

Documentation for this project can be found as html version to be opened in an internet browser or via the pdf document.
The html documentation can be found by opening `elevator/html/index.html` (or by clicking [here](elevator/html/index.html)) and the `pdf` version can be found by opening `elevator/latex/refman.pdf`(or by clicking [here](elevator/latex/refman.pdf)).
The various diagrams for the project can be found in the `elevator/docs/` folder and can also be seen below.
It is **HIGHLY RECOMMENDED** to view the `html` documentation instead of the `pdf` version as the formatting makes it easier to read.

--------------------

### Running the program

The program can be run in the lab by starting up a terminal by typing in the command: 

```
ElevatorServer
```

In another terminal instance the following can be written to compile and run the elevator:

```
make
./heis
```

Alternatively, the following command can be run once to grant permission to a bash script:

```
chmod +x run_elevator
```

Followed by the following command every time the program is to be compiled and run:

```
./run_elevator
```

--------------------

### Diagrams

The following diagrams were created to more easily design and understand the system. These can also be found as `.pdf` versions in the `elevator/docs/` folder 

![Class Diagram of the Elevator System](file:///home/olav/tilpdat-heis/elevator/docs/Klassediagram.png)

![Sequence Diagram of the Elevator System](file:///home/olav/tilpdat-heis/elevator/docs/Sekvensdiagram.png)

![State Diagram of the Elevator System](file:///home/olav/tilpdat-heis/elevator/docs/Tilstandsdiagram.png)