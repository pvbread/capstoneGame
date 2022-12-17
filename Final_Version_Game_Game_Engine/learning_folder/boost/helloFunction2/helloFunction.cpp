#define PY_SSIZE_T_CLEAN

#include <Python.h>

int main(int argc, char const *argv[])
{
    Py_Initialize();
    PyObject* obj = Py_BuildValue("s", "../multiply.py");
    //insert error checking
    FILE* fp = _Py_fopen_obj(obj, "r+");
    if (fp != NULL)
    {
        PyRun_SimpleFile(fp, "../multiply.py");
    }

    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}