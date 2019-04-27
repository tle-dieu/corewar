import sys
import os
import subprocess as sp

ASM_42 = "resources/asm_42"
ASM= "asm"

def get_file(asm, f):
    cmd = "./" + asm + " " + f + ">> /dev/null"
    os.system(cmd)
    f = f.split('.')
    my_file = f[0] + ".cor"
    output = sp.getoutput('hexdump -C ' + my_file)
    return (output)

def compare(str1, str2):
    if len(str1) > len(str2):
        max = len(str1)
    else:
        max = len(str2)
    i = 0
    asm = ""
    asm_42 = ""
    while i < max:
        if i < len(str1) and i < len(str2):
            if str1[i] == str2[i]:
                asm += "\033[1;32;40m"
                asm_42 += "\033[1;32;40m"
                asm += str1[i]
                asm_42 += str2[i]
            else:
                asm += "\033[1;31;40m"
                asm_42 += "\033[1;31;40m"
                asm += str1[i]
                asm_42 += str2[i]
        elif i < len(str1):
            asm += "\033[1;31;40m"
            asm += str1[i]
        elif i < len(str2):
            asm_42 += str2[i]
            asm_42 += "\033[1;31;40m"

        i += 1
    i = 0
    asm = asm.split('\n')
    asm_42 = asm_42.split('\n')
    error = 0
    while i < max:
        if i < len(asm_42) and i < len(asm):
            if asm_42[i] != asm[i]:
                print(asm_42[i])
                print(asm[i])
                error += 1
        elif i < len(asm_42):
            print(asm_42[i])
            print("\033[1;31;40m null")
            error += 1
        elif i < len(asm):
            print(asm[i])
            print("\033[1;31;40m null")
            error += 1
        i += 1
    return (error)



def main():
    i = 0
    for arg in sys.argv:
        error = 0
        if i > 0:
            print("\n- - - - - - - - - - - - " + arg.upper() + " - - - - - - - - - - - -\n")
            asm_42 = get_file(ASM_42, arg).split('\n')
            asm = get_file(ASM, arg).split('\n')
            j = 0
            if len(asm_42) > len(asm):
                max = len(asm_42)
            else:
                max = len(asm)
            while j < max:
                error += compare(asm_42[j], asm[j])
                j += 1
            if error > 0:
                print("\033[1;31;40mDiff = KO (%d bad line(s))" % error)
            else:
                print("\033[1;32;40mDiff = OK")
        i += 1

if __name__== "__main__":
  main()