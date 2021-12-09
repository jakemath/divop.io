#!/usr/local/bin/python

import ctypes

lib = ctypes.cdll.LoadLibrary('/src/libbigint.so')

class Bigint(object):
    def __init__(self, size_or_digits, randomize=False):
        lib.generate.argtypes = [ctypes.c_uint64, ctypes.c_bool]
        lib.generate.restype = ctypes.c_void_p
        lib.from_str.argtypes = [ctypes.c_char_p]
        lib.from_str.restype = ctypes.c_void_p
        lib.size.argtypes = [ctypes.c_void_p]
        lib.size.restype = ctypes.c_ulonglong
        lib.divides.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
        lib.divides.restype = ctypes.c_bool
        lib.as_cstr.argtypes = [ctypes.c_void_p]
        lib.as_cstr.restype = ctypes.c_char_p
        if type(size_or_digits) is str:
            self.obj = lib.from_str(size_or_digits.encode('utf-8'))
        else:
            self.obj = lib.generate(size_or_digits, randomize)

    def size(self):
        return lib.size(self.obj)

    def as_str(self):
        return lib.as_cstr(self.obj).decode('utf-8')
        
    def divides(self, dividend):
        return lib.divides(dividend.obj, self.obj)