#include <core/lowdiscrepancy.h>

namespace phyr {

// Low Discrepancy Data Definitions
const int Primes[PrimeTableSize] = {
    2, 3, 5, 7, 11,
    // Subsequent prime numbers
    13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
    173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251,
    257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347,
    349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
    439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523,
    541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619,
    631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727,
    733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827,
    829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937,
    941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031,
    1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103,
    1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201,
    1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289,
    1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381,
    1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471,
    1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553,
    1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621,
    1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723,
    1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823,
    1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913,
    1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999, 2003, 2011,
    2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099,
    2111, 2113, 2129, 2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207,
    2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293,
    2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371, 2377, 2381,
    2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437, 2441, 2447, 2459, 2467,
    2473, 2477, 2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591,
    2593, 2609, 2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683,
    2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749,
    2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843,
    2851, 2857, 2861, 2879, 2887, 2897, 2903, 2909, 2917, 2927, 2939, 2953,
    2957, 2963, 2969, 2971, 2999, 3001, 3011, 3019, 3023, 3037, 3041, 3049,
    3061, 3067, 3079, 3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169,
    3181, 3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259,
    3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347, 3359,
    3361, 3371, 3373, 3389, 3391, 3407, 3413, 3433, 3449, 3457, 3461, 3463,
    3467, 3469, 3491, 3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541, 3547,
    3557, 3559, 3571, 3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637,
    3643, 3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733,
    3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823, 3833, 3847,
    3851, 3853, 3863, 3877, 3881, 3889, 3907, 3911, 3917, 3919, 3923, 3929,
    3931, 3943, 3947, 3967, 3989, 4001, 4003, 4007, 4013, 4019, 4021, 4027,
    4049, 4051, 4057, 4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133,
    4139, 4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231, 4241,
    4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297, 4327, 4337, 4339,
    4349, 4357, 4363, 4373, 4391, 4397, 4409, 4421, 4423, 4441, 4447, 4451,
    4457, 4463, 4481, 4483, 4493, 4507, 4513, 4517, 4519, 4523, 4547, 4549,
    4561, 4567, 4583, 4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651,
    4657, 4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751, 4759,
    4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 4861, 4871, 4877,
    4889, 4903, 4909, 4919, 4931, 4933, 4937, 4943, 4951, 4957, 4967, 4969,
    4973, 4987, 4993, 4999, 5003, 5009, 5011, 5021, 5023, 5039, 5051, 5059,
    5077, 5081, 5087, 5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171,
    5179, 5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279, 5281,
    5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399, 5407,
    5413, 5417, 5419, 5431, 5437, 5441, 5443, 5449, 5471, 5477, 5479, 5483,
    5501, 5503, 5507, 5519, 5521, 5527, 5531, 5557, 5563, 5569, 5573, 5581,
    5591, 5623, 5639, 5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689,
    5693, 5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791, 5801,
    5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857, 5861, 5867, 5869,
    5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953, 5981, 5987, 6007, 6011,
    6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091, 6101, 6113,
    6121, 6131, 6133, 6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217,
    6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301, 6311,
    6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379, 6389,
    6397, 6421, 6427, 6449, 6451, 6469, 6473, 6481, 6491, 6521, 6529, 6547,
    6551, 6553, 6563, 6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637, 6653,
    6659, 6661, 6673, 6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737,
    6761, 6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833, 6841,
    6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 6947, 6949, 6959,
    6961, 6967, 6971, 6977, 6983, 6991, 6997, 7001, 7013, 7019, 7027, 7039,
    7043, 7057, 7069, 7079, 7103, 7109, 7121, 7127, 7129, 7151, 7159, 7177,
    7187, 7193, 7207, 7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283,
    7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411, 7417,
    7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499, 7507, 7517, 7523,
    7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573, 7577, 7583, 7589, 7591,
    7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699,
    7703, 7717, 7723, 7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823,
    7829, 7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919};

const int PrimeSums[PrimeTableSize] = {
    0, 2, 5, 10, 17,
    // Subsequent prime sums
    28, 41, 58, 77, 100, 129, 160, 197, 238, 281, 328, 381, 440, 501, 568, 639,
    712, 791, 874, 963, 1060, 1161, 1264, 1371, 1480, 1593, 1720, 1851, 1988,
    2127, 2276, 2427, 2584, 2747, 2914, 3087, 3266, 3447, 3638, 3831, 4028,
    4227, 4438, 4661, 4888, 5117, 5350, 5589, 5830, 6081, 6338, 6601, 6870,
    7141, 7418, 7699, 7982, 8275, 8582, 8893, 9206, 9523, 9854, 10191, 10538,
    10887, 11240, 11599, 11966, 12339, 12718, 13101, 13490, 13887, 14288, 14697,
    15116, 15537, 15968, 16401, 16840, 17283, 17732, 18189, 18650, 19113, 19580,
    20059, 20546, 21037, 21536, 22039, 22548, 23069, 23592, 24133, 24680, 25237,
    25800, 26369, 26940, 27517, 28104, 28697, 29296, 29897, 30504, 31117, 31734,
    32353, 32984, 33625, 34268, 34915, 35568, 36227, 36888, 37561, 38238, 38921,
    39612, 40313, 41022, 41741, 42468, 43201, 43940, 44683, 45434, 46191, 46952,
    47721, 48494, 49281, 50078, 50887, 51698, 52519, 53342, 54169, 54998, 55837,
    56690, 57547, 58406, 59269, 60146, 61027, 61910, 62797, 63704, 64615, 65534,
    66463, 67400, 68341, 69288, 70241, 71208, 72179, 73156, 74139, 75130, 76127,
    77136, 78149, 79168, 80189, 81220, 82253, 83292, 84341, 85392, 86453, 87516,
    88585, 89672, 90763, 91856, 92953, 94056, 95165, 96282, 97405, 98534, 99685,
    100838, 102001, 103172, 104353, 105540, 106733, 107934, 109147, 110364,
    111587, 112816, 114047, 115284, 116533, 117792, 119069, 120348, 121631,
    122920, 124211, 125508, 126809, 128112, 129419, 130738, 132059, 133386,
    134747, 136114, 137487, 138868, 140267, 141676, 143099, 144526, 145955,
    147388, 148827, 150274, 151725, 153178, 154637, 156108, 157589, 159072,
    160559, 162048, 163541, 165040, 166551, 168074, 169605, 171148, 172697,
    174250, 175809, 177376, 178947, 180526, 182109, 183706, 185307, 186914,
    188523, 190136, 191755, 193376, 195003, 196640, 198297, 199960, 201627,
    203296, 204989, 206686, 208385, 210094, 211815, 213538, 215271, 217012,
    218759, 220512, 222271, 224048, 225831, 227618, 229407, 231208, 233019,
    234842, 236673, 238520, 240381, 242248, 244119, 245992, 247869, 249748,
    251637, 253538, 255445, 257358, 259289, 261222, 263171, 265122, 267095,
    269074, 271061, 273054, 275051, 277050, 279053, 281064, 283081, 285108,
    287137, 289176, 291229, 293292, 295361, 297442, 299525, 301612, 303701,
    305800, 307911, 310024, 312153, 314284, 316421, 318562, 320705, 322858,
    325019, 327198, 329401, 331608, 333821, 336042, 338279, 340518, 342761,
    345012, 347279, 349548, 351821, 354102, 356389, 358682, 360979, 363288,
    365599, 367932, 370271, 372612, 374959, 377310, 379667, 382038, 384415,
    386796, 389179, 391568, 393961, 396360, 398771, 401188, 403611, 406048,
    408489, 410936, 413395, 415862, 418335, 420812, 423315, 425836, 428367,
    430906, 433449, 435998, 438549, 441106, 443685, 446276, 448869, 451478,
    454095, 456716, 459349, 461996, 464653, 467312, 469975, 472646, 475323,
    478006, 480693, 483382, 486075, 488774, 491481, 494192, 496905, 499624,
    502353, 505084, 507825, 510574, 513327, 516094, 518871, 521660, 524451,
    527248, 530049, 532852, 535671, 538504, 541341, 544184, 547035, 549892,
    552753, 555632, 558519, 561416, 564319, 567228, 570145, 573072, 576011,
    578964, 581921, 584884, 587853, 590824, 593823, 596824, 599835, 602854,
    605877, 608914, 611955, 615004, 618065, 621132, 624211, 627294, 630383,
    633492, 636611, 639732, 642869, 646032, 649199, 652368, 655549, 658736,
    661927, 665130, 668339, 671556, 674777, 678006, 681257, 684510, 687767,
    691026, 694297, 697596, 700897, 704204, 707517, 710836, 714159, 717488,
    720819, 724162, 727509, 730868, 734229, 737600, 740973, 744362, 747753,
    751160, 754573, 758006, 761455, 764912, 768373, 771836, 775303, 778772,
    782263, 785762, 789273, 792790, 796317, 799846, 803379, 806918, 810459,
    814006, 817563, 821122, 824693, 828274, 831857, 835450, 839057, 842670,
    846287, 849910, 853541, 857178, 860821, 864480, 868151, 871824, 875501,
    879192, 882889, 886590, 890299, 894018, 897745, 901478, 905217, 908978,
    912745, 916514, 920293, 924086, 927883, 931686, 935507, 939330, 943163,
    947010, 950861, 954714, 958577, 962454, 966335, 970224, 974131, 978042,
    981959, 985878, 989801, 993730, 997661, 1001604, 1005551, 1009518, 1013507,
    1017508, 1021511, 1025518, 1029531, 1033550, 1037571, 1041598, 1045647,
    1049698, 1053755, 1057828, 1061907, 1065998, 1070091, 1074190, 1078301,
    1082428, 1086557, 1090690, 1094829, 1098982, 1103139, 1107298, 1111475,
    1115676, 1119887, 1124104, 1128323, 1132552, 1136783, 1141024, 1145267,
    1149520, 1153779, 1158040, 1162311, 1166584, 1170867, 1175156, 1179453,
    1183780, 1188117, 1192456, 1196805, 1201162, 1205525, 1209898, 1214289,
    1218686, 1223095, 1227516, 1231939, 1236380, 1240827, 1245278, 1249735,
    1254198, 1258679, 1263162, 1267655, 1272162, 1276675, 1281192, 1285711,
    1290234, 1294781, 1299330, 1303891, 1308458, 1313041, 1317632, 1322229,
    1326832, 1331453, 1336090, 1340729, 1345372, 1350021, 1354672, 1359329,
    1363992, 1368665, 1373344, 1378035, 1382738, 1387459, 1392182, 1396911,
    1401644, 1406395, 1411154, 1415937, 1420724, 1425513, 1430306, 1435105,
    1439906, 1444719, 1449536, 1454367, 1459228, 1464099, 1468976, 1473865,
    1478768, 1483677, 1488596, 1493527, 1498460, 1503397, 1508340, 1513291,
    1518248, 1523215, 1528184, 1533157, 1538144, 1543137, 1548136, 1553139,
    1558148, 1563159, 1568180, 1573203, 1578242, 1583293, 1588352, 1593429,
    1598510, 1603597, 1608696, 1613797, 1618904, 1624017, 1629136, 1634283,
    1639436, 1644603, 1649774, 1654953, 1660142, 1665339, 1670548, 1675775,
    1681006, 1686239, 1691476, 1696737, 1702010, 1707289, 1712570, 1717867,
    1723170, 1728479, 1733802, 1739135, 1744482, 1749833, 1755214, 1760601,
    1765994, 1771393, 1776800, 1782213, 1787630, 1793049, 1798480, 1803917,
    1809358, 1814801, 1820250, 1825721, 1831198, 1836677, 1842160, 1847661,
    1853164, 1858671, 1864190, 1869711, 1875238, 1880769, 1886326, 1891889,
    1897458, 1903031, 1908612, 1914203, 1919826, 1925465, 1931106, 1936753,
    1942404, 1948057, 1953714, 1959373, 1965042, 1970725, 1976414, 1982107,
    1987808, 1993519, 1999236, 2004973, 2010714, 2016457, 2022206, 2027985,
    2033768, 2039559, 2045360, 2051167, 2056980, 2062801, 2068628, 2074467,
    2080310, 2086159, 2092010, 2097867, 2103728, 2109595, 2115464, 2121343,
    2127224, 2133121, 2139024, 2144947, 2150874, 2156813, 2162766, 2168747,
    2174734, 2180741, 2186752, 2192781, 2198818, 2204861, 2210908, 2216961,
    2223028, 2229101, 2235180, 2241269, 2247360, 2253461, 2259574, 2265695,
    2271826, 2277959, 2284102, 2290253, 2296416, 2302589, 2308786, 2314985,
    2321188, 2327399, 2333616, 2339837, 2346066, 2352313, 2358570, 2364833,
    2371102, 2377373, 2383650, 2389937, 2396236, 2402537, 2408848, 2415165,
    2421488, 2427817, 2434154, 2440497, 2446850, 2453209, 2459570, 2465937,
    2472310, 2478689, 2485078, 2491475, 2497896, 2504323, 2510772, 2517223,
    2523692, 2530165, 2536646, 2543137, 2549658, 2556187, 2562734, 2569285,
    2575838, 2582401, 2588970, 2595541, 2602118, 2608699, 2615298, 2621905,
    2628524, 2635161, 2641814, 2648473, 2655134, 2661807, 2668486, 2675175,
    2681866, 2688567, 2695270, 2701979, 2708698, 2715431, 2722168, 2728929,
    2735692, 2742471, 2749252, 2756043, 2762836, 2769639, 2776462, 2783289,
    2790118, 2796951, 2803792, 2810649, 2817512, 2824381, 2831252, 2838135,
    2845034, 2851941, 2858852, 2865769, 2872716, 2879665, 2886624, 2893585,
    2900552, 2907523, 2914500, 2921483, 2928474, 2935471, 2942472, 2949485,
    2956504, 2963531, 2970570, 2977613, 2984670, 2991739, 2998818, 3005921,
    3013030, 3020151, 3027278, 3034407, 3041558, 3048717, 3055894, 3063081,
    3070274, 3077481, 3084692, 3091905, 3099124, 3106353, 3113590, 3120833,
    3128080, 3135333, 3142616, 3149913, 3157220, 3164529, 3171850, 3179181,
    3186514, 3193863, 3201214, 3208583, 3215976, 3223387, 3230804, 3238237,
    3245688, 3253145, 3260604, 3268081, 3275562, 3283049, 3290538, 3298037,
    3305544, 3313061, 3320584, 3328113, 3335650, 3343191, 3350738, 3358287,
    3365846, 3373407, 3380980, 3388557, 3396140, 3403729, 3411320, 3418923,
    3426530, 3434151, 3441790, 3449433, 3457082, 3464751, 3472424, 3480105,
    3487792, 3495483, 3503182, 3510885, 3518602, 3526325, 3534052, 3541793,
    3549546, 3557303, 3565062, 3572851, 3580644, 3588461, 3596284, 3604113,
    3611954, 3619807, 3627674, 3635547, 3643424, 3651303, 3659186, 3667087,
    3674994,
};

// To convert matrices from the "(t, m, s)-Nets and Maximized Minimum
// Distance" paper:
// - Recall that each uint32_t represents a column of the matrix, so
//   copy columns of bits.
// - Reverse the bits in each column (so we don't need to reverse the
//   result after the matrix multiply.)

uint32_t CMaxMinDist[17][32] = {
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x80000000,
    },
    {
        0xc0000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0xa0000000, 0x40000000, 0x80000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0xc0000000, 0x50000000, 0x20000000, 0x30000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x88000000, 0x58000000, 0x20000000, 0x40000000, 0x80000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0xe0000000, 0x60000000, 0x28000000, 0x10000000, 0x18000000, 0x04000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x82000000, 0x44000000, 0x2c000000, 0x10000000, 0x20000000, 0x40000000,
        0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x80000000, 0x70000000, 0x30000000, 0x14000000, 0x08000000, 0x0c000000,
        0x02000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0xc0000000, 0x41000000, 0x22000000, 0x16000000, 0x08000000, 0x10000000,
        0x20000000, 0x40800000, 0x80000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x80000000, 0x40000000, 0x20000000, 0x18000000, 0x08000000, 0x1c000000,
        0x1e000000, 0x03000000, 0x00800000, 0x00400000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x80000000, 0x60400000, 0x20800000, 0x11000000, 0x0b000000, 0x04000000,
        0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x00200000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x80000000, 0x40000000, 0x20000000, 0x1c000000, 0x0c000000, 0x05000000,
        0x02000000, 0x03000000, 0x00800000, 0x00400000, 0x00200000, 0x00100000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x80000000, 0x40000000, 0x30200000, 0x10400000, 0x08800000, 0x05800000,
        0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x00100000,
        0x00080000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x0e000000, 0x06000000,
        0x02800000, 0x01000000, 0x01800000, 0x00400000, 0x00200000, 0x00100000,
        0x00080000, 0x00040000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x80000000, 0x40000000, 0x20000000, 0x18100000, 0x08200000, 0x04400000,
        0x02c00000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
        0x00080000, 0x00040000, 0x00020000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
    {
        0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x07000000,
        0x03000000, 0x01400000, 0x00800000, 0x00c00000, 0x00200000, 0x00100000,
        0x00080000, 0x00040000, 0x00020000, 0x00010000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    },
};

// Low Discrepancy Static Functions
template <int base>
static Real radicalInverseSpecialized(uint64_t a) {
    const Real invBase = (Real)1 / (Real)base;
    uint64_t reversedDigits = 0;
    Real invBaseN = 1;

    while (a) {
        uint64_t next = a / base;
        uint64_t digit = a - next * base;
        reversedDigits = reversedDigits * base + digit;
        invBaseN *= invBase;
        a = next;
    }

    ASSERT(reversedDigits * invBaseN < 1.00001);
    return std::min(reversedDigits * invBaseN, OneMinusEpsilon);
}

template <int base>
static Real scrambledRadicalInverseSpecialized(const uint16_t* perm, uint64_t a) {
    const Real invBase = (Real)1 / (Real)base;
    uint64_t reversedDigits = 0;
    Real invBaseN = 1;

    while (a) {
        uint64_t next = a / base;
        uint64_t digit = a - next * base;
        ASSERT(perm[digit] < base);
        reversedDigits = reversedDigits * base + perm[digit];
        invBaseN *= invBase;
        a = next;
    }
    ASSERT(invBaseN * (reversedDigits + invBase * perm[0] / (1 - invBase)) < 1.00001);
    return std::min(
        invBaseN * (reversedDigits + invBase * perm[0] / (1 - invBase)),
        OneMinusEpsilon);
}

// Low Discrepancy Function Definitions
Real radicalInverse(int baseIndex, uint64_t a) {
    switch (baseIndex) {
    case 0:
        // Compute base-2 radical inverse
        return reverseBits64(a) * 5.4210108624275222e-20;
    case 1:
        return radicalInverseSpecialized<3>(a);
    case 2:
        return radicalInverseSpecialized<5>(a);
    case 3:
        return radicalInverseSpecialized<7>(a);
    // Remainder of cases for _RadicalInverse()_
    case 4:
        return radicalInverseSpecialized<11>(a);
    case 5:
        return radicalInverseSpecialized<13>(a);
    case 6:
        return radicalInverseSpecialized<17>(a);
    case 7:
        return radicalInverseSpecialized<19>(a);
    case 8:
        return radicalInverseSpecialized<23>(a);
    case 9:
        return radicalInverseSpecialized<29>(a);
    case 10:
        return radicalInverseSpecialized<31>(a);
    case 11:
        return radicalInverseSpecialized<37>(a);
    case 12:
        return radicalInverseSpecialized<41>(a);
    case 13:
        return radicalInverseSpecialized<43>(a);
    case 14:
        return radicalInverseSpecialized<47>(a);
    case 15:
        return radicalInverseSpecialized<53>(a);
    case 16:
        return radicalInverseSpecialized<59>(a);
    case 17:
        return radicalInverseSpecialized<61>(a);
    case 18:
        return radicalInverseSpecialized<67>(a);
    case 19:
        return radicalInverseSpecialized<71>(a);
    case 20:
        return radicalInverseSpecialized<73>(a);
    case 21:
        return radicalInverseSpecialized<79>(a);
    case 22:
        return radicalInverseSpecialized<83>(a);
    case 23:
        return radicalInverseSpecialized<89>(a);
    case 24:
        return radicalInverseSpecialized<97>(a);
    case 25:
        return radicalInverseSpecialized<101>(a);
    case 26:
        return radicalInverseSpecialized<103>(a);
    case 27:
        return radicalInverseSpecialized<107>(a);
    case 28:
        return radicalInverseSpecialized<109>(a);
    case 29:
        return radicalInverseSpecialized<113>(a);
    case 30:
        return radicalInverseSpecialized<127>(a);
    case 31:
        return radicalInverseSpecialized<131>(a);
    case 32:
        return radicalInverseSpecialized<137>(a);
    case 33:
        return radicalInverseSpecialized<139>(a);
    case 34:
        return radicalInverseSpecialized<149>(a);
    case 35:
        return radicalInverseSpecialized<151>(a);
    case 36:
        return radicalInverseSpecialized<157>(a);
    case 37:
        return radicalInverseSpecialized<163>(a);
    case 38:
        return radicalInverseSpecialized<167>(a);
    case 39:
        return radicalInverseSpecialized<173>(a);
    case 40:
        return radicalInverseSpecialized<179>(a);
    case 41:
        return radicalInverseSpecialized<181>(a);
    case 42:
        return radicalInverseSpecialized<191>(a);
    case 43:
        return radicalInverseSpecialized<193>(a);
    case 44:
        return radicalInverseSpecialized<197>(a);
    case 45:
        return radicalInverseSpecialized<199>(a);
    case 46:
        return radicalInverseSpecialized<211>(a);
    case 47:
        return radicalInverseSpecialized<223>(a);
    case 48:
        return radicalInverseSpecialized<227>(a);
    case 49:
        return radicalInverseSpecialized<229>(a);
    case 50:
        return radicalInverseSpecialized<233>(a);
    case 51:
        return radicalInverseSpecialized<239>(a);
    case 52:
        return radicalInverseSpecialized<241>(a);
    case 53:
        return radicalInverseSpecialized<251>(a);
    case 54:
        return radicalInverseSpecialized<257>(a);
    case 55:
        return radicalInverseSpecialized<263>(a);
    case 56:
        return radicalInverseSpecialized<269>(a);
    case 57:
        return radicalInverseSpecialized<271>(a);
    case 58:
        return radicalInverseSpecialized<277>(a);
    case 59:
        return radicalInverseSpecialized<281>(a);
    case 60:
        return radicalInverseSpecialized<283>(a);
    case 61:
        return radicalInverseSpecialized<293>(a);
    case 62:
        return radicalInverseSpecialized<307>(a);
    case 63:
        return radicalInverseSpecialized<311>(a);
    case 64:
        return radicalInverseSpecialized<313>(a);
    case 65:
        return radicalInverseSpecialized<317>(a);
    case 66:
        return radicalInverseSpecialized<331>(a);
    case 67:
        return radicalInverseSpecialized<337>(a);
    case 68:
        return radicalInverseSpecialized<347>(a);
    case 69:
        return radicalInverseSpecialized<349>(a);
    case 70:
        return radicalInverseSpecialized<353>(a);
    case 71:
        return radicalInverseSpecialized<359>(a);
    case 72:
        return radicalInverseSpecialized<367>(a);
    case 73:
        return radicalInverseSpecialized<373>(a);
    case 74:
        return radicalInverseSpecialized<379>(a);
    case 75:
        return radicalInverseSpecialized<383>(a);
    case 76:
        return radicalInverseSpecialized<389>(a);
    case 77:
        return radicalInverseSpecialized<397>(a);
    case 78:
        return radicalInverseSpecialized<401>(a);
    case 79:
        return radicalInverseSpecialized<409>(a);
    case 80:
        return radicalInverseSpecialized<419>(a);
    case 81:
        return radicalInverseSpecialized<421>(a);
    case 82:
        return radicalInverseSpecialized<431>(a);
    case 83:
        return radicalInverseSpecialized<433>(a);
    case 84:
        return radicalInverseSpecialized<439>(a);
    case 85:
        return radicalInverseSpecialized<443>(a);
    case 86:
        return radicalInverseSpecialized<449>(a);
    case 87:
        return radicalInverseSpecialized<457>(a);
    case 88:
        return radicalInverseSpecialized<461>(a);
    case 89:
        return radicalInverseSpecialized<463>(a);
    case 90:
        return radicalInverseSpecialized<467>(a);
    case 91:
        return radicalInverseSpecialized<479>(a);
    case 92:
        return radicalInverseSpecialized<487>(a);
    case 93:
        return radicalInverseSpecialized<491>(a);
    case 94:
        return radicalInverseSpecialized<499>(a);
    case 95:
        return radicalInverseSpecialized<503>(a);
    case 96:
        return radicalInverseSpecialized<509>(a);
    case 97:
        return radicalInverseSpecialized<521>(a);
    case 98:
        return radicalInverseSpecialized<523>(a);
    case 99:
        return radicalInverseSpecialized<541>(a);
    case 100:
        return radicalInverseSpecialized<547>(a);
    case 101:
        return radicalInverseSpecialized<557>(a);
    case 102:
        return radicalInverseSpecialized<563>(a);
    case 103:
        return radicalInverseSpecialized<569>(a);
    case 104:
        return radicalInverseSpecialized<571>(a);
    case 105:
        return radicalInverseSpecialized<577>(a);
    case 106:
        return radicalInverseSpecialized<587>(a);
    case 107:
        return radicalInverseSpecialized<593>(a);
    case 108:
        return radicalInverseSpecialized<599>(a);
    case 109:
        return radicalInverseSpecialized<601>(a);
    case 110:
        return radicalInverseSpecialized<607>(a);
    case 111:
        return radicalInverseSpecialized<613>(a);
    case 112:
        return radicalInverseSpecialized<617>(a);
    case 113:
        return radicalInverseSpecialized<619>(a);
    case 114:
        return radicalInverseSpecialized<631>(a);
    case 115:
        return radicalInverseSpecialized<641>(a);
    case 116:
        return radicalInverseSpecialized<643>(a);
    case 117:
        return radicalInverseSpecialized<647>(a);
    case 118:
        return radicalInverseSpecialized<653>(a);
    case 119:
        return radicalInverseSpecialized<659>(a);
    case 120:
        return radicalInverseSpecialized<661>(a);
    case 121:
        return radicalInverseSpecialized<673>(a);
    case 122:
        return radicalInverseSpecialized<677>(a);
    case 123:
        return radicalInverseSpecialized<683>(a);
    case 124:
        return radicalInverseSpecialized<691>(a);
    case 125:
        return radicalInverseSpecialized<701>(a);
    case 126:
        return radicalInverseSpecialized<709>(a);
    case 127:
        return radicalInverseSpecialized<719>(a);
    case 128:
        return radicalInverseSpecialized<727>(a);
    case 129:
        return radicalInverseSpecialized<733>(a);
    case 130:
        return radicalInverseSpecialized<739>(a);
    case 131:
        return radicalInverseSpecialized<743>(a);
    case 132:
        return radicalInverseSpecialized<751>(a);
    case 133:
        return radicalInverseSpecialized<757>(a);
    case 134:
        return radicalInverseSpecialized<761>(a);
    case 135:
        return radicalInverseSpecialized<769>(a);
    case 136:
        return radicalInverseSpecialized<773>(a);
    case 137:
        return radicalInverseSpecialized<787>(a);
    case 138:
        return radicalInverseSpecialized<797>(a);
    case 139:
        return radicalInverseSpecialized<809>(a);
    case 140:
        return radicalInverseSpecialized<811>(a);
    case 141:
        return radicalInverseSpecialized<821>(a);
    case 142:
        return radicalInverseSpecialized<823>(a);
    case 143:
        return radicalInverseSpecialized<827>(a);
    case 144:
        return radicalInverseSpecialized<829>(a);
    case 145:
        return radicalInverseSpecialized<839>(a);
    case 146:
        return radicalInverseSpecialized<853>(a);
    case 147:
        return radicalInverseSpecialized<857>(a);
    case 148:
        return radicalInverseSpecialized<859>(a);
    case 149:
        return radicalInverseSpecialized<863>(a);
    case 150:
        return radicalInverseSpecialized<877>(a);
    case 151:
        return radicalInverseSpecialized<881>(a);
    case 152:
        return radicalInverseSpecialized<883>(a);
    case 153:
        return radicalInverseSpecialized<887>(a);
    case 154:
        return radicalInverseSpecialized<907>(a);
    case 155:
        return radicalInverseSpecialized<911>(a);
    case 156:
        return radicalInverseSpecialized<919>(a);
    case 157:
        return radicalInverseSpecialized<929>(a);
    case 158:
        return radicalInverseSpecialized<937>(a);
    case 159:
        return radicalInverseSpecialized<941>(a);
    case 160:
        return radicalInverseSpecialized<947>(a);
    case 161:
        return radicalInverseSpecialized<953>(a);
    case 162:
        return radicalInverseSpecialized<967>(a);
    case 163:
        return radicalInverseSpecialized<971>(a);
    case 164:
        return radicalInverseSpecialized<977>(a);
    case 165:
        return radicalInverseSpecialized<983>(a);
    case 166:
        return radicalInverseSpecialized<991>(a);
    case 167:
        return radicalInverseSpecialized<997>(a);
    case 168:
        return radicalInverseSpecialized<1009>(a);
    case 169:
        return radicalInverseSpecialized<1013>(a);
    case 170:
        return radicalInverseSpecialized<1019>(a);
    case 171:
        return radicalInverseSpecialized<1021>(a);
    case 172:
        return radicalInverseSpecialized<1031>(a);
    case 173:
        return radicalInverseSpecialized<1033>(a);
    case 174:
        return radicalInverseSpecialized<1039>(a);
    case 175:
        return radicalInverseSpecialized<1049>(a);
    case 176:
        return radicalInverseSpecialized<1051>(a);
    case 177:
        return radicalInverseSpecialized<1061>(a);
    case 178:
        return radicalInverseSpecialized<1063>(a);
    case 179:
        return radicalInverseSpecialized<1069>(a);
    case 180:
        return radicalInverseSpecialized<1087>(a);
    case 181:
        return radicalInverseSpecialized<1091>(a);
    case 182:
        return radicalInverseSpecialized<1093>(a);
    case 183:
        return radicalInverseSpecialized<1097>(a);
    case 184:
        return radicalInverseSpecialized<1103>(a);
    case 185:
        return radicalInverseSpecialized<1109>(a);
    case 186:
        return radicalInverseSpecialized<1117>(a);
    case 187:
        return radicalInverseSpecialized<1123>(a);
    case 188:
        return radicalInverseSpecialized<1129>(a);
    case 189:
        return radicalInverseSpecialized<1151>(a);
    case 190:
        return radicalInverseSpecialized<1153>(a);
    case 191:
        return radicalInverseSpecialized<1163>(a);
    case 192:
        return radicalInverseSpecialized<1171>(a);
    case 193:
        return radicalInverseSpecialized<1181>(a);
    case 194:
        return radicalInverseSpecialized<1187>(a);
    case 195:
        return radicalInverseSpecialized<1193>(a);
    case 196:
        return radicalInverseSpecialized<1201>(a);
    case 197:
        return radicalInverseSpecialized<1213>(a);
    case 198:
        return radicalInverseSpecialized<1217>(a);
    case 199:
        return radicalInverseSpecialized<1223>(a);
    case 200:
        return radicalInverseSpecialized<1229>(a);
    case 201:
        return radicalInverseSpecialized<1231>(a);
    case 202:
        return radicalInverseSpecialized<1237>(a);
    case 203:
        return radicalInverseSpecialized<1249>(a);
    case 204:
        return radicalInverseSpecialized<1259>(a);
    case 205:
        return radicalInverseSpecialized<1277>(a);
    case 206:
        return radicalInverseSpecialized<1279>(a);
    case 207:
        return radicalInverseSpecialized<1283>(a);
    case 208:
        return radicalInverseSpecialized<1289>(a);
    case 209:
        return radicalInverseSpecialized<1291>(a);
    case 210:
        return radicalInverseSpecialized<1297>(a);
    case 211:
        return radicalInverseSpecialized<1301>(a);
    case 212:
        return radicalInverseSpecialized<1303>(a);
    case 213:
        return radicalInverseSpecialized<1307>(a);
    case 214:
        return radicalInverseSpecialized<1319>(a);
    case 215:
        return radicalInverseSpecialized<1321>(a);
    case 216:
        return radicalInverseSpecialized<1327>(a);
    case 217:
        return radicalInverseSpecialized<1361>(a);
    case 218:
        return radicalInverseSpecialized<1367>(a);
    case 219:
        return radicalInverseSpecialized<1373>(a);
    case 220:
        return radicalInverseSpecialized<1381>(a);
    case 221:
        return radicalInverseSpecialized<1399>(a);
    case 222:
        return radicalInverseSpecialized<1409>(a);
    case 223:
        return radicalInverseSpecialized<1423>(a);
    case 224:
        return radicalInverseSpecialized<1427>(a);
    case 225:
        return radicalInverseSpecialized<1429>(a);
    case 226:
        return radicalInverseSpecialized<1433>(a);
    case 227:
        return radicalInverseSpecialized<1439>(a);
    case 228:
        return radicalInverseSpecialized<1447>(a);
    case 229:
        return radicalInverseSpecialized<1451>(a);
    case 230:
        return radicalInverseSpecialized<1453>(a);
    case 231:
        return radicalInverseSpecialized<1459>(a);
    case 232:
        return radicalInverseSpecialized<1471>(a);
    case 233:
        return radicalInverseSpecialized<1481>(a);
    case 234:
        return radicalInverseSpecialized<1483>(a);
    case 235:
        return radicalInverseSpecialized<1487>(a);
    case 236:
        return radicalInverseSpecialized<1489>(a);
    case 237:
        return radicalInverseSpecialized<1493>(a);
    case 238:
        return radicalInverseSpecialized<1499>(a);
    case 239:
        return radicalInverseSpecialized<1511>(a);
    case 240:
        return radicalInverseSpecialized<1523>(a);
    case 241:
        return radicalInverseSpecialized<1531>(a);
    case 242:
        return radicalInverseSpecialized<1543>(a);
    case 243:
        return radicalInverseSpecialized<1549>(a);
    case 244:
        return radicalInverseSpecialized<1553>(a);
    case 245:
        return radicalInverseSpecialized<1559>(a);
    case 246:
        return radicalInverseSpecialized<1567>(a);
    case 247:
        return radicalInverseSpecialized<1571>(a);
    case 248:
        return radicalInverseSpecialized<1579>(a);
    case 249:
        return radicalInverseSpecialized<1583>(a);
    case 250:
        return radicalInverseSpecialized<1597>(a);
    case 251:
        return radicalInverseSpecialized<1601>(a);
    case 252:
        return radicalInverseSpecialized<1607>(a);
    case 253:
        return radicalInverseSpecialized<1609>(a);
    case 254:
        return radicalInverseSpecialized<1613>(a);
    case 255:
        return radicalInverseSpecialized<1619>(a);
    case 256:
        return radicalInverseSpecialized<1621>(a);
    case 257:
        return radicalInverseSpecialized<1627>(a);
    case 258:
        return radicalInverseSpecialized<1637>(a);
    case 259:
        return radicalInverseSpecialized<1657>(a);
    case 260:
        return radicalInverseSpecialized<1663>(a);
    case 261:
        return radicalInverseSpecialized<1667>(a);
    case 262:
        return radicalInverseSpecialized<1669>(a);
    case 263:
        return radicalInverseSpecialized<1693>(a);
    case 264:
        return radicalInverseSpecialized<1697>(a);
    case 265:
        return radicalInverseSpecialized<1699>(a);
    case 266:
        return radicalInverseSpecialized<1709>(a);
    case 267:
        return radicalInverseSpecialized<1721>(a);
    case 268:
        return radicalInverseSpecialized<1723>(a);
    case 269:
        return radicalInverseSpecialized<1733>(a);
    case 270:
        return radicalInverseSpecialized<1741>(a);
    case 271:
        return radicalInverseSpecialized<1747>(a);
    case 272:
        return radicalInverseSpecialized<1753>(a);
    case 273:
        return radicalInverseSpecialized<1759>(a);
    case 274:
        return radicalInverseSpecialized<1777>(a);
    case 275:
        return radicalInverseSpecialized<1783>(a);
    case 276:
        return radicalInverseSpecialized<1787>(a);
    case 277:
        return radicalInverseSpecialized<1789>(a);
    case 278:
        return radicalInverseSpecialized<1801>(a);
    case 279:
        return radicalInverseSpecialized<1811>(a);
    case 280:
        return radicalInverseSpecialized<1823>(a);
    case 281:
        return radicalInverseSpecialized<1831>(a);
    case 282:
        return radicalInverseSpecialized<1847>(a);
    case 283:
        return radicalInverseSpecialized<1861>(a);
    case 284:
        return radicalInverseSpecialized<1867>(a);
    case 285:
        return radicalInverseSpecialized<1871>(a);
    case 286:
        return radicalInverseSpecialized<1873>(a);
    case 287:
        return radicalInverseSpecialized<1877>(a);
    case 288:
        return radicalInverseSpecialized<1879>(a);
    case 289:
        return radicalInverseSpecialized<1889>(a);
    case 290:
        return radicalInverseSpecialized<1901>(a);
    case 291:
        return radicalInverseSpecialized<1907>(a);
    case 292:
        return radicalInverseSpecialized<1913>(a);
    case 293:
        return radicalInverseSpecialized<1931>(a);
    case 294:
        return radicalInverseSpecialized<1933>(a);
    case 295:
        return radicalInverseSpecialized<1949>(a);
    case 296:
        return radicalInverseSpecialized<1951>(a);
    case 297:
        return radicalInverseSpecialized<1973>(a);
    case 298:
        return radicalInverseSpecialized<1979>(a);
    case 299:
        return radicalInverseSpecialized<1987>(a);
    case 300:
        return radicalInverseSpecialized<1993>(a);
    case 301:
        return radicalInverseSpecialized<1997>(a);
    case 302:
        return radicalInverseSpecialized<1999>(a);
    case 303:
        return radicalInverseSpecialized<2003>(a);
    case 304:
        return radicalInverseSpecialized<2011>(a);
    case 305:
        return radicalInverseSpecialized<2017>(a);
    case 306:
        return radicalInverseSpecialized<2027>(a);
    case 307:
        return radicalInverseSpecialized<2029>(a);
    case 308:
        return radicalInverseSpecialized<2039>(a);
    case 309:
        return radicalInverseSpecialized<2053>(a);
    case 310:
        return radicalInverseSpecialized<2063>(a);
    case 311:
        return radicalInverseSpecialized<2069>(a);
    case 312:
        return radicalInverseSpecialized<2081>(a);
    case 313:
        return radicalInverseSpecialized<2083>(a);
    case 314:
        return radicalInverseSpecialized<2087>(a);
    case 315:
        return radicalInverseSpecialized<2089>(a);
    case 316:
        return radicalInverseSpecialized<2099>(a);
    case 317:
        return radicalInverseSpecialized<2111>(a);
    case 318:
        return radicalInverseSpecialized<2113>(a);
    case 319:
        return radicalInverseSpecialized<2129>(a);
    case 320:
        return radicalInverseSpecialized<2131>(a);
    case 321:
        return radicalInverseSpecialized<2137>(a);
    case 322:
        return radicalInverseSpecialized<2141>(a);
    case 323:
        return radicalInverseSpecialized<2143>(a);
    case 324:
        return radicalInverseSpecialized<2153>(a);
    case 325:
        return radicalInverseSpecialized<2161>(a);
    case 326:
        return radicalInverseSpecialized<2179>(a);
    case 327:
        return radicalInverseSpecialized<2203>(a);
    case 328:
        return radicalInverseSpecialized<2207>(a);
    case 329:
        return radicalInverseSpecialized<2213>(a);
    case 330:
        return radicalInverseSpecialized<2221>(a);
    case 331:
        return radicalInverseSpecialized<2237>(a);
    case 332:
        return radicalInverseSpecialized<2239>(a);
    case 333:
        return radicalInverseSpecialized<2243>(a);
    case 334:
        return radicalInverseSpecialized<2251>(a);
    case 335:
        return radicalInverseSpecialized<2267>(a);
    case 336:
        return radicalInverseSpecialized<2269>(a);
    case 337:
        return radicalInverseSpecialized<2273>(a);
    case 338:
        return radicalInverseSpecialized<2281>(a);
    case 339:
        return radicalInverseSpecialized<2287>(a);
    case 340:
        return radicalInverseSpecialized<2293>(a);
    case 341:
        return radicalInverseSpecialized<2297>(a);
    case 342:
        return radicalInverseSpecialized<2309>(a);
    case 343:
        return radicalInverseSpecialized<2311>(a);
    case 344:
        return radicalInverseSpecialized<2333>(a);
    case 345:
        return radicalInverseSpecialized<2339>(a);
    case 346:
        return radicalInverseSpecialized<2341>(a);
    case 347:
        return radicalInverseSpecialized<2347>(a);
    case 348:
        return radicalInverseSpecialized<2351>(a);
    case 349:
        return radicalInverseSpecialized<2357>(a);
    case 350:
        return radicalInverseSpecialized<2371>(a);
    case 351:
        return radicalInverseSpecialized<2377>(a);
    case 352:
        return radicalInverseSpecialized<2381>(a);
    case 353:
        return radicalInverseSpecialized<2383>(a);
    case 354:
        return radicalInverseSpecialized<2389>(a);
    case 355:
        return radicalInverseSpecialized<2393>(a);
    case 356:
        return radicalInverseSpecialized<2399>(a);
    case 357:
        return radicalInverseSpecialized<2411>(a);
    case 358:
        return radicalInverseSpecialized<2417>(a);
    case 359:
        return radicalInverseSpecialized<2423>(a);
    case 360:
        return radicalInverseSpecialized<2437>(a);
    case 361:
        return radicalInverseSpecialized<2441>(a);
    case 362:
        return radicalInverseSpecialized<2447>(a);
    case 363:
        return radicalInverseSpecialized<2459>(a);
    case 364:
        return radicalInverseSpecialized<2467>(a);
    case 365:
        return radicalInverseSpecialized<2473>(a);
    case 366:
        return radicalInverseSpecialized<2477>(a);
    case 367:
        return radicalInverseSpecialized<2503>(a);
    case 368:
        return radicalInverseSpecialized<2521>(a);
    case 369:
        return radicalInverseSpecialized<2531>(a);
    case 370:
        return radicalInverseSpecialized<2539>(a);
    case 371:
        return radicalInverseSpecialized<2543>(a);
    case 372:
        return radicalInverseSpecialized<2549>(a);
    case 373:
        return radicalInverseSpecialized<2551>(a);
    case 374:
        return radicalInverseSpecialized<2557>(a);
    case 375:
        return radicalInverseSpecialized<2579>(a);
    case 376:
        return radicalInverseSpecialized<2591>(a);
    case 377:
        return radicalInverseSpecialized<2593>(a);
    case 378:
        return radicalInverseSpecialized<2609>(a);
    case 379:
        return radicalInverseSpecialized<2617>(a);
    case 380:
        return radicalInverseSpecialized<2621>(a);
    case 381:
        return radicalInverseSpecialized<2633>(a);
    case 382:
        return radicalInverseSpecialized<2647>(a);
    case 383:
        return radicalInverseSpecialized<2657>(a);
    case 384:
        return radicalInverseSpecialized<2659>(a);
    case 385:
        return radicalInverseSpecialized<2663>(a);
    case 386:
        return radicalInverseSpecialized<2671>(a);
    case 387:
        return radicalInverseSpecialized<2677>(a);
    case 388:
        return radicalInverseSpecialized<2683>(a);
    case 389:
        return radicalInverseSpecialized<2687>(a);
    case 390:
        return radicalInverseSpecialized<2689>(a);
    case 391:
        return radicalInverseSpecialized<2693>(a);
    case 392:
        return radicalInverseSpecialized<2699>(a);
    case 393:
        return radicalInverseSpecialized<2707>(a);
    case 394:
        return radicalInverseSpecialized<2711>(a);
    case 395:
        return radicalInverseSpecialized<2713>(a);
    case 396:
        return radicalInverseSpecialized<2719>(a);
    case 397:
        return radicalInverseSpecialized<2729>(a);
    case 398:
        return radicalInverseSpecialized<2731>(a);
    case 399:
        return radicalInverseSpecialized<2741>(a);
    case 400:
        return radicalInverseSpecialized<2749>(a);
    case 401:
        return radicalInverseSpecialized<2753>(a);
    case 402:
        return radicalInverseSpecialized<2767>(a);
    case 403:
        return radicalInverseSpecialized<2777>(a);
    case 404:
        return radicalInverseSpecialized<2789>(a);
    case 405:
        return radicalInverseSpecialized<2791>(a);
    case 406:
        return radicalInverseSpecialized<2797>(a);
    case 407:
        return radicalInverseSpecialized<2801>(a);
    case 408:
        return radicalInverseSpecialized<2803>(a);
    case 409:
        return radicalInverseSpecialized<2819>(a);
    case 410:
        return radicalInverseSpecialized<2833>(a);
    case 411:
        return radicalInverseSpecialized<2837>(a);
    case 412:
        return radicalInverseSpecialized<2843>(a);
    case 413:
        return radicalInverseSpecialized<2851>(a);
    case 414:
        return radicalInverseSpecialized<2857>(a);
    case 415:
        return radicalInverseSpecialized<2861>(a);
    case 416:
        return radicalInverseSpecialized<2879>(a);
    case 417:
        return radicalInverseSpecialized<2887>(a);
    case 418:
        return radicalInverseSpecialized<2897>(a);
    case 419:
        return radicalInverseSpecialized<2903>(a);
    case 420:
        return radicalInverseSpecialized<2909>(a);
    case 421:
        return radicalInverseSpecialized<2917>(a);
    case 422:
        return radicalInverseSpecialized<2927>(a);
    case 423:
        return radicalInverseSpecialized<2939>(a);
    case 424:
        return radicalInverseSpecialized<2953>(a);
    case 425:
        return radicalInverseSpecialized<2957>(a);
    case 426:
        return radicalInverseSpecialized<2963>(a);
    case 427:
        return radicalInverseSpecialized<2969>(a);
    case 428:
        return radicalInverseSpecialized<2971>(a);
    case 429:
        return radicalInverseSpecialized<2999>(a);
    case 430:
        return radicalInverseSpecialized<3001>(a);
    case 431:
        return radicalInverseSpecialized<3011>(a);
    case 432:
        return radicalInverseSpecialized<3019>(a);
    case 433:
        return radicalInverseSpecialized<3023>(a);
    case 434:
        return radicalInverseSpecialized<3037>(a);
    case 435:
        return radicalInverseSpecialized<3041>(a);
    case 436:
        return radicalInverseSpecialized<3049>(a);
    case 437:
        return radicalInverseSpecialized<3061>(a);
    case 438:
        return radicalInverseSpecialized<3067>(a);
    case 439:
        return radicalInverseSpecialized<3079>(a);
    case 440:
        return radicalInverseSpecialized<3083>(a);
    case 441:
        return radicalInverseSpecialized<3089>(a);
    case 442:
        return radicalInverseSpecialized<3109>(a);
    case 443:
        return radicalInverseSpecialized<3119>(a);
    case 444:
        return radicalInverseSpecialized<3121>(a);
    case 445:
        return radicalInverseSpecialized<3137>(a);
    case 446:
        return radicalInverseSpecialized<3163>(a);
    case 447:
        return radicalInverseSpecialized<3167>(a);
    case 448:
        return radicalInverseSpecialized<3169>(a);
    case 449:
        return radicalInverseSpecialized<3181>(a);
    case 450:
        return radicalInverseSpecialized<3187>(a);
    case 451:
        return radicalInverseSpecialized<3191>(a);
    case 452:
        return radicalInverseSpecialized<3203>(a);
    case 453:
        return radicalInverseSpecialized<3209>(a);
    case 454:
        return radicalInverseSpecialized<3217>(a);
    case 455:
        return radicalInverseSpecialized<3221>(a);
    case 456:
        return radicalInverseSpecialized<3229>(a);
    case 457:
        return radicalInverseSpecialized<3251>(a);
    case 458:
        return radicalInverseSpecialized<3253>(a);
    case 459:
        return radicalInverseSpecialized<3257>(a);
    case 460:
        return radicalInverseSpecialized<3259>(a);
    case 461:
        return radicalInverseSpecialized<3271>(a);
    case 462:
        return radicalInverseSpecialized<3299>(a);
    case 463:
        return radicalInverseSpecialized<3301>(a);
    case 464:
        return radicalInverseSpecialized<3307>(a);
    case 465:
        return radicalInverseSpecialized<3313>(a);
    case 466:
        return radicalInverseSpecialized<3319>(a);
    case 467:
        return radicalInverseSpecialized<3323>(a);
    case 468:
        return radicalInverseSpecialized<3329>(a);
    case 469:
        return radicalInverseSpecialized<3331>(a);
    case 470:
        return radicalInverseSpecialized<3343>(a);
    case 471:
        return radicalInverseSpecialized<3347>(a);
    case 472:
        return radicalInverseSpecialized<3359>(a);
    case 473:
        return radicalInverseSpecialized<3361>(a);
    case 474:
        return radicalInverseSpecialized<3371>(a);
    case 475:
        return radicalInverseSpecialized<3373>(a);
    case 476:
        return radicalInverseSpecialized<3389>(a);
    case 477:
        return radicalInverseSpecialized<3391>(a);
    case 478:
        return radicalInverseSpecialized<3407>(a);
    case 479:
        return radicalInverseSpecialized<3413>(a);
    case 480:
        return radicalInverseSpecialized<3433>(a);
    case 481:
        return radicalInverseSpecialized<3449>(a);
    case 482:
        return radicalInverseSpecialized<3457>(a);
    case 483:
        return radicalInverseSpecialized<3461>(a);
    case 484:
        return radicalInverseSpecialized<3463>(a);
    case 485:
        return radicalInverseSpecialized<3467>(a);
    case 486:
        return radicalInverseSpecialized<3469>(a);
    case 487:
        return radicalInverseSpecialized<3491>(a);
    case 488:
        return radicalInverseSpecialized<3499>(a);
    case 489:
        return radicalInverseSpecialized<3511>(a);
    case 490:
        return radicalInverseSpecialized<3517>(a);
    case 491:
        return radicalInverseSpecialized<3527>(a);
    case 492:
        return radicalInverseSpecialized<3529>(a);
    case 493:
        return radicalInverseSpecialized<3533>(a);
    case 494:
        return radicalInverseSpecialized<3539>(a);
    case 495:
        return radicalInverseSpecialized<3541>(a);
    case 496:
        return radicalInverseSpecialized<3547>(a);
    case 497:
        return radicalInverseSpecialized<3557>(a);
    case 498:
        return radicalInverseSpecialized<3559>(a);
    case 499:
        return radicalInverseSpecialized<3571>(a);
    case 500:
        return radicalInverseSpecialized<3581>(a);
    case 501:
        return radicalInverseSpecialized<3583>(a);
    case 502:
        return radicalInverseSpecialized<3593>(a);
    case 503:
        return radicalInverseSpecialized<3607>(a);
    case 504:
        return radicalInverseSpecialized<3613>(a);
    case 505:
        return radicalInverseSpecialized<3617>(a);
    case 506:
        return radicalInverseSpecialized<3623>(a);
    case 507:
        return radicalInverseSpecialized<3631>(a);
    case 508:
        return radicalInverseSpecialized<3637>(a);
    case 509:
        return radicalInverseSpecialized<3643>(a);
    case 510:
        return radicalInverseSpecialized<3659>(a);
    case 511:
        return radicalInverseSpecialized<3671>(a);
    case 512:
        return radicalInverseSpecialized<3673>(a);
    case 513:
        return radicalInverseSpecialized<3677>(a);
    case 514:
        return radicalInverseSpecialized<3691>(a);
    case 515:
        return radicalInverseSpecialized<3697>(a);
    case 516:
        return radicalInverseSpecialized<3701>(a);
    case 517:
        return radicalInverseSpecialized<3709>(a);
    case 518:
        return radicalInverseSpecialized<3719>(a);
    case 519:
        return radicalInverseSpecialized<3727>(a);
    case 520:
        return radicalInverseSpecialized<3733>(a);
    case 521:
        return radicalInverseSpecialized<3739>(a);
    case 522:
        return radicalInverseSpecialized<3761>(a);
    case 523:
        return radicalInverseSpecialized<3767>(a);
    case 524:
        return radicalInverseSpecialized<3769>(a);
    case 525:
        return radicalInverseSpecialized<3779>(a);
    case 526:
        return radicalInverseSpecialized<3793>(a);
    case 527:
        return radicalInverseSpecialized<3797>(a);
    case 528:
        return radicalInverseSpecialized<3803>(a);
    case 529:
        return radicalInverseSpecialized<3821>(a);
    case 530:
        return radicalInverseSpecialized<3823>(a);
    case 531:
        return radicalInverseSpecialized<3833>(a);
    case 532:
        return radicalInverseSpecialized<3847>(a);
    case 533:
        return radicalInverseSpecialized<3851>(a);
    case 534:
        return radicalInverseSpecialized<3853>(a);
    case 535:
        return radicalInverseSpecialized<3863>(a);
    case 536:
        return radicalInverseSpecialized<3877>(a);
    case 537:
        return radicalInverseSpecialized<3881>(a);
    case 538:
        return radicalInverseSpecialized<3889>(a);
    case 539:
        return radicalInverseSpecialized<3907>(a);
    case 540:
        return radicalInverseSpecialized<3911>(a);
    case 541:
        return radicalInverseSpecialized<3917>(a);
    case 542:
        return radicalInverseSpecialized<3919>(a);
    case 543:
        return radicalInverseSpecialized<3923>(a);
    case 544:
        return radicalInverseSpecialized<3929>(a);
    case 545:
        return radicalInverseSpecialized<3931>(a);
    case 546:
        return radicalInverseSpecialized<3943>(a);
    case 547:
        return radicalInverseSpecialized<3947>(a);
    case 548:
        return radicalInverseSpecialized<3967>(a);
    case 549:
        return radicalInverseSpecialized<3989>(a);
    case 550:
        return radicalInverseSpecialized<4001>(a);
    case 551:
        return radicalInverseSpecialized<4003>(a);
    case 552:
        return radicalInverseSpecialized<4007>(a);
    case 553:
        return radicalInverseSpecialized<4013>(a);
    case 554:
        return radicalInverseSpecialized<4019>(a);
    case 555:
        return radicalInverseSpecialized<4021>(a);
    case 556:
        return radicalInverseSpecialized<4027>(a);
    case 557:
        return radicalInverseSpecialized<4049>(a);
    case 558:
        return radicalInverseSpecialized<4051>(a);
    case 559:
        return radicalInverseSpecialized<4057>(a);
    case 560:
        return radicalInverseSpecialized<4073>(a);
    case 561:
        return radicalInverseSpecialized<4079>(a);
    case 562:
        return radicalInverseSpecialized<4091>(a);
    case 563:
        return radicalInverseSpecialized<4093>(a);
    case 564:
        return radicalInverseSpecialized<4099>(a);
    case 565:
        return radicalInverseSpecialized<4111>(a);
    case 566:
        return radicalInverseSpecialized<4127>(a);
    case 567:
        return radicalInverseSpecialized<4129>(a);
    case 568:
        return radicalInverseSpecialized<4133>(a);
    case 569:
        return radicalInverseSpecialized<4139>(a);
    case 570:
        return radicalInverseSpecialized<4153>(a);
    case 571:
        return radicalInverseSpecialized<4157>(a);
    case 572:
        return radicalInverseSpecialized<4159>(a);
    case 573:
        return radicalInverseSpecialized<4177>(a);
    case 574:
        return radicalInverseSpecialized<4201>(a);
    case 575:
        return radicalInverseSpecialized<4211>(a);
    case 576:
        return radicalInverseSpecialized<4217>(a);
    case 577:
        return radicalInverseSpecialized<4219>(a);
    case 578:
        return radicalInverseSpecialized<4229>(a);
    case 579:
        return radicalInverseSpecialized<4231>(a);
    case 580:
        return radicalInverseSpecialized<4241>(a);
    case 581:
        return radicalInverseSpecialized<4243>(a);
    case 582:
        return radicalInverseSpecialized<4253>(a);
    case 583:
        return radicalInverseSpecialized<4259>(a);
    case 584:
        return radicalInverseSpecialized<4261>(a);
    case 585:
        return radicalInverseSpecialized<4271>(a);
    case 586:
        return radicalInverseSpecialized<4273>(a);
    case 587:
        return radicalInverseSpecialized<4283>(a);
    case 588:
        return radicalInverseSpecialized<4289>(a);
    case 589:
        return radicalInverseSpecialized<4297>(a);
    case 590:
        return radicalInverseSpecialized<4327>(a);
    case 591:
        return radicalInverseSpecialized<4337>(a);
    case 592:
        return radicalInverseSpecialized<4339>(a);
    case 593:
        return radicalInverseSpecialized<4349>(a);
    case 594:
        return radicalInverseSpecialized<4357>(a);
    case 595:
        return radicalInverseSpecialized<4363>(a);
    case 596:
        return radicalInverseSpecialized<4373>(a);
    case 597:
        return radicalInverseSpecialized<4391>(a);
    case 598:
        return radicalInverseSpecialized<4397>(a);
    case 599:
        return radicalInverseSpecialized<4409>(a);
    case 600:
        return radicalInverseSpecialized<4421>(a);
    case 601:
        return radicalInverseSpecialized<4423>(a);
    case 602:
        return radicalInverseSpecialized<4441>(a);
    case 603:
        return radicalInverseSpecialized<4447>(a);
    case 604:
        return radicalInverseSpecialized<4451>(a);
    case 605:
        return radicalInverseSpecialized<4457>(a);
    case 606:
        return radicalInverseSpecialized<4463>(a);
    case 607:
        return radicalInverseSpecialized<4481>(a);
    case 608:
        return radicalInverseSpecialized<4483>(a);
    case 609:
        return radicalInverseSpecialized<4493>(a);
    case 610:
        return radicalInverseSpecialized<4507>(a);
    case 611:
        return radicalInverseSpecialized<4513>(a);
    case 612:
        return radicalInverseSpecialized<4517>(a);
    case 613:
        return radicalInverseSpecialized<4519>(a);
    case 614:
        return radicalInverseSpecialized<4523>(a);
    case 615:
        return radicalInverseSpecialized<4547>(a);
    case 616:
        return radicalInverseSpecialized<4549>(a);
    case 617:
        return radicalInverseSpecialized<4561>(a);
    case 618:
        return radicalInverseSpecialized<4567>(a);
    case 619:
        return radicalInverseSpecialized<4583>(a);
    case 620:
        return radicalInverseSpecialized<4591>(a);
    case 621:
        return radicalInverseSpecialized<4597>(a);
    case 622:
        return radicalInverseSpecialized<4603>(a);
    case 623:
        return radicalInverseSpecialized<4621>(a);
    case 624:
        return radicalInverseSpecialized<4637>(a);
    case 625:
        return radicalInverseSpecialized<4639>(a);
    case 626:
        return radicalInverseSpecialized<4643>(a);
    case 627:
        return radicalInverseSpecialized<4649>(a);
    case 628:
        return radicalInverseSpecialized<4651>(a);
    case 629:
        return radicalInverseSpecialized<4657>(a);
    case 630:
        return radicalInverseSpecialized<4663>(a);
    case 631:
        return radicalInverseSpecialized<4673>(a);
    case 632:
        return radicalInverseSpecialized<4679>(a);
    case 633:
        return radicalInverseSpecialized<4691>(a);
    case 634:
        return radicalInverseSpecialized<4703>(a);
    case 635:
        return radicalInverseSpecialized<4721>(a);
    case 636:
        return radicalInverseSpecialized<4723>(a);
    case 637:
        return radicalInverseSpecialized<4729>(a);
    case 638:
        return radicalInverseSpecialized<4733>(a);
    case 639:
        return radicalInverseSpecialized<4751>(a);
    case 640:
        return radicalInverseSpecialized<4759>(a);
    case 641:
        return radicalInverseSpecialized<4783>(a);
    case 642:
        return radicalInverseSpecialized<4787>(a);
    case 643:
        return radicalInverseSpecialized<4789>(a);
    case 644:
        return radicalInverseSpecialized<4793>(a);
    case 645:
        return radicalInverseSpecialized<4799>(a);
    case 646:
        return radicalInverseSpecialized<4801>(a);
    case 647:
        return radicalInverseSpecialized<4813>(a);
    case 648:
        return radicalInverseSpecialized<4817>(a);
    case 649:
        return radicalInverseSpecialized<4831>(a);
    case 650:
        return radicalInverseSpecialized<4861>(a);
    case 651:
        return radicalInverseSpecialized<4871>(a);
    case 652:
        return radicalInverseSpecialized<4877>(a);
    case 653:
        return radicalInverseSpecialized<4889>(a);
    case 654:
        return radicalInverseSpecialized<4903>(a);
    case 655:
        return radicalInverseSpecialized<4909>(a);
    case 656:
        return radicalInverseSpecialized<4919>(a);
    case 657:
        return radicalInverseSpecialized<4931>(a);
    case 658:
        return radicalInverseSpecialized<4933>(a);
    case 659:
        return radicalInverseSpecialized<4937>(a);
    case 660:
        return radicalInverseSpecialized<4943>(a);
    case 661:
        return radicalInverseSpecialized<4951>(a);
    case 662:
        return radicalInverseSpecialized<4957>(a);
    case 663:
        return radicalInverseSpecialized<4967>(a);
    case 664:
        return radicalInverseSpecialized<4969>(a);
    case 665:
        return radicalInverseSpecialized<4973>(a);
    case 666:
        return radicalInverseSpecialized<4987>(a);
    case 667:
        return radicalInverseSpecialized<4993>(a);
    case 668:
        return radicalInverseSpecialized<4999>(a);
    case 669:
        return radicalInverseSpecialized<5003>(a);
    case 670:
        return radicalInverseSpecialized<5009>(a);
    case 671:
        return radicalInverseSpecialized<5011>(a);
    case 672:
        return radicalInverseSpecialized<5021>(a);
    case 673:
        return radicalInverseSpecialized<5023>(a);
    case 674:
        return radicalInverseSpecialized<5039>(a);
    case 675:
        return radicalInverseSpecialized<5051>(a);
    case 676:
        return radicalInverseSpecialized<5059>(a);
    case 677:
        return radicalInverseSpecialized<5077>(a);
    case 678:
        return radicalInverseSpecialized<5081>(a);
    case 679:
        return radicalInverseSpecialized<5087>(a);
    case 680:
        return radicalInverseSpecialized<5099>(a);
    case 681:
        return radicalInverseSpecialized<5101>(a);
    case 682:
        return radicalInverseSpecialized<5107>(a);
    case 683:
        return radicalInverseSpecialized<5113>(a);
    case 684:
        return radicalInverseSpecialized<5119>(a);
    case 685:
        return radicalInverseSpecialized<5147>(a);
    case 686:
        return radicalInverseSpecialized<5153>(a);
    case 687:
        return radicalInverseSpecialized<5167>(a);
    case 688:
        return radicalInverseSpecialized<5171>(a);
    case 689:
        return radicalInverseSpecialized<5179>(a);
    case 690:
        return radicalInverseSpecialized<5189>(a);
    case 691:
        return radicalInverseSpecialized<5197>(a);
    case 692:
        return radicalInverseSpecialized<5209>(a);
    case 693:
        return radicalInverseSpecialized<5227>(a);
    case 694:
        return radicalInverseSpecialized<5231>(a);
    case 695:
        return radicalInverseSpecialized<5233>(a);
    case 696:
        return radicalInverseSpecialized<5237>(a);
    case 697:
        return radicalInverseSpecialized<5261>(a);
    case 698:
        return radicalInverseSpecialized<5273>(a);
    case 699:
        return radicalInverseSpecialized<5279>(a);
    case 700:
        return radicalInverseSpecialized<5281>(a);
    case 701:
        return radicalInverseSpecialized<5297>(a);
    case 702:
        return radicalInverseSpecialized<5303>(a);
    case 703:
        return radicalInverseSpecialized<5309>(a);
    case 704:
        return radicalInverseSpecialized<5323>(a);
    case 705:
        return radicalInverseSpecialized<5333>(a);
    case 706:
        return radicalInverseSpecialized<5347>(a);
    case 707:
        return radicalInverseSpecialized<5351>(a);
    case 708:
        return radicalInverseSpecialized<5381>(a);
    case 709:
        return radicalInverseSpecialized<5387>(a);
    case 710:
        return radicalInverseSpecialized<5393>(a);
    case 711:
        return radicalInverseSpecialized<5399>(a);
    case 712:
        return radicalInverseSpecialized<5407>(a);
    case 713:
        return radicalInverseSpecialized<5413>(a);
    case 714:
        return radicalInverseSpecialized<5417>(a);
    case 715:
        return radicalInverseSpecialized<5419>(a);
    case 716:
        return radicalInverseSpecialized<5431>(a);
    case 717:
        return radicalInverseSpecialized<5437>(a);
    case 718:
        return radicalInverseSpecialized<5441>(a);
    case 719:
        return radicalInverseSpecialized<5443>(a);
    case 720:
        return radicalInverseSpecialized<5449>(a);
    case 721:
        return radicalInverseSpecialized<5471>(a);
    case 722:
        return radicalInverseSpecialized<5477>(a);
    case 723:
        return radicalInverseSpecialized<5479>(a);
    case 724:
        return radicalInverseSpecialized<5483>(a);
    case 725:
        return radicalInverseSpecialized<5501>(a);
    case 726:
        return radicalInverseSpecialized<5503>(a);
    case 727:
        return radicalInverseSpecialized<5507>(a);
    case 728:
        return radicalInverseSpecialized<5519>(a);
    case 729:
        return radicalInverseSpecialized<5521>(a);
    case 730:
        return radicalInverseSpecialized<5527>(a);
    case 731:
        return radicalInverseSpecialized<5531>(a);
    case 732:
        return radicalInverseSpecialized<5557>(a);
    case 733:
        return radicalInverseSpecialized<5563>(a);
    case 734:
        return radicalInverseSpecialized<5569>(a);
    case 735:
        return radicalInverseSpecialized<5573>(a);
    case 736:
        return radicalInverseSpecialized<5581>(a);
    case 737:
        return radicalInverseSpecialized<5591>(a);
    case 738:
        return radicalInverseSpecialized<5623>(a);
    case 739:
        return radicalInverseSpecialized<5639>(a);
    case 740:
        return radicalInverseSpecialized<5641>(a);
    case 741:
        return radicalInverseSpecialized<5647>(a);
    case 742:
        return radicalInverseSpecialized<5651>(a);
    case 743:
        return radicalInverseSpecialized<5653>(a);
    case 744:
        return radicalInverseSpecialized<5657>(a);
    case 745:
        return radicalInverseSpecialized<5659>(a);
    case 746:
        return radicalInverseSpecialized<5669>(a);
    case 747:
        return radicalInverseSpecialized<5683>(a);
    case 748:
        return radicalInverseSpecialized<5689>(a);
    case 749:
        return radicalInverseSpecialized<5693>(a);
    case 750:
        return radicalInverseSpecialized<5701>(a);
    case 751:
        return radicalInverseSpecialized<5711>(a);
    case 752:
        return radicalInverseSpecialized<5717>(a);
    case 753:
        return radicalInverseSpecialized<5737>(a);
    case 754:
        return radicalInverseSpecialized<5741>(a);
    case 755:
        return radicalInverseSpecialized<5743>(a);
    case 756:
        return radicalInverseSpecialized<5749>(a);
    case 757:
        return radicalInverseSpecialized<5779>(a);
    case 758:
        return radicalInverseSpecialized<5783>(a);
    case 759:
        return radicalInverseSpecialized<5791>(a);
    case 760:
        return radicalInverseSpecialized<5801>(a);
    case 761:
        return radicalInverseSpecialized<5807>(a);
    case 762:
        return radicalInverseSpecialized<5813>(a);
    case 763:
        return radicalInverseSpecialized<5821>(a);
    case 764:
        return radicalInverseSpecialized<5827>(a);
    case 765:
        return radicalInverseSpecialized<5839>(a);
    case 766:
        return radicalInverseSpecialized<5843>(a);
    case 767:
        return radicalInverseSpecialized<5849>(a);
    case 768:
        return radicalInverseSpecialized<5851>(a);
    case 769:
        return radicalInverseSpecialized<5857>(a);
    case 770:
        return radicalInverseSpecialized<5861>(a);
    case 771:
        return radicalInverseSpecialized<5867>(a);
    case 772:
        return radicalInverseSpecialized<5869>(a);
    case 773:
        return radicalInverseSpecialized<5879>(a);
    case 774:
        return radicalInverseSpecialized<5881>(a);
    case 775:
        return radicalInverseSpecialized<5897>(a);
    case 776:
        return radicalInverseSpecialized<5903>(a);
    case 777:
        return radicalInverseSpecialized<5923>(a);
    case 778:
        return radicalInverseSpecialized<5927>(a);
    case 779:
        return radicalInverseSpecialized<5939>(a);
    case 780:
        return radicalInverseSpecialized<5953>(a);
    case 781:
        return radicalInverseSpecialized<5981>(a);
    case 782:
        return radicalInverseSpecialized<5987>(a);
    case 783:
        return radicalInverseSpecialized<6007>(a);
    case 784:
        return radicalInverseSpecialized<6011>(a);
    case 785:
        return radicalInverseSpecialized<6029>(a);
    case 786:
        return radicalInverseSpecialized<6037>(a);
    case 787:
        return radicalInverseSpecialized<6043>(a);
    case 788:
        return radicalInverseSpecialized<6047>(a);
    case 789:
        return radicalInverseSpecialized<6053>(a);
    case 790:
        return radicalInverseSpecialized<6067>(a);
    case 791:
        return radicalInverseSpecialized<6073>(a);
    case 792:
        return radicalInverseSpecialized<6079>(a);
    case 793:
        return radicalInverseSpecialized<6089>(a);
    case 794:
        return radicalInverseSpecialized<6091>(a);
    case 795:
        return radicalInverseSpecialized<6101>(a);
    case 796:
        return radicalInverseSpecialized<6113>(a);
    case 797:
        return radicalInverseSpecialized<6121>(a);
    case 798:
        return radicalInverseSpecialized<6131>(a);
    case 799:
        return radicalInverseSpecialized<6133>(a);
    case 800:
        return radicalInverseSpecialized<6143>(a);
    case 801:
        return radicalInverseSpecialized<6151>(a);
    case 802:
        return radicalInverseSpecialized<6163>(a);
    case 803:
        return radicalInverseSpecialized<6173>(a);
    case 804:
        return radicalInverseSpecialized<6197>(a);
    case 805:
        return radicalInverseSpecialized<6199>(a);
    case 806:
        return radicalInverseSpecialized<6203>(a);
    case 807:
        return radicalInverseSpecialized<6211>(a);
    case 808:
        return radicalInverseSpecialized<6217>(a);
    case 809:
        return radicalInverseSpecialized<6221>(a);
    case 810:
        return radicalInverseSpecialized<6229>(a);
    case 811:
        return radicalInverseSpecialized<6247>(a);
    case 812:
        return radicalInverseSpecialized<6257>(a);
    case 813:
        return radicalInverseSpecialized<6263>(a);
    case 814:
        return radicalInverseSpecialized<6269>(a);
    case 815:
        return radicalInverseSpecialized<6271>(a);
    case 816:
        return radicalInverseSpecialized<6277>(a);
    case 817:
        return radicalInverseSpecialized<6287>(a);
    case 818:
        return radicalInverseSpecialized<6299>(a);
    case 819:
        return radicalInverseSpecialized<6301>(a);
    case 820:
        return radicalInverseSpecialized<6311>(a);
    case 821:
        return radicalInverseSpecialized<6317>(a);
    case 822:
        return radicalInverseSpecialized<6323>(a);
    case 823:
        return radicalInverseSpecialized<6329>(a);
    case 824:
        return radicalInverseSpecialized<6337>(a);
    case 825:
        return radicalInverseSpecialized<6343>(a);
    case 826:
        return radicalInverseSpecialized<6353>(a);
    case 827:
        return radicalInverseSpecialized<6359>(a);
    case 828:
        return radicalInverseSpecialized<6361>(a);
    case 829:
        return radicalInverseSpecialized<6367>(a);
    case 830:
        return radicalInverseSpecialized<6373>(a);
    case 831:
        return radicalInverseSpecialized<6379>(a);
    case 832:
        return radicalInverseSpecialized<6389>(a);
    case 833:
        return radicalInverseSpecialized<6397>(a);
    case 834:
        return radicalInverseSpecialized<6421>(a);
    case 835:
        return radicalInverseSpecialized<6427>(a);
    case 836:
        return radicalInverseSpecialized<6449>(a);
    case 837:
        return radicalInverseSpecialized<6451>(a);
    case 838:
        return radicalInverseSpecialized<6469>(a);
    case 839:
        return radicalInverseSpecialized<6473>(a);
    case 840:
        return radicalInverseSpecialized<6481>(a);
    case 841:
        return radicalInverseSpecialized<6491>(a);
    case 842:
        return radicalInverseSpecialized<6521>(a);
    case 843:
        return radicalInverseSpecialized<6529>(a);
    case 844:
        return radicalInverseSpecialized<6547>(a);
    case 845:
        return radicalInverseSpecialized<6551>(a);
    case 846:
        return radicalInverseSpecialized<6553>(a);
    case 847:
        return radicalInverseSpecialized<6563>(a);
    case 848:
        return radicalInverseSpecialized<6569>(a);
    case 849:
        return radicalInverseSpecialized<6571>(a);
    case 850:
        return radicalInverseSpecialized<6577>(a);
    case 851:
        return radicalInverseSpecialized<6581>(a);
    case 852:
        return radicalInverseSpecialized<6599>(a);
    case 853:
        return radicalInverseSpecialized<6607>(a);
    case 854:
        return radicalInverseSpecialized<6619>(a);
    case 855:
        return radicalInverseSpecialized<6637>(a);
    case 856:
        return radicalInverseSpecialized<6653>(a);
    case 857:
        return radicalInverseSpecialized<6659>(a);
    case 858:
        return radicalInverseSpecialized<6661>(a);
    case 859:
        return radicalInverseSpecialized<6673>(a);
    case 860:
        return radicalInverseSpecialized<6679>(a);
    case 861:
        return radicalInverseSpecialized<6689>(a);
    case 862:
        return radicalInverseSpecialized<6691>(a);
    case 863:
        return radicalInverseSpecialized<6701>(a);
    case 864:
        return radicalInverseSpecialized<6703>(a);
    case 865:
        return radicalInverseSpecialized<6709>(a);
    case 866:
        return radicalInverseSpecialized<6719>(a);
    case 867:
        return radicalInverseSpecialized<6733>(a);
    case 868:
        return radicalInverseSpecialized<6737>(a);
    case 869:
        return radicalInverseSpecialized<6761>(a);
    case 870:
        return radicalInverseSpecialized<6763>(a);
    case 871:
        return radicalInverseSpecialized<6779>(a);
    case 872:
        return radicalInverseSpecialized<6781>(a);
    case 873:
        return radicalInverseSpecialized<6791>(a);
    case 874:
        return radicalInverseSpecialized<6793>(a);
    case 875:
        return radicalInverseSpecialized<6803>(a);
    case 876:
        return radicalInverseSpecialized<6823>(a);
    case 877:
        return radicalInverseSpecialized<6827>(a);
    case 878:
        return radicalInverseSpecialized<6829>(a);
    case 879:
        return radicalInverseSpecialized<6833>(a);
    case 880:
        return radicalInverseSpecialized<6841>(a);
    case 881:
        return radicalInverseSpecialized<6857>(a);
    case 882:
        return radicalInverseSpecialized<6863>(a);
    case 883:
        return radicalInverseSpecialized<6869>(a);
    case 884:
        return radicalInverseSpecialized<6871>(a);
    case 885:
        return radicalInverseSpecialized<6883>(a);
    case 886:
        return radicalInverseSpecialized<6899>(a);
    case 887:
        return radicalInverseSpecialized<6907>(a);
    case 888:
        return radicalInverseSpecialized<6911>(a);
    case 889:
        return radicalInverseSpecialized<6917>(a);
    case 890:
        return radicalInverseSpecialized<6947>(a);
    case 891:
        return radicalInverseSpecialized<6949>(a);
    case 892:
        return radicalInverseSpecialized<6959>(a);
    case 893:
        return radicalInverseSpecialized<6961>(a);
    case 894:
        return radicalInverseSpecialized<6967>(a);
    case 895:
        return radicalInverseSpecialized<6971>(a);
    case 896:
        return radicalInverseSpecialized<6977>(a);
    case 897:
        return radicalInverseSpecialized<6983>(a);
    case 898:
        return radicalInverseSpecialized<6991>(a);
    case 899:
        return radicalInverseSpecialized<6997>(a);
    case 900:
        return radicalInverseSpecialized<7001>(a);
    case 901:
        return radicalInverseSpecialized<7013>(a);
    case 902:
        return radicalInverseSpecialized<7019>(a);
    case 903:
        return radicalInverseSpecialized<7027>(a);
    case 904:
        return radicalInverseSpecialized<7039>(a);
    case 905:
        return radicalInverseSpecialized<7043>(a);
    case 906:
        return radicalInverseSpecialized<7057>(a);
    case 907:
        return radicalInverseSpecialized<7069>(a);
    case 908:
        return radicalInverseSpecialized<7079>(a);
    case 909:
        return radicalInverseSpecialized<7103>(a);
    case 910:
        return radicalInverseSpecialized<7109>(a);
    case 911:
        return radicalInverseSpecialized<7121>(a);
    case 912:
        return radicalInverseSpecialized<7127>(a);
    case 913:
        return radicalInverseSpecialized<7129>(a);
    case 914:
        return radicalInverseSpecialized<7151>(a);
    case 915:
        return radicalInverseSpecialized<7159>(a);
    case 916:
        return radicalInverseSpecialized<7177>(a);
    case 917:
        return radicalInverseSpecialized<7187>(a);
    case 918:
        return radicalInverseSpecialized<7193>(a);
    case 919:
        return radicalInverseSpecialized<7207>(a);
    case 920:
        return radicalInverseSpecialized<7211>(a);
    case 921:
        return radicalInverseSpecialized<7213>(a);
    case 922:
        return radicalInverseSpecialized<7219>(a);
    case 923:
        return radicalInverseSpecialized<7229>(a);
    case 924:
        return radicalInverseSpecialized<7237>(a);
    case 925:
        return radicalInverseSpecialized<7243>(a);
    case 926:
        return radicalInverseSpecialized<7247>(a);
    case 927:
        return radicalInverseSpecialized<7253>(a);
    case 928:
        return radicalInverseSpecialized<7283>(a);
    case 929:
        return radicalInverseSpecialized<7297>(a);
    case 930:
        return radicalInverseSpecialized<7307>(a);
    case 931:
        return radicalInverseSpecialized<7309>(a);
    case 932:
        return radicalInverseSpecialized<7321>(a);
    case 933:
        return radicalInverseSpecialized<7331>(a);
    case 934:
        return radicalInverseSpecialized<7333>(a);
    case 935:
        return radicalInverseSpecialized<7349>(a);
    case 936:
        return radicalInverseSpecialized<7351>(a);
    case 937:
        return radicalInverseSpecialized<7369>(a);
    case 938:
        return radicalInverseSpecialized<7393>(a);
    case 939:
        return radicalInverseSpecialized<7411>(a);
    case 940:
        return radicalInverseSpecialized<7417>(a);
    case 941:
        return radicalInverseSpecialized<7433>(a);
    case 942:
        return radicalInverseSpecialized<7451>(a);
    case 943:
        return radicalInverseSpecialized<7457>(a);
    case 944:
        return radicalInverseSpecialized<7459>(a);
    case 945:
        return radicalInverseSpecialized<7477>(a);
    case 946:
        return radicalInverseSpecialized<7481>(a);
    case 947:
        return radicalInverseSpecialized<7487>(a);
    case 948:
        return radicalInverseSpecialized<7489>(a);
    case 949:
        return radicalInverseSpecialized<7499>(a);
    case 950:
        return radicalInverseSpecialized<7507>(a);
    case 951:
        return radicalInverseSpecialized<7517>(a);
    case 952:
        return radicalInverseSpecialized<7523>(a);
    case 953:
        return radicalInverseSpecialized<7529>(a);
    case 954:
        return radicalInverseSpecialized<7537>(a);
    case 955:
        return radicalInverseSpecialized<7541>(a);
    case 956:
        return radicalInverseSpecialized<7547>(a);
    case 957:
        return radicalInverseSpecialized<7549>(a);
    case 958:
        return radicalInverseSpecialized<7559>(a);
    case 959:
        return radicalInverseSpecialized<7561>(a);
    case 960:
        return radicalInverseSpecialized<7573>(a);
    case 961:
        return radicalInverseSpecialized<7577>(a);
    case 962:
        return radicalInverseSpecialized<7583>(a);
    case 963:
        return radicalInverseSpecialized<7589>(a);
    case 964:
        return radicalInverseSpecialized<7591>(a);
    case 965:
        return radicalInverseSpecialized<7603>(a);
    case 966:
        return radicalInverseSpecialized<7607>(a);
    case 967:
        return radicalInverseSpecialized<7621>(a);
    case 968:
        return radicalInverseSpecialized<7639>(a);
    case 969:
        return radicalInverseSpecialized<7643>(a);
    case 970:
        return radicalInverseSpecialized<7649>(a);
    case 971:
        return radicalInverseSpecialized<7669>(a);
    case 972:
        return radicalInverseSpecialized<7673>(a);
    case 973:
        return radicalInverseSpecialized<7681>(a);
    case 974:
        return radicalInverseSpecialized<7687>(a);
    case 975:
        return radicalInverseSpecialized<7691>(a);
    case 976:
        return radicalInverseSpecialized<7699>(a);
    case 977:
        return radicalInverseSpecialized<7703>(a);
    case 978:
        return radicalInverseSpecialized<7717>(a);
    case 979:
        return radicalInverseSpecialized<7723>(a);
    case 980:
        return radicalInverseSpecialized<7727>(a);
    case 981:
        return radicalInverseSpecialized<7741>(a);
    case 982:
        return radicalInverseSpecialized<7753>(a);
    case 983:
        return radicalInverseSpecialized<7757>(a);
    case 984:
        return radicalInverseSpecialized<7759>(a);
    case 985:
        return radicalInverseSpecialized<7789>(a);
    case 986:
        return radicalInverseSpecialized<7793>(a);
    case 987:
        return radicalInverseSpecialized<7817>(a);
    case 988:
        return radicalInverseSpecialized<7823>(a);
    case 989:
        return radicalInverseSpecialized<7829>(a);
    case 990:
        return radicalInverseSpecialized<7841>(a);
    case 991:
        return radicalInverseSpecialized<7853>(a);
    case 992:
        return radicalInverseSpecialized<7867>(a);
    case 993:
        return radicalInverseSpecialized<7873>(a);
    case 994:
        return radicalInverseSpecialized<7877>(a);
    case 995:
        return radicalInverseSpecialized<7879>(a);
    case 996:
        return radicalInverseSpecialized<7883>(a);
    case 997:
        return radicalInverseSpecialized<7901>(a);
    case 998:
        return radicalInverseSpecialized<7907>(a);
    case 999:
        return radicalInverseSpecialized<7919>(a);
    case 1000:
        return radicalInverseSpecialized<7927>(a);
    case 1001:
        return radicalInverseSpecialized<7933>(a);
    case 1002:
        return radicalInverseSpecialized<7937>(a);
    case 1003:
        return radicalInverseSpecialized<7949>(a);
    case 1004:
        return radicalInverseSpecialized<7951>(a);
    case 1005:
        return radicalInverseSpecialized<7963>(a);
    case 1006:
        return radicalInverseSpecialized<7993>(a);
    case 1007:
        return radicalInverseSpecialized<8009>(a);
    case 1008:
        return radicalInverseSpecialized<8011>(a);
    case 1009:
        return radicalInverseSpecialized<8017>(a);
    case 1010:
        return radicalInverseSpecialized<8039>(a);
    case 1011:
        return radicalInverseSpecialized<8053>(a);
    case 1012:
        return radicalInverseSpecialized<8059>(a);
    case 1013:
        return radicalInverseSpecialized<8069>(a);
    case 1014:
        return radicalInverseSpecialized<8081>(a);
    case 1015:
        return radicalInverseSpecialized<8087>(a);
    case 1016:
        return radicalInverseSpecialized<8089>(a);
    case 1017:
        return radicalInverseSpecialized<8093>(a);
    case 1018:
        return radicalInverseSpecialized<8101>(a);
    case 1019:
        return radicalInverseSpecialized<8111>(a);
    case 1020:
        return radicalInverseSpecialized<8117>(a);
    case 1021:
        return radicalInverseSpecialized<8123>(a);
    case 1022:
        return radicalInverseSpecialized<8147>(a);
    case 1023:
        return radicalInverseSpecialized<8161>(a);
    default:
        return 0;
    }
}

std::vector<uint16_t> computeRadicalInversePermutations(RNG& rng) {
    std::vector<uint16_t> perms;
    // Allocate space in _perms_ for radical inverse permutations
    int permArraySize = 0;
    for (int i = 0; i < PrimeTableSize; ++i) permArraySize += Primes[i];
    perms.resize(permArraySize);

    uint16_t *p = &perms[0];
    for (int i = 0; i < PrimeTableSize; ++i) {
        // Generate random permutation for $i$th prime base
        for (int j = 0; j < Primes[i]; ++j) p[j] = j;
        shuffle(p, Primes[i], 1, rng);
        p += Primes[i];
    }

    return perms;
}

Real scrambledRadicalInverse(int baseIndex, uint64_t a, const uint16_t* perm) {
    switch (baseIndex) {
    case 0:
        return scrambledRadicalInverseSpecialized<2>(perm, a);
    case 1:
        return scrambledRadicalInverseSpecialized<3>(perm, a);
    case 2:
        return scrambledRadicalInverseSpecialized<5>(perm, a);
    case 3:
        return scrambledRadicalInverseSpecialized<7>(perm, a);
    // Remainder of cases for _ScrambledRadicalInverse()_
    case 4:
        return scrambledRadicalInverseSpecialized<11>(perm, a);
    case 5:
        return scrambledRadicalInverseSpecialized<13>(perm, a);
    case 6:
        return scrambledRadicalInverseSpecialized<17>(perm, a);
    case 7:
        return scrambledRadicalInverseSpecialized<19>(perm, a);
    case 8:
        return scrambledRadicalInverseSpecialized<23>(perm, a);
    case 9:
        return scrambledRadicalInverseSpecialized<29>(perm, a);
    case 10:
        return scrambledRadicalInverseSpecialized<31>(perm, a);
    case 11:
        return scrambledRadicalInverseSpecialized<37>(perm, a);
    case 12:
        return scrambledRadicalInverseSpecialized<41>(perm, a);
    case 13:
        return scrambledRadicalInverseSpecialized<43>(perm, a);
    case 14:
        return scrambledRadicalInverseSpecialized<47>(perm, a);
    case 15:
        return scrambledRadicalInverseSpecialized<53>(perm, a);
    case 16:
        return scrambledRadicalInverseSpecialized<59>(perm, a);
    case 17:
        return scrambledRadicalInverseSpecialized<61>(perm, a);
    case 18:
        return scrambledRadicalInverseSpecialized<67>(perm, a);
    case 19:
        return scrambledRadicalInverseSpecialized<71>(perm, a);
    case 20:
        return scrambledRadicalInverseSpecialized<73>(perm, a);
    case 21:
        return scrambledRadicalInverseSpecialized<79>(perm, a);
    case 22:
        return scrambledRadicalInverseSpecialized<83>(perm, a);
    case 23:
        return scrambledRadicalInverseSpecialized<89>(perm, a);
    case 24:
        return scrambledRadicalInverseSpecialized<97>(perm, a);
    case 25:
        return scrambledRadicalInverseSpecialized<101>(perm, a);
    case 26:
        return scrambledRadicalInverseSpecialized<103>(perm, a);
    case 27:
        return scrambledRadicalInverseSpecialized<107>(perm, a);
    case 28:
        return scrambledRadicalInverseSpecialized<109>(perm, a);
    case 29:
        return scrambledRadicalInverseSpecialized<113>(perm, a);
    case 30:
        return scrambledRadicalInverseSpecialized<127>(perm, a);
    case 31:
        return scrambledRadicalInverseSpecialized<131>(perm, a);
    case 32:
        return scrambledRadicalInverseSpecialized<137>(perm, a);
    case 33:
        return scrambledRadicalInverseSpecialized<139>(perm, a);
    case 34:
        return scrambledRadicalInverseSpecialized<149>(perm, a);
    case 35:
        return scrambledRadicalInverseSpecialized<151>(perm, a);
    case 36:
        return scrambledRadicalInverseSpecialized<157>(perm, a);
    case 37:
        return scrambledRadicalInverseSpecialized<163>(perm, a);
    case 38:
        return scrambledRadicalInverseSpecialized<167>(perm, a);
    case 39:
        return scrambledRadicalInverseSpecialized<173>(perm, a);
    case 40:
        return scrambledRadicalInverseSpecialized<179>(perm, a);
    case 41:
        return scrambledRadicalInverseSpecialized<181>(perm, a);
    case 42:
        return scrambledRadicalInverseSpecialized<191>(perm, a);
    case 43:
        return scrambledRadicalInverseSpecialized<193>(perm, a);
    case 44:
        return scrambledRadicalInverseSpecialized<197>(perm, a);
    case 45:
        return scrambledRadicalInverseSpecialized<199>(perm, a);
    case 46:
        return scrambledRadicalInverseSpecialized<211>(perm, a);
    case 47:
        return scrambledRadicalInverseSpecialized<223>(perm, a);
    case 48:
        return scrambledRadicalInverseSpecialized<227>(perm, a);
    case 49:
        return scrambledRadicalInverseSpecialized<229>(perm, a);
    case 50:
        return scrambledRadicalInverseSpecialized<233>(perm, a);
    case 51:
        return scrambledRadicalInverseSpecialized<239>(perm, a);
    case 52:
        return scrambledRadicalInverseSpecialized<241>(perm, a);
    case 53:
        return scrambledRadicalInverseSpecialized<251>(perm, a);
    case 54:
        return scrambledRadicalInverseSpecialized<257>(perm, a);
    case 55:
        return scrambledRadicalInverseSpecialized<263>(perm, a);
    case 56:
        return scrambledRadicalInverseSpecialized<269>(perm, a);
    case 57:
        return scrambledRadicalInverseSpecialized<271>(perm, a);
    case 58:
        return scrambledRadicalInverseSpecialized<277>(perm, a);
    case 59:
        return scrambledRadicalInverseSpecialized<281>(perm, a);
    case 60:
        return scrambledRadicalInverseSpecialized<283>(perm, a);
    case 61:
        return scrambledRadicalInverseSpecialized<293>(perm, a);
    case 62:
        return scrambledRadicalInverseSpecialized<307>(perm, a);
    case 63:
        return scrambledRadicalInverseSpecialized<311>(perm, a);
    case 64:
        return scrambledRadicalInverseSpecialized<313>(perm, a);
    case 65:
        return scrambledRadicalInverseSpecialized<317>(perm, a);
    case 66:
        return scrambledRadicalInverseSpecialized<331>(perm, a);
    case 67:
        return scrambledRadicalInverseSpecialized<337>(perm, a);
    case 68:
        return scrambledRadicalInverseSpecialized<347>(perm, a);
    case 69:
        return scrambledRadicalInverseSpecialized<349>(perm, a);
    case 70:
        return scrambledRadicalInverseSpecialized<353>(perm, a);
    case 71:
        return scrambledRadicalInverseSpecialized<359>(perm, a);
    case 72:
        return scrambledRadicalInverseSpecialized<367>(perm, a);
    case 73:
        return scrambledRadicalInverseSpecialized<373>(perm, a);
    case 74:
        return scrambledRadicalInverseSpecialized<379>(perm, a);
    case 75:
        return scrambledRadicalInverseSpecialized<383>(perm, a);
    case 76:
        return scrambledRadicalInverseSpecialized<389>(perm, a);
    case 77:
        return scrambledRadicalInverseSpecialized<397>(perm, a);
    case 78:
        return scrambledRadicalInverseSpecialized<401>(perm, a);
    case 79:
        return scrambledRadicalInverseSpecialized<409>(perm, a);
    case 80:
        return scrambledRadicalInverseSpecialized<419>(perm, a);
    case 81:
        return scrambledRadicalInverseSpecialized<421>(perm, a);
    case 82:
        return scrambledRadicalInverseSpecialized<431>(perm, a);
    case 83:
        return scrambledRadicalInverseSpecialized<433>(perm, a);
    case 84:
        return scrambledRadicalInverseSpecialized<439>(perm, a);
    case 85:
        return scrambledRadicalInverseSpecialized<443>(perm, a);
    case 86:
        return scrambledRadicalInverseSpecialized<449>(perm, a);
    case 87:
        return scrambledRadicalInverseSpecialized<457>(perm, a);
    case 88:
        return scrambledRadicalInverseSpecialized<461>(perm, a);
    case 89:
        return scrambledRadicalInverseSpecialized<463>(perm, a);
    case 90:
        return scrambledRadicalInverseSpecialized<467>(perm, a);
    case 91:
        return scrambledRadicalInverseSpecialized<479>(perm, a);
    case 92:
        return scrambledRadicalInverseSpecialized<487>(perm, a);
    case 93:
        return scrambledRadicalInverseSpecialized<491>(perm, a);
    case 94:
        return scrambledRadicalInverseSpecialized<499>(perm, a);
    case 95:
        return scrambledRadicalInverseSpecialized<503>(perm, a);
    case 96:
        return scrambledRadicalInverseSpecialized<509>(perm, a);
    case 97:
        return scrambledRadicalInverseSpecialized<521>(perm, a);
    case 98:
        return scrambledRadicalInverseSpecialized<523>(perm, a);
    case 99:
        return scrambledRadicalInverseSpecialized<541>(perm, a);
    case 100:
        return scrambledRadicalInverseSpecialized<547>(perm, a);
    case 101:
        return scrambledRadicalInverseSpecialized<557>(perm, a);
    case 102:
        return scrambledRadicalInverseSpecialized<563>(perm, a);
    case 103:
        return scrambledRadicalInverseSpecialized<569>(perm, a);
    case 104:
        return scrambledRadicalInverseSpecialized<571>(perm, a);
    case 105:
        return scrambledRadicalInverseSpecialized<577>(perm, a);
    case 106:
        return scrambledRadicalInverseSpecialized<587>(perm, a);
    case 107:
        return scrambledRadicalInverseSpecialized<593>(perm, a);
    case 108:
        return scrambledRadicalInverseSpecialized<599>(perm, a);
    case 109:
        return scrambledRadicalInverseSpecialized<601>(perm, a);
    case 110:
        return scrambledRadicalInverseSpecialized<607>(perm, a);
    case 111:
        return scrambledRadicalInverseSpecialized<613>(perm, a);
    case 112:
        return scrambledRadicalInverseSpecialized<617>(perm, a);
    case 113:
        return scrambledRadicalInverseSpecialized<619>(perm, a);
    case 114:
        return scrambledRadicalInverseSpecialized<631>(perm, a);
    case 115:
        return scrambledRadicalInverseSpecialized<641>(perm, a);
    case 116:
        return scrambledRadicalInverseSpecialized<643>(perm, a);
    case 117:
        return scrambledRadicalInverseSpecialized<647>(perm, a);
    case 118:
        return scrambledRadicalInverseSpecialized<653>(perm, a);
    case 119:
        return scrambledRadicalInverseSpecialized<659>(perm, a);
    case 120:
        return scrambledRadicalInverseSpecialized<661>(perm, a);
    case 121:
        return scrambledRadicalInverseSpecialized<673>(perm, a);
    case 122:
        return scrambledRadicalInverseSpecialized<677>(perm, a);
    case 123:
        return scrambledRadicalInverseSpecialized<683>(perm, a);
    case 124:
        return scrambledRadicalInverseSpecialized<691>(perm, a);
    case 125:
        return scrambledRadicalInverseSpecialized<701>(perm, a);
    case 126:
        return scrambledRadicalInverseSpecialized<709>(perm, a);
    case 127:
        return scrambledRadicalInverseSpecialized<719>(perm, a);
    case 128:
        return scrambledRadicalInverseSpecialized<727>(perm, a);
    case 129:
        return scrambledRadicalInverseSpecialized<733>(perm, a);
    case 130:
        return scrambledRadicalInverseSpecialized<739>(perm, a);
    case 131:
        return scrambledRadicalInverseSpecialized<743>(perm, a);
    case 132:
        return scrambledRadicalInverseSpecialized<751>(perm, a);
    case 133:
        return scrambledRadicalInverseSpecialized<757>(perm, a);
    case 134:
        return scrambledRadicalInverseSpecialized<761>(perm, a);
    case 135:
        return scrambledRadicalInverseSpecialized<769>(perm, a);
    case 136:
        return scrambledRadicalInverseSpecialized<773>(perm, a);
    case 137:
        return scrambledRadicalInverseSpecialized<787>(perm, a);
    case 138:
        return scrambledRadicalInverseSpecialized<797>(perm, a);
    case 139:
        return scrambledRadicalInverseSpecialized<809>(perm, a);
    case 140:
        return scrambledRadicalInverseSpecialized<811>(perm, a);
    case 141:
        return scrambledRadicalInverseSpecialized<821>(perm, a);
    case 142:
        return scrambledRadicalInverseSpecialized<823>(perm, a);
    case 143:
        return scrambledRadicalInverseSpecialized<827>(perm, a);
    case 144:
        return scrambledRadicalInverseSpecialized<829>(perm, a);
    case 145:
        return scrambledRadicalInverseSpecialized<839>(perm, a);
    case 146:
        return scrambledRadicalInverseSpecialized<853>(perm, a);
    case 147:
        return scrambledRadicalInverseSpecialized<857>(perm, a);
    case 148:
        return scrambledRadicalInverseSpecialized<859>(perm, a);
    case 149:
        return scrambledRadicalInverseSpecialized<863>(perm, a);
    case 150:
        return scrambledRadicalInverseSpecialized<877>(perm, a);
    case 151:
        return scrambledRadicalInverseSpecialized<881>(perm, a);
    case 152:
        return scrambledRadicalInverseSpecialized<883>(perm, a);
    case 153:
        return scrambledRadicalInverseSpecialized<887>(perm, a);
    case 154:
        return scrambledRadicalInverseSpecialized<907>(perm, a);
    case 155:
        return scrambledRadicalInverseSpecialized<911>(perm, a);
    case 156:
        return scrambledRadicalInverseSpecialized<919>(perm, a);
    case 157:
        return scrambledRadicalInverseSpecialized<929>(perm, a);
    case 158:
        return scrambledRadicalInverseSpecialized<937>(perm, a);
    case 159:
        return scrambledRadicalInverseSpecialized<941>(perm, a);
    case 160:
        return scrambledRadicalInverseSpecialized<947>(perm, a);
    case 161:
        return scrambledRadicalInverseSpecialized<953>(perm, a);
    case 162:
        return scrambledRadicalInverseSpecialized<967>(perm, a);
    case 163:
        return scrambledRadicalInverseSpecialized<971>(perm, a);
    case 164:
        return scrambledRadicalInverseSpecialized<977>(perm, a);
    case 165:
        return scrambledRadicalInverseSpecialized<983>(perm, a);
    case 166:
        return scrambledRadicalInverseSpecialized<991>(perm, a);
    case 167:
        return scrambledRadicalInverseSpecialized<997>(perm, a);
    case 168:
        return scrambledRadicalInverseSpecialized<1009>(perm, a);
    case 169:
        return scrambledRadicalInverseSpecialized<1013>(perm, a);
    case 170:
        return scrambledRadicalInverseSpecialized<1019>(perm, a);
    case 171:
        return scrambledRadicalInverseSpecialized<1021>(perm, a);
    case 172:
        return scrambledRadicalInverseSpecialized<1031>(perm, a);
    case 173:
        return scrambledRadicalInverseSpecialized<1033>(perm, a);
    case 174:
        return scrambledRadicalInverseSpecialized<1039>(perm, a);
    case 175:
        return scrambledRadicalInverseSpecialized<1049>(perm, a);
    case 176:
        return scrambledRadicalInverseSpecialized<1051>(perm, a);
    case 177:
        return scrambledRadicalInverseSpecialized<1061>(perm, a);
    case 178:
        return scrambledRadicalInverseSpecialized<1063>(perm, a);
    case 179:
        return scrambledRadicalInverseSpecialized<1069>(perm, a);
    case 180:
        return scrambledRadicalInverseSpecialized<1087>(perm, a);
    case 181:
        return scrambledRadicalInverseSpecialized<1091>(perm, a);
    case 182:
        return scrambledRadicalInverseSpecialized<1093>(perm, a);
    case 183:
        return scrambledRadicalInverseSpecialized<1097>(perm, a);
    case 184:
        return scrambledRadicalInverseSpecialized<1103>(perm, a);
    case 185:
        return scrambledRadicalInverseSpecialized<1109>(perm, a);
    case 186:
        return scrambledRadicalInverseSpecialized<1117>(perm, a);
    case 187:
        return scrambledRadicalInverseSpecialized<1123>(perm, a);
    case 188:
        return scrambledRadicalInverseSpecialized<1129>(perm, a);
    case 189:
        return scrambledRadicalInverseSpecialized<1151>(perm, a);
    case 190:
        return scrambledRadicalInverseSpecialized<1153>(perm, a);
    case 191:
        return scrambledRadicalInverseSpecialized<1163>(perm, a);
    case 192:
        return scrambledRadicalInverseSpecialized<1171>(perm, a);
    case 193:
        return scrambledRadicalInverseSpecialized<1181>(perm, a);
    case 194:
        return scrambledRadicalInverseSpecialized<1187>(perm, a);
    case 195:
        return scrambledRadicalInverseSpecialized<1193>(perm, a);
    case 196:
        return scrambledRadicalInverseSpecialized<1201>(perm, a);
    case 197:
        return scrambledRadicalInverseSpecialized<1213>(perm, a);
    case 198:
        return scrambledRadicalInverseSpecialized<1217>(perm, a);
    case 199:
        return scrambledRadicalInverseSpecialized<1223>(perm, a);
    case 200:
        return scrambledRadicalInverseSpecialized<1229>(perm, a);
    case 201:
        return scrambledRadicalInverseSpecialized<1231>(perm, a);
    case 202:
        return scrambledRadicalInverseSpecialized<1237>(perm, a);
    case 203:
        return scrambledRadicalInverseSpecialized<1249>(perm, a);
    case 204:
        return scrambledRadicalInverseSpecialized<1259>(perm, a);
    case 205:
        return scrambledRadicalInverseSpecialized<1277>(perm, a);
    case 206:
        return scrambledRadicalInverseSpecialized<1279>(perm, a);
    case 207:
        return scrambledRadicalInverseSpecialized<1283>(perm, a);
    case 208:
        return scrambledRadicalInverseSpecialized<1289>(perm, a);
    case 209:
        return scrambledRadicalInverseSpecialized<1291>(perm, a);
    case 210:
        return scrambledRadicalInverseSpecialized<1297>(perm, a);
    case 211:
        return scrambledRadicalInverseSpecialized<1301>(perm, a);
    case 212:
        return scrambledRadicalInverseSpecialized<1303>(perm, a);
    case 213:
        return scrambledRadicalInverseSpecialized<1307>(perm, a);
    case 214:
        return scrambledRadicalInverseSpecialized<1319>(perm, a);
    case 215:
        return scrambledRadicalInverseSpecialized<1321>(perm, a);
    case 216:
        return scrambledRadicalInverseSpecialized<1327>(perm, a);
    case 217:
        return scrambledRadicalInverseSpecialized<1361>(perm, a);
    case 218:
        return scrambledRadicalInverseSpecialized<1367>(perm, a);
    case 219:
        return scrambledRadicalInverseSpecialized<1373>(perm, a);
    case 220:
        return scrambledRadicalInverseSpecialized<1381>(perm, a);
    case 221:
        return scrambledRadicalInverseSpecialized<1399>(perm, a);
    case 222:
        return scrambledRadicalInverseSpecialized<1409>(perm, a);
    case 223:
        return scrambledRadicalInverseSpecialized<1423>(perm, a);
    case 224:
        return scrambledRadicalInverseSpecialized<1427>(perm, a);
    case 225:
        return scrambledRadicalInverseSpecialized<1429>(perm, a);
    case 226:
        return scrambledRadicalInverseSpecialized<1433>(perm, a);
    case 227:
        return scrambledRadicalInverseSpecialized<1439>(perm, a);
    case 228:
        return scrambledRadicalInverseSpecialized<1447>(perm, a);
    case 229:
        return scrambledRadicalInverseSpecialized<1451>(perm, a);
    case 230:
        return scrambledRadicalInverseSpecialized<1453>(perm, a);
    case 231:
        return scrambledRadicalInverseSpecialized<1459>(perm, a);
    case 232:
        return scrambledRadicalInverseSpecialized<1471>(perm, a);
    case 233:
        return scrambledRadicalInverseSpecialized<1481>(perm, a);
    case 234:
        return scrambledRadicalInverseSpecialized<1483>(perm, a);
    case 235:
        return scrambledRadicalInverseSpecialized<1487>(perm, a);
    case 236:
        return scrambledRadicalInverseSpecialized<1489>(perm, a);
    case 237:
        return scrambledRadicalInverseSpecialized<1493>(perm, a);
    case 238:
        return scrambledRadicalInverseSpecialized<1499>(perm, a);
    case 239:
        return scrambledRadicalInverseSpecialized<1511>(perm, a);
    case 240:
        return scrambledRadicalInverseSpecialized<1523>(perm, a);
    case 241:
        return scrambledRadicalInverseSpecialized<1531>(perm, a);
    case 242:
        return scrambledRadicalInverseSpecialized<1543>(perm, a);
    case 243:
        return scrambledRadicalInverseSpecialized<1549>(perm, a);
    case 244:
        return scrambledRadicalInverseSpecialized<1553>(perm, a);
    case 245:
        return scrambledRadicalInverseSpecialized<1559>(perm, a);
    case 246:
        return scrambledRadicalInverseSpecialized<1567>(perm, a);
    case 247:
        return scrambledRadicalInverseSpecialized<1571>(perm, a);
    case 248:
        return scrambledRadicalInverseSpecialized<1579>(perm, a);
    case 249:
        return scrambledRadicalInverseSpecialized<1583>(perm, a);
    case 250:
        return scrambledRadicalInverseSpecialized<1597>(perm, a);
    case 251:
        return scrambledRadicalInverseSpecialized<1601>(perm, a);
    case 252:
        return scrambledRadicalInverseSpecialized<1607>(perm, a);
    case 253:
        return scrambledRadicalInverseSpecialized<1609>(perm, a);
    case 254:
        return scrambledRadicalInverseSpecialized<1613>(perm, a);
    case 255:
        return scrambledRadicalInverseSpecialized<1619>(perm, a);
    case 256:
        return scrambledRadicalInverseSpecialized<1621>(perm, a);
    case 257:
        return scrambledRadicalInverseSpecialized<1627>(perm, a);
    case 258:
        return scrambledRadicalInverseSpecialized<1637>(perm, a);
    case 259:
        return scrambledRadicalInverseSpecialized<1657>(perm, a);
    case 260:
        return scrambledRadicalInverseSpecialized<1663>(perm, a);
    case 261:
        return scrambledRadicalInverseSpecialized<1667>(perm, a);
    case 262:
        return scrambledRadicalInverseSpecialized<1669>(perm, a);
    case 263:
        return scrambledRadicalInverseSpecialized<1693>(perm, a);
    case 264:
        return scrambledRadicalInverseSpecialized<1697>(perm, a);
    case 265:
        return scrambledRadicalInverseSpecialized<1699>(perm, a);
    case 266:
        return scrambledRadicalInverseSpecialized<1709>(perm, a);
    case 267:
        return scrambledRadicalInverseSpecialized<1721>(perm, a);
    case 268:
        return scrambledRadicalInverseSpecialized<1723>(perm, a);
    case 269:
        return scrambledRadicalInverseSpecialized<1733>(perm, a);
    case 270:
        return scrambledRadicalInverseSpecialized<1741>(perm, a);
    case 271:
        return scrambledRadicalInverseSpecialized<1747>(perm, a);
    case 272:
        return scrambledRadicalInverseSpecialized<1753>(perm, a);
    case 273:
        return scrambledRadicalInverseSpecialized<1759>(perm, a);
    case 274:
        return scrambledRadicalInverseSpecialized<1777>(perm, a);
    case 275:
        return scrambledRadicalInverseSpecialized<1783>(perm, a);
    case 276:
        return scrambledRadicalInverseSpecialized<1787>(perm, a);
    case 277:
        return scrambledRadicalInverseSpecialized<1789>(perm, a);
    case 278:
        return scrambledRadicalInverseSpecialized<1801>(perm, a);
    case 279:
        return scrambledRadicalInverseSpecialized<1811>(perm, a);
    case 280:
        return scrambledRadicalInverseSpecialized<1823>(perm, a);
    case 281:
        return scrambledRadicalInverseSpecialized<1831>(perm, a);
    case 282:
        return scrambledRadicalInverseSpecialized<1847>(perm, a);
    case 283:
        return scrambledRadicalInverseSpecialized<1861>(perm, a);
    case 284:
        return scrambledRadicalInverseSpecialized<1867>(perm, a);
    case 285:
        return scrambledRadicalInverseSpecialized<1871>(perm, a);
    case 286:
        return scrambledRadicalInverseSpecialized<1873>(perm, a);
    case 287:
        return scrambledRadicalInverseSpecialized<1877>(perm, a);
    case 288:
        return scrambledRadicalInverseSpecialized<1879>(perm, a);
    case 289:
        return scrambledRadicalInverseSpecialized<1889>(perm, a);
    case 290:
        return scrambledRadicalInverseSpecialized<1901>(perm, a);
    case 291:
        return scrambledRadicalInverseSpecialized<1907>(perm, a);
    case 292:
        return scrambledRadicalInverseSpecialized<1913>(perm, a);
    case 293:
        return scrambledRadicalInverseSpecialized<1931>(perm, a);
    case 294:
        return scrambledRadicalInverseSpecialized<1933>(perm, a);
    case 295:
        return scrambledRadicalInverseSpecialized<1949>(perm, a);
    case 296:
        return scrambledRadicalInverseSpecialized<1951>(perm, a);
    case 297:
        return scrambledRadicalInverseSpecialized<1973>(perm, a);
    case 298:
        return scrambledRadicalInverseSpecialized<1979>(perm, a);
    case 299:
        return scrambledRadicalInverseSpecialized<1987>(perm, a);
    case 300:
        return scrambledRadicalInverseSpecialized<1993>(perm, a);
    case 301:
        return scrambledRadicalInverseSpecialized<1997>(perm, a);
    case 302:
        return scrambledRadicalInverseSpecialized<1999>(perm, a);
    case 303:
        return scrambledRadicalInverseSpecialized<2003>(perm, a);
    case 304:
        return scrambledRadicalInverseSpecialized<2011>(perm, a);
    case 305:
        return scrambledRadicalInverseSpecialized<2017>(perm, a);
    case 306:
        return scrambledRadicalInverseSpecialized<2027>(perm, a);
    case 307:
        return scrambledRadicalInverseSpecialized<2029>(perm, a);
    case 308:
        return scrambledRadicalInverseSpecialized<2039>(perm, a);
    case 309:
        return scrambledRadicalInverseSpecialized<2053>(perm, a);
    case 310:
        return scrambledRadicalInverseSpecialized<2063>(perm, a);
    case 311:
        return scrambledRadicalInverseSpecialized<2069>(perm, a);
    case 312:
        return scrambledRadicalInverseSpecialized<2081>(perm, a);
    case 313:
        return scrambledRadicalInverseSpecialized<2083>(perm, a);
    case 314:
        return scrambledRadicalInverseSpecialized<2087>(perm, a);
    case 315:
        return scrambledRadicalInverseSpecialized<2089>(perm, a);
    case 316:
        return scrambledRadicalInverseSpecialized<2099>(perm, a);
    case 317:
        return scrambledRadicalInverseSpecialized<2111>(perm, a);
    case 318:
        return scrambledRadicalInverseSpecialized<2113>(perm, a);
    case 319:
        return scrambledRadicalInverseSpecialized<2129>(perm, a);
    case 320:
        return scrambledRadicalInverseSpecialized<2131>(perm, a);
    case 321:
        return scrambledRadicalInverseSpecialized<2137>(perm, a);
    case 322:
        return scrambledRadicalInverseSpecialized<2141>(perm, a);
    case 323:
        return scrambledRadicalInverseSpecialized<2143>(perm, a);
    case 324:
        return scrambledRadicalInverseSpecialized<2153>(perm, a);
    case 325:
        return scrambledRadicalInverseSpecialized<2161>(perm, a);
    case 326:
        return scrambledRadicalInverseSpecialized<2179>(perm, a);
    case 327:
        return scrambledRadicalInverseSpecialized<2203>(perm, a);
    case 328:
        return scrambledRadicalInverseSpecialized<2207>(perm, a);
    case 329:
        return scrambledRadicalInverseSpecialized<2213>(perm, a);
    case 330:
        return scrambledRadicalInverseSpecialized<2221>(perm, a);
    case 331:
        return scrambledRadicalInverseSpecialized<2237>(perm, a);
    case 332:
        return scrambledRadicalInverseSpecialized<2239>(perm, a);
    case 333:
        return scrambledRadicalInverseSpecialized<2243>(perm, a);
    case 334:
        return scrambledRadicalInverseSpecialized<2251>(perm, a);
    case 335:
        return scrambledRadicalInverseSpecialized<2267>(perm, a);
    case 336:
        return scrambledRadicalInverseSpecialized<2269>(perm, a);
    case 337:
        return scrambledRadicalInverseSpecialized<2273>(perm, a);
    case 338:
        return scrambledRadicalInverseSpecialized<2281>(perm, a);
    case 339:
        return scrambledRadicalInverseSpecialized<2287>(perm, a);
    case 340:
        return scrambledRadicalInverseSpecialized<2293>(perm, a);
    case 341:
        return scrambledRadicalInverseSpecialized<2297>(perm, a);
    case 342:
        return scrambledRadicalInverseSpecialized<2309>(perm, a);
    case 343:
        return scrambledRadicalInverseSpecialized<2311>(perm, a);
    case 344:
        return scrambledRadicalInverseSpecialized<2333>(perm, a);
    case 345:
        return scrambledRadicalInverseSpecialized<2339>(perm, a);
    case 346:
        return scrambledRadicalInverseSpecialized<2341>(perm, a);
    case 347:
        return scrambledRadicalInverseSpecialized<2347>(perm, a);
    case 348:
        return scrambledRadicalInverseSpecialized<2351>(perm, a);
    case 349:
        return scrambledRadicalInverseSpecialized<2357>(perm, a);
    case 350:
        return scrambledRadicalInverseSpecialized<2371>(perm, a);
    case 351:
        return scrambledRadicalInverseSpecialized<2377>(perm, a);
    case 352:
        return scrambledRadicalInverseSpecialized<2381>(perm, a);
    case 353:
        return scrambledRadicalInverseSpecialized<2383>(perm, a);
    case 354:
        return scrambledRadicalInverseSpecialized<2389>(perm, a);
    case 355:
        return scrambledRadicalInverseSpecialized<2393>(perm, a);
    case 356:
        return scrambledRadicalInverseSpecialized<2399>(perm, a);
    case 357:
        return scrambledRadicalInverseSpecialized<2411>(perm, a);
    case 358:
        return scrambledRadicalInverseSpecialized<2417>(perm, a);
    case 359:
        return scrambledRadicalInverseSpecialized<2423>(perm, a);
    case 360:
        return scrambledRadicalInverseSpecialized<2437>(perm, a);
    case 361:
        return scrambledRadicalInverseSpecialized<2441>(perm, a);
    case 362:
        return scrambledRadicalInverseSpecialized<2447>(perm, a);
    case 363:
        return scrambledRadicalInverseSpecialized<2459>(perm, a);
    case 364:
        return scrambledRadicalInverseSpecialized<2467>(perm, a);
    case 365:
        return scrambledRadicalInverseSpecialized<2473>(perm, a);
    case 366:
        return scrambledRadicalInverseSpecialized<2477>(perm, a);
    case 367:
        return scrambledRadicalInverseSpecialized<2503>(perm, a);
    case 368:
        return scrambledRadicalInverseSpecialized<2521>(perm, a);
    case 369:
        return scrambledRadicalInverseSpecialized<2531>(perm, a);
    case 370:
        return scrambledRadicalInverseSpecialized<2539>(perm, a);
    case 371:
        return scrambledRadicalInverseSpecialized<2543>(perm, a);
    case 372:
        return scrambledRadicalInverseSpecialized<2549>(perm, a);
    case 373:
        return scrambledRadicalInverseSpecialized<2551>(perm, a);
    case 374:
        return scrambledRadicalInverseSpecialized<2557>(perm, a);
    case 375:
        return scrambledRadicalInverseSpecialized<2579>(perm, a);
    case 376:
        return scrambledRadicalInverseSpecialized<2591>(perm, a);
    case 377:
        return scrambledRadicalInverseSpecialized<2593>(perm, a);
    case 378:
        return scrambledRadicalInverseSpecialized<2609>(perm, a);
    case 379:
        return scrambledRadicalInverseSpecialized<2617>(perm, a);
    case 380:
        return scrambledRadicalInverseSpecialized<2621>(perm, a);
    case 381:
        return scrambledRadicalInverseSpecialized<2633>(perm, a);
    case 382:
        return scrambledRadicalInverseSpecialized<2647>(perm, a);
    case 383:
        return scrambledRadicalInverseSpecialized<2657>(perm, a);
    case 384:
        return scrambledRadicalInverseSpecialized<2659>(perm, a);
    case 385:
        return scrambledRadicalInverseSpecialized<2663>(perm, a);
    case 386:
        return scrambledRadicalInverseSpecialized<2671>(perm, a);
    case 387:
        return scrambledRadicalInverseSpecialized<2677>(perm, a);
    case 388:
        return scrambledRadicalInverseSpecialized<2683>(perm, a);
    case 389:
        return scrambledRadicalInverseSpecialized<2687>(perm, a);
    case 390:
        return scrambledRadicalInverseSpecialized<2689>(perm, a);
    case 391:
        return scrambledRadicalInverseSpecialized<2693>(perm, a);
    case 392:
        return scrambledRadicalInverseSpecialized<2699>(perm, a);
    case 393:
        return scrambledRadicalInverseSpecialized<2707>(perm, a);
    case 394:
        return scrambledRadicalInverseSpecialized<2711>(perm, a);
    case 395:
        return scrambledRadicalInverseSpecialized<2713>(perm, a);
    case 396:
        return scrambledRadicalInverseSpecialized<2719>(perm, a);
    case 397:
        return scrambledRadicalInverseSpecialized<2729>(perm, a);
    case 398:
        return scrambledRadicalInverseSpecialized<2731>(perm, a);
    case 399:
        return scrambledRadicalInverseSpecialized<2741>(perm, a);
    case 400:
        return scrambledRadicalInverseSpecialized<2749>(perm, a);
    case 401:
        return scrambledRadicalInverseSpecialized<2753>(perm, a);
    case 402:
        return scrambledRadicalInverseSpecialized<2767>(perm, a);
    case 403:
        return scrambledRadicalInverseSpecialized<2777>(perm, a);
    case 404:
        return scrambledRadicalInverseSpecialized<2789>(perm, a);
    case 405:
        return scrambledRadicalInverseSpecialized<2791>(perm, a);
    case 406:
        return scrambledRadicalInverseSpecialized<2797>(perm, a);
    case 407:
        return scrambledRadicalInverseSpecialized<2801>(perm, a);
    case 408:
        return scrambledRadicalInverseSpecialized<2803>(perm, a);
    case 409:
        return scrambledRadicalInverseSpecialized<2819>(perm, a);
    case 410:
        return scrambledRadicalInverseSpecialized<2833>(perm, a);
    case 411:
        return scrambledRadicalInverseSpecialized<2837>(perm, a);
    case 412:
        return scrambledRadicalInverseSpecialized<2843>(perm, a);
    case 413:
        return scrambledRadicalInverseSpecialized<2851>(perm, a);
    case 414:
        return scrambledRadicalInverseSpecialized<2857>(perm, a);
    case 415:
        return scrambledRadicalInverseSpecialized<2861>(perm, a);
    case 416:
        return scrambledRadicalInverseSpecialized<2879>(perm, a);
    case 417:
        return scrambledRadicalInverseSpecialized<2887>(perm, a);
    case 418:
        return scrambledRadicalInverseSpecialized<2897>(perm, a);
    case 419:
        return scrambledRadicalInverseSpecialized<2903>(perm, a);
    case 420:
        return scrambledRadicalInverseSpecialized<2909>(perm, a);
    case 421:
        return scrambledRadicalInverseSpecialized<2917>(perm, a);
    case 422:
        return scrambledRadicalInverseSpecialized<2927>(perm, a);
    case 423:
        return scrambledRadicalInverseSpecialized<2939>(perm, a);
    case 424:
        return scrambledRadicalInverseSpecialized<2953>(perm, a);
    case 425:
        return scrambledRadicalInverseSpecialized<2957>(perm, a);
    case 426:
        return scrambledRadicalInverseSpecialized<2963>(perm, a);
    case 427:
        return scrambledRadicalInverseSpecialized<2969>(perm, a);
    case 428:
        return scrambledRadicalInverseSpecialized<2971>(perm, a);
    case 429:
        return scrambledRadicalInverseSpecialized<2999>(perm, a);
    case 430:
        return scrambledRadicalInverseSpecialized<3001>(perm, a);
    case 431:
        return scrambledRadicalInverseSpecialized<3011>(perm, a);
    case 432:
        return scrambledRadicalInverseSpecialized<3019>(perm, a);
    case 433:
        return scrambledRadicalInverseSpecialized<3023>(perm, a);
    case 434:
        return scrambledRadicalInverseSpecialized<3037>(perm, a);
    case 435:
        return scrambledRadicalInverseSpecialized<3041>(perm, a);
    case 436:
        return scrambledRadicalInverseSpecialized<3049>(perm, a);
    case 437:
        return scrambledRadicalInverseSpecialized<3061>(perm, a);
    case 438:
        return scrambledRadicalInverseSpecialized<3067>(perm, a);
    case 439:
        return scrambledRadicalInverseSpecialized<3079>(perm, a);
    case 440:
        return scrambledRadicalInverseSpecialized<3083>(perm, a);
    case 441:
        return scrambledRadicalInverseSpecialized<3089>(perm, a);
    case 442:
        return scrambledRadicalInverseSpecialized<3109>(perm, a);
    case 443:
        return scrambledRadicalInverseSpecialized<3119>(perm, a);
    case 444:
        return scrambledRadicalInverseSpecialized<3121>(perm, a);
    case 445:
        return scrambledRadicalInverseSpecialized<3137>(perm, a);
    case 446:
        return scrambledRadicalInverseSpecialized<3163>(perm, a);
    case 447:
        return scrambledRadicalInverseSpecialized<3167>(perm, a);
    case 448:
        return scrambledRadicalInverseSpecialized<3169>(perm, a);
    case 449:
        return scrambledRadicalInverseSpecialized<3181>(perm, a);
    case 450:
        return scrambledRadicalInverseSpecialized<3187>(perm, a);
    case 451:
        return scrambledRadicalInverseSpecialized<3191>(perm, a);
    case 452:
        return scrambledRadicalInverseSpecialized<3203>(perm, a);
    case 453:
        return scrambledRadicalInverseSpecialized<3209>(perm, a);
    case 454:
        return scrambledRadicalInverseSpecialized<3217>(perm, a);
    case 455:
        return scrambledRadicalInverseSpecialized<3221>(perm, a);
    case 456:
        return scrambledRadicalInverseSpecialized<3229>(perm, a);
    case 457:
        return scrambledRadicalInverseSpecialized<3251>(perm, a);
    case 458:
        return scrambledRadicalInverseSpecialized<3253>(perm, a);
    case 459:
        return scrambledRadicalInverseSpecialized<3257>(perm, a);
    case 460:
        return scrambledRadicalInverseSpecialized<3259>(perm, a);
    case 461:
        return scrambledRadicalInverseSpecialized<3271>(perm, a);
    case 462:
        return scrambledRadicalInverseSpecialized<3299>(perm, a);
    case 463:
        return scrambledRadicalInverseSpecialized<3301>(perm, a);
    case 464:
        return scrambledRadicalInverseSpecialized<3307>(perm, a);
    case 465:
        return scrambledRadicalInverseSpecialized<3313>(perm, a);
    case 466:
        return scrambledRadicalInverseSpecialized<3319>(perm, a);
    case 467:
        return scrambledRadicalInverseSpecialized<3323>(perm, a);
    case 468:
        return scrambledRadicalInverseSpecialized<3329>(perm, a);
    case 469:
        return scrambledRadicalInverseSpecialized<3331>(perm, a);
    case 470:
        return scrambledRadicalInverseSpecialized<3343>(perm, a);
    case 471:
        return scrambledRadicalInverseSpecialized<3347>(perm, a);
    case 472:
        return scrambledRadicalInverseSpecialized<3359>(perm, a);
    case 473:
        return scrambledRadicalInverseSpecialized<3361>(perm, a);
    case 474:
        return scrambledRadicalInverseSpecialized<3371>(perm, a);
    case 475:
        return scrambledRadicalInverseSpecialized<3373>(perm, a);
    case 476:
        return scrambledRadicalInverseSpecialized<3389>(perm, a);
    case 477:
        return scrambledRadicalInverseSpecialized<3391>(perm, a);
    case 478:
        return scrambledRadicalInverseSpecialized<3407>(perm, a);
    case 479:
        return scrambledRadicalInverseSpecialized<3413>(perm, a);
    case 480:
        return scrambledRadicalInverseSpecialized<3433>(perm, a);
    case 481:
        return scrambledRadicalInverseSpecialized<3449>(perm, a);
    case 482:
        return scrambledRadicalInverseSpecialized<3457>(perm, a);
    case 483:
        return scrambledRadicalInverseSpecialized<3461>(perm, a);
    case 484:
        return scrambledRadicalInverseSpecialized<3463>(perm, a);
    case 485:
        return scrambledRadicalInverseSpecialized<3467>(perm, a);
    case 486:
        return scrambledRadicalInverseSpecialized<3469>(perm, a);
    case 487:
        return scrambledRadicalInverseSpecialized<3491>(perm, a);
    case 488:
        return scrambledRadicalInverseSpecialized<3499>(perm, a);
    case 489:
        return scrambledRadicalInverseSpecialized<3511>(perm, a);
    case 490:
        return scrambledRadicalInverseSpecialized<3517>(perm, a);
    case 491:
        return scrambledRadicalInverseSpecialized<3527>(perm, a);
    case 492:
        return scrambledRadicalInverseSpecialized<3529>(perm, a);
    case 493:
        return scrambledRadicalInverseSpecialized<3533>(perm, a);
    case 494:
        return scrambledRadicalInverseSpecialized<3539>(perm, a);
    case 495:
        return scrambledRadicalInverseSpecialized<3541>(perm, a);
    case 496:
        return scrambledRadicalInverseSpecialized<3547>(perm, a);
    case 497:
        return scrambledRadicalInverseSpecialized<3557>(perm, a);
    case 498:
        return scrambledRadicalInverseSpecialized<3559>(perm, a);
    case 499:
        return scrambledRadicalInverseSpecialized<3571>(perm, a);
    case 500:
        return scrambledRadicalInverseSpecialized<3581>(perm, a);
    case 501:
        return scrambledRadicalInverseSpecialized<3583>(perm, a);
    case 502:
        return scrambledRadicalInverseSpecialized<3593>(perm, a);
    case 503:
        return scrambledRadicalInverseSpecialized<3607>(perm, a);
    case 504:
        return scrambledRadicalInverseSpecialized<3613>(perm, a);
    case 505:
        return scrambledRadicalInverseSpecialized<3617>(perm, a);
    case 506:
        return scrambledRadicalInverseSpecialized<3623>(perm, a);
    case 507:
        return scrambledRadicalInverseSpecialized<3631>(perm, a);
    case 508:
        return scrambledRadicalInverseSpecialized<3637>(perm, a);
    case 509:
        return scrambledRadicalInverseSpecialized<3643>(perm, a);
    case 510:
        return scrambledRadicalInverseSpecialized<3659>(perm, a);
    case 511:
        return scrambledRadicalInverseSpecialized<3671>(perm, a);
    case 512:
        return scrambledRadicalInverseSpecialized<3673>(perm, a);
    case 513:
        return scrambledRadicalInverseSpecialized<3677>(perm, a);
    case 514:
        return scrambledRadicalInverseSpecialized<3691>(perm, a);
    case 515:
        return scrambledRadicalInverseSpecialized<3697>(perm, a);
    case 516:
        return scrambledRadicalInverseSpecialized<3701>(perm, a);
    case 517:
        return scrambledRadicalInverseSpecialized<3709>(perm, a);
    case 518:
        return scrambledRadicalInverseSpecialized<3719>(perm, a);
    case 519:
        return scrambledRadicalInverseSpecialized<3727>(perm, a);
    case 520:
        return scrambledRadicalInverseSpecialized<3733>(perm, a);
    case 521:
        return scrambledRadicalInverseSpecialized<3739>(perm, a);
    case 522:
        return scrambledRadicalInverseSpecialized<3761>(perm, a);
    case 523:
        return scrambledRadicalInverseSpecialized<3767>(perm, a);
    case 524:
        return scrambledRadicalInverseSpecialized<3769>(perm, a);
    case 525:
        return scrambledRadicalInverseSpecialized<3779>(perm, a);
    case 526:
        return scrambledRadicalInverseSpecialized<3793>(perm, a);
    case 527:
        return scrambledRadicalInverseSpecialized<3797>(perm, a);
    case 528:
        return scrambledRadicalInverseSpecialized<3803>(perm, a);
    case 529:
        return scrambledRadicalInverseSpecialized<3821>(perm, a);
    case 530:
        return scrambledRadicalInverseSpecialized<3823>(perm, a);
    case 531:
        return scrambledRadicalInverseSpecialized<3833>(perm, a);
    case 532:
        return scrambledRadicalInverseSpecialized<3847>(perm, a);
    case 533:
        return scrambledRadicalInverseSpecialized<3851>(perm, a);
    case 534:
        return scrambledRadicalInverseSpecialized<3853>(perm, a);
    case 535:
        return scrambledRadicalInverseSpecialized<3863>(perm, a);
    case 536:
        return scrambledRadicalInverseSpecialized<3877>(perm, a);
    case 537:
        return scrambledRadicalInverseSpecialized<3881>(perm, a);
    case 538:
        return scrambledRadicalInverseSpecialized<3889>(perm, a);
    case 539:
        return scrambledRadicalInverseSpecialized<3907>(perm, a);
    case 540:
        return scrambledRadicalInverseSpecialized<3911>(perm, a);
    case 541:
        return scrambledRadicalInverseSpecialized<3917>(perm, a);
    case 542:
        return scrambledRadicalInverseSpecialized<3919>(perm, a);
    case 543:
        return scrambledRadicalInverseSpecialized<3923>(perm, a);
    case 544:
        return scrambledRadicalInverseSpecialized<3929>(perm, a);
    case 545:
        return scrambledRadicalInverseSpecialized<3931>(perm, a);
    case 546:
        return scrambledRadicalInverseSpecialized<3943>(perm, a);
    case 547:
        return scrambledRadicalInverseSpecialized<3947>(perm, a);
    case 548:
        return scrambledRadicalInverseSpecialized<3967>(perm, a);
    case 549:
        return scrambledRadicalInverseSpecialized<3989>(perm, a);
    case 550:
        return scrambledRadicalInverseSpecialized<4001>(perm, a);
    case 551:
        return scrambledRadicalInverseSpecialized<4003>(perm, a);
    case 552:
        return scrambledRadicalInverseSpecialized<4007>(perm, a);
    case 553:
        return scrambledRadicalInverseSpecialized<4013>(perm, a);
    case 554:
        return scrambledRadicalInverseSpecialized<4019>(perm, a);
    case 555:
        return scrambledRadicalInverseSpecialized<4021>(perm, a);
    case 556:
        return scrambledRadicalInverseSpecialized<4027>(perm, a);
    case 557:
        return scrambledRadicalInverseSpecialized<4049>(perm, a);
    case 558:
        return scrambledRadicalInverseSpecialized<4051>(perm, a);
    case 559:
        return scrambledRadicalInverseSpecialized<4057>(perm, a);
    case 560:
        return scrambledRadicalInverseSpecialized<4073>(perm, a);
    case 561:
        return scrambledRadicalInverseSpecialized<4079>(perm, a);
    case 562:
        return scrambledRadicalInverseSpecialized<4091>(perm, a);
    case 563:
        return scrambledRadicalInverseSpecialized<4093>(perm, a);
    case 564:
        return scrambledRadicalInverseSpecialized<4099>(perm, a);
    case 565:
        return scrambledRadicalInverseSpecialized<4111>(perm, a);
    case 566:
        return scrambledRadicalInverseSpecialized<4127>(perm, a);
    case 567:
        return scrambledRadicalInverseSpecialized<4129>(perm, a);
    case 568:
        return scrambledRadicalInverseSpecialized<4133>(perm, a);
    case 569:
        return scrambledRadicalInverseSpecialized<4139>(perm, a);
    case 570:
        return scrambledRadicalInverseSpecialized<4153>(perm, a);
    case 571:
        return scrambledRadicalInverseSpecialized<4157>(perm, a);
    case 572:
        return scrambledRadicalInverseSpecialized<4159>(perm, a);
    case 573:
        return scrambledRadicalInverseSpecialized<4177>(perm, a);
    case 574:
        return scrambledRadicalInverseSpecialized<4201>(perm, a);
    case 575:
        return scrambledRadicalInverseSpecialized<4211>(perm, a);
    case 576:
        return scrambledRadicalInverseSpecialized<4217>(perm, a);
    case 577:
        return scrambledRadicalInverseSpecialized<4219>(perm, a);
    case 578:
        return scrambledRadicalInverseSpecialized<4229>(perm, a);
    case 579:
        return scrambledRadicalInverseSpecialized<4231>(perm, a);
    case 580:
        return scrambledRadicalInverseSpecialized<4241>(perm, a);
    case 581:
        return scrambledRadicalInverseSpecialized<4243>(perm, a);
    case 582:
        return scrambledRadicalInverseSpecialized<4253>(perm, a);
    case 583:
        return scrambledRadicalInverseSpecialized<4259>(perm, a);
    case 584:
        return scrambledRadicalInverseSpecialized<4261>(perm, a);
    case 585:
        return scrambledRadicalInverseSpecialized<4271>(perm, a);
    case 586:
        return scrambledRadicalInverseSpecialized<4273>(perm, a);
    case 587:
        return scrambledRadicalInverseSpecialized<4283>(perm, a);
    case 588:
        return scrambledRadicalInverseSpecialized<4289>(perm, a);
    case 589:
        return scrambledRadicalInverseSpecialized<4297>(perm, a);
    case 590:
        return scrambledRadicalInverseSpecialized<4327>(perm, a);
    case 591:
        return scrambledRadicalInverseSpecialized<4337>(perm, a);
    case 592:
        return scrambledRadicalInverseSpecialized<4339>(perm, a);
    case 593:
        return scrambledRadicalInverseSpecialized<4349>(perm, a);
    case 594:
        return scrambledRadicalInverseSpecialized<4357>(perm, a);
    case 595:
        return scrambledRadicalInverseSpecialized<4363>(perm, a);
    case 596:
        return scrambledRadicalInverseSpecialized<4373>(perm, a);
    case 597:
        return scrambledRadicalInverseSpecialized<4391>(perm, a);
    case 598:
        return scrambledRadicalInverseSpecialized<4397>(perm, a);
    case 599:
        return scrambledRadicalInverseSpecialized<4409>(perm, a);
    case 600:
        return scrambledRadicalInverseSpecialized<4421>(perm, a);
    case 601:
        return scrambledRadicalInverseSpecialized<4423>(perm, a);
    case 602:
        return scrambledRadicalInverseSpecialized<4441>(perm, a);
    case 603:
        return scrambledRadicalInverseSpecialized<4447>(perm, a);
    case 604:
        return scrambledRadicalInverseSpecialized<4451>(perm, a);
    case 605:
        return scrambledRadicalInverseSpecialized<4457>(perm, a);
    case 606:
        return scrambledRadicalInverseSpecialized<4463>(perm, a);
    case 607:
        return scrambledRadicalInverseSpecialized<4481>(perm, a);
    case 608:
        return scrambledRadicalInverseSpecialized<4483>(perm, a);
    case 609:
        return scrambledRadicalInverseSpecialized<4493>(perm, a);
    case 610:
        return scrambledRadicalInverseSpecialized<4507>(perm, a);
    case 611:
        return scrambledRadicalInverseSpecialized<4513>(perm, a);
    case 612:
        return scrambledRadicalInverseSpecialized<4517>(perm, a);
    case 613:
        return scrambledRadicalInverseSpecialized<4519>(perm, a);
    case 614:
        return scrambledRadicalInverseSpecialized<4523>(perm, a);
    case 615:
        return scrambledRadicalInverseSpecialized<4547>(perm, a);
    case 616:
        return scrambledRadicalInverseSpecialized<4549>(perm, a);
    case 617:
        return scrambledRadicalInverseSpecialized<4561>(perm, a);
    case 618:
        return scrambledRadicalInverseSpecialized<4567>(perm, a);
    case 619:
        return scrambledRadicalInverseSpecialized<4583>(perm, a);
    case 620:
        return scrambledRadicalInverseSpecialized<4591>(perm, a);
    case 621:
        return scrambledRadicalInverseSpecialized<4597>(perm, a);
    case 622:
        return scrambledRadicalInverseSpecialized<4603>(perm, a);
    case 623:
        return scrambledRadicalInverseSpecialized<4621>(perm, a);
    case 624:
        return scrambledRadicalInverseSpecialized<4637>(perm, a);
    case 625:
        return scrambledRadicalInverseSpecialized<4639>(perm, a);
    case 626:
        return scrambledRadicalInverseSpecialized<4643>(perm, a);
    case 627:
        return scrambledRadicalInverseSpecialized<4649>(perm, a);
    case 628:
        return scrambledRadicalInverseSpecialized<4651>(perm, a);
    case 629:
        return scrambledRadicalInverseSpecialized<4657>(perm, a);
    case 630:
        return scrambledRadicalInverseSpecialized<4663>(perm, a);
    case 631:
        return scrambledRadicalInverseSpecialized<4673>(perm, a);
    case 632:
        return scrambledRadicalInverseSpecialized<4679>(perm, a);
    case 633:
        return scrambledRadicalInverseSpecialized<4691>(perm, a);
    case 634:
        return scrambledRadicalInverseSpecialized<4703>(perm, a);
    case 635:
        return scrambledRadicalInverseSpecialized<4721>(perm, a);
    case 636:
        return scrambledRadicalInverseSpecialized<4723>(perm, a);
    case 637:
        return scrambledRadicalInverseSpecialized<4729>(perm, a);
    case 638:
        return scrambledRadicalInverseSpecialized<4733>(perm, a);
    case 639:
        return scrambledRadicalInverseSpecialized<4751>(perm, a);
    case 640:
        return scrambledRadicalInverseSpecialized<4759>(perm, a);
    case 641:
        return scrambledRadicalInverseSpecialized<4783>(perm, a);
    case 642:
        return scrambledRadicalInverseSpecialized<4787>(perm, a);
    case 643:
        return scrambledRadicalInverseSpecialized<4789>(perm, a);
    case 644:
        return scrambledRadicalInverseSpecialized<4793>(perm, a);
    case 645:
        return scrambledRadicalInverseSpecialized<4799>(perm, a);
    case 646:
        return scrambledRadicalInverseSpecialized<4801>(perm, a);
    case 647:
        return scrambledRadicalInverseSpecialized<4813>(perm, a);
    case 648:
        return scrambledRadicalInverseSpecialized<4817>(perm, a);
    case 649:
        return scrambledRadicalInverseSpecialized<4831>(perm, a);
    case 650:
        return scrambledRadicalInverseSpecialized<4861>(perm, a);
    case 651:
        return scrambledRadicalInverseSpecialized<4871>(perm, a);
    case 652:
        return scrambledRadicalInverseSpecialized<4877>(perm, a);
    case 653:
        return scrambledRadicalInverseSpecialized<4889>(perm, a);
    case 654:
        return scrambledRadicalInverseSpecialized<4903>(perm, a);
    case 655:
        return scrambledRadicalInverseSpecialized<4909>(perm, a);
    case 656:
        return scrambledRadicalInverseSpecialized<4919>(perm, a);
    case 657:
        return scrambledRadicalInverseSpecialized<4931>(perm, a);
    case 658:
        return scrambledRadicalInverseSpecialized<4933>(perm, a);
    case 659:
        return scrambledRadicalInverseSpecialized<4937>(perm, a);
    case 660:
        return scrambledRadicalInverseSpecialized<4943>(perm, a);
    case 661:
        return scrambledRadicalInverseSpecialized<4951>(perm, a);
    case 662:
        return scrambledRadicalInverseSpecialized<4957>(perm, a);
    case 663:
        return scrambledRadicalInverseSpecialized<4967>(perm, a);
    case 664:
        return scrambledRadicalInverseSpecialized<4969>(perm, a);
    case 665:
        return scrambledRadicalInverseSpecialized<4973>(perm, a);
    case 666:
        return scrambledRadicalInverseSpecialized<4987>(perm, a);
    case 667:
        return scrambledRadicalInverseSpecialized<4993>(perm, a);
    case 668:
        return scrambledRadicalInverseSpecialized<4999>(perm, a);
    case 669:
        return scrambledRadicalInverseSpecialized<5003>(perm, a);
    case 670:
        return scrambledRadicalInverseSpecialized<5009>(perm, a);
    case 671:
        return scrambledRadicalInverseSpecialized<5011>(perm, a);
    case 672:
        return scrambledRadicalInverseSpecialized<5021>(perm, a);
    case 673:
        return scrambledRadicalInverseSpecialized<5023>(perm, a);
    case 674:
        return scrambledRadicalInverseSpecialized<5039>(perm, a);
    case 675:
        return scrambledRadicalInverseSpecialized<5051>(perm, a);
    case 676:
        return scrambledRadicalInverseSpecialized<5059>(perm, a);
    case 677:
        return scrambledRadicalInverseSpecialized<5077>(perm, a);
    case 678:
        return scrambledRadicalInverseSpecialized<5081>(perm, a);
    case 679:
        return scrambledRadicalInverseSpecialized<5087>(perm, a);
    case 680:
        return scrambledRadicalInverseSpecialized<5099>(perm, a);
    case 681:
        return scrambledRadicalInverseSpecialized<5101>(perm, a);
    case 682:
        return scrambledRadicalInverseSpecialized<5107>(perm, a);
    case 683:
        return scrambledRadicalInverseSpecialized<5113>(perm, a);
    case 684:
        return scrambledRadicalInverseSpecialized<5119>(perm, a);
    case 685:
        return scrambledRadicalInverseSpecialized<5147>(perm, a);
    case 686:
        return scrambledRadicalInverseSpecialized<5153>(perm, a);
    case 687:
        return scrambledRadicalInverseSpecialized<5167>(perm, a);
    case 688:
        return scrambledRadicalInverseSpecialized<5171>(perm, a);
    case 689:
        return scrambledRadicalInverseSpecialized<5179>(perm, a);
    case 690:
        return scrambledRadicalInverseSpecialized<5189>(perm, a);
    case 691:
        return scrambledRadicalInverseSpecialized<5197>(perm, a);
    case 692:
        return scrambledRadicalInverseSpecialized<5209>(perm, a);
    case 693:
        return scrambledRadicalInverseSpecialized<5227>(perm, a);
    case 694:
        return scrambledRadicalInverseSpecialized<5231>(perm, a);
    case 695:
        return scrambledRadicalInverseSpecialized<5233>(perm, a);
    case 696:
        return scrambledRadicalInverseSpecialized<5237>(perm, a);
    case 697:
        return scrambledRadicalInverseSpecialized<5261>(perm, a);
    case 698:
        return scrambledRadicalInverseSpecialized<5273>(perm, a);
    case 699:
        return scrambledRadicalInverseSpecialized<5279>(perm, a);
    case 700:
        return scrambledRadicalInverseSpecialized<5281>(perm, a);
    case 701:
        return scrambledRadicalInverseSpecialized<5297>(perm, a);
    case 702:
        return scrambledRadicalInverseSpecialized<5303>(perm, a);
    case 703:
        return scrambledRadicalInverseSpecialized<5309>(perm, a);
    case 704:
        return scrambledRadicalInverseSpecialized<5323>(perm, a);
    case 705:
        return scrambledRadicalInverseSpecialized<5333>(perm, a);
    case 706:
        return scrambledRadicalInverseSpecialized<5347>(perm, a);
    case 707:
        return scrambledRadicalInverseSpecialized<5351>(perm, a);
    case 708:
        return scrambledRadicalInverseSpecialized<5381>(perm, a);
    case 709:
        return scrambledRadicalInverseSpecialized<5387>(perm, a);
    case 710:
        return scrambledRadicalInverseSpecialized<5393>(perm, a);
    case 711:
        return scrambledRadicalInverseSpecialized<5399>(perm, a);
    case 712:
        return scrambledRadicalInverseSpecialized<5407>(perm, a);
    case 713:
        return scrambledRadicalInverseSpecialized<5413>(perm, a);
    case 714:
        return scrambledRadicalInverseSpecialized<5417>(perm, a);
    case 715:
        return scrambledRadicalInverseSpecialized<5419>(perm, a);
    case 716:
        return scrambledRadicalInverseSpecialized<5431>(perm, a);
    case 717:
        return scrambledRadicalInverseSpecialized<5437>(perm, a);
    case 718:
        return scrambledRadicalInverseSpecialized<5441>(perm, a);
    case 719:
        return scrambledRadicalInverseSpecialized<5443>(perm, a);
    case 720:
        return scrambledRadicalInverseSpecialized<5449>(perm, a);
    case 721:
        return scrambledRadicalInverseSpecialized<5471>(perm, a);
    case 722:
        return scrambledRadicalInverseSpecialized<5477>(perm, a);
    case 723:
        return scrambledRadicalInverseSpecialized<5479>(perm, a);
    case 724:
        return scrambledRadicalInverseSpecialized<5483>(perm, a);
    case 725:
        return scrambledRadicalInverseSpecialized<5501>(perm, a);
    case 726:
        return scrambledRadicalInverseSpecialized<5503>(perm, a);
    case 727:
        return scrambledRadicalInverseSpecialized<5507>(perm, a);
    case 728:
        return scrambledRadicalInverseSpecialized<5519>(perm, a);
    case 729:
        return scrambledRadicalInverseSpecialized<5521>(perm, a);
    case 730:
        return scrambledRadicalInverseSpecialized<5527>(perm, a);
    case 731:
        return scrambledRadicalInverseSpecialized<5531>(perm, a);
    case 732:
        return scrambledRadicalInverseSpecialized<5557>(perm, a);
    case 733:
        return scrambledRadicalInverseSpecialized<5563>(perm, a);
    case 734:
        return scrambledRadicalInverseSpecialized<5569>(perm, a);
    case 735:
        return scrambledRadicalInverseSpecialized<5573>(perm, a);
    case 736:
        return scrambledRadicalInverseSpecialized<5581>(perm, a);
    case 737:
        return scrambledRadicalInverseSpecialized<5591>(perm, a);
    case 738:
        return scrambledRadicalInverseSpecialized<5623>(perm, a);
    case 739:
        return scrambledRadicalInverseSpecialized<5639>(perm, a);
    case 740:
        return scrambledRadicalInverseSpecialized<5641>(perm, a);
    case 741:
        return scrambledRadicalInverseSpecialized<5647>(perm, a);
    case 742:
        return scrambledRadicalInverseSpecialized<5651>(perm, a);
    case 743:
        return scrambledRadicalInverseSpecialized<5653>(perm, a);
    case 744:
        return scrambledRadicalInverseSpecialized<5657>(perm, a);
    case 745:
        return scrambledRadicalInverseSpecialized<5659>(perm, a);
    case 746:
        return scrambledRadicalInverseSpecialized<5669>(perm, a);
    case 747:
        return scrambledRadicalInverseSpecialized<5683>(perm, a);
    case 748:
        return scrambledRadicalInverseSpecialized<5689>(perm, a);
    case 749:
        return scrambledRadicalInverseSpecialized<5693>(perm, a);
    case 750:
        return scrambledRadicalInverseSpecialized<5701>(perm, a);
    case 751:
        return scrambledRadicalInverseSpecialized<5711>(perm, a);
    case 752:
        return scrambledRadicalInverseSpecialized<5717>(perm, a);
    case 753:
        return scrambledRadicalInverseSpecialized<5737>(perm, a);
    case 754:
        return scrambledRadicalInverseSpecialized<5741>(perm, a);
    case 755:
        return scrambledRadicalInverseSpecialized<5743>(perm, a);
    case 756:
        return scrambledRadicalInverseSpecialized<5749>(perm, a);
    case 757:
        return scrambledRadicalInverseSpecialized<5779>(perm, a);
    case 758:
        return scrambledRadicalInverseSpecialized<5783>(perm, a);
    case 759:
        return scrambledRadicalInverseSpecialized<5791>(perm, a);
    case 760:
        return scrambledRadicalInverseSpecialized<5801>(perm, a);
    case 761:
        return scrambledRadicalInverseSpecialized<5807>(perm, a);
    case 762:
        return scrambledRadicalInverseSpecialized<5813>(perm, a);
    case 763:
        return scrambledRadicalInverseSpecialized<5821>(perm, a);
    case 764:
        return scrambledRadicalInverseSpecialized<5827>(perm, a);
    case 765:
        return scrambledRadicalInverseSpecialized<5839>(perm, a);
    case 766:
        return scrambledRadicalInverseSpecialized<5843>(perm, a);
    case 767:
        return scrambledRadicalInverseSpecialized<5849>(perm, a);
    case 768:
        return scrambledRadicalInverseSpecialized<5851>(perm, a);
    case 769:
        return scrambledRadicalInverseSpecialized<5857>(perm, a);
    case 770:
        return scrambledRadicalInverseSpecialized<5861>(perm, a);
    case 771:
        return scrambledRadicalInverseSpecialized<5867>(perm, a);
    case 772:
        return scrambledRadicalInverseSpecialized<5869>(perm, a);
    case 773:
        return scrambledRadicalInverseSpecialized<5879>(perm, a);
    case 774:
        return scrambledRadicalInverseSpecialized<5881>(perm, a);
    case 775:
        return scrambledRadicalInverseSpecialized<5897>(perm, a);
    case 776:
        return scrambledRadicalInverseSpecialized<5903>(perm, a);
    case 777:
        return scrambledRadicalInverseSpecialized<5923>(perm, a);
    case 778:
        return scrambledRadicalInverseSpecialized<5927>(perm, a);
    case 779:
        return scrambledRadicalInverseSpecialized<5939>(perm, a);
    case 780:
        return scrambledRadicalInverseSpecialized<5953>(perm, a);
    case 781:
        return scrambledRadicalInverseSpecialized<5981>(perm, a);
    case 782:
        return scrambledRadicalInverseSpecialized<5987>(perm, a);
    case 783:
        return scrambledRadicalInverseSpecialized<6007>(perm, a);
    case 784:
        return scrambledRadicalInverseSpecialized<6011>(perm, a);
    case 785:
        return scrambledRadicalInverseSpecialized<6029>(perm, a);
    case 786:
        return scrambledRadicalInverseSpecialized<6037>(perm, a);
    case 787:
        return scrambledRadicalInverseSpecialized<6043>(perm, a);
    case 788:
        return scrambledRadicalInverseSpecialized<6047>(perm, a);
    case 789:
        return scrambledRadicalInverseSpecialized<6053>(perm, a);
    case 790:
        return scrambledRadicalInverseSpecialized<6067>(perm, a);
    case 791:
        return scrambledRadicalInverseSpecialized<6073>(perm, a);
    case 792:
        return scrambledRadicalInverseSpecialized<6079>(perm, a);
    case 793:
        return scrambledRadicalInverseSpecialized<6089>(perm, a);
    case 794:
        return scrambledRadicalInverseSpecialized<6091>(perm, a);
    case 795:
        return scrambledRadicalInverseSpecialized<6101>(perm, a);
    case 796:
        return scrambledRadicalInverseSpecialized<6113>(perm, a);
    case 797:
        return scrambledRadicalInverseSpecialized<6121>(perm, a);
    case 798:
        return scrambledRadicalInverseSpecialized<6131>(perm, a);
    case 799:
        return scrambledRadicalInverseSpecialized<6133>(perm, a);
    case 800:
        return scrambledRadicalInverseSpecialized<6143>(perm, a);
    case 801:
        return scrambledRadicalInverseSpecialized<6151>(perm, a);
    case 802:
        return scrambledRadicalInverseSpecialized<6163>(perm, a);
    case 803:
        return scrambledRadicalInverseSpecialized<6173>(perm, a);
    case 804:
        return scrambledRadicalInverseSpecialized<6197>(perm, a);
    case 805:
        return scrambledRadicalInverseSpecialized<6199>(perm, a);
    case 806:
        return scrambledRadicalInverseSpecialized<6203>(perm, a);
    case 807:
        return scrambledRadicalInverseSpecialized<6211>(perm, a);
    case 808:
        return scrambledRadicalInverseSpecialized<6217>(perm, a);
    case 809:
        return scrambledRadicalInverseSpecialized<6221>(perm, a);
    case 810:
        return scrambledRadicalInverseSpecialized<6229>(perm, a);
    case 811:
        return scrambledRadicalInverseSpecialized<6247>(perm, a);
    case 812:
        return scrambledRadicalInverseSpecialized<6257>(perm, a);
    case 813:
        return scrambledRadicalInverseSpecialized<6263>(perm, a);
    case 814:
        return scrambledRadicalInverseSpecialized<6269>(perm, a);
    case 815:
        return scrambledRadicalInverseSpecialized<6271>(perm, a);
    case 816:
        return scrambledRadicalInverseSpecialized<6277>(perm, a);
    case 817:
        return scrambledRadicalInverseSpecialized<6287>(perm, a);
    case 818:
        return scrambledRadicalInverseSpecialized<6299>(perm, a);
    case 819:
        return scrambledRadicalInverseSpecialized<6301>(perm, a);
    case 820:
        return scrambledRadicalInverseSpecialized<6311>(perm, a);
    case 821:
        return scrambledRadicalInverseSpecialized<6317>(perm, a);
    case 822:
        return scrambledRadicalInverseSpecialized<6323>(perm, a);
    case 823:
        return scrambledRadicalInverseSpecialized<6329>(perm, a);
    case 824:
        return scrambledRadicalInverseSpecialized<6337>(perm, a);
    case 825:
        return scrambledRadicalInverseSpecialized<6343>(perm, a);
    case 826:
        return scrambledRadicalInverseSpecialized<6353>(perm, a);
    case 827:
        return scrambledRadicalInverseSpecialized<6359>(perm, a);
    case 828:
        return scrambledRadicalInverseSpecialized<6361>(perm, a);
    case 829:
        return scrambledRadicalInverseSpecialized<6367>(perm, a);
    case 830:
        return scrambledRadicalInverseSpecialized<6373>(perm, a);
    case 831:
        return scrambledRadicalInverseSpecialized<6379>(perm, a);
    case 832:
        return scrambledRadicalInverseSpecialized<6389>(perm, a);
    case 833:
        return scrambledRadicalInverseSpecialized<6397>(perm, a);
    case 834:
        return scrambledRadicalInverseSpecialized<6421>(perm, a);
    case 835:
        return scrambledRadicalInverseSpecialized<6427>(perm, a);
    case 836:
        return scrambledRadicalInverseSpecialized<6449>(perm, a);
    case 837:
        return scrambledRadicalInverseSpecialized<6451>(perm, a);
    case 838:
        return scrambledRadicalInverseSpecialized<6469>(perm, a);
    case 839:
        return scrambledRadicalInverseSpecialized<6473>(perm, a);
    case 840:
        return scrambledRadicalInverseSpecialized<6481>(perm, a);
    case 841:
        return scrambledRadicalInverseSpecialized<6491>(perm, a);
    case 842:
        return scrambledRadicalInverseSpecialized<6521>(perm, a);
    case 843:
        return scrambledRadicalInverseSpecialized<6529>(perm, a);
    case 844:
        return scrambledRadicalInverseSpecialized<6547>(perm, a);
    case 845:
        return scrambledRadicalInverseSpecialized<6551>(perm, a);
    case 846:
        return scrambledRadicalInverseSpecialized<6553>(perm, a);
    case 847:
        return scrambledRadicalInverseSpecialized<6563>(perm, a);
    case 848:
        return scrambledRadicalInverseSpecialized<6569>(perm, a);
    case 849:
        return scrambledRadicalInverseSpecialized<6571>(perm, a);
    case 850:
        return scrambledRadicalInverseSpecialized<6577>(perm, a);
    case 851:
        return scrambledRadicalInverseSpecialized<6581>(perm, a);
    case 852:
        return scrambledRadicalInverseSpecialized<6599>(perm, a);
    case 853:
        return scrambledRadicalInverseSpecialized<6607>(perm, a);
    case 854:
        return scrambledRadicalInverseSpecialized<6619>(perm, a);
    case 855:
        return scrambledRadicalInverseSpecialized<6637>(perm, a);
    case 856:
        return scrambledRadicalInverseSpecialized<6653>(perm, a);
    case 857:
        return scrambledRadicalInverseSpecialized<6659>(perm, a);
    case 858:
        return scrambledRadicalInverseSpecialized<6661>(perm, a);
    case 859:
        return scrambledRadicalInverseSpecialized<6673>(perm, a);
    case 860:
        return scrambledRadicalInverseSpecialized<6679>(perm, a);
    case 861:
        return scrambledRadicalInverseSpecialized<6689>(perm, a);
    case 862:
        return scrambledRadicalInverseSpecialized<6691>(perm, a);
    case 863:
        return scrambledRadicalInverseSpecialized<6701>(perm, a);
    case 864:
        return scrambledRadicalInverseSpecialized<6703>(perm, a);
    case 865:
        return scrambledRadicalInverseSpecialized<6709>(perm, a);
    case 866:
        return scrambledRadicalInverseSpecialized<6719>(perm, a);
    case 867:
        return scrambledRadicalInverseSpecialized<6733>(perm, a);
    case 868:
        return scrambledRadicalInverseSpecialized<6737>(perm, a);
    case 869:
        return scrambledRadicalInverseSpecialized<6761>(perm, a);
    case 870:
        return scrambledRadicalInverseSpecialized<6763>(perm, a);
    case 871:
        return scrambledRadicalInverseSpecialized<6779>(perm, a);
    case 872:
        return scrambledRadicalInverseSpecialized<6781>(perm, a);
    case 873:
        return scrambledRadicalInverseSpecialized<6791>(perm, a);
    case 874:
        return scrambledRadicalInverseSpecialized<6793>(perm, a);
    case 875:
        return scrambledRadicalInverseSpecialized<6803>(perm, a);
    case 876:
        return scrambledRadicalInverseSpecialized<6823>(perm, a);
    case 877:
        return scrambledRadicalInverseSpecialized<6827>(perm, a);
    case 878:
        return scrambledRadicalInverseSpecialized<6829>(perm, a);
    case 879:
        return scrambledRadicalInverseSpecialized<6833>(perm, a);
    case 880:
        return scrambledRadicalInverseSpecialized<6841>(perm, a);
    case 881:
        return scrambledRadicalInverseSpecialized<6857>(perm, a);
    case 882:
        return scrambledRadicalInverseSpecialized<6863>(perm, a);
    case 883:
        return scrambledRadicalInverseSpecialized<6869>(perm, a);
    case 884:
        return scrambledRadicalInverseSpecialized<6871>(perm, a);
    case 885:
        return scrambledRadicalInverseSpecialized<6883>(perm, a);
    case 886:
        return scrambledRadicalInverseSpecialized<6899>(perm, a);
    case 887:
        return scrambledRadicalInverseSpecialized<6907>(perm, a);
    case 888:
        return scrambledRadicalInverseSpecialized<6911>(perm, a);
    case 889:
        return scrambledRadicalInverseSpecialized<6917>(perm, a);
    case 890:
        return scrambledRadicalInverseSpecialized<6947>(perm, a);
    case 891:
        return scrambledRadicalInverseSpecialized<6949>(perm, a);
    case 892:
        return scrambledRadicalInverseSpecialized<6959>(perm, a);
    case 893:
        return scrambledRadicalInverseSpecialized<6961>(perm, a);
    case 894:
        return scrambledRadicalInverseSpecialized<6967>(perm, a);
    case 895:
        return scrambledRadicalInverseSpecialized<6971>(perm, a);
    case 896:
        return scrambledRadicalInverseSpecialized<6977>(perm, a);
    case 897:
        return scrambledRadicalInverseSpecialized<6983>(perm, a);
    case 898:
        return scrambledRadicalInverseSpecialized<6991>(perm, a);
    case 899:
        return scrambledRadicalInverseSpecialized<6997>(perm, a);
    case 900:
        return scrambledRadicalInverseSpecialized<7001>(perm, a);
    case 901:
        return scrambledRadicalInverseSpecialized<7013>(perm, a);
    case 902:
        return scrambledRadicalInverseSpecialized<7019>(perm, a);
    case 903:
        return scrambledRadicalInverseSpecialized<7027>(perm, a);
    case 904:
        return scrambledRadicalInverseSpecialized<7039>(perm, a);
    case 905:
        return scrambledRadicalInverseSpecialized<7043>(perm, a);
    case 906:
        return scrambledRadicalInverseSpecialized<7057>(perm, a);
    case 907:
        return scrambledRadicalInverseSpecialized<7069>(perm, a);
    case 908:
        return scrambledRadicalInverseSpecialized<7079>(perm, a);
    case 909:
        return scrambledRadicalInverseSpecialized<7103>(perm, a);
    case 910:
        return scrambledRadicalInverseSpecialized<7109>(perm, a);
    case 911:
        return scrambledRadicalInverseSpecialized<7121>(perm, a);
    case 912:
        return scrambledRadicalInverseSpecialized<7127>(perm, a);
    case 913:
        return scrambledRadicalInverseSpecialized<7129>(perm, a);
    case 914:
        return scrambledRadicalInverseSpecialized<7151>(perm, a);
    case 915:
        return scrambledRadicalInverseSpecialized<7159>(perm, a);
    case 916:
        return scrambledRadicalInverseSpecialized<7177>(perm, a);
    case 917:
        return scrambledRadicalInverseSpecialized<7187>(perm, a);
    case 918:
        return scrambledRadicalInverseSpecialized<7193>(perm, a);
    case 919:
        return scrambledRadicalInverseSpecialized<7207>(perm, a);
    case 920:
        return scrambledRadicalInverseSpecialized<7211>(perm, a);
    case 921:
        return scrambledRadicalInverseSpecialized<7213>(perm, a);
    case 922:
        return scrambledRadicalInverseSpecialized<7219>(perm, a);
    case 923:
        return scrambledRadicalInverseSpecialized<7229>(perm, a);
    case 924:
        return scrambledRadicalInverseSpecialized<7237>(perm, a);
    case 925:
        return scrambledRadicalInverseSpecialized<7243>(perm, a);
    case 926:
        return scrambledRadicalInverseSpecialized<7247>(perm, a);
    case 927:
        return scrambledRadicalInverseSpecialized<7253>(perm, a);
    case 928:
        return scrambledRadicalInverseSpecialized<7283>(perm, a);
    case 929:
        return scrambledRadicalInverseSpecialized<7297>(perm, a);
    case 930:
        return scrambledRadicalInverseSpecialized<7307>(perm, a);
    case 931:
        return scrambledRadicalInverseSpecialized<7309>(perm, a);
    case 932:
        return scrambledRadicalInverseSpecialized<7321>(perm, a);
    case 933:
        return scrambledRadicalInverseSpecialized<7331>(perm, a);
    case 934:
        return scrambledRadicalInverseSpecialized<7333>(perm, a);
    case 935:
        return scrambledRadicalInverseSpecialized<7349>(perm, a);
    case 936:
        return scrambledRadicalInverseSpecialized<7351>(perm, a);
    case 937:
        return scrambledRadicalInverseSpecialized<7369>(perm, a);
    case 938:
        return scrambledRadicalInverseSpecialized<7393>(perm, a);
    case 939:
        return scrambledRadicalInverseSpecialized<7411>(perm, a);
    case 940:
        return scrambledRadicalInverseSpecialized<7417>(perm, a);
    case 941:
        return scrambledRadicalInverseSpecialized<7433>(perm, a);
    case 942:
        return scrambledRadicalInverseSpecialized<7451>(perm, a);
    case 943:
        return scrambledRadicalInverseSpecialized<7457>(perm, a);
    case 944:
        return scrambledRadicalInverseSpecialized<7459>(perm, a);
    case 945:
        return scrambledRadicalInverseSpecialized<7477>(perm, a);
    case 946:
        return scrambledRadicalInverseSpecialized<7481>(perm, a);
    case 947:
        return scrambledRadicalInverseSpecialized<7487>(perm, a);
    case 948:
        return scrambledRadicalInverseSpecialized<7489>(perm, a);
    case 949:
        return scrambledRadicalInverseSpecialized<7499>(perm, a);
    case 950:
        return scrambledRadicalInverseSpecialized<7507>(perm, a);
    case 951:
        return scrambledRadicalInverseSpecialized<7517>(perm, a);
    case 952:
        return scrambledRadicalInverseSpecialized<7523>(perm, a);
    case 953:
        return scrambledRadicalInverseSpecialized<7529>(perm, a);
    case 954:
        return scrambledRadicalInverseSpecialized<7537>(perm, a);
    case 955:
        return scrambledRadicalInverseSpecialized<7541>(perm, a);
    case 956:
        return scrambledRadicalInverseSpecialized<7547>(perm, a);
    case 957:
        return scrambledRadicalInverseSpecialized<7549>(perm, a);
    case 958:
        return scrambledRadicalInverseSpecialized<7559>(perm, a);
    case 959:
        return scrambledRadicalInverseSpecialized<7561>(perm, a);
    case 960:
        return scrambledRadicalInverseSpecialized<7573>(perm, a);
    case 961:
        return scrambledRadicalInverseSpecialized<7577>(perm, a);
    case 962:
        return scrambledRadicalInverseSpecialized<7583>(perm, a);
    case 963:
        return scrambledRadicalInverseSpecialized<7589>(perm, a);
    case 964:
        return scrambledRadicalInverseSpecialized<7591>(perm, a);
    case 965:
        return scrambledRadicalInverseSpecialized<7603>(perm, a);
    case 966:
        return scrambledRadicalInverseSpecialized<7607>(perm, a);
    case 967:
        return scrambledRadicalInverseSpecialized<7621>(perm, a);
    case 968:
        return scrambledRadicalInverseSpecialized<7639>(perm, a);
    case 969:
        return scrambledRadicalInverseSpecialized<7643>(perm, a);
    case 970:
        return scrambledRadicalInverseSpecialized<7649>(perm, a);
    case 971:
        return scrambledRadicalInverseSpecialized<7669>(perm, a);
    case 972:
        return scrambledRadicalInverseSpecialized<7673>(perm, a);
    case 973:
        return scrambledRadicalInverseSpecialized<7681>(perm, a);
    case 974:
        return scrambledRadicalInverseSpecialized<7687>(perm, a);
    case 975:
        return scrambledRadicalInverseSpecialized<7691>(perm, a);
    case 976:
        return scrambledRadicalInverseSpecialized<7699>(perm, a);
    case 977:
        return scrambledRadicalInverseSpecialized<7703>(perm, a);
    case 978:
        return scrambledRadicalInverseSpecialized<7717>(perm, a);
    case 979:
        return scrambledRadicalInverseSpecialized<7723>(perm, a);
    case 980:
        return scrambledRadicalInverseSpecialized<7727>(perm, a);
    case 981:
        return scrambledRadicalInverseSpecialized<7741>(perm, a);
    case 982:
        return scrambledRadicalInverseSpecialized<7753>(perm, a);
    case 983:
        return scrambledRadicalInverseSpecialized<7757>(perm, a);
    case 984:
        return scrambledRadicalInverseSpecialized<7759>(perm, a);
    case 985:
        return scrambledRadicalInverseSpecialized<7789>(perm, a);
    case 986:
        return scrambledRadicalInverseSpecialized<7793>(perm, a);
    case 987:
        return scrambledRadicalInverseSpecialized<7817>(perm, a);
    case 988:
        return scrambledRadicalInverseSpecialized<7823>(perm, a);
    case 989:
        return scrambledRadicalInverseSpecialized<7829>(perm, a);
    case 990:
        return scrambledRadicalInverseSpecialized<7841>(perm, a);
    case 991:
        return scrambledRadicalInverseSpecialized<7853>(perm, a);
    case 992:
        return scrambledRadicalInverseSpecialized<7867>(perm, a);
    case 993:
        return scrambledRadicalInverseSpecialized<7873>(perm, a);
    case 994:
        return scrambledRadicalInverseSpecialized<7877>(perm, a);
    case 995:
        return scrambledRadicalInverseSpecialized<7879>(perm, a);
    case 996:
        return scrambledRadicalInverseSpecialized<7883>(perm, a);
    case 997:
        return scrambledRadicalInverseSpecialized<7901>(perm, a);
    case 998:
        return scrambledRadicalInverseSpecialized<7907>(perm, a);
    case 999:
        return scrambledRadicalInverseSpecialized<7919>(perm, a);
    case 1000:
        return scrambledRadicalInverseSpecialized<7927>(perm, a);
    case 1001:
        return scrambledRadicalInverseSpecialized<7933>(perm, a);
    case 1002:
        return scrambledRadicalInverseSpecialized<7937>(perm, a);
    case 1003:
        return scrambledRadicalInverseSpecialized<7949>(perm, a);
    case 1004:
        return scrambledRadicalInverseSpecialized<7951>(perm, a);
    case 1005:
        return scrambledRadicalInverseSpecialized<7963>(perm, a);
    case 1006:
        return scrambledRadicalInverseSpecialized<7993>(perm, a);
    case 1007:
        return scrambledRadicalInverseSpecialized<8009>(perm, a);
    case 1008:
        return scrambledRadicalInverseSpecialized<8011>(perm, a);
    case 1009:
        return scrambledRadicalInverseSpecialized<8017>(perm, a);
    case 1010:
        return scrambledRadicalInverseSpecialized<8039>(perm, a);
    case 1011:
        return scrambledRadicalInverseSpecialized<8053>(perm, a);
    case 1012:
        return scrambledRadicalInverseSpecialized<8059>(perm, a);
    case 1013:
        return scrambledRadicalInverseSpecialized<8069>(perm, a);
    case 1014:
        return scrambledRadicalInverseSpecialized<8081>(perm, a);
    case 1015:
        return scrambledRadicalInverseSpecialized<8087>(perm, a);
    case 1016:
        return scrambledRadicalInverseSpecialized<8089>(perm, a);
    case 1017:
        return scrambledRadicalInverseSpecialized<8093>(perm, a);
    case 1018:
        return scrambledRadicalInverseSpecialized<8101>(perm, a);
    case 1019:
        return scrambledRadicalInverseSpecialized<8111>(perm, a);
    case 1020:
        return scrambledRadicalInverseSpecialized<8117>(perm, a);
    case 1021:
        return scrambledRadicalInverseSpecialized<8123>(perm, a);
    case 1022:
        return scrambledRadicalInverseSpecialized<8147>(perm, a);
    case 1023:
        return scrambledRadicalInverseSpecialized<8161>(perm, a);
    default:
        return 0;
    }
}

}  // namespace phyr
