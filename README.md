# MLPBench - pointer chasing benchmark for Windows

## Overview

This project is non-1:1 copy of [Lemire's testingmlp](https://github.com/lemire/testingmlp), but for Windows specifically.

:warning: WORK IN PROGRESS :warning:

This project is not finished yet. It may contain bugs, probably won't support your setup, and the code is 120% messy.

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
Memory aligned to 1073741824 bytes: data pointer = 0x2e240000000

Allocating paged memory of size 2147483648 bytes with default (4KB size) pages
Memory aligned to 4096 bytes: data pointer = 0x2e2c0005000

Initializing array made of 268435456 (2048 MB) 64-bit words...
Verifying sequential/random access and paged/non-paged access sums...

Calculating time for sequential access (64 rounds)...
-----------------------------------------------------------------------------------------------
- huge pages --- # of threads --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -
-----------------------------------------------------------------------------------------------
       false                1        5.650       24325.5        1.00      0.329          47.5
       false                2        3.339       41161.7        1.69      0.194          80.4
       false                4        3.002       45782.5        1.88      0.175          89.4
       false                8        3.152       43603.7        1.79      0.183          85.2
       false               16        3.239       42432.5        1.74      0.189          82.9
       false               32        3.238       42445.6        1.74      0.188          82.9
= ------------------------------------------------------------------------------------------- =
        true                1        4.894       28083.2        1.15      0.285          54.8
        true                2        3.034       45299.6        1.86      0.177          88.5
        true                4        3.030       45359.4        1.86      0.176          88.6
        true                8        3.205       42882.7        1.76      0.187          83.8
        true               16        3.246       42341.0        1.74      0.189          82.7
        true               32        3.247       42328.0        1.74      0.189          82.7

Calculating time for random access (4 rounds)...
--------------------------------------------------------------------------------------------------------------------
- huge pages --- # of threads --- lanes per thread --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff seq -
--------------------------------------------------------------------------------------------------------------------
       false                1                    1      139.328          61.7        1.00      129.8           0.1
       false                1                    2       72.565         118.4        1.92       67.6           0.3
       false                1                    4       34.847         246.5        4.00       32.5           0.5
       false                1                    8       17.112         502.0        8.14       15.9           1.1
       false                1                   16        8.685         989.1       16.04        8.1           2.2
       false                1                   32        6.684        1285.1       20.85        6.2           2.8
= ---------------------------------------------------------------------------------------------------------------- =
       false                2                    1       77.997         110.1        1.79       72.6           0.2
       false                2                    2       35.159         244.3        3.96       32.7           0.5
       false                2                    4       16.713         514.0        8.34       15.6           1.1
       false                2                    8        8.514        1008.9       16.36        7.9           2.2
       false                2                   16        4.554        1886.2       30.59        4.2           4.1
       false                2                   32        3.369        2549.7       41.36        3.1           5.6
= ---------------------------------------------------------------------------------------------------------------- =
       false                4                    1       36.859         233.0        3.78       34.3           0.5
       false                4                    2       16.687         514.8        8.35       15.5           1.1
       false                4                    4        8.414        1020.9       16.56        7.8           2.2
       false                4                    8        4.480        1917.4       31.10        4.2           4.2
       false                4                   16        2.557        3359.4       54.49        2.4           7.3
       false                4                   32        2.047        4196.4       68.06        1.9           9.2
= ---------------------------------------------------------------------------------------------------------------- =
       false                8                    1       17.921         479.3        7.77       16.7           1.0
       false                8                    2        8.313        1033.3       16.76        7.7           2.3
       false                8                    4        4.438        1935.5       31.39        4.1           4.2
       false                8                    8        2.501        3434.6       55.71        2.3           7.5
       false                8                   16        1.839        4671.0       75.76        1.7          10.2
       false                8                   32        1.925        4462.3       72.38        1.8           9.7
= ---------------------------------------------------------------------------------------------------------------- =
       false               16                    1        9.209         932.8       15.13        8.6           2.0
       false               16                    2        4.622        1858.5       30.14        4.3           4.1
       false               16                    4        2.738        3137.3       50.89        2.5           6.9
       false               16                    8        1.807        4753.7       77.10        1.7          10.4
       false               16                   16        1.819        4722.3       76.60        1.7          10.3
       false               16                   32        1.834        4683.7       75.97        1.7          10.2
= ---------------------------------------------------------------------------------------------------------------- =
        true                1                    1      105.505          81.4        1.32       98.3           0.2
        true                1                    2       53.484         160.6        2.61       49.8           0.4
        true                1                    4       25.970         330.8        5.36       24.2           0.7
        true                1                    8       12.863         667.8       10.83       12.0           1.5
        true                1                   16        6.685        1285.0       20.84        6.2           2.8
        true                1                   32        4.910        1749.5       28.38        4.6           3.8
= ---------------------------------------------------------------------------------------------------------------- =
        true                2                    1       52.975         162.2        2.63       49.3           0.4
        true                2                    2       25.658         334.8        5.43       23.9           0.7
        true                2                    4       12.704         676.2       10.97       11.8           1.5
        true                2                    8        6.579        1305.7       21.18        6.1           2.9
        true                2                   16        3.649        2354.1       38.18        3.4           5.1
        true                2                   32        2.696        3186.2       51.68        2.5           7.0
= ---------------------------------------------------------------------------------------------------------------- =
        true                4                    1       26.331         326.2        5.29       24.5           0.7
        true                4                    2       12.610         681.2       11.05       11.7           1.5
        true                4                    4        6.531        1315.3       21.33        6.1           2.9
        true                4                    8        3.571        2405.5       39.02        3.3           5.3
        true                4                   16        2.217        3874.6       62.85        2.1           8.5
        true                4                   32        1.870        4593.5       74.51        1.7          10.0
= ---------------------------------------------------------------------------------------------------------------- =
        true                8                    1       12.940         663.8       10.77       12.1           1.4
        true                8                    2        6.391        1344.1       21.80        6.0           2.9
        true                8                    4        3.580        2399.4       38.92        3.3           5.2
        true                8                    8        2.174        3951.2       64.09        2.0           8.6
        true                8                   16        1.807        4753.7       77.10        1.7          10.4
        true                8                   32        1.812        4740.6       76.89        1.7          10.4
= ---------------------------------------------------------------------------------------------------------------- =
        true               16                    1        6.491        1323.4       21.46        6.0           2.9
        true               16                    2        3.579        2400.1       38.93        3.3           5.2
        true               16                    4        2.337        3675.6       59.62        2.2           8.0
        true               16                    8        1.747        4917.0       79.75        1.6          10.7
        true               16                   16        1.795        4785.5       77.62        1.7          10.5
        true               16                   32        1.811        4743.2       76.93        1.7          10.4
= ---------------------------------------------------------------------------------------------------------------- =

Total benchmark time: 2261.523s
```

## TODOs

* Compare times with and without hardware prefetching
* Add benchmarks for SSD data access
* Add verbose/non-verbose mode
* Add benchmark granularity (with --flags)

## Changelog

* **v0.0.5 - 25.08.2023:** Added thread affinity, removed thread setup time from benchmarks, fixed rng repeatability
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
C++                              1             81             24            398
C++ Header                       5             70             39            263
Markdown                         1             24              0            145
-------------------------------------------------------------------------------
TOTAL                            7            175             63            806
-------------------------------------------------------------------------------
```
