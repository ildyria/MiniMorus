#!/usr/bin/env python3

import random
import math
# import sys
from functools import reduce

def randword():
    return random.randrange(0,2**32)

def rotl(word, rotations=1, width=32):
    ones = 2**width-1
    word &= ones
    return ((word << rotations) & ones) | (word >> (width - rotations))

def printstates(statelist, desc=""):
    print(desc)
    for state in statelist:
        print("[" + " ".join([hex(word)[2:].zfill(8) for word in state]) + "]")

def xor(l):
    return reduce((lambda x, y: x ^ y), l, 0)

def mask(word, bits):
    return xor([(word & (1 << bit)) >> bit for bit in bits])

def minimorus_stateupdate(S, C=[], M=0, wordsize=32):
    Si = list(S[-1])
    C.append(M ^ Si[0] ^ Si[1] ^ (Si[2] & Si[3]))
    if wordsize == 32:
        Si[0] = rotl((Si[0] ^ (Si[1] & Si[2]) ^ Si[3]),      5);
        Si[1] = rotl((Si[1] ^ (Si[2] & Si[3]) ^ Si[4] ^ M), 31);
        Si[2] = rotl((Si[2] ^ (Si[3] & Si[4]) ^ Si[0] ^ M),  7);
        Si[3] = rotl((Si[3] ^ (Si[4] & Si[0]) ^ Si[1] ^ M), 22);
        Si[4] = rotl((Si[4] ^ (Si[0] & Si[1]) ^ Si[2] ^ M), 13); S.append(list(Si))
    else:
        pass

def minimorus_example():
    S = []
    C = []
    S.append([randword() for w in range(5)]); printstates(S[0], "init")
    minimorus_stateupdate(S, C);              printstates(S[-1], "S^1")
    minimorus_stateupdate(S, C);              printstates(S[-1], "S^2")

def minimorus_linearsample():
    S = []
    C = []
    S.append([randword() for w in range(5)])
    minimorus_stateupdate(S, C)
    minimorus_stateupdate(S, C)
    minimorus_stateupdate(S, C)
    minimorus_stateupdate(S, C)
    minimorus_stateupdate(S, C)
    bit = 0

    return xor([mask(S[1][0], [0]),
                mask(S[1][4], [0]),
                mask(C[1], [0,8,26]),
                mask(C[2], [13,31]),
                # 2^-3 version
                mask(S[2][4], [13]),
                mask(S[3][1], [12]),
                # 2^-4 version
                #mask(S[2][1], [13]),
                ]) # 2^-3

    return xor([mask(S[1][1], [0]),
                mask(C[1], [8,26]),
                mask(C[2], [13,31]),
                mask(S[2][4], [13]),
                mask(S[1][4], [0]),
                mask(S[3][1], [12]),
                ]) # 2^-4



    return xor([mask(C[0], [27]),
                mask(C[1], [0,26]),
                mask(C[2], [31]),
                mask(S[1][4], [0])]) # 2^-3

    # Approx 1 for S2
    #return xor([mask(C[0], [27]),
    #            mask(C[1], [0,26,8]),
    #            mask(C[2], [31,13,7]),
    #            mask(C[3], [12]),
    #            mask(S[2][2], [0])]) # 2^-7

    # Approx 2 for S2
    # TESTED WITH 2^-22
    #return xor([mask(C[1], [2]),
    #            mask(C[2], [1,7,15,27]),
    #            mask(C[3], [6,20,14]),
    #            mask(C[4], [19]),
    #            mask(S[2][2], [0])]) # 2^-9

def minimorus_linearstats(num):
    count = 0
    imbalance2 = 0
    for i in range(num):
        res = minimorus_linearsample()
        count += res
        imbalance2 += (-1)**res  # imbalance: 1/2 (#0 - #1)
    bias = (count - num/2)/float(num)
    correlation = imbalance2 / float(num)
    weight = math.fabs(math.log(math.fabs(correlation), 2))
    print ("correl", correlation)
    print ("weight", weight)
    print ("logDat", 2*weight + 2)

if __name__ == "__main__":
    minimorus_linearstats(2**16)
