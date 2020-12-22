# cython: language_level=3
# distutils: language=c++
from libcpp.vector cimport vector
from libcpp.string cimport string

import sys
sys.path.insert(0, '')

from fitness import fitness

cdef public float call_fitness(vector[string] & genes):
    return fitness(genes)
