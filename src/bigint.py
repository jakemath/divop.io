#!/usr/local/bin/python

import ctypes

lib = ctypes.cdll.LoadLibrary('/src/libbigint.so')

class Bigint(object):

    def __init__(self, size_or_digits, randomize=False):
        lib.generate.argtypes = [ctypes.c_uint64, ctypes.c_bool]
        lib.generate.restype = ctypes.c_void_p
        lib.from_str.argtypes = [ctypes.c_char_p]
        lib.from_str.restype = ctypes.c_void_p
        lib.print.argtypes = [ctypes.c_void_p]
        lib.print.restype = ctypes.c_void_p
        lib.divides.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
        lib.divides.restype = ctypes.c_bool
        lib.as_digits.argtypes = [ctypes.c_void_p]
        lib.as_digits.restype = ctypes.POINTER(
            ctypes.c_short * (
                len(size_or_digits) if type(size_or_digits) is str else size_or_digits
            )
        )
        if type(size_or_digits) is str:
            self.obj = lib.from_str(size_or_digits.encode('utf-8'))
        else:
            self.obj = lib.generate(size_or_digits, randomize)

    def print(self):
        lib.print(self.obj)

    def get_digits(self):
        digits = lib.as_digits(self.obj)
        return [x for x in digits.contents]

    def as_str(self):
        return ''.join([str(x) for x in self.get_digits()])

    def divides(self, dividend):
        return lib.divides(dividend.obj, self.obj)
