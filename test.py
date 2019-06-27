#!/usr/bin/python3

import sys
import os
import shutil
import fnmatch
import glob
import subprocess as sp


REPO = "../"
OBJECTS_PATH = "objects"
LIB_PATH = "libft"
FILE_NAME_ASM = "*"
LOG_PATH = "error/"
LOG_PATH_ASM = LOG_PATH + "asm/"
ASM_OPTIONS = ['-x', '-xl', '-b', '-bl', '-o test.cor', '-d', '']
ASM_GLOBAL_OPTIONS = ['--color=f', '--color=a', '--color=forcea', '--color=al', '--color=n', '--color=aut']
ASM_DEFAULT_CHAMP = "zork.s"
COREWAR_SOURCE_CHAMP = "zork.s"
FILE_NAME_COREWAR = "*.cor"
LOG_PATH_COREWAR = LOG_PATH + "corewar/"
COREWAR_OPTIONS = ['-verbose 0', '-verbose 1', '-verbose 2', '-verbose 3', '-verbose 4', '-dump 32', '-dump 4324324', '']
COREWAR_GLOBAL_OPTIONS = ['-n 3', '-n 2334322343432', '-n -1', '-dump 0']
COREWAR_DEFAULT_CHAMP = "zork.cor"
LOG_FILE = "fsanitize.log"
TIMEOUT = 10

def progress(count, total, status=''):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))
    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)
    if count > 1:
        print("\033[2K", end='')
    print('\r' + '[' + bar + ']' +  str(percents) + '% ' + status)

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
        i = 0
        for file in file_list:
            for opt in options:
                latest_error = check_run(program, opt, latest_error, file, log_path)
                i += 1
                progress(i, percent, './' + program + ' ' + opt + ' ' + file)
        for opt in global_options:
            latest_error = check_run(program, opt, latest_error, file, log_path)
            i += 1
            progress(i, percent, './' + program + ' ' + opt + ' ' + file)


def get_files(directory, pattern):
    file_list = []
    for d_name, sd_name, f_list in os.walk(directory):
        for filename in f_list:
            if fnmatch.fnmatch(filename, pattern):
                file_list.append(os.path.join(d_name, filename))
    return file_list


def remove_if_exist(path):
    if os.path.exists(path):
        if os.path.isdir(path):
            shutil.rmtree(path)
        else:
            os.remove(path)


def init_directory():
    remove_if_exist('tmp')
    shutil.copytree(sys.argv[1], 'tmp')
    if not os.path.exists(LOG_PATH_ASM):
        os.makedirs(LOG_PATH_ASM)
    if os.path.exists('invalid_cor') and not os.path.isdir('invalid_cor'):
        os.remove('invalid_cor')
    if not os.path.exists('invalid_cor'):
        os.makedirs('invalid_cor')
    file_list = get_files(sys.argv[1], '*.cor')
    for file in file_list:
        shutil.copyfile(file, 'invalid_cor/')


def init_executable():
    remove_if_exist('error')
    os.makedirs(LOG_PATH_ASM)
    os.makedirs(LOG_PATH_COREWAR)
    file_list = get_files(OBJECTS_PATH, '*.o')
    if LIB_PATH:
        file_list.extend(get_files(LIB_PATH, '*.o'))
    for file in file_list:
        if not sp.getoutput('nm -u ' + file + ' | grep asan').strip():
            remove_if_exist(file)
    os.system('make -C ' + REPO + ' silent=y CFLAGS="-fsanitize=address,undefined,integer,bounds" > /dev/null 2>&1')
    if not os.path.exists(REPO + 'asm') or not os.path.exists(REPO + 'corewar'):
        print("Program not find")
        exit(1)
    shutil.copyfile(REPO + 'asm', 'asm')
    shutil.copyfile(REPO + 'corewar', 'corewar')


def restore_directory(champs):
    if os.path.exists('tmp'):
        shutil.rmtree("tmp")
    remove_if_exist('invalid_cor')
    remove_if_exist(champs)
    os.rename('tmp', champs)


def asm_test(champs):
    file_list = get_files(champs, FILE_NAME_ASM)
    test_program('asm', file_list, ASM_OPTIONS, ASM_GLOBAL_OPTIONS, LOG_PATH_ASM)


def corewar_test(champs):
    os.system('./asm ' + COREWAR_DEFAULT_CHAMP + ' ' + COREWAR_SOURCE_CHAMP + ' > /dev/null')
    shutil.move(os.path.splitext(COREWAR_DEFAULT_CHAMP)[0] + ".cor", COREWAR_SOURCE_CHAMP)
    if os.path.exists(COREWAR_DEFAULT_CHAMP):
        file_list = get_files(sys.argv[1], FILE_NAME_COREWAR)
        i = len(COREWAR_OPTIONS)
        while i >= 0:
            i -= 1
            COREWAR_OPTIONS[i] =  COREWAR_OPTIONS[i] + ' ' + COREWAR_DEFAULT_CHAMP
        while i >= 0:
            i -= 1
            COREWAR_GLOBAL_OPTIONS[i] = COREWAR_GLOBAL_OPTIONS[i] + ' ' + COREWAR_DEFAULT_CHAMP
        test_program('corewar', file_list, COREWAR_OPTIONS, COREWAR_GLOBAL_OPTIONS, LOG_PATH_COREWAR)


def main():
    if len(sys.argv) >= 2 and os.path.exists(sys.argv[1]):
        init_directory()
        print("init directory")
        os.system('ls -R')
        init_executable()
        print("executable")
        os.system('ls -R')
        asm_test(sys.argv[1])
        print("asm")
        os.system('ls -R')
        corewar_test(sys.argv[1])
        print("corewar")
        os.system('ls -R')
        restore_directory(sys.argv[1])
        print("restore directory")
        os.system('ls -R')
    else:
        print("Usage: " + sys.argv[0] + ' <directory>')


main()
