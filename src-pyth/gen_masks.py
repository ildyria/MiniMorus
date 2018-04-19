#!/usr/bin/env python3

from tools import save, gen_name, bcolors
from masks import masks_list

def header_head():
    z = ''
    z += '#pragma once\n'
    z += '\n'
    z += '#include "../Utilities/utilities.h"\n'
    z += '\n'
    return z

def main_head(name):
    z = '#include "' + name + '.h"\n'
    z += '\n'
    return z

def gen_mask(list_index):
    mask = 0;
    for idx in list_index:
        mask ^= 1 << idx
    return mask

def gen_union(kind,width):
    union = ''
    if kind == 1:
        union = 'm' + str(width)
    if kind == 2:
        union = 'f' + str(width)
    return union


def gen_function(kind,width,num,states,ciphers, weight):

    union = gen_union(kind,width)
    name = gen_name(kind, width)
    name += '_' + str(num)

    z = '\n'
    z += 'uint64_t ' + name + '(state *saved_states, union Register *saved_ciphers) {\n'
    z += '\tuint' + str(width) + '_t res = 0;\n'

    for K, C in ciphers.items():
        z += '\tres ^= saved_ciphers' + K + '.' + union + ' & ' + hex(gen_mask(C)) + ';\n'

    for K, C in states.items():
         z += '\tres ^= saved_states' + K + '.' + union + ' & ' + hex(gen_mask(C)) + ';\n'

    z += '\treturn 1 & HW' + str(width) + '(res);\n'

    z += '}\n';

    z += 'uint32_t ' + name + '_weight() {\n'
    z += '\treturn ' + str(weight) + ';\n';
    z += '}\n';
    z += '\n';

    return z

def gen_function_sig(kind,width,num):
    union = gen_union(kind,width)
    name = gen_name(kind, width)
    name += '_' + str(num)

    z = '\n'
    z += 'uint64_t ' + name + '(state *saved_states, union Register *saved_ciphers);\n'
    z += 'uint32_t ' + name + '_weight();\n'
    return z;


if __name__ == "__main__":

    for i in range(len(masks_list)):
        file_name = 'masks_' + gen_name(masks_list[i]['kind'],masks_list[i]['width'])
        H = ''
        H += header_head()
        Z = ''
        Z += main_head(file_name)

        for j in range(len(masks_list[i]['ciphers'])):
            Z += gen_function(masks_list[i]['kind'],
                              masks_list[i]['width'],
                              (j+1),
                              masks_list[i]['states'][j],
                              masks_list[i]['ciphers'][j],
                              masks_list[i]['weight'][j])
            H += gen_function_sig(masks_list[i]['kind'],masks_list[i]['width'],(j+1))

        print(bcolors.YELLOW + "Generate " + file_name + ".[h|c] "+ bcolors.ENDC)
        save('Masks/' + file_name + '.h', H);
        print(bcolors.MAGENTA + H + bcolors.ENDC)
        save('Masks/' + file_name + '.c', Z);
        print(bcolors.PURPLE + Z  + bcolors.ENDC)
