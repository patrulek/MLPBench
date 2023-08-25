#pragma once

// https://www.amd.com/system/files/TechDocs/42301_15h_Mod_00h-0Fh_BKDG.pdf
// Zen CPUs (15h) 

// MSR 0xc001102b (Combined Unit Configuration 3)
// 0x2007CC1D - disable prefetchers  (default value: 0x2000CC15)

// MSR 0xC0011022 Data Cache Configuration (DC_CFG)
// 0x40150A000 - disable hardware prefetches for software and disable DC prefetches (default value: 0x401500000)