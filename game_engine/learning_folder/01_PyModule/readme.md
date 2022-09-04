# writing a cpp module extension for python

## Windows

I think it might be just cleanest to use WSL as I think the setup for Python.h is less fiddly.

In WSL you may need to also install python-dev but I have to look into this. I might try it on a virtual machine later (I'm on mac).

## Mac

Should run fine.

## To run

In this directory (assuming you have python3 and pip installed)

### Quick and dirty way
```
pip install .
```

Now you can run python3 to test
```
python3
>>> import _hello
>>> _hello.hello_game() # should print a nice message
```

### More formal

```
python3 setup.py install
```

Now you can run python3 to test
```
python3
>>> import _hello
>>> _hello.hello_game() # should print a nice message
```

Why is it more formal? The wildcard `.` in linux targets the entire directory. If we hypothetically had multiple modules and only wanted to target a specific once, we would have more control by installing the setup.py file.