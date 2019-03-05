# QNoise
C++ based implementation of (orignal) Perlin Noise and OpenSimplex Noise, intended to be used with your Qt-Application


Perlin noise is a type of gradient noise developed by Ken Perlin in 1983 as a result of his frustration with the "machine-like" look of computer graphics at the time.

Since then the algorithem has been updated in 2001 with 3D+ implementations of simplex noise.

- PerlinNoise is a simple c++ class. Implementing the original noise algorithem adapted to 3D.

The source implementation is in Java, made by the New York University Media Research Lab.
See for reference here: https://mrl.nyu.edu/~perlin/noise/

And the translation to c++11 is made by sol-prog
See for reference here: https://github.com/sol-prog/Perlin_Noise

- QNoise is a cpp class implementing OpenSimplex Noise algorithem, related to Ken Perlin's updated algorithem (2001) but only related and open source

The source implementation was made in Java by Kurt Spencer and was translated to c++11 by me.
See for reference here: https://gist.github.com/KdotJPG/b1270127455a94ac5d19

The example is a ready to compile Qt-Application (c++11 compiler and Qt5 needed)
A noise map is drawn on top of the QWidget and shows the algorithem in action.

A couple of Keyboard input change what is drawn on the Widget:
- wasd "moves the window over the map", ctrl key will amplify that movement
- +/- used to "zoom in/out of the map"
- Q-Key switches between Perlin Noise  and OpenSimplex Noise (QNoise)
- C-Key switches between "normal" and contour shapes (can be interpretaed as rings on a tree or hight map)
- T-Key toggles the "animation"
- H-Key toggles the high contrast mode, 2 colors compared to greyscale
- L-Key "loops the animation" - only works with QNoise, as it requieres 4D-Noise generation
