#!/bin/bash

import sys
import os
import shutil
import fnmatch
import glob
import subprocess as sp


FILE_NAME_ASM = "*"
LOG_PATH_ASM = "error/asm/"
ASM_OPTIONS = ['-x', '-xl', '-b', '-bl', '-o test.cor', '-d', '']
ASM_GLOBAL_OPTIONS = ['--color=f', '--color=a', '--color=forcea', '--color=al', '--color=n', '--color=aut']
ASM_DEFAULT_CHAMP = "resources/champs/basics/zork.s"
COREWAR_SOURCE_CHAMP = "resources/champs/basics/zork.s"
FILE_NAME_COREWAR = "*.cor"
LOG_PATH_COREWAR = "error/corewar/"
COREWAR_OPTIONS = ['-verbose 0', '-verbose 1', '-verbose 2', '-verbose 3', '-verbose 4', '-dump 32', '-dump 4324324', '']
COREWAR_GLOBAL_OPTIONS = ['-n 3', '-n 2334322343432', '-n -1', '-dump 0']
COREWAR_DEFAULT_CHAMP = "zork.cor"
LOG_FILE = "fsanitize.log"
TIMEOUT = 10

def check_run(program, opt, latest_error, file, log_path):
    os.system('timeout ' + str(TIMEOUT) + ' ./' + program + ' ' + opt + ' ' + file + ' > /dev/null 2>&1')
    new_error = ''
    if os.path.exists(log_path):
        files_error = glob.glob(log_path + '*')
    if len(files_error):
        new_error =  max(files_error, key=os.path.getctime)
    if new_error != latest_error:
        with open(new_error, "r+") as f:
            old_text = f.read()
        with open(new_error, "w+") as f:
            f.write('./' + program + ' ' + opt + ' ' + file + '\n' + old_text)
        latest_error = new_error
    return latest_error


def test_program(program, file_list, options, global_options, log_path):
    os.environ["ASAN_OPTIONS"] = "log_path=" + log_path + LOG_FILE + " color=always"
    latest_error = ''
    percent = len(file_list) * len(options) + len(global_options)
    if percent:
        print("0%", end='')
        i = 0
        for file in file_list:
            for opt in options:
                latest_error = check_run(program, opt, latest_error, file, log_path)
                i += 1
                print("\033[2K\r" + program + ": " + str(round(i * 100 / percent, 1)) + "%", end='')
        for opt in global_options:
            latest_error = check_run(program, opt, latest_error, file, log_path)
            i += 1
            print("\033[2K\r" + program + ": " + str(round(i * 100 / percent, 1)) + "%", end='')
        print('')

def get_files(directory, pattern):
    file_list = []
    for d_name, sd_name, f_list in os.walk(directory):
        for filename in f_list:
            if fnmatch.fnmatch(filename, pattern):
                file_list.append(os.path.join(d_name, filename))
    return file_list


def main():
    if len(sys.argv) >= 2:
        file_list = get_files('objects', '*.o')
        file_list.extend(get_files('libft', '*.o'))
        for file in file_list:
            if not sp.getoutput('nm -u ' + file + ' | grep asan').strip():
                os.remove(file)
        os.system('make silent=y fsanitize=y > /dev/null 2>&1')
        if not os.path.exists('asm') or not os.path.exists('corewar'):
            exit(1)
        if os.path.exists('error') and os.path.isdir('error'):
            shutil.rmtree("error")
        if not os.path.exists(LOG_PATH_ASM):
            os.makedirs(LOG_PATH_ASM)
        if not os.path.exists(LOG_PATH_COREWAR):
            os.makedirs(LOG_PATH_COREWAR)
        file_list = get_files(sys.argv[1], FILE_NAME_ASM)
        test_program('asm', file_list, ASM_OPTIONS, ASM_GLOBAL_OPTIONS, LOG_PATH_ASM)
        os.system('./asm -o ' + COREWAR_DEFAULT_CHAMP + ' ' + COREWAR_SOURCE_CHAMP + ' > /dev/null')
        os.system('git checkout @ resources/champs/invalid_cor')
        if os.path.exists(COREWAR_DEFAULT_CHAMP):
            file_list = get_files(sys.argv[1], FILE_NAME_COREWAR)
            i = len(COREWAR_OPTIONS)
            while i >= 0:
                i -= 1
                COREWAR_OPTIONS[i] = COREWAR_DEFAULT_CHAMP + ' ' + COREWAR_OPTIONS[i]
            while i >= 0:
                i -= 1
                COREWAR_GLOBAL_OPTIONS[i] = COREWAR_DEFAULT_CHAMP + ' ' + COREWAR_GLOBAL_OPTIONS[i]
            test_program('corewar', file_list, COREWAR_OPTIONS, COREWAR_GLOBAL_OPTIONS, LOG_PATH_COREWAR)
        os.system('make clean-test > /dev/null 2>&1')


main()
