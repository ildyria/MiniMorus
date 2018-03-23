#!/usr/bin/env python3

import random
import math
import sys
from functools import reduce

width=64
approximations = dict( \
        alpha=dict(w=1, masks=lambda S, C, width: [mask(C[0], [27]),
                                                   mask(S[1][0], [0])]) if width==32 else \
              dict(w=1, masks=lambda S, C, width: [mask(C[0], [51]),
                                                   mask(S[1][0], [0])]),
        appr1=dict(w=7, masks=lambda S, C, width: [mask(C[0], [27]),
                                                   mask(C[1], [0,26,8]),
                                                   mask(C[2], [31,13,7]),
                                                   mask(C[3], [12]),
                                                   mask(S[2][2], [0])]) if width==32 else \
              dict(w=7, masks=lambda S, C, width: [mask(C[0], [51]),
                                                   mask(C[1], [0,33,55]),
                                                   mask(C[2], [4,37,46]),
                                                   mask(C[3], [50]),
                                                   mask(S[2][2], [0])]),
        appr2=dict(w=9, masks=lambda S, C, width: [mask(C[1], [2]),
                                                   mask(C[2], [1,7,15,27]),
                                                   mask(C[3], [6,20,14]),
                                                   mask(C[4], [19]),
                                                   mask(S[2][2], [0])]) if width==32 else \
              dict(w=9, masks=lambda S, C, width: [mask(C[1], [25]),
                                                   mask(C[2], [7,29,38,51]),
                                                   mask(C[3], [11,20,42]),
                                                   mask(C[4], [24]),
                                                   mask(S[2][2], [0])]),
        )
#           xor([mask(S[1][0], [0]),
#                mask(S[1][4], [0]),
#                mask(C[1], [0,8,26]),
#                mask(C[2], [13,31]),
#                # 2^-3 version
#                mask(S[2][4], [13]),
#                mask(S[3][1], [12]),
#                # 2^-4 version
#                #mask(S[2][1], [13]),
#                ]) # 2^-3
#
#    return xor([mask(S[1][1], [0]),
#                mask(C[1], [8,26]),
#                mask(C[2], [13,31]),
#                mask(S[2][4], [13]),
#                mask(S[1][4], [0]),
#                mask(S[3][1], [12]),
#                ]) # 2^-4
#
#
#
#    return xor([mask(C[0], [27]),
#                mask(C[1], [0,26]),
#                mask(C[2], [31]),
#                mask(S[1][4], [0])]) # 2^-3

#approximation = approximations['appr1']
approximation = approximations['appr2']

def randword(width=32):
    return random.randrange(0,2**width)

def rotl(word, rotations=1, width=32):
    ones = 2**width-1
    word &= ones
    return ((word << rotations) & ones) | (word >> (width - rotations))

def printstates(statelist, desc="", width=32):
    print(desc)
    for state in statelist:
        print("[" + " ".join([hex(word)[2:].zfill(width/4) for word in state]) + "]")

def xor(l):
    return reduce((lambda x, y: x ^ y), l, 0)

def mask(word, bits):
    return xor([(word & (1 << bit)) >> bit for bit in bits])

def minimorus_stateupdate(S, C=[], M=0, width=32):
    Si = list(S[-1])
    C.append(M ^ Si[0] ^ Si[1] ^ (Si[2] & Si[3]))
    if width == 32:
        Si[0] = rotl((Si[0] ^ (Si[1] & Si[2]) ^ Si[3]),      5, 32);
        Si[1] = rotl((Si[1] ^ (Si[2] & Si[3]) ^ Si[4] ^ M), 31, 32);
        Si[2] = rotl((Si[2] ^ (Si[3] & Si[4]) ^ Si[0] ^ M),  7, 32);
        Si[3] = rotl((Si[3] ^ (Si[4] & Si[0]) ^ Si[1] ^ M), 22, 32);
        Si[4] = rotl((Si[4] ^ (Si[0] & Si[1]) ^ Si[2] ^ M), 13, 32); S.append(list(Si))
    elif width == 64:
        Si[0] = rotl((Si[0] ^ (Si[1] & Si[2]) ^ Si[3]),     13, 64);
        Si[1] = rotl((Si[1] ^ (Si[2] & Si[3]) ^ Si[4] ^ M), 46, 64);
        Si[2] = rotl((Si[2] ^ (Si[3] & Si[4]) ^ Si[0] ^ M), 38, 64);
        Si[3] = rotl((Si[3] ^ (Si[4] & Si[0]) ^ Si[1] ^ M),  7, 64);
        Si[4] = rotl((Si[4] ^ (Si[0] & Si[1]) ^ Si[2] ^ M),  4, 64); S.append(list(Si))

def minimorus_linearsample(masks, width=32):
    S = []
    C = []
    S.append([randword(width) for w in range(5)])
    minimorus_stateupdate(S, C, 0, width)
    minimorus_stateupdate(S, C, 0, width)
    minimorus_stateupdate(S, C, 0, width)
    minimorus_stateupdate(S, C, 0, width)
    minimorus_stateupdate(S, C, 0, width)
    bit = 0

    return xor(masks(S, C, width))

def minimorus_linearstats(approximation, width):
    expweight = approximation['w']
    masks = approximation['masks']
    num = 2**(2*expweight+5)

    count = 0
    imbalance = 0
    for i in range(num):
        res = minimorus_linearsample(masks, width)
        count += res
        imbalance += (-1)**res  # imbalance: 1/2 (#0 - #1)
    bias = (count - num/2)/float(num)
    correlation = imbalance / float(num)
    weight = math.fabs(math.log(math.fabs(correlation), 2))
    #print ("correl", correlation)
    print ("exp.w.", expweight)
    print ("weight", weight)
    print ("attack", 2*weight + 2)

if __name__ == "__main__":
    minimorus_linearstats(approximation, width)
