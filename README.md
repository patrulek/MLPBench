# MLPBench - pointer chasing benchmark for Windows

## Overview

This project is non-1:1 copy of [Lemire's testingmlp](https://github.com/lemire/testingmlp), but for Windows specifically.

## Build and run

To build this project from sources most recent Visual Studio (at least 17.7) is needed.

This project uses [large pages](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support) which are not allowed by default for any user. To enable it follow instructions from [this link](https://learn.microsoft.com/en-us/sql/database-engine/configure-windows/enable-the-lock-pages-in-memory-option-windows?view=sql-server-ver16).

### Troubleshooting

In a case of such error:

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

CPU Info (AMD Ryzen 7 5800H with Radeon Graphics         ):
 * Physical cores: 8
 * Logical cores: 16
 * Hyperthreading: true

Allocating non-paged (locked) memory of size 2147483648 bytes with system decided page sizes...
Memory aligned to 1073741824 bytes: data pointer = 0x249c0000000

Allocating paged memory of size 2147483648 bytes with default (4KB size) pages
Memory aligned to 4096 bytes: data pointer = 0x24a4000b000

Initializing array made of 268435456 (2048 MB) 64-bit words...
Verifying sequential/random access and paged/non-paged access sums...

Calculating time for sequential access (64 rounds)...
-----------------------------------------------------------------------------------------------
- huge pages --- # of threads --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -
-----------------------------------------------------------------------------------------------
       false                  1        5.292       25971.1        1.00      0.308          50.7
       false                  2        3.676       37388.2        1.44      0.214          73.0
       false                  4        3.145       43700.8        1.68      0.183          85.4
       false                  8        3.224       42629.9        1.64      0.188          83.3
       false                 16        3.249       42301.9        1.63      0.189          82.6
       false                 32        3.335       41211.1        1.59      0.194          80.5
= ------------------------------------------------------------------------------------------- =
        true                  1        4.923       27917.7        1.07      0.287          54.5
        true                  2        3.284       41851.1        1.61      0.191          81.7
        true                  4        3.248       42314.9        1.63      0.189          82.6
        true                  8        3.374       40734.7        1.57      0.196          79.6
        true                 16        3.371       40771.0        1.57      0.196          79.6
        true                 32        3.415       40245.7        1.55      0.199          78.6

Calculating time for random access (single round)...
--------------------------------------------------------------------------------------------------------------------
- huge pages --- # of threads --- lanes per thread --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff seq -
--------------------------------------------------------------------------------------------------------------------
       false                1                    1       36.704          58.5        1.00      136.7           0.1
       false                1                    2       18.175         118.2        2.02       67.7           0.3
       false                1                    4        8.572         250.5        4.28       31.9           0.6
       false                1                    8        4.243         506.1        8.65       15.8           1.2
       false                1                   16        2.210         971.7       16.61        8.2           2.2
       false                1                   32        1.627        1319.9       22.56        6.1           3.0
= ---------------------------------------------------------------------------------------------------------------- =
       false                2                    1       19.389         110.8        1.89       72.2           0.3
       false                2                    2        8.874         242.0        4.14       33.1           0.6
       false                2                    4        4.194         512.0        8.75       15.6           1.2
       false                2                    8        2.178         986.0       16.85        8.1           2.3
       false                2                   16        1.425        1507.0       25.76        5.3           3.4
       false                2                   32        1.130        1900.4       32.48        4.2           4.3
= ---------------------------------------------------------------------------------------------------------------- =
       false                4                    1        9.571         224.4        3.83       35.7           0.5
       false                4                    2        4.306         498.7        8.52       16.0           1.1
       false                4                    4        2.129        1008.7       17.24        7.9           2.3
       false                4                    8        1.141        1882.1       32.17        4.3           4.3
       false                4                   16        0.802        2677.7       45.77        3.0           6.1
       false                4                   32        0.743        2890.3       49.40        2.8           6.6
= ---------------------------------------------------------------------------------------------------------------- =
       false                8                    1        4.530         474.1        8.10       16.9           1.1
       false                8                    2        2.172         988.7       16.90        8.1           2.3
       false                8                    4        1.223        1755.9       30.01        4.6           4.0
       false                8                    8        0.714        3007.7       51.41        2.7           6.9
       false                8                   16        0.539        3984.2       68.10        2.0           9.1
       false                8                   32        0.569        3774.1       64.51        2.1           8.6
= ---------------------------------------------------------------------------------------------------------------- =
       false               16                    1        4.222         508.6        8.69       15.7           1.2
       false               16                    2        2.024        1061.0       18.13        7.5           2.4
       false               16                    4        0.884        2429.3       41.52        3.3           5.6
       false               16                    8        0.778        2760.3       47.18        2.9           6.3
       false               16                   16        0.568        3780.8       64.62        2.1           8.7
       false               16                   32        0.577        3721.8       63.61        2.1           8.5
= ---------------------------------------------------------------------------------------------------------------- =
        true                1                    1       26.709          80.4        1.37       99.5           0.2
        true                1                    2       13.452         159.6        2.73       50.1           0.4
        true                1                    4        6.527         329.0        5.62       24.3           0.8
        true                1                    8        3.242         662.4       11.32       12.1           1.5
        true                1                   16        1.722        1247.1       21.31        6.4           2.9
        true                1                   32        1.270        1690.9       28.90        4.7           3.9
= ---------------------------------------------------------------------------------------------------------------- =
        true                2                    1       13.305         161.4        2.76       49.6           0.4
        true                2                    2        6.580         326.4        5.58       24.5           0.7
        true                2                    4        3.246         661.6       11.31       12.1           1.5
        true                2                    8        1.683        1276.0       21.81        6.3           2.9
        true                2                   16        1.021        2103.3       35.95        3.8           4.8
        true                2                   32        0.853        2517.6       43.03        3.2           5.8
= ---------------------------------------------------------------------------------------------------------------- =
        true                4                    1        6.621         324.3        5.54       24.7           0.7
        true                4                    2        3.243         662.2       11.32       12.1           1.5
        true                4                    4        1.670        1285.9       21.98        6.2           2.9
        true                4                    8        0.911        2357.3       40.29        3.4           5.4
        true                4                   16        0.626        3430.5       58.63        2.3           7.8
        true                4                   32        0.593        3621.4       61.90        2.2           8.3
= ---------------------------------------------------------------------------------------------------------------- =
        true                8                    1        3.387         634.0       10.84       12.6           1.5
        true                8                    2        1.686        1273.7       21.77        6.3           2.9
        true                8                    4        0.938        2289.4       39.13        3.5           5.2
        true                8                    8        0.608        3532.0       60.37        2.3           8.1
        true                8                   16        0.491        4373.7       74.75        1.8          10.0
        true                8                   32        0.487        4409.6       75.37        1.8          10.1
= ---------------------------------------------------------------------------------------------------------------- =
        true               16                    1        3.057         702.5       12.01       11.4           1.6
        true               16                    2        1.620        1325.6       22.66        6.0           3.0
        true               16                    4        0.808        2657.8       45.43        3.0           6.1
        true               16                    8        0.539        3984.2       68.10        2.0           9.1
        true               16                   16        0.525        4090.4       69.91        2.0           9.4
        true               16                   32        0.520        4129.8       70.58        1.9           9.5
= ---------------------------------------------------------------------------------------------------------------- =

Total benchmark time: 1603.498s
```

## TODOs

* ~~Compare times with and without large pages~~
* Compare times with and without hardware prefetching

## Changelog

* **v0.0.4 - 22.08.2023:** Added benchmarks with normal pages
* **v0.0.3 - 21.08.2023:** Added multithreaded random access benchmark
* **v0.0.2 - 17.08.2023:** Added system info, improved sequential access benchmark
* **v0.0.1 - 16.08.2023:** Initial version

## Code statistics (via [gocloc](https://github.com/hhatto/gocloc))

```console
$> gocloc /exclude-ext json,txt .
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                              1             75             27            386
C++ Header                       3             50             30            204
Markdown                         1             24              0            142
-------------------------------------------------------------------------------
TOTAL                            5            149             57            732
-------------------------------------------------------------------------------
```
