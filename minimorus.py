#!/usr/bin/env python

import random
import math
import sys

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

def minimorus_stateupdate(S, C=[], M=0):
    Si = list(S[-1][-1])
    C.append(M ^ Si[0] ^ Si[1] ^ (Si[2] & Si[3]))
    S.append([]);                                            #S[-1].append(list(Si))  # S^i_0,*
    Si[0] = rotl((Si[0] ^ (Si[1] & Si[2]) ^ Si[3]),      5); #S[-1].append(list(Si))  # S^i_1,*
    Si[1] = rotl((Si[1] ^ (Si[2] & Si[3]) ^ Si[4] ^ M), 31); #S[-1].append(list(Si))  # S^i_2,*
    Si[2] = rotl((Si[2] ^ (Si[3] & Si[4]) ^ Si[0] ^ M),  7); #S[-1].append(list(Si))  # S^i_3,*
    Si[3] = rotl((Si[3] ^ (Si[4] & Si[0]) ^ Si[1] ^ M), 22); #S[-1].append(list(Si))  # S^i_4,*
    Si[4] = rotl((Si[4] ^ (Si[0] & Si[1]) ^ Si[2] ^ M), 13); S[-1].append(list(Si))  # S^i_5,* = S^i+1_0,*

def minimorus_example():
    S = []
    C = []
    # S.append([[randword() for w in range(5)]]); printstates(S[0], "init")
    S.append([[1 for w in range(5)]]);
    printstates(S[0], "init")
    minimorus_stateupdate(S, C);
    printstates(S[-1], "S^1")
    minimorus_stateupdate(S, C);
    printstates(S[-1], "S^2")

minimorus_example()
sys.exit("Stop")

def minimorus_linearsample():
    S = []
    C = []
    S.append([[randword() for w in range(5)]])
    minimorus_stateupdate(S, C)
    minimorus_stateupdate(S, C)
    minimorus_stateupdate(S, C)
    minimorus_stateupdate(S, C)
    minimorus_stateupdate(S, C)
    bit = 0

    #return xor([mask(C[0], [27]),
    #            mask(C[1], [0,26]),
    #            mask(C[2], [31]),
    #            mask(S[1][-1][4], [0])]) # 2^-3

    # Approx 1 for S2
    #return xor([mask(C[0], [27]),
    #            mask(C[1], [0,26,8]),
    #            mask(C[2], [31,13,7]),
    #            mask(C[3], [12]),
    #            mask(S[2][-1][2], [0])]) # 2^-7

    # Approx 2 for S2
    # TESTED WITH 2^-22
    return xor([mask(C[1], [2]),
                mask(C[2], [1,7,15,27]),
                mask(C[3], [6,20,14]),
                mask(C[4], [19]),
                mask(S[2][-1][2], [0])]) # 2^-9

    # STEP A
    #return xor([mask(C[0], [27]),
    #            mask(S[1][-1][0], [0])])

    # STEP B
    #return xor([mask(C[1], [0]),
    #            mask(S[2][-1][0], [5])])

    # STEP C
    #return xor([mask(S[1][-1][0], [0]),
    #            mask(S[1][-1][3], [0]),
    #            mask(S[2][-1][0], [5])])

    # B+C - UNBIASED!!!
    #return xor([mask(C[1], [0]),
    #            mask(S[1][-1][0], [0]),
    #            mask(S[1][-1][3], [0])])

    # STEP D
    #return xor([mask(C[0], [27]),
    #            mask(C[1], [0]),
    #            mask(S[1][-1][3], [0])])

    # STEP D -- FIXED
    return xor([mask(C[0], [27]),
                mask(C[1], [0]),
                mask(S[1][-1][1], [0]),
                mask(S[1][-1][3], [0])]) # OPTIONAL

    # FINAL
    return xor([mask(C[0], [27]),
                mask(C[1], [0,17,27]),
                mask(C[2], [0,22])])

    # WRONG
    #return xor([mask(C[0], [bit]), # BUG?
    #            mask(S[1][-1][0], [bit, (bit+5)%32]),
    #            mask(S[1][-1][1], [bit]),
    #            mask(S[1][-1][3], [(bit-10)%32]) ])  # cost=2?

    # CORRECT and UNUSEABLE
    #return xor([mask(S[1][0][0], [bit]),
    #            mask(S[1][-1][0], [bit, (bit+5)%32]),
    #            mask(S[1][-1][1], [bit]),
    #            mask(S[1][-1][3], [(bit-10)%32]) ])  # cost=2?

    # RED
    return xor([mask(C[0], [bit]),
                mask(S[1][1][0], [(bit+5)%32])])  # cost=1

    # RED + WRONG
    #return xor([mask(C[0], [bit]),
    #            mask(S[1][1][0], [(bit+5)%32]),
    #            mask(S[2][-1][0], [(bit+5)%32, (bit+5+5)%32]),
    #            mask(S[2][-1][1], [(bit+5)%32]),
    #            mask(S[2][-1][3], [(bit-10+5)%32]) ])

    # CORRECT AND SLOW (RED LONG)
    return xor([mask(C[0], [bit]),
                mask(S[1][-1][0], [(bit+5)%32])])  # cost=1
    #return xor([mask(C[0], [bit]), # BUG?
    #            mask(S[1][-1][0], [bit, (bit+5)%32]),
    #            mask(S[1][-1][1], [bit]),
    #            mask(S[1][-1][3], [(bit-10)%32]) ])  # cost=2?

def minimorus_linearstats(num):
    imbalance2 = 0
    bias = 0
    for i in xrange(num):
        res = minimorus_linearsample()
        imbalance2 += (-1)**res  # imbalance: 1/2 (#0 - #1)
        bias += res
    probability = float(bias)/num
    correlation = imbalance2 / float(num)
    bias = (bias - num/2)/float(num)
    potential = correlation**2
    #print "prob", probability
    print("corr", correlation)
    #print "bias", bias
    #print "pote", potential, math.log(potential, 2)
    print("logD", math.log(bias**(-2), 2))
    biascost = math.fabs(math.log(2*math.fabs(bias), 2))  # math.copysign(1,bias)
    print("bias: +- 2^-", biascost)


    # p = 1/4
    # bias = 1/2 - p = (num/2 - bias)/num

if __name__ == "__main__":
    minimorus_linearstats(2**20)
