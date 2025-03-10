# Solar-System-in-C
Simulation of the Solar System in C using SDL2 library, and Verlet Integration.


To run this project it is necessary to have installed SDL2 graphics library.
To run using gcc the command is "gcc -Isrc/Include -Lsrc/lib -o main main.c -lmingw32 -lSDL2main -lSDL2"

The tecnhique used to solve the ODE(Ordinary Differential Equations) that update the positions of the planets is Verlet Integration.

There are two buttons on the screen:
Left - Start/Stop Simulation 
Right - Start/Stop PlanetTrack


Start - Right click Mouse   
Stop - Left Click Mouse

To see Simulation Results - 
Activate PlanetTrack on the beginning of the Sim and let it run. Results will be printed in the Terminal.


To increase SIMULATION SPEED - 
Increment the constant DT.


To increase the distance in pixels between planets - Increment the constant SPACE.


Results improve for smaller DT but takes much longer.

Enjoy!

  
