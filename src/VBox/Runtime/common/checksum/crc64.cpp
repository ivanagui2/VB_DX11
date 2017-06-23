/* $Id: crc64.cpp 62477 2016-07-22 18:27:37Z vboxsync $ */
/** @file
 * IPRT - CRC64.
 *
 * The method to compute the CRC64 is referred to as CRC-64-ISO:
 *     http://en.wikipedia.org/wiki/Cyclic_redundancy_check
 * The generator polynomial is x^64 + x^4 + x^3 + x + 1.
 *     Reverse polynom: 0xd800000000000000ULL
 */

/*
 * Copyright (C) 2006-2016 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 */


/*********************************************************************************************************************************
*   Header Files                                                                                                                 *
*********************************************************************************************************************************/
#include <iprt/crc.h>
#include "internal/iprt.h"


/*********************************************************************************************************************************
*   Global Variables                                                                                                             *
*********************************************************************************************************************************/
/**
 * Lookup table (precomputed CRC64 values for each 8 bit string) computation
 * takes into account the fact that the reverse polynom has zeros in lower 8 bits:
 *
 * @code
 *    for (i = 0; i < 256; i++)
 *    {
 *        shiftRegister = i;
 *        for (j = 0; j < 8; j++)
 *        {
 *            if (shiftRegister & 1)
 *                shiftRegister = (shiftRegister >> 1) ^ Reverse_polynom;
 *            else
 *                shiftRegister >>= 1;
 *        }
 *        CRCTable[i] = shiftRegister;
 *    }
 * @endcode
 *
 * Generic code would look as follows:
 *
 * @code
 *    for (i = 0; i < 256; i++)
 *    {
 *        shiftRegister = 0;
 *        bitString = i;
 *        for (j = 0; j < 8; j++)
 *        {
 *            if ((shiftRegister ^ (bitString >> j)) & 1)
 *                shiftRegister = (shiftRegister >> 1) ^ Reverse_polynom;
 *            else
 *                shiftRegister >>= 1;
 *        }
 *        CRCTable[i] = shiftRegister;
 *    }
 * @endcode
 *
 * @remark  Since the lookup table elements have 0 in the lower 32 bit word,
 *          the 32 bit assembler implementation of CRC64Process can be optimized,
 *          avoiding at least one 'xor' operation.
 */
static const uint64_t g_au64CRC64[] =
{
    0x0000000000000000ULL, 0x01B0000000000000ULL, 0x0360000000000000ULL, 0x02D0000000000000ULL,
    0x06C0000000000000ULL, 0x0770000000000000ULL, 0x05A0000000000000ULL, 0x0410000000000000ULL,
    0x0D80000000000000ULL, 0x0C30000000000000ULL, 0x0EE0000000000000ULL, 0x0F50000000000000ULL,
    0x0B40000000000000ULL, 0x0AF0000000000000ULL, 0x0820000000000000ULL, 0x0990000000000000ULL,
    0x1B00000000000000ULL, 0x1AB0000000000000ULL, 0x1860000000000000ULL, 0x19D0000000000000ULL,
    0x1DC0000000000000ULL, 0x1C70000000000000ULL, 0x1EA0000000000000ULL, 0x1F10000000000000ULL,
    0x1680000000000000ULL, 0x1730000000000000ULL, 0x15E0000000000000ULL, 0x1450000000000000ULL,
    0x1040000000000000ULL, 0x11F0000000000000ULL, 0x1320000000000000ULL, 0x1290000000000000ULL,
    0x3600000000000000ULL, 0x37B0000000000000ULL, 0x3560000000000000ULL, 0x34D0000000000000ULL,
    0x30C0000000000000ULL, 0x3170000000000000ULL, 0x33A0000000000000ULL, 0x3210000000000000ULL,
    0x3B80000000000000ULL, 0x3A30000000000000ULL, 0x38E0000000000000ULL, 0x3950000000000000ULL,
    0x3D40000000000000ULL, 0x3CF0000000000000ULL, 0x3E20000000000000ULL, 0x3F90000000000000ULL,
    0x2D00000000000000ULL, 0x2CB0000000000000ULL, 0x2E60000000000000ULL, 0x2FD0000000000000ULL,
    0x2BC0000000000000ULL, 0x2A70000000000000ULL, 0x28A0000000000000ULL, 0x2910000000000000ULL,
    0x2080000000000000ULL, 0x2130000000000000ULL, 0x23E0000000000000ULL, 0x2250000000000000ULL,
    0x2640000000000000ULL, 0x27F0000000000000ULL, 0x2520000000000000ULL, 0x2490000000000000ULL,
    0x6C00000000000000ULL, 0x6DB0000000000000ULL, 0x6F60000000000000ULL, 0x6ED0000000000000ULL,
    0x6AC0000000000000ULL, 0x6B70000000000000ULL, 0x69A0000000000000ULL, 0x6810000000000000ULL,
    0x6180000000000000ULL, 0x6030000000000000ULL, 0x62E0000000000000ULL, 0x6350000000000000ULL,
    0x6740000000000000ULL, 0x66F0000000000000ULL, 0x6420000000000000ULL, 0x6590000000000000ULL,
    0x7700000000000000ULL, 0x76B0000000000000ULL, 0x7460000000000000ULL, 0x75D0000000000000ULL,
    0x71C0000000000000ULL, 0x7070000000000000ULL, 0x72A0000000000000ULL, 0x7310000000000000ULL,
    0x7A80000000000000ULL, 0x7B30000000000000ULL, 0x79E0000000000000ULL, 0x7850000000000000ULL,
    0x7C40000000000000ULL, 0x7DF0000000000000ULL, 0x7F20000000000000ULL, 0x7E90000000000000ULL,
    0x5A00000000000000ULL, 0x5BB0000000000000ULL, 0x5960000000000000ULL, 0x58D0000000000000ULL,
    0x5CC0000000000000ULL, 0x5D70000000000000ULL, 0x5FA0000000000000ULL, 0x5E10000000000000ULL,
    0x5780000000000000ULL, 0x5630000000000000ULL, 0x54E0000000000000ULL, 0x5550000000000000ULL,
    0x5140000000000000ULL, 0x50F0000000000000ULL, 0x5220000000000000ULL, 0x5390000000000000ULL,
    0x4100000000000000ULL, 0x40B0000000000000ULL, 0x4260000000000000ULL, 0x43D0000000000000ULL,
    0x47C0000000000000ULL, 0x4670000000000000ULL, 0x44A0000000000000ULL, 0x4510000000000000ULL,
    0x4C80000000000000ULL, 0x4D30000000000000ULL, 0x4FE0000000000000ULL, 0x4E50000000000000ULL,
    0x4A40000000000000ULL, 0x4BF0000000000000ULL, 0x4920000000000000ULL, 0x4890000000000000ULL,
    0xD800000000000000ULL, 0xD9B0000000000000ULL, 0xDB60000000000000ULL, 0xDAD0000000000000ULL,
    0xDEC0000000000000ULL, 0xDF70000000000000ULL, 0xDDA0000000000000ULL, 0xDC10000000000000ULL,
    0xD580000000000000ULL, 0xD430000000000000ULL, 0xD6E0000000000000ULL, 0xD750000000000000ULL,
    0xD340000000000000ULL, 0xD2F0000000000000ULL, 0xD020000000000000ULL, 0xD190000000000000ULL,
    0xC300000000000000ULL, 0xC2B0000000000000ULL, 0xC060000000000000ULL, 0xC1D0000000000000ULL,
    0xC5C0000000000000ULL, 0xC470000000000000ULL, 0xC6A0000000000000ULL, 0xC710000000000000ULL,
    0xCE80000000000000ULL, 0xCF30000000000000ULL, 0xCDE0000000000000ULL, 0xCC50000000000000ULL,
    0xC840000000000000ULL, 0xC9F0000000000000ULL, 0xCB20000000000000ULL, 0xCA90000000000000ULL,
    0xEE00000000000000ULL, 0xEFB0000000000000ULL, 0xED60000000000000ULL, 0xECD0000000000000ULL,
    0xE8C0000000000000ULL, 0xE970000000000000ULL, 0xEBA0000000000000ULL, 0xEA10000000000000ULL,
    0xE380000000000000ULL, 0xE230000000000000ULL, 0xE0E0000000000000ULL, 0xE150000000000000ULL,
    0xE540000000000000ULL, 0xE4F0000000000000ULL, 0xE620000000000000ULL, 0xE790000000000000ULL,
    0xF500000000000000ULL, 0xF4B0000000000000ULL, 0xF660000000000000ULL, 0xF7D0000000000000ULL,
    0xF3C0000000000000ULL, 0xF270000000000000ULL, 0xF0A0000000000000ULL, 0xF110000000000000ULL,
    0xF880000000000000ULL, 0xF930000000000000ULL, 0xFBE0000000000000ULL, 0xFA50000000000000ULL,
    0xFE40000000000000ULL, 0xFFF0000000000000ULL, 0xFD20000000000000ULL, 0xFC90000000000000ULL,
    0xB400000000000000ULL, 0xB5B0000000000000ULL, 0xB760000000000000ULL, 0xB6D0000000000000ULL,
    0xB2C0000000000000ULL, 0xB370000000000000ULL, 0xB1A0000000000000ULL, 0xB010000000000000ULL,
    0xB980000000000000ULL, 0xB830000000000000ULL, 0xBAE0000000000000ULL, 0xBB50000000000000ULL,
    0xBF40000000000000ULL, 0xBEF0000000000000ULL, 0xBC20000000000000ULL, 0xBD90000000000000ULL,
    0xAF00000000000000ULL, 0xAEB0000000000000ULL, 0xAC60000000000000ULL, 0xADD0000000000000ULL,
    0xA9C0000000000000ULL, 0xA870000000000000ULL, 0xAAA0000000000000ULL, 0xAB10000000000000ULL,
    0xA280000000000000ULL, 0xA330000000000000ULL, 0xA1E0000000000000ULL, 0xA050000000000000ULL,
    0xA440000000000000ULL, 0xA5F0000000000000ULL, 0xA720000000000000ULL, 0xA690000000000000ULL,
    0x8200000000000000ULL, 0x83B0000000000000ULL, 0x8160000000000000ULL, 0x80D0000000000000ULL,
    0x84C0000000000000ULL, 0x8570000000000000ULL, 0x87A0000000000000ULL, 0x8610000000000000ULL,
    0x8F80000000000000ULL, 0x8E30000000000000ULL, 0x8CE0000000000000ULL, 0x8D50000000000000ULL,
    0x8940000000000000ULL, 0x88F0000000000000ULL, 0x8A20000000000000ULL, 0x8B90000000000000ULL,
    0x9900000000000000ULL, 0x98B0000000000000ULL, 0x9A60000000000000ULL, 0x9BD0000000000000ULL,
    0x9FC0000000000000ULL, 0x9E70000000000000ULL, 0x9CA0000000000000ULL, 0x9D10000000000000ULL,
    0x9480000000000000ULL, 0x9530000000000000ULL, 0x97E0000000000000ULL, 0x9650000000000000ULL,
    0x9240000000000000ULL, 0x93F0000000000000ULL, 0x9120000000000000ULL, 0x9090000000000000ULL
};


/**
 * Calculate CRC64 for a memory block.
 *
 * @returns CRC64 for the memory block.
 * @param   pv      Pointer to the memory block.
 * @param   cb      Size of the memory block in bytes.
 */
RTDECL(uint64_t) RTCrc64(const void *pv, size_t cb)
{
    const uint8_t  *pu8 = (const uint8_t *)pv;
    uint64_t        uCRC64 = 0ULL;
    while (cb--)
        uCRC64 = g_au64CRC64[(uCRC64 ^ *pu8++) & 0xff] ^ (uCRC64 >> 8);
    return uCRC64;
}
RT_EXPORT_SYMBOL(RTCrc64);


/**
 * Start a multiblock CRC64 calculation.
 *
 * @returns Start CRC64.
 */
RTDECL(uint64_t) RTCrc64Start(void)
{
    return 0ULL;
}
RT_EXPORT_SYMBOL(RTCrc64Start);


/**
 * Processes a multiblock of a CRC64 calculation.
 *
 * @returns Intermediate CRC64 value.
 * @param   uCRC64  Current CRC64 intermediate value.
 * @param   pv      The data block to process.
 * @param   cb      The size of the data block in bytes.
 */
RTDECL(uint64_t) RTCrc64Process(uint64_t uCRC64, const void *pv, size_t cb)
{
    const uint8_t *pu8 = (const uint8_t *)pv;
    while (cb--)
        uCRC64 = g_au64CRC64[(uCRC64 ^ *pu8++) & 0xff] ^ (uCRC64 >> 8);
    return uCRC64;
}
RT_EXPORT_SYMBOL(RTCrc64Process);


/**
 * Complete a multiblock CRC64 calculation.
 *
 * @returns CRC64 value.
 * @param   uCRC64  Current CRC64 intermediate value.
 */
RTDECL(uint64_t) RTCrc64Finish(uint64_t uCRC64)
{
    return uCRC64;
}
RT_EXPORT_SYMBOL(RTCrc64Finish);

