
![Logo](Public/logo.png)

***

**TerraLite** is a terrain generation tool that gives users full agency and control over it's various procedural functions. Written in C++ and OpenGL, TerraLite can generate complex, high-resolution heightmaps in real-time -- all on the CPU.

# References

The terrain generation methods/algorithms in this projects source are based on the ideas presented in:

* "Realtime Procedural Terrain Generation: Realtime Synthesis of Eroded Fractal Terrain for Use in Computer Games" by Jacob Olsen, 2004.   [Link](https://web.mit.edu/cesium/Public/terrain.pdf)

# Features

* Noise Generation
    * Diamond Square (1/f noise)
    * Voronoi diagrams (for feature points)
    * Pertubation displacement
* Erosion simulation
    * Combination of hydraulic and thermal erosion techniques
    * Iterative solver that displaces material from high areas to low areas
    * Samples Von Neumman neighborhoods for efficency
* I/O
    * Export heightmap as .png
    * Import supported heightmaps
 
# Preview
<img src="Public/preview1.png" alt="" width="50%"/><img src="Public/preview2.png" alt="" width="50%"/>

You can watch the video I made about TerraLite here: https://youtu.be/2ibV22CyZ3I

# Build
* Build automation requires Python 3.10+ and Jinja2
  
Project files and build are generated using CMake. I recommend creating a build folder and running 
```
cmake ..
```
from that directory. This will generate relevant project files which you can then use to compile a working executable.
