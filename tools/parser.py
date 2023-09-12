#!/usr/bin/env python

import os
import sys
from parse import *

if __name__ == "__main__":
    fd =  open(sys.argv[1], 'r')
    lines = fd.readlines()
    i = 0
    while (i < len(lines)):
        iter = (parse("Iteration {}:\n", lines[i]))[0]
        offset = (parse("Achieved score {} with offset {} and depth {}\n", lines[i+2]))[1]
        input = (parse("Input: {}\n", lines[i+3]))[0]
        print(iter+","+offset+","+input)
        i+=4

