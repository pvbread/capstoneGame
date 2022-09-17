# Setting up SDL2 Windows

Download SDL2 for mingw (add notes for mingw setup) and set it in a nice final spot.

Checkout the CMakeLists.txt file in this the directory that needs SDL and make sure
`SDL2_INCLUDE_DIR` and `SDL2_LIBRARY_DIR` are pointing to the include and lib directories respectively.

Create a build folder. cd into that folder.
Building commands
(the first creates a makefile, the second compiles the project)
```
cmake .. -G "MinGW Makefiles"
mingw32-make
```
<br>
<b>Very important</b>
<br>
Copy the SDL2.dll from the SDL2/bin folder into the same build folder as your program.
