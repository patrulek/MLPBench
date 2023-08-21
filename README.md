# MLPBench - pointer chasing benchmark for Windows

## Overview

This project is non-1:1 copy of [Lemire's testingmlp](https://github.com/lemire/testingmlp), but for Windows specifically.

## Build and run

To build this project from sources most recent Visual Studio (at least 17.7) is needed.

This project uses [large pages](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support) which are not allowed by default for any user. To enable it follow instructions from [this link](https://learn.microsoft.com/en-us/sql/database-engine/configure-windows/enable-the-lock-pages-in-memory-option-windows?view=sql-server-ver16).

### Troubleshooting

In a case when you see an error like this:

```console
Memory incorrectly allocated: error = 1450
```

system reboot may be required. If it doesn't help, you may not have enough RAM for system to allocate 2GB of non-paged memory.

## Sample output

```console
RAM Info:
 * Channels: 2
 * Speed [MT/s]: 3200
 * Theoretical bandwidth: 51.2GB/s

CPU Info:
 * Physical cores: 8
 * Logical cores: 16
 * Hyperthreading: true

Allocating non-paged (locked) memory of size 2147483648 bytes with system decided page sizes...
Memory aligned to 1073741824 bytes: data pointer = 0x27f80000000
Allocating array made of 268435456 (2048 MB) 64-bit words...
Initializing array...
Verifying sequential and random access sums...

Calculating time for sequential access (64 rounds)...
--------------------------------------------------------------------------------
- # of threads --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -
--------------------------------------------------------------------------------
             1        4.817       28532.1        1.00      0.280          55.7
             2        3.369       40795.2        1.43      0.196          79.7
             4        3.118       44079.2        1.54      0.181          86.1
             8        3.261       42146.3        1.48      0.190          82.3
            16        3.270       42030.3        1.47      0.190          82.1
            32        3.283       41863.8        1.47      0.191          81.8
            64        3.290       41774.8        1.46      0.192          81.6

Calculating time for random access (single round)...
-----------------------------------------------------------------------------------------------------
- # of threads --- lanes per thread --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -
-----------------------------------------------------------------------------------------------------
             1                    1       26.322          81.6        1.00       98.1           0.2
             1                    2       13.175         163.0        2.00       49.1           0.4
             1                    4        6.489         330.9        4.06       24.2           0.8
             1                    8        3.194         672.3        8.24       11.9           1.5
             1                   16        1.702        1261.7       15.47        6.3           2.9
             1                   32        1.267        1694.9       20.78        4.7           3.8
             1                   64        1.200        1789.6       21.93        4.5           4.1
= ------------------------------------------------------------------------------------------------- =
             2                    1       13.077         164.2        2.01       48.7           0.4
             2                    2        6.475         331.7        4.07       24.1           0.8
             2                    4        3.199         671.3        8.23       11.9           1.5
             2                    8        1.680        1278.3       15.67        6.3           2.9
             2                   16        1.055        2035.5       24.95        3.9           4.6
             2                   32        0.869        2471.2       30.29        3.2           5.6
             2                   64        0.875        2454.3       30.08        3.3           5.6
= ------------------------------------------------------------------------------------------------- =
             4                    1        6.530         328.9        4.03       24.3           0.7
             4                    2        3.179         675.5        8.28       11.8           1.5
             4                    4        1.646        1304.7       15.99        6.1           3.0
             4                    8        0.903        2378.2       29.15        3.4           5.4
             4                   16        0.618        3474.9       42.59        2.3           7.9
             4                   32        0.584        3677.2       45.07        2.2           8.3
             4                   64        0.564        3807.6       46.67        2.1           8.6
= ------------------------------------------------------------------------------------------------- =
             8                    1        3.247         661.4        8.11       12.1           1.5
             8                    2        1.635        1313.4       16.10        6.1           3.0
             8                    4        0.927        2316.6       28.39        3.5           5.3
             8                    8        0.587        3658.4       44.84        2.2           8.3
             8                   16        0.485        4427.8       54.27        1.8          10.0
             8                   32        0.485        4427.8       54.27        1.8          10.0
             8                   64        0.501        4286.4       52.54        1.9           9.7
= ------------------------------------------------------------------------------------------------- =
            16                    1        3.212         668.6        8.19       12.0           1.5
            16                    2        1.591        1349.8       16.54        5.9           3.1
            16                    4        0.850        2526.5       30.97        3.2           5.7
            16                    8        0.655        3278.6       40.19        2.4           7.4
            16                   16        0.519        4137.7       50.72        1.9           9.4
            16                   32        0.518        4145.7       50.81        1.9           9.4
            16                   64        0.504        4260.9       52.23        1.9           9.7
= ------------------------------------------------------------------------------------------------- =
            32                    1        3.441         624.1        7.65       12.8           1.4
            32                    2        1.800        1193.0       14.62        6.7           2.7
            32                    4        0.855        2511.7       30.79        3.2           5.7
            32                    8        0.541        3969.5       48.65        2.0           9.0
            32                   16        0.486        4418.7       54.16        1.8          10.0
            32                   32        0.489        4391.6       53.83        1.8          10.0
            32                   64        0.491        4373.7       53.61        1.8           9.9
= ------------------------------------------------------------------------------------------------- =
            64                    1        3.379         635.5        7.79       12.6           1.4
            64                    2        1.821        1179.3       14.45        6.8           2.7
            64                    4        0.839        2559.6       31.37        3.1           5.8
            64                    8        0.532        4036.6       49.48        2.0           9.2
            64                   16        0.483        4446.1       54.50        1.8          10.1
            64                   32        0.491        4373.7       53.61        1.8           9.9
            64                   64        0.502        4277.9       52.43        1.9           9.7

Total benchmark time: 1105.490s
```

## TODOs

* Compare times with and without large pages
* Compare times with and without hardware prefetching

## Changelog

* **v0.0.3 - 21.08.2023:** Added multithreaded random access benchmark
* **v0.0.2 - 17.08.2023:** Added system info, improved sequential access benchmark
* **v0.0.1 - 16.08.2023:** Initial version

## Code statistics (via [gocloc](https://github.com/hhatto/gocloc))

```console
$> gocloc /exclude-ext json,txt .
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                              1             59             20            316
C++ Header                       3             46             27            181
Markdown                         1             22              0            119
-------------------------------------------------------------------------------
TOTAL                            5            127             47            616
-------------------------------------------------------------------------------
```
