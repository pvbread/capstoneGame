# Working on Integrating CMake with our project

Idea: Have a CMake setup so that both windows and mac builds will
be streamlined.

Importantly, this will standardize the way we format our CPP
projects without needing to worry about format/IDE once its set.
(i.e.: mac and win can now both `#include <SDL.h>` without having
to switch to `#include <SDL2/SDL.h>` depending on platform).

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

