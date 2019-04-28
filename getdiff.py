#!/usr/bin/python3

import sys
import os
import subprocess as sp

ASM_42 = "resources/asm_42"
ASM = "asm"
GREEN = "\x1b[1;38;2;35;209;139m"
RED = "\x1b[1;38;2;241;76;76m"
WHITE = "\x1b[1;38;2;255;255;255m"

def get_file(asm, f):
    open = sp.getoutput("./" + asm + " " + f + " | grep 'Writing output program to'")
    output = ""
    error = ""
    my_file = ""
    if open:
        f = f.split('.')
        my_file = f[0] + ".cor"
        output = sp.getoutput('xxd ' + my_file)
        test = sp.getoutput('rm ' + my_file)
    else:
        error = RED + "ERROR: " + f + " doesn't match any file"
    
    if not error and output[0] == 'x' :
        print(open)
        error = "\n" + RED + "ERROR: " + asm + " doesn't create " + my_file
    return (output, error)

def compare(str1, str2):
    i = 0
    asm = ""
    asm_42 = ""
    while i < len(str1) and i < len(str2):
        if str1[i] == str2[i]:
            asm_42 += GREEN
            asm_42 += str1[i]
            asm += GREEN
            asm += str2[i]
        else:
            asm_42 += RED
            asm_42 += str1[i]
            asm += RED
            asm += str2[i]
        i += 1
    while i < len(str1):
        asm_42 += RED
        asm_42 += str1[i]
        i += 1
    while i < len(str2):
        asm += str2[i]
        asm += RED
        i += 1
    i = 0
    asm = asm.split('\n')
    asm_42 = asm_42.split('\n')
    error = 0
    while i < len(asm_42) and i < len(asm):
        if asm_42[i] != asm[i]:
            print(WHITE + "- " + asm_42[i])
            print(WHITE + "+ " + asm[i])
            error += 1
            print("")
        i += 1
    while i < len(asm_42):
        print(WHITE + "- " + asm_42[i])
        print(RED + "null")
        print("\nc", end='')
        error += 1
        i += 1
    while i < len(asm):
        print(WHITE + "+ " + asm[i])
        print(RED + "null")
        print("\nc", end='')
        error += 1
        i += 1
    return (error)

def main():
    i = 0
    print(ASM_42 + ": -")
    print(ASM + ": +")
    for arg in sys.argv:
        error = 0
        err = ""
        if i > 0:
            asm_42, err = get_file(ASM_42, arg)
            asm_42 = asm_42.split('\n')
            asm, err = get_file(ASM, arg)
            asm = asm.split('\n')
            print(WHITE + "\n- - - - - - - - - - - - " + arg.upper() + " - - - - - - - - - - - -\n")
            if not err:
                j = 0
                while j < len(asm) and j < len(asm_42):
                    error += compare(asm_42[j], asm[j])
                    j += 1
                while j < len(asm_42):
                    error += compare(asm_42[j], "")
                    j += 1
                while j < len(asm):
                    error += compare("", asm[j])
                    j += 1
                if error > 0:
                    print(RED + "Diff = KO (%d bad line(s))" % error)
                else:
                    print(GREEN + "Diff = OK")
            else:
                print(err)
        i += 1

if __name__== "__main__":
  main()
