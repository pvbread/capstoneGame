# Setting up SDL2

### What is SDL2? (recommended)
<p>
 https://youtu.be/QM4WW8hcsPU
</p>

## Windows setup 
<p>
(I'll add different ways to do this, particularly from Visual studio and vscode): https://youtu.be/DQ-NBjBFLJ4
</p>

## Mac setup
<p>
https://youtu.be/V6ACATpl2LQ
</p>

# Testing
<p>
Test in SDLTest1 directory if you can succesfully compile without error! (note, we don't need glad for now like in some of the tutorial videos) 
<br>
Commands to run:
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
