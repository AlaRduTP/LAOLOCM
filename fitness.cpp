#include "fitness.h"
#include "caller.h"

int fitness_initialize() {
    int ret = PyImport_AppendInittab("caller", PyInit_caller);
    Py_Initialize();
    return ret;
}

float fitness(std::vector<std::string> & genes) {
    PyImport_ImportModule("caller");
    float ret = call_fitness(genes);
    return ret;
}

void fitness_finalize() {
    Py_Finalize();
}
