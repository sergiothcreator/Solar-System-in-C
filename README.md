# Solar-System-in-C
Simulation of the Solar System in C using SDL2 library, and Verlet Integration.


To run this project it is necessary to have installed SDL2 graphics library.
To run using gcc the command is "gcc -Isrc/Include -Lsrc/lib -o main main.c -lmingw32 -lSDL2main -lSDL2"

The tecnhique used to solve the ODE(Ordinary Differential Equations) that update the positions of the planets is Verlet Integration.

There are two buttons on the screen: Left - Start/Stop Simulation , Right - Start/Stop PlanetTrack
Start - Right click Mouse   
Stop - Left Click Mouse

To see Simulation Results - Activate PlanetTrack on the beginning of the sim and let it run. Results will be printed in the Terminal.
To increase SIMULATION SPEED - Increment the constant DT.
To increase the distance in pixels between planets - Increment the constant SPACE.

The results obtained for a DT = 800000 (1sec in real life = 800000sec in sim) are:
MERCURY TOOK 10 DAYS = 0 DAYS
VENUS TOOK 24 DAYS = 0 DAYS
MARTE TOOK 399 DAYS = 1 DAYS
JUPITER TOOK 4368 DAYS = 12 DAYS
SATURNO TOOK 10587 DAYS = 29 DAYS
URANO TOOK 30614 DAYS = 84 DAYS
NEPTUNO TOOK 60450 DAYS = 166 DAYS

If you run for DT = 100000:
MERCURY TOOK 76 DAYS = 0 YEARS
VENUS TOOK 194 DAYS = 0 YEARS
MARS TOOK 641 DAYS = 1 YEARS

It starts to improve very much for smaller DT but takes much longer.

Enjoy!

  
