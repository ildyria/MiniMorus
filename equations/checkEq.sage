'''
This script takes as input an equation eq (represented as a string, in the format output by eqs.sage), and checks if it holds across nTrials = 30 trial runs.
The version of MORUS that is run is determined by the 'large' (0 for MORUS640, 1 for MORUS1280) and 'nWords' (1 for mini version, 4 for real version) parameters.
'''

import random

eq = "S_0_1_27*S_0_2_27 + S_0_1_59*S_0_2_59 + S_0_1_91*S_0_2_91 + S_0_1_123*S_0_2_123 + S_0_2_27*S_0_3_27 + S_0_2_59*S_0_3_59 + S_0_2_91*S_0_3_91 + S_0_2_123*S_0_3_123 + S_1_1_2*S_1_2_2 + S_1_1_8*S_1_2_8 + S_1_1_26*S_1_2_26 + S_1_1_34*S_1_2_34 + S_1_1_40*S_1_2_40 + S_1_1_58*S_1_2_58 + S_1_1_66*S_1_2_66 + S_1_1_72*S_1_2_72 + S_1_1_90*S_1_2_90 + S_1_1_98*S_1_2_98 + S_1_1_104*S_1_2_104 + S_1_1_122*S_1_2_122 + S_1_2_0*S_1_3_0 + S_1_2_32*S_1_3_0 + S_1_2_2*S_1_3_2 + S_1_2_8*S_1_3_8 + S_1_2_26*S_1_3_26 + S_1_2_32*S_1_3_32 + S_1_2_64*S_1_3_32 + S_1_2_34*S_1_3_34 + S_1_2_40*S_1_3_40 + S_1_2_58*S_1_3_58 + S_1_2_64*S_1_3_64 + S_1_2_96*S_1_3_64 + S_1_2_66*S_1_3_66 + S_1_2_72*S_1_3_72 + S_1_2_90*S_1_3_90 + S_1_2_0*S_1_3_96 + S_1_2_96*S_1_3_96 + S_1_2_98*S_1_3_98 + S_1_2_104*S_1_3_104 + S_1_2_122*S_1_3_122 + S_2_0_0*S_2_1_0 + S_2_0_32*S_2_1_32 + S_2_0_64*S_2_1_64 + S_2_0_96*S_2_1_96 + S_2_1_1*S_2_2_1 + S_2_1_7*S_2_2_7 + S_2_1_15*S_2_2_15 + S_2_1_27*S_2_2_27 + S_2_1_33*S_2_2_33 + S_2_1_39*S_2_2_39 + S_2_1_47*S_2_2_47 + S_2_1_59*S_2_2_59 + S_2_1_65*S_2_2_65 + S_2_1_71*S_2_2_71 + S_2_1_79*S_2_2_79 + S_2_1_91*S_2_2_91 + S_2_1_97*S_2_2_97 + S_2_1_103*S_2_2_103 + S_2_1_111*S_2_2_111 + S_2_1_123*S_2_2_123 + S_2_2_1*S_2_3_1 + S_2_2_39*S_2_3_7 + S_2_2_13*S_2_3_13 + S_2_2_45*S_2_3_13 + S_2_2_15*S_2_3_15 + S_2_2_27*S_2_3_27 + S_2_2_31*S_2_3_31 + S_2_2_33*S_2_3_33 + S_2_2_71*S_2_3_39 + S_2_2_45*S_2_3_45 + S_2_2_77*S_2_3_45 + S_2_2_47*S_2_3_47 + S_2_2_59*S_2_3_59 + S_2_2_63*S_2_3_63 + S_2_2_65*S_2_3_65 + S_2_2_103*S_2_3_71 + S_2_2_77*S_2_3_77 + S_2_2_109*S_2_3_77 + S_2_2_79*S_2_3_79 + S_2_2_91*S_2_3_91 + S_2_2_95*S_2_3_95 + S_2_2_97*S_2_3_97 + S_2_2_7*S_2_3_103 + S_2_2_13*S_2_3_109 + S_2_2_109*S_2_3_109 + S_2_2_111*S_2_3_111 + S_2_2_123*S_2_3_123 + S_2_2_127*S_2_3_127 + S_2_3_32*S_2_4_0 + S_2_3_64*S_2_4_32 + S_2_3_96*S_2_4_64 + S_2_3_0*S_2_4_96 + S_3_0_7*S_3_1_7 + S_3_0_39*S_3_1_39 + S_3_0_71*S_3_1_71 + S_3_0_103*S_3_1_103 + S_3_1_14*S_3_2_14 + S_3_1_46*S_3_2_46 + S_3_1_78*S_3_2_78 + S_3_1_110*S_3_2_110 + S_3_2_6*S_3_3_6 + S_3_2_12*S_3_3_12 + S_3_2_14*S_3_3_14 + S_3_2_20*S_3_3_20 + S_3_2_52*S_3_3_20 + S_3_2_38*S_3_3_38 + S_3_2_44*S_3_3_44 + S_3_2_46*S_3_3_46 + S_3_2_52*S_3_3_52 + S_3_2_84*S_3_3_52 + S_3_2_70*S_3_3_70 + S_3_2_76*S_3_3_76 + S_3_2_78*S_3_3_78 + S_3_2_84*S_3_3_84 + S_3_2_116*S_3_3_84 + S_3_2_102*S_3_3_102 + S_3_2_108*S_3_3_108 + S_3_2_110*S_3_3_110 + S_3_2_20*S_3_3_116 + S_3_2_116*S_3_3_116 + S_4_2_19*S_4_3_19 + S_4_2_51*S_4_3_51 + S_4_2_83*S_4_3_83 + S_4_2_115*S_4_3_115 + C_0_27 + C_0_59 + C_0_91 + C_0_123 + C_1_0 + C_1_2 + C_1_8 + C_1_26 + C_1_32 + C_1_34 + C_1_40 + C_1_58 + C_1_64 + C_1_66 + C_1_72 + C_1_90 + C_1_96 + C_1_98 + C_1_104 + C_1_122 + C_2_1 + C_2_13 + C_2_15 + C_2_27 + C_2_31 + C_2_33 + C_2_45 + C_2_47 + C_2_59 + C_2_63 + C_2_65 + C_2_77 + C_2_79 + C_2_91 + C_2_95 + C_2_97 + C_2_109 + C_2_111 + C_2_123 + C_2_127 + C_3_6 + C_3_12 + C_3_14 + C_3_20 + C_3_38 + C_3_44 + C_3_46 + C_3_52 + C_3_70 + C_3_76 + C_3_78 + C_3_84 + C_3_102 + C_3_108 + C_3_110 + C_3_116 + C_4_19 + C_4_51 + C_4_83 + C_4_115 + S_0_1_27 + S_0_1_59 + S_0_1_91 + S_0_1_123 + S_0_3_27 + S_0_3_59 + S_0_3_91 + S_0_3_123 + S_1_1_2 + S_1_1_8 + S_1_1_26 + S_1_1_34 + S_1_1_40 + S_1_1_58 + S_1_1_66 + S_1_1_72 + S_1_1_90 + S_1_1_98 + S_1_1_104 + S_1_1_122 + S_1_3_2 + S_1_3_8 + S_1_3_26 + S_1_3_34 + S_1_3_40 + S_1_3_58 + S_1_3_66 + S_1_3_72 + S_1_3_90 + S_1_3_98 + S_1_3_104 + S_1_3_122 + S_2_1_1 + S_2_1_7 + S_2_1_15 + S_2_1_27 + S_2_1_33 + S_2_1_39 + S_2_1_47 + S_2_1_59 + S_2_1_65 + S_2_1_71 + S_2_1_79 + S_2_1_91 + S_2_1_97 + S_2_1_103 + S_2_1_111 + S_2_1_123 + S_2_3_1 + S_2_3_7 + S_2_3_15 + S_2_3_27 + S_2_3_33 + S_2_3_39 + S_2_3_47 + S_2_3_59 + S_2_3_65 + S_2_3_71 + S_2_3_79 + S_2_3_91 + S_2_3_97 + S_2_3_103 + S_2_3_111 + S_2_3_123 + S_3_1_14 + S_3_1_46 + S_3_1_78 + S_3_1_110 + S_3_3_14 + S_3_3_46 + S_3_3_78 + S_3_3_110"

large = 0
#0 means use MORUS-640 rotation constants & word size, 1 means use MORUS-1280's

nWords = 4
#nWords = 1 for mini MORUS, 4 for the full MORUS

wordSize = [32,64][large]

size = wordSize * nWords
#size of a register

nRounds = 5
nTrials = 30

#rotation constants
b = [[5,31,7,22,13], [13,46,38,7,4]][large]
w = [[32,64,96,64,32], [64,128,192,128,64]][large]

c = []
s = []

def rotl(si, x):
    return ((si << x) | (si >> (size - x))) & ((1<<size) - 1)

def rotl_x_y(si, x):
    mask = (1<<wordSize) - 1
    word = [si>>(wordSize*i) & mask for i in range(nWords)]
    res = 0
    for i in range(nWords):
        word[i] = ((word[i] << x) | (word[i] >> (wordSize - x))) & mask
        res |= word[i] << (wordSize*i)
    return res

def update():
    global c, s
    si = copy(s[-1])
    
    si[0] = rotl_x_y(si[0] ^^ (si[1] & si[2]) ^^ si[3], b[0])
    si[3] = rotl(si[3], w[0])
    
    si[1] = rotl_x_y(si[1] ^^ (si[2] & si[3]) ^^ si[4], b[1])
    si[4] = rotl(si[4], w[1])

    si[2] = rotl_x_y(si[2] ^^ (si[3] & si[4]) ^^ si[0], b[2])
    si[0] = rotl(si[0], w[2])

    si[3] = rotl_x_y(si[3] ^^ (si[4] & si[0]) ^^ si[1], b[3])
    si[1] = rotl(si[1], w[3])

    si[4] = rotl_x_y(si[4] ^^ (si[0] & si[1]) ^^ si[2], b[4])
    si[2] = rotl(si[2], w[4])

    c.append(si[0] ^^ rotl(si[1],w[2]) ^^ (si[2] & si[3]))
    s.append(si)

def ref_rotl(x,n):
    return (((x) << (n)) | ((x) >> (wordSize-n))) & ((1<<wordSize) - 1)

def ref_update(si):
    mask = (1 << wordSize) - 1
    state = [[((si[j]>>(wordSize*i)) & mask) for i in range(nWords)] for j in range(5)]
    msgblk = [0]*4
    
    state[0][0] ^^= state[3][0]; state[0][1] ^^= state[3][1]; state[0][2] ^^= state[3][2]; state[0][3] ^^= state[3][3];
    state[0][0] ^^= state[1][0] & state[2][0]; state[0][1] ^^= state[1][1] & state[2][1]; state[0][2] ^^= state[1][2] & state[2][2]; state[0][3] ^^= state[1][3] & state[2][3];
    state[0][0] = ref_rotl(state[0][0], b[0]);  state[0][1] = ref_rotl(state[0][1], b[0]);       state[0][2] = ref_rotl(state[0][2], b[0]);       state[0][3] = ref_rotl(state[0][3], b[0]);
    temp = state[3][3];    state[3][3] = state[3][2];  state[3][2] = state[3][1];  state[3][1] = state[3][0];  state[3][0] = temp;
    
    state[1][0] ^^= msgblk[0];   state[1][1] ^^= msgblk[1];   state[1][2] ^^= msgblk[2];   state[1][3] ^^= msgblk[3];
    state[1][0] ^^= state[4][0]; state[1][1] ^^= state[4][1]; state[1][2] ^^= state[4][2]; state[1][3] ^^= state[4][3];
    state[1][0] ^^= (state[2][0] & state[3][0]); state[1][1] ^^= (state[2][1] & state[3][1]); state[1][2] ^^= (state[2][2] & state[3][2]); state[1][3] ^^= (state[2][3] & state[3][3]);
    state[1][0] = ref_rotl(state[1][0], b[1]);  state[1][1] = ref_rotl(state[1][1], b[1]);       state[1][2] = ref_rotl(state[1][2], b[1]);       state[1][3] = ref_rotl(state[1][3], b[1]);
    temp = state[4][3];    state[4][3] = state[4][1];  state[4][1] = temp;
    temp = state[4][2];    state[4][2] = state[4][0];  state[4][0] = temp;
    
    state[2][0] ^^= msgblk[0];   state[2][1] ^^= msgblk[1];   state[2][2] ^^= msgblk[2];   state[2][3] ^^= msgblk[3];
    state[2][0] ^^= state[0][0]; state[2][1] ^^= state[0][1]; state[2][2] ^^= state[0][2]; state[2][3] ^^= state[0][3];
    state[2][0] ^^= state[3][0] & state[4][0]; state[2][1] ^^= state[3][1] & state[4][1]; state[2][2] ^^= state[3][2] & state[4][2]; state[2][3] ^^= state[3][3] & state[4][3];
    state[2][0] = ref_rotl(state[2][0], b[2]);  state[2][1] = ref_rotl(state[2][1], b[2]);       state[2][2] = ref_rotl(state[2][2], b[2]);       state[2][3] = ref_rotl(state[2][3], b[2]);
    temp = state[0][0];    state[0][0] = state[0][1];  state[0][1] = state[0][2];  state[0][2] = state[0][3];  state[0][3] = temp;
    
    state[3][0] ^^= msgblk[0];   state[3][1] ^^= msgblk[1];   state[3][2] ^^= msgblk[2];   state[3][3] ^^= msgblk[3];
    state[3][0] ^^= state[1][0]; state[3][1] ^^= state[1][1]; state[3][2] ^^= state[1][2]; state[3][3] ^^= state[1][3];
    state[3][0] ^^= state[4][0] & state[0][0]; state[3][1] ^^= state[4][1] & state[0][1]; state[3][2] ^^= state[4][2] & state[0][2]; state[3][3] ^^= state[4][3] & state[0][3];
    state[3][0] = ref_rotl(state[3][0], b[3]);  state[3][1] = ref_rotl(state[3][1], b[3]);       state[3][2] = ref_rotl(state[3][2], b[3]);       state[3][3] = ref_rotl(state[3][3], b[3]);
    temp = state[1][3];    state[1][3] = state[1][1];  state[1][1] = temp;
    temp = state[1][2];    state[1][2] = state[1][0];  state[1][0] = temp;
    
    state[4][0] ^^= msgblk[0];   state[4][1] ^^= msgblk[1];   state[4][2] ^^= msgblk[2];   state[4][3] ^^= msgblk[3];
    state[4][0] ^^= state[2][0]; state[4][1] ^^= state[2][1]; state[4][2] ^^= state[2][2]; state[4][3] ^^= state[2][3];
    state[4][0] ^^= state[0][0] & state[1][0]; state[4][1] ^^= state[0][1] & state[1][1]; state[4][2] ^^= state[0][2] & state[1][2]; state[4][3] ^^= state[0][3] & state[1][3];
    state[4][0] = ref_rotl(state[4][0], b[4]);  state[4][1] = ref_rotl(state[4][1], b[4]);       state[4][2] = ref_rotl(state[4][2], b[4]);       state[4][3] = ref_rotl(state[4][3], b[4]);
    temp = state[2][3];    state[2][3] = state[2][2];  state[2][2] = state[2][1];  state[2][1] = state[2][0];  state[2][0] = temp;
    
    res = [sum([(state[i][k]&mask)<<(wordSize*k) for k in range(nWords)]) for i in range(5)]
    return res

def ref_check():
    global c, s
    c = []
    s = []
    initState()
    #s = [[0, 0, 0, 0, 1]]
    
    s2 = copy(s[0])
    res_ref = ref_update(s2)

    update()
    res = s[1]

    print "Start:"
    printState(s2)
    print "Reference implem:"
    printState(res_ref)
    print "current implem:"
    printState(res)

    if res_ref == res:
        print "SUCCESS"
    else:
        print "FAIL"

def strSi(si):
    res = ""
    for i in range(nWords):
        for j in range(wordSize):
            res = "%d"%((si >> (i*wordSize + j)) & 1) + res
        res = " " + res
    return res

def printState(state):
    for i in range(5):
        print "S%d: "%(i) + strSi(state[i])

def printAll():
    for i in range(len(s)):
        print " -- Round %d:"%(i)
        print " C: " + strSi(c[i])
        printState(s[i])

def initState():
    s.append([random.randrange(0,2^size) for i in range(5)])
    c.append(s[0][0] ^^ rotl(s[0][1],w[2]) ^^ (s[0][2] & s[0][3]))

def S(i,j,k):
    return (s[i][j]>>k)&1

def C(i,k):
    return (c[i]>>k)&1

def evalEq():
    res = eq
    for i in range(nRounds-1,-1,-1):
        for j in range(5-1,-1,-1):
            for k in range(size-1,-1,-1):
                res = res.replace("S_%d_%d_%d"%(i,j,k),str(S(i,j,k)))
        for k in range(size-1,-1,-1):
                res = res.replace("C_%d_%d"%(i,k),str(C(i,k)))
    return eval(res)%2
    
def trial():
    global c, s
    c = []
    s = []
    initState()

    for i in range(nRounds):
        update()

    return 1-evalEq()

print "Testing:"
print eq
print "Result:"

count = 0
for i in range(nTrials):
    count += trial()
print "%d out of %d"%(count,nTrials)

if count < nTrials:
    print "FAIL"
else:
    print "SUCCESS"
    
