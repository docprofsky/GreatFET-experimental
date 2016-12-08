/*
 * Copyright 2016 Schuyler St. Leger
 *
 * This file is part of GreatFET.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <libopencm3/cm3/common.h>


// Macros for accessing a register by its offset
#define SCT_REG(offset) MMIO32(SCT_BASE + offset)
#define SCT_REG16(offset) MMIO16(SCT_BASE + offset)


// List of register offsets from SCT_BASE for the SCTimer registers

// Page 954
#define SCT_CONFIG          0x000
#define SCT_CTRL            0x004
#define SCT_CTRL_L          0x004
#define SCT_CTRL_H          0x006
#define SCT_LIMIT           0x008
#define SCT_LIMIT_L         0x008
#define SCT_LIMIT_H         0x00A
#define SCT_HALT            0x00C
#define SCT_HALT_L          0x00C
#define SCT_HALT_H          0x00E
#define SCT_STOP            0x010
#define SCT_STOP_L          0x010
#define SCT_STOP_H          0x012
#define SCT_START           0x014
#define SCT_START_L         0x014
#define SCT_START_H         0x016
#define SCT_COUNT_L         0x040
#define SCT_COUNT_H         0x042
#define SCT_STATE           0x044

// Page 955
#define SCT_STATE_L         0x044
#define SCT_STATE_H         0x046
#define SCT_INPUT           0x048
#define SCT_REGMODE         0x04C
#define SCT_REGMODE_L       0x04C
#define SCT_REGMODE_H       0x04E
#define SCT_OUTPUT          0x050
#define SCT_OUTPUTDIRCTRL   0x054
#define SCT_RES             0x058
#define SCT_DMAREQ0         0x05C
#define SCT_DMAREQ1         0x060
#define SCT_EVFLAG          0x0F4
#define SCT_CONEN           0x0F8
#define SCT_CONFLAG         0x0FC
// Skipping rest of page 955
/*
MATCH0
CAP0
 */
#define SCT_MATCH0_L 0x100
#define SCT_MATCH1_L 0x104
#define SCT_MATCH2_L 0x108

// Page 956
/*
MATCHREL
CAPCTRL
 */
#define SCT_MATCHREL0_L 0x0200
#define SCT_MATCHREL1_L 0x0204
#define SCT_MATCHREL2_L 0x0208
// Skipping to EVSTATEMSK0
#define SCT_EVSTATEMSK0     0x300
#define SCT_EVCTRL0         0x304
#define SCT_EVSTATEMSK1     0x308
#define SCT_EVCTRL1         0x30C
#define SCT_EVSTATEMSK2     0x310
#define SCT_EVCTRL2         0x314
#define SCT_EVSTATEMSK3     0x318
#define SCT_EVCTRL3         0x31C
#define SCT_EVSTATEMSK4     0x320
#define SCT_EVCTRL4         0x324

// Page 957
#define SCT_EVCTRL5         0x32C
#define SCT_EVSTATEMSK6     0x330
#define SCT_EVCTRL6         0x334
#define SCT_EVSTATEMSK7     0x338
#define SCT_EVCTRL7         0x33C
#define SCT_EVSTATEMSK8     0x340
#define SCT_EVCTRL8         0x344
#define SCT_EVSTATEMSK9     0x348
#define SCT_EVCTRL9         0x34C
#define SCT_EVSTATEMSK10    0x350
#define SCT_EVCTRL10        0x354
#define SCT_EVSTATEMSK11    0x358
#define SCT_EVCTRL11        0x35C
#define SCT_EVSTATEMSK12    0x360
#define SCT_EVCTRL12        0x364
#define SCT_EVSTATEMSK13    0x368
#define SCT_EVCTRL13        0x36C
#define SCT_EVSTATEMSK14    0x370
#define SCT_EVCTRL14        0x374
#define SCT_EVSTATEMSK15    0x378
#define SCT_EVCTRL15        0x37C
#define SCT_OUTPUTSET0      0x500
#define SCT_OUTPUTCL0       0x504
#define SCT_OUTPUTSET1      0x508
#define SCT_OUTPUTCL1       0x50C
#define SCT_OUTPUTSET2      0x510
#define SCT_OUTPUTCL2       0x514
#define SCT_OUTPUTSET3      0x518
#define SCT_OUTPUTCL3       0x51C
#define SCT_OUTPUTSET4      0x520
#define SCT_OUTPUTCL4       0x524
#define SCT_OUTPUTSET5      0x528
#define SCT_OUTPUTCL5       0x52C
#define SCT_OUTPUTSET6      0x530
#define SCT_OUTPUTCL6       0x534
#define SCT_OUTPUTSET7      0x538
#define SCT_OUTPUTCL7       0x53C
#define SCT_OUTPUTSET8      0x540
#define SCT_OUTPUTCL8       0x544

// Page 958
#define SCT_OUTPUTCL9       0x54C
#define SCT_OUTPUTSET10     0x550
#define SCT_OUTPUTCL10      0x554
#define SCT_OUTPUTSET11     0x558
#define SCT_OUTPUTCL11      0x55C
#define SCT_OUTPUTSET12     0x560
#define SCT_OUTPUTCL12      0x564
#define SCT_OUTPUTSET13     0x568
#define SCT_OUTPUTCL13      0x56C
#define SCT_OUTPUTSET14     0x570
#define SCT_OUTPUTCL14      0x574
#define SCT_OUTPUTSET15     0x578
#define SCT_OUTPUTCL15      0x57C
