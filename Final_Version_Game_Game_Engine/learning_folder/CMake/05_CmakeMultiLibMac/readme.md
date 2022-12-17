# Working on Integrating CMake with our project

Idea: Have a CMake setup so that both windows and mac builds will
be streamlined.

Importantly, this will standardize the way we format our CPP
projects without needing to worry about format/IDE once its set.
(i.e.: mac and win can now both `#include <SDL.h>` without having
to switch to `#include <SDL2/SDL.h>` depending on platform).

## Instructions to compile with windows

In CMakeLists.txt, verify

```
set(SDL2_DIR "C:/dev/SDL2-2.24.0/cmake")
set(SDL2_mixer_DIR "C:/dev/SDL2_mixer-2.6.2/cmake")
```

points to wherever the sdl2_-config file is (e.g. sdle2_mixer-config) in your computer.

Create a build directory. If it's in the project directory the commands will look like

```
mkdir build
cd build
```

Once in build, we have to specify and create the build generator with cmake

```
cmake .. -G "MinGW Makefiles"
```

and finally build

```
mingw32-make
```

<b>We're not done</b>
<br>
We need to move all the binary .dll files into this build folder as well.
And potentially audio assets if they're not pointed to correctly (we'll fix this over time). 
(The make compiles it Alex you dumb fuck remeber this)
## Instructions to compile with mac

<b>This requires SDL2 lib to be present in /Library/Frameworks</b>

In project this project dir, create a build folder and cd into it.

```
mkdir build
cd build
```

then build the project and test (myExec is just a filler name)

```
cmake ../
cmake --build .
./myExec # test run
```

# TODO

Add different project sources to compile.


