#include <Python.h>

static PyObject* _hello_game(PyObject* self) {
    return PyUnicode_FromString("Hello, Game!");
}

static struct PyMethodDef methods[] = {
    {"hello_game", (PyCFunction)_hello_game, METH_NOARGS, "Prints Hello, Game!"},
    {NULL, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "_hello",
    "A module that prints Hello, Game!",
    -1,
    methods
};

PyMODINIT_FUNC PyInit__hello(void) {
    return PyModule_Create(&module);
}