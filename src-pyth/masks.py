############################################################
#                       MORUS 640
############################################################

ciphers_640 = [
{   '[0]': [27]},               # alpha
{   '[0]': [0]},                # beta
{},                             # gamma
{},                             # delta
{},                             # epsil
{   '[0]': [27],                # appr1
    '[1]': [0,26,8],
    '[2]': [31,13,7],
    '[3]': [12]},
{   '[1]': [2],                 # appr2
    '[2]': [1,7,15,27],
    '[3]': [6,20,14],
    '[4]': [19]},
{   '[0]': [27],                # full
    '[1]': [0,2,8,26],
    '[2]': [31,27,15,13,1],
    '[3]': [20,14,12,6],
    '[4]': [19]}
]

states_640 = [
{   '[1][0]': [0]},             # alpha
{   '[0][0]': [0],              # beta
    '[0][1]': [0]},
{   '[0][1]': [0],              # gamma
    '[0][4]': [0],
    '[1][1]': [31]},
{   '[0][4]': [0],              # delta
    '[1][2]': [0],
    '[1][4]': [13]},
{   '[0][2]': [0],              # epsil
    '[1][0]': [0],
    '[1][2]': [7]},
{   '[2][2]': [0]},             # appr1
{   '[2][2]': [0]},             # appr2
{}                              # full
]

weight_mini_640 = [
1,              # alpha
1,              # beta
1,              # gamma
1,              # delta
1,              # epsil
7,              # appr1
9,              # appr2
16,             # full
]

weight_640 = [
10,              # alpha
10,              # beta
10,              # gamma
10,              # delta
10,              # epsil
# 14,              # appr1
# 18,              # appr2
# 16,             # full
]


############################################################
#                       MORUS 1280
############################################################

ciphers_1280 = [
{   '[0]': [51]},               # alpha
{   '[0]': [0]},                # beta
{},                             # gamma
{},                             # delta
{},                             # epsil
{   '[0]': [51],                # appr1
    '[1]': [0,33,55],
    '[2]': [4,37,46],
    '[3]': [50]},
{   '[1]': [25],                 # appr2
    '[2]': [7,29,38,51],
    '[3]': [11,20,42],
    '[4]': [24]},
{   '[0]': [51],                # full
    '[1]': [0,25,33,55],
    '[2]': [4,7,29,37,38,46,51],
    '[3]': [11,20,42,50],
    '[4]': [24]}
]

states_1280 = [
{   '[1][0]': [0]},             # alpha
{   '[0][0]': [0],              # beta
    '[0][1]': [0]},
{   '[0][1]': [0],              # gamma
    '[0][4]': [0],
    '[1][1]': [46]},
{   '[0][4]': [0],              # delta
    '[1][2]': [0],
    '[1][4]': [4]},
{   '[0][2]': [0],              # epsil
    '[1][0]': [0],
    '[1][2]': [38]},
{   '[2][2]': [0]},             # appr1
{   '[2][2]': [0]},             # appr2
{}                              # full
]

weight_mini_1280 = [
1,              # alpha
1,              # beta
1,              # gamma
1,              # delta
1,              # epsil
7,              # appr1
9,              # appr2
16,             # full
]

# weight_1280 = [
# 1,              # alpha
# 1,              # beta
# 1,              # gamma
# 1,              # delta
# 1,              # epsil
# 7,              # appr1
# 9,              # appr2
# 16,             # full
# ]

masks_list = [
{'kind': 1, 'width': 32, 'states': states_640, 'ciphers': ciphers_640, 'weight': weight_mini_640},
{'kind': 1, 'width': 64, 'states': states_1280, 'ciphers': ciphers_1280, 'weight': weight_mini_1280},
{'kind': 2, 'width': 32, 'states': states_640, 'ciphers': ciphers_640, 'weight': weight_640},
# {'kind': 2, 'width': 64, 'states': states_1280, 'ciphers': ciphers_1280, 'weight': weight_1280},
]
