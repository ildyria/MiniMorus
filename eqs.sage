LL = []

large = 0
#0 means use MORUS-640 rotation constants, 1 means use MORUS-1280's

wordSize = [32,64][large]
nWords = 1

size = wordSize * nWords

nRounds = 5

#rotation constants
b = [[5,31,7,22,13], [13,46,38,7,4]][large]
w = [[32,64,96,64,32], [64,128,192,128,64]][large]

C = [[ "C_{}_{}".format(i, j)  for j in range(size)] for i in range(nRounds)]
LL += sum(C,[])

S = [[[ "S_{}_{}_{}".format(i, j, k)  for k in range(size)] for j in range(5)] for i in range(nRounds) ]
LL += sum(sum(S,[]),[])


R = PolynomialRing(GF(2),(nRounds+1)*size*5, LL)
C = [[ R.gens_dict()[x] for x in c ] for c in C ]
S = [[[ R.gens_dict()[x] for x in s ] for s in ss ] for ss in S ]
def eq1(i,j):
    return C[i][j] + S[i][0][j] + S[i][1][(j-w[2])%size] + S[i][2][j] * S[i][3][j]
def eq2(i,j):
    k = (wordSize*floor(j/wordSize) + (j-b[0])%wordSize - w[2])%size
    return S[i+1][0][j] + S[i][0][k] + S[i][3][k]  + S[i][1][k] * S[i][2][k]
def eq3(i,j):
    k = (wordSize*floor(j/wordSize) + (j-b[1])%wordSize - w[3])%size
    return S[i+1][1][j] + S[i][1][k]  + S[i][4][k]   + S[i][2][k] * S[i][3][(k - w[0])%size]
def eq4(i,j):
    k = wordSize*floor(j/wordSize) + (j-b[4])%wordSize
    return S[i+1][4][j] + S[i][4][(k - w[1])%size] + S[i+1][2][(k - w[4])%size]+ S[i+1][0][k]*S[i+1][1][k]
def eq5(i,j):
    k = (wordSize*floor(j/wordSize) + (j-b[2])%wordSize-w[4] - w[4])%size
    return S[i+1][2][j] + S[i][2][k] + S[i+1][0][(k-w[2])%size]+ S[i][3][(k-w[0])%size] * S[i][4][(k-w[1])%size]

def eqS0(i,j):
    k = wordSize*floor(j/wordSize)
    return eq1(i-1,k + (j-b[0])%wordSize) + eq2(i-1,j)
def eqS1(i,j):
    return eqS0(i,j)+eq1(i,j)
def eqS4(i,j):
    k = wordSize*floor(j/wordSize)
    return eqS1(i,j)+eqS1(i+1,k + (j+b[1])%wordSize)+eq3(i,k + (j+b[1])%wordSize)
def eqS2(i,j):
    k = wordSize*floor(j/wordSize)
    return eqS4(i-1,j)+eqS4(i,k + (j+b[4])%wordSize)+eq4(i-1,k + (j+b[4])%wordSize)
def eqS0b(i,j):
    k = wordSize*floor(j/wordSize)
    return eqS2(i-1,j) + eqS2(i,k + (j+b[2])%wordSize) + eq5(i-1,k + (j+b[2])%wordSize)

#trail equation from round i, shifted by j positions (the one we used for mini Morus is i,j = 3,0)
def eq(i,j):
    return eqS0b(i,j)+eqS0(i,j)

