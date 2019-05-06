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
    my_file = f.split('.')[0] + ".cor"
    sp.getoutput('rm ' + my_file)
    open = sp.getoutput("./" + asm + " --color=force " + f)
    output = ""
    error = ""
    success = ""
    if open:
        output = sp.getoutput('xxd ' + my_file)
    else:
        error = RED + "ERROR: " + f + " doesn't match any file"
    if not error and output[0] == 'x' :
        error += "\n" + RED + "ERROR: " + "for " + asm + " " + my_file + " hasn' t been created"
        error += "\n" + WHITE + open
    else:
        success += "\n" + GREEN + "SUCCES: for " + asm + " " + my_file + " has been created"
    return (output, error, success)

def printDiff(asm, asm_42):
    asm = asm.split('\n')
    asm_42 = asm_42.split('\n')
    error = 0
    i = 0
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
    return (printDiff(asm, asm_42))

def main():
    i = 0
    print(ASM_42 + ": -")
    print(ASM + ": +")
    tmp = ""
    succes = ""
    bad_file = ""
    nb_err = 0
    for arg in sys.argv:
        error = 0
        err = ""
        if i > 0:
            asm_42, err , success = get_file(ASM_42, arg)
            asm_42 = asm_42.split('\n')
            asm, tmp, success_tmp = get_file(ASM, arg)
            err += tmp

            success += success_tmp
            asm = asm.split('\n')
            print(WHITE + "\n- - - - - - - - - - - - " + arg.upper() + " - - - - - - - - - - - -\n")
            if success:
                print(success + WHITE)
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
                print(GREEN + "Diff = OK")
            elif "hasn' t" in tmp and "hasn' t" in err:
                print(err)
                print(GREEN + "Diff = OK (both files haven't been created)")
            else:
                bad_file += "\t- " + arg + "\n"
                nb_err += 1
                print(err)
                print(RED + "Diff = KO (%d bad line(s))" % error)
        i += 1
    if nb_err == 0:
        print("\n%s[%d/%d]\tSUCCES" %(GREEN, i - 1 - nb_err, i - 1))
    else:
        print("\n%s[%d/%d]\tfound %d error(s):" %(RED, i - 1 - nb_err, i - 1, nb_err))
        print(WHITE + bad_file)

    

if __name__== "__main__":
  main()
