#include <Python.h>

#include "fitness.h"
#include "caller.h"

float fitness(std::vector<std::string> & genes) {
    PyImport_AppendInittab("caller", PyInit_caller);
    Py_Initialize();
    PyImport_ImportModule("caller");
    float ret = call_fitness(genes);
    Py_Finalize();
    return ret;
}
