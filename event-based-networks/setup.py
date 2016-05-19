#!/usr/bin/env python

"""
setup.py file for Python wrapping of a C++ network data structure
"""

from distutils.core import setup, Extension


example_module = Extension('_eventpy',
                           sources=['eventpy_wrap.cxx'],
                           )

#                           sources=['eventpy_wrap.cxx', 'eventList.h'],
setup (name = 'eventpy',
       version = '0.1',
       author      = "Mikko Kivela",
       description = """Python wrapping of a C++ event list data structure.""",
       ext_modules = [example_module],
       py_modules = ["eventpy"],
       )
