# MLPBench - pointer chasing benchmark for Windows

## Overview

This project is non-1:1 copy of [Lemire's testingmlp](https://github.com/lemire/testingmlp), but for Windows specifically.

## Build and run

To build this project from sources most recent Visual Studio (at least 17.7) is needed.

This project uses [large pages](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support) which are not allowed by default for any user. To enable it follow instructions from [this link](https://learn.microsoft.com/en-us/sql/database-engine/configure-windows/enable-the-lock-pages-in-memory-option-windows?view=sql-server-ver16).

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
Memory aligned to 1073741824 bytes: data pointer = 0x1bb80000000
Allocating array made of 268435456 (2048 MB) 64-bit words...
Initializing array...
Verifying sequential and random access sums...

Calculating time for sequential access (64 rounds)...
--------------------------------------------------------------------------------
- # of threads --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -
--------------------------------------------------------------------------------
             1        4.926       27900.7        1.00      0.287          54.5
             2        3.398       40447.0        1.45      0.198          79.0
             4        3.103       44292.3        1.59      0.181          86.5
             8        3.251       42275.9        1.52      0.189          82.6
            16        3.259       42172.1        1.51      0.190          82.4
            32        3.325       41335.0        1.48      0.194          80.7
            64        3.313       41484.7        1.49      0.193          81.0

Calculating time for random access (single round)...
-----------------------------------------------------------------------------------------------------
- # of threads --- lanes per thread --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -
-----------------------------------------------------------------------------------------------------
             1                    1       26.696          80.4        1.00       99.5           0.2
             1                    2       13.375         160.6        2.00       49.8           0.4
             1                    4        6.566         327.1        4.07       24.5           0.7
             1                    8        3.242         662.4        8.23       12.1           1.5
             1                   16        1.715        1252.2       15.57        6.4           2.8
             1                   32        1.246        1723.5       21.43        4.6           3.9
             1                   64        1.264        1699.0       21.12        4.7           3.8

Total benchmark time: 245.882s
```

## Changelog

* **v0.0.2 - 17.08.2023:** Added system info, improved sequential access benchmark
* **v0.0.1 - 16.08.2023:** Initial version

## Code statistics (via [gocloc](https://github.com/hhatto/gocloc))

```console
$> gocloc /exclude-ext json,txt .
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                              1             49             16            246
C++ Header                       3             46             27            180
Markdown                         1             16              0             61
-------------------------------------------------------------------------------
TOTAL                            5            111             43            487
-------------------------------------------------------------------------------
```
