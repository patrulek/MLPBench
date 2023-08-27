# MLPBench - pointer chasing benchmark for Windows

## Overview

This project is non-1:1 copy of [Lemire's testingmlp](https://github.com/lemire/testingmlp), but for Windows specifically.

:warning: WORK IN PROGRESS :warning:

This project is not finished yet. It may contain bugs, probably won't support your setup, and the code is 120% messy.

## Build and run

To build this project from sources most recent Visual Studio (at least 17.7) is needed.

This project uses [large pages](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support) which are not allowed by default for any user. To enable it follow instructions from [this link](https://learn.microsoft.com/en-us/sql/database-engine/configure-windows/enable-the-lock-pages-in-memory-option-windows?view=sql-server-ver16).

Admin privileges are required to run this program properly (to manipulate MSR registers).

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

CPU Info (AMD Ryzen 7 5800H with Radeon Graphics):
 * Physical cores: 8
 * Logical cores: 16
 * Hyperthreading: true

Allocating non-paged (locked) memory of size 2147483648 bytes with system decided page sizes...
Memory aligned to 1073741824 bytes: data pointer = 0x24100000000

Allocating paged memory of size 2147483648 bytes with default (4KB size) pages
Memory aligned to 4096 bytes: data pointer = 0x24180009000

Initializing array made of 268435456 (2048 MB) 64-bit words...
Verifying sequential/random access and paged/non-paged access sums...

Calculating time for sequential access (64 rounds)...
--------------------------------------------------------------------------------------------------------------
- prefetcher --- huge pages --- # of threads --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff max -
--------------------------------------------------------------------------------------------------------------
        true          false                1        5.482       25071.0        1.00      0.319          49.0
        true          false                2        3.310       41522.3        1.66      0.193          81.1
        true          false                4        3.050       45062.0        1.80      0.178          88.0
        true          false                8        3.193       43043.8        1.72      0.186          84.1
        true          false               16        3.250       42288.9        1.69      0.189          82.6
        true          false               32        3.253       42249.9        1.69      0.189          82.5
= ---------------------------------------------------------------------------------------------------------- =
        true           true                1        4.938       27832.9        1.11      0.287          54.4
        true           true                2        3.112       44164.2        1.76      0.181          86.3
        true           true                4        3.055       44988.2        1.79      0.178          87.9
        true           true                8        3.178       43247.0        1.72      0.185          84.5
        true           true               16        3.254       42236.9        1.68      0.189          82.5
        true           true               32        3.289       41787.5        1.67      0.191          81.6
= ---------------------------------------------------------------------------------------------------------- =
       false          false                1       17.871        7690.6        0.31      1.040          15.0
       false          false                2        8.897       15447.8        0.62      0.518          30.2
       false          false                4        4.681       29361.0        1.17      0.272          57.3
       false          false                8        3.056       44973.5        1.79      0.178          87.8
       false          false               16        3.074       44710.1        1.78      0.179          87.3
       false          false               32        3.097       44378.1        1.77      0.180          86.7
= ---------------------------------------------------------------------------------------------------------- =
       false           true                1       17.754        7741.3        0.31      1.033          15.1
       false           true                2        8.844       15540.4        0.62      0.515          30.4
       false           true                4        4.642       29607.7        1.18      0.270          57.8
       false           true                8        3.039       45225.1        1.80      0.177          88.3
       false           true               16        3.074       44710.1        1.78      0.179          87.3
       false           true               32        3.078       44652.0        1.78      0.179          87.2

Calculating time for random access (4 rounds)...
-----------------------------------------------------------------------------------------------------------------------------------
- prefetcher --- huge pages --- # of threads --- lanes per thread --- time (s) --- bandwidth --- speedup --- ns/hit --- % eff seq -
-----------------------------------------------------------------------------------------------------------------------------------
        true          false                1                    1      139.237          61.7        1.00      129.7           0.1
        true          false                1                    2       70.968         121.0        1.96       66.1           0.3
        true          false                1                    4       34.676         247.7        4.02       32.3           0.5
        true          false                1                    8       17.082         502.9        8.15       15.9           1.1
        true          false                1                   16        8.599         998.9       16.19        8.0           2.2
        true          false                1                   32        6.248        1374.8       22.29        5.8           3.0
-----------------------------------------------------------------------------------------------------------------------------------
        true          false                2                    1       75.510         113.8        1.84       70.3           0.3
        true          false                2                    2       35.502         242.0        3.92       33.1           0.5
        true          false                2                    4       16.707         514.2        8.33       15.6           1.1
        true          false                2                    8        8.513        1009.0       16.36        7.9           2.2
        true          false                2                   16        4.555        1885.8       30.57        4.2           4.2
        true          false                2                   32        3.332        2578.0       41.79        3.1           5.7
-----------------------------------------------------------------------------------------------------------------------------------
        true          false                4                    1       36.501         235.3        3.81       34.0           0.5
        true          false                4                    2       16.669         515.3        8.35       15.5           1.1
        true          false                4                    4        8.432        1018.7       16.51        7.9           2.3
        true          false                4                    8        4.477        1918.7       31.10        4.2           4.2
        true          false                4                   16        2.545        3375.2       54.71        2.4           7.5
        true          false                4                   32        2.052        4186.1       67.85        1.9           9.3
-----------------------------------------------------------------------------------------------------------------------------------
        true          false                8                    1       17.772         483.3        7.83       16.6           1.1
        true          false                8                    2        8.267        1039.1       16.84        7.7           2.3
        true          false                8                    4        4.409        1948.3       31.58        4.1           4.3
        true          false                8                    8        2.529        3396.6       55.06        2.4           7.5
        true          false                8                   16        1.955        4393.8       71.22        1.8           9.7
        true          false                8                   32        1.832        4688.8       76.00        1.7          10.4
-----------------------------------------------------------------------------------------------------------------------------------
        true          false               16                    1        9.153         938.5       15.21        8.5           2.1
        true          false               16                    2        4.635        1853.3       30.04        4.3           4.1
        true          false               16                    4        2.726        3151.1       51.08        2.5           7.0
        true          false               16                    8        1.814        4735.4       76.76        1.7          10.5
        true          false               16                   16        1.811        4743.2       76.88        1.7          10.5
        true          false               16                   32        1.885        4557.0       73.87        1.8          10.1
-----------------------------------------------------------------------------------------------------------------------------------
        true           true                1                    1      106.127          80.9        1.31       98.8           0.2
        true           true                1                    2       53.622         160.2        2.60       49.9           0.4
        true           true                1                    4       26.070         329.5        5.34       24.3           0.7
        true           true                1                    8       12.843         668.8       10.84       12.0           1.5
        true           true                1                   16        6.692        1283.6       20.81        6.2           2.8
        true           true                1                   32        4.886        1758.1       28.50        4.6           3.9
-----------------------------------------------------------------------------------------------------------------------------------
        true           true                2                    1       52.924         162.3        2.63       49.3           0.4
        true           true                2                    2       25.904         331.6        5.38       24.1           0.7
        true           true                2                    4       12.813         670.4       10.87       11.9           1.5
        true           true                2                    8        6.634        1294.8       20.99        6.2           2.9
        true           true                2                   16        3.648        2354.7       38.17        3.4           5.2
        true           true                2                   32        2.707        3173.2       51.44        2.5           7.0
-----------------------------------------------------------------------------------------------------------------------------------
        true           true                4                    1       26.504         324.1        5.25       24.7           0.7
        true           true                4                    2       12.703         676.2       10.96       11.8           1.5
        true           true                4                    4        6.564        1308.6       21.21        6.1           2.9
        true           true                4                    8        3.587        2394.7       38.82        3.3           5.3
        true           true                4                   16        2.214        3879.8       62.89        2.1           8.6
        true           true                4                   32        1.863        4610.8       74.74        1.7          10.2
-----------------------------------------------------------------------------------------------------------------------------------
        true           true                8                    1       12.984         661.6       10.72       12.1           1.5
        true           true                8                    2        6.425        1337.0       21.67        6.0           3.0
        true           true                8                    4        3.587        2394.7       38.82        3.3           5.3
        true           true                8                    8        2.178        3944.0       63.93        2.0           8.7
        true           true                8                   16        1.817        4727.5       76.63        1.7          10.5
        true           true                8                   32        1.814        4735.4       76.76        1.7          10.5
-----------------------------------------------------------------------------------------------------------------------------------
        true           true               16                    1        6.486        1324.4       21.47        6.0           2.9
        true           true               16                    2        3.753        2288.8       37.10        3.5           5.1
        true           true               16                    4        2.353        3650.6       59.17        2.2           8.1
        true           true               16                    8        1.742        4931.1       79.93        1.6          10.9
        true           true               16                   16        1.788        4804.2       77.87        1.7          10.6
        true           true               16                   32        1.823        4712.0       76.38        1.7          10.4
-----------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------
       false          false                1                    1      135.900          63.2        1.02      126.6           0.1
       false          false                1                    2       69.389         123.8        2.01       64.6           0.3
       false          false                1                    4       35.101         244.7        3.97       32.7           0.5
       false          false                1                    8       17.114         501.9        8.14       15.9           1.1
       false          false                1                   16        8.643         993.9       16.11        8.0           2.2
       false          false                1                   32        6.249        1374.6       22.28        5.8           3.0
-----------------------------------------------------------------------------------------------------------------------------------
       false          false                2                    1       72.784         118.0        1.91       67.8           0.3
       false          false                2                    2       35.358         242.9        3.94       32.9           0.5
       false          false                2                    4       16.721         513.7        8.33       15.6           1.1
       false          false                2                    8        8.397        1023.0       16.58        7.8           2.3
       false          false                2                   16        4.567        1880.9       30.49        4.3           4.2
       false          false                2                   32        3.330        2579.6       41.81        3.1           5.7
-----------------------------------------------------------------------------------------------------------------------------------
       false          false                4                    1       36.300         236.6        3.84       33.8           0.5
       false          false                4                    2       16.609         517.2        8.38       15.5           1.1
       false          false                4                    4        8.311        1033.6       16.75        7.7           2.3
       false          false                4                    8        4.371        1965.2       31.85        4.1           4.3
       false          false                4                   16        2.534        3389.9       54.95        2.4           7.5
       false          false                4                   32        2.017        4258.8       69.03        1.9           9.4
-----------------------------------------------------------------------------------------------------------------------------------
       false          false                8                    1       17.963         478.2        7.75       16.7           1.1
       false          false                8                    2        8.190        1048.8       17.00        7.6           2.3
       false          false                8                    4        4.272        2010.8       32.59        4.0           4.4
       false          false                8                    8        2.421        3548.1       57.51        2.3           7.8
       false          false                8                   16        1.839        4671.0       75.71        1.7          10.3
       false          false                8                   32        1.832        4688.8       76.00        1.7          10.4
-----------------------------------------------------------------------------------------------------------------------------------
       false          false               16                    1        9.218         931.9       15.10        8.6           2.1
       false          false               16                    2        4.383        1959.8       31.77        4.1           4.3
       false          false               16                    4        2.468        3480.5       56.42        2.3           7.7
       false          false               16                    8        1.773        4844.9       78.53        1.7          10.7
       false          false               16                   16        1.815        4732.7       76.71        1.7          10.5
       false          false               16                   32        1.830        4694.0       76.09        1.7          10.4
-----------------------------------------------------------------------------------------------------------------------------------
       false           true                1                    1      106.548          80.6        1.31       99.2           0.2
       false           true                1                    2       53.806         159.6        2.59       50.1           0.4
       false           true                1                    4       26.194         327.9        5.32       24.4           0.7
       false           true                1                    8       12.858         668.1       10.83       12.0           1.5
       false           true                1                   16        6.803        1262.7       20.47        6.3           2.8
       false           true                1                   32        4.899        1753.4       28.42        4.6           3.9
-----------------------------------------------------------------------------------------------------------------------------------
       false           true                2                    1       52.417         163.9        2.66       48.8           0.4
       false           true                2                    2       26.570         323.3        5.24       24.7           0.7
       false           true                2                    4       12.834         669.3       10.85       12.0           1.5
       false           true                2                    8        6.643        1293.1       20.96        6.2           2.9
       false           true                2                   16        3.654        2350.8       38.11        3.4           5.2
       false           true                2                   32        2.703        3177.9       51.51        2.5           7.0
-----------------------------------------------------------------------------------------------------------------------------------
       false           true                4                    1       26.988         318.3        5.16       25.1           0.7
       false           true                4                    2       12.718         675.4       10.95       11.8           1.5
       false           true                4                    4        6.523        1316.9       21.35        6.1           2.9
       false           true                4                    8        3.600        2386.1       38.68        3.4           5.3
       false           true                4                   16        2.248        3821.1       61.94        2.1           8.4
       false           true                4                   32        1.877        4576.4       74.18        1.7          10.1
-----------------------------------------------------------------------------------------------------------------------------------
       false           true                8                    1       13.185         651.5       10.56       12.3           1.4
       false           true                8                    2        6.352        1352.3       21.92        5.9           3.0
       false           true                8                    4        3.412        2517.6       40.81        3.2           5.6
       false           true                8                    8        2.170        3958.5       64.16        2.0           8.8
       false           true                8                   16        1.817        4727.5       76.63        1.7          10.5
       false           true                8                   32        1.823        4712.0       76.38        1.7          10.4
-----------------------------------------------------------------------------------------------------------------------------------
       false           true               16                    1        6.582        1305.1       21.15        6.1           2.9
       false           true               16                    2        3.436        2500.0       40.52        3.2           5.5
       false           true               16                    4        2.091        4108.1       66.59        1.9           9.1
       false           true               16                    8        1.770        4853.1       78.66        1.6          10.7
       false           true               16                   16        1.814        4735.4       76.76        1.7          10.5
       false           true               16                   32        1.817        4727.5       76.63        1.7          10.5
-----------------------------------------------------------------------------------------------------------------------------------

Total benchmark time: 4516.004s
```

## TODOs

* Add benchmarks for SSD data access
* Add verbose/non-verbose mode
* Add benchmark granularity (with --flags)

## Changelog

* **v0.0.6 - 27.08.2023:** Added benchmarks with hardware prefetching disabled
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
C++                              1            101             39            534
C++ Header                       5            101             45            404
Markdown                         1             26              0            234
C Header                         1             10             32             27
-------------------------------------------------------------------------------
TOTAL                            8            238            116           1199
-------------------------------------------------------------------------------
```
