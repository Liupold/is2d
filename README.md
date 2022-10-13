# is2d

## Demo (YouTube):
[![IMAGE ALT TEXT](http://img.youtube.com/vi/4uOXUqEYe50/0.jpg)](http://www.youtube.com/watch?v=4uOXUqEYe50 "Video Title")

### dependencies: `OpenGL <GL/glut.h>`
### build : `make`

### help:

```
is2d [OPTIONS] ...

Spin Up: Green, Spin Down: Blue
Simulates ising model on a 2d grid. with help of C & OpenGL

Writen By: Rohn Chatterjee  (https://github.com/liupold)

-T <Float>	Set simulation temperature. [default: 1]
-j1 <Float>	Interaction strength (left-right) [default: 1]
-j2 <Float>	Interaction strength (up-down) [default: 1]
-h <Float>	External Magnetic field interaction [default: 0]
-dS <Integer>	Number of steps per frame [default: 100000]
-dT <Float>	Change of temperature per dF [default: 0]
-dF <Integer>	Frame Spacing [default: 120]
-fps <Float>	Frames per second [default: 60]
-W <Integer>	Width of the window [default: 500]
-H <Integer>	Height of the window [default: 500]
-R <Integer>	Number of row in lattice [default: 100]
-C <Integer>	Number of column in lattice [default: 100]
-Seed <Integer>	Random Seed [default: Unix epoc]
```
