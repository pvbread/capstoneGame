from setuptools import setup
from setuptools import Extension

setup(
    name='hello-game-lib',
    version='1.0',
    ext_modules=[Extension('_hello', ['_hello.cpp'])],
)