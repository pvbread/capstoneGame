#Setting up SDL2

<p>
What is SDL2? (recommended)
 https://youtu.be/QM4WW8hcsPU
</p>

<p>
Windows setup (I'll add different ways to do this, particularly from Visual studio and vscode): https://youtu.be/DQ-NBjBFLJ4
</p>

<p>
Mac setup: https://youtu.be/V6ACATpl2LQ
</p>

<p>
Test in SDLTest1 directory if you can succesfully compile without error! (note, we don't need glad) commands to run:
<br>
Windows
```
g++ windowTest.cpp -o windowTest.exe -lmingw32 -lSDLS2main -lSDL2
```

Mac
```
g++ windowTest.cpp -o windowTest -F/Library/Frameworks -framework SDL2
```
</p>
