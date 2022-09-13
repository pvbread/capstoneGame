from setuptools import setup
from setuptools import Extension

#cpp_args = ['-std=c++14', '-I/Library/Frameworks/SDL2.framework', '-F/Library/Frameworks', '-framework', 'SDL2']

sfc_module = Extension(
    '_helloSDL_game', 
    sources=['_helloSDL.cpp'],
    extra_compile_args=['-std=c++14 -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2'],
)

setup(
    name='hello-SDL-lib',  
    version='1.0',
    ext_modules=[sfc_module],
)






