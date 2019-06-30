#!/usr/bin/python3

import sys
import os
import shutil
import fnmatch
import glob
import subprocess as sp
import argparse


# Compile corewar and libft with -fsanitize=address,undefined,integer,bounds

LOG_PATH = "error/"
LOG_FILE = "fsanitize.log"
TIMEOUT = 10

FILE_NAME_ASM = "*"
LOG_PATH_ASM = LOG_PATH + "asm/"
ASM_OPTIONS = ['-x', '-xl', '-b', '-bl', '-o test.cor', '-d', '']
ASM_GLOBAL_OPTIONS = ['--color=f', '--color=a', '--color=forcea', '--color=al', '--color=n', '--color=aut']

FILE_NAME_COREWAR = "*.cor"
LOG_PATH_COREWAR = LOG_PATH + "corewar/"
COREWAR_OPTIONS = ['-verbose 0', '-verbose 1', '-verbose 2', '-verbose 3', '-verbose 4', '-dump 32', '-dump 4324324', '']
COREWAR_GLOBAL_OPTIONS = ['-n 3', '-n 2334322343432', '-n -1', '-dump 0', '$(find . -type f -name "*.cor")']

def progress(count, total, status=''):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))
    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)
    if count > 1:
        print("\033[2K", end='')
    print('\r' + '[' + bar + '] ' +  str(percents) + '% ' + status, end="")

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
            latest_error = check_run(program, opt, latest_error, '', log_path)
            i += 1
            progress(i, percent, './' + program + ' ' + opt + ' ' + '')
    print('')


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


def init_directory(args):
    remove_if_exist('tmp')
    shutil.copytree(args.champs, 'tmp')
    args.champs = 'tmp'
    if not os.path.exists(LOG_PATH_ASM):
        os.makedirs(LOG_PATH_ASM)
    if os.path.exists('invalid_cor') and not os.path.isdir('invalid_cor'):
        os.remove('invalid_cor')
    if not os.path.exists('invalid_cor'):
        os.makedirs('invalid_cor')
    file_list = get_files(args.champs, '*.cor')
    for file in file_list:
        shutil.copy(file, 'invalid_cor/')
    remove_if_exist('error')
    os.makedirs(LOG_PATH_ASM)
    os.makedirs(LOG_PATH_COREWAR)
    if not os.path.exists(args.repo + 'asm') or not os.path.exists(args.repo + 'corewar'):
        print("Program not find")
        exit(1)
    shutil.copy(args.repo + 'asm', 'asm')
    shutil.copy(args.repo + 'corewar', 'corewar')


def restore_directory(champs):
    remove_if_exist('invalid_cor')
    remove_if_exist(champs)


def asm_test(args):
    i = len(ASM_GLOBAL_OPTIONS)
    while i > 0:
        i -= 1
        ASM_GLOBAL_OPTIONS[i] = ASM_GLOBAL_OPTIONS[i] + ' ' + args.asm_default_champ
    file_list = get_files(args.champs, FILE_NAME_ASM)
    test_program('asm', file_list, ASM_OPTIONS, ASM_GLOBAL_OPTIONS, LOG_PATH_ASM)


def corewar_test(args):
    os.system('./asm ' + args.corewar_default_champ + ' > /dev/null')
    default_cor = os.path.splitext(args.corewar_default_champ)[0] + ".cor"
    if os.path.exists(default_cor):
        file_list = get_files(args.champs, FILE_NAME_COREWAR)
        i = len(COREWAR_OPTIONS)
        while i > 0:
            i -= 1
            COREWAR_OPTIONS[i] =  COREWAR_OPTIONS[i] + ' ' + default_cor
        i = len(COREWAR_GLOBAL_OPTIONS)
        while i > 0:
            i -= 1
            COREWAR_GLOBAL_OPTIONS[i] = COREWAR_GLOBAL_OPTIONS[i] + ' ' + default_cor + ' ' + default_cor
        test_program('corewar', file_list, COREWAR_OPTIONS, COREWAR_GLOBAL_OPTIONS, LOG_PATH_COREWAR)

def is_valid_file(parser, arg, dir):
    if not os.path.exists(arg):
        parser.error("The file %s does not exist" % arg)
    elif os.path.isdir(arg) and dir is False:
        parser.error("The file %s cannot be a directory" % arg)
    elif not os.path.isdir(arg) and dir is True:
        parser.error("The file %s must be a directory" % arg)
    return arg

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("repo", help="repo directory", type=lambda x: is_valid_file(parser, x, True))
    parser.add_argument("champs", help="directory containing champions", type=lambda x: is_valid_file(parser, x, True))
    parser.add_argument("asm_default_champ", help="default champion for asm", type=lambda x: is_valid_file(parser, x, False))
    parser.add_argument("corewar_default_champ", help="default champion for corewar", type=lambda x: is_valid_file(parser, x, False))
    args = parser.parse_args()
    init_directory(args)
    asm_test(args)
    corewar_test(args)
    restore_directory(args.champs)

main()
