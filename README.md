# MLPBench - pointer chasing benchmark for Windows

## Overview

This project is non-1:1 copy of [Lemire's testingmlp](https://github.com/lemire/testingmlp), but for Windows specifically.

## Build and run

To build this project from sources most recent Visual Studio (at least 17.7) is needed.

This project uses [large pages](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support) which are not allowed by default for any user. To enable it follow instructions from [this link](https://learn.microsoft.com/en-us/sql/database-engine/configure-windows/enable-the-lock-pages-in-memory-option-windows?view=sql-server-ver16).

## Sample output

```console
Allocating non-paged (locked) memory of size 2147483648 bytes with system decided page sizes...
Memory aligned to 1073741824 bytes: data pointer = 0x27340000000
Allocating array made of 268435456 (2048 MB) 64-bit words...
Initializing array...
Calculating time for sequential access...
Time to sum up the array 16 times = 1.203s , calculated bandwidth = 28561.7MB/s
Calculating time for random access...
--------------------------------------------------------------------------
- # of lanes --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff -
--------------------------------------------------------------------------
           1       26.575          80.8         1.0       99.0       0.3
           2       13.311         161.3         2.0       49.6       0.6
           4        6.500         330.4         4.1       24.2       1.2
           8        3.224         666.1         8.2       12.0       2.3
          16        1.703        1261.0        15.6        6.3       4.4
          32        1.246        1723.5        21.3        4.6       6.0
          64        1.224        1754.5        21.7        4.6       6.1
```

## Changelog

* **v0.0.1 - 16.08.2022:** Initial version

## Code statistics (via [gocloc](https://github.com/hhatto/gocloc))

```console
$> gocloc /exclude-ext json,txt .
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++ Header                       2             45             15            284
C++                              1             27              7            272
Markdown                         1             11              0             41
-------------------------------------------------------------------------------
TOTAL                            4             83             22            597
-------------------------------------------------------------------------------
```
