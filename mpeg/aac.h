/*****************************************************************************
 * aac.h: ISO/IEC 14496-3 Advanced Audio Coding
 *****************************************************************************
 * Copyright (C) 2010 VideoLAN
 * $Id$
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 *****************************************************************************/

/*
 * Normative references:
 *  - ISO/IEC 14496-3 Subpart 4:1998(E)
 *    (Advanced Audio Coding, Time/Frequency Coding)
 */

#ifndef __BITSTREAM_MPEG_AAC_H__
#define __BITSTREAM_MPEG_AAC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * ADTS header
 *****************************************************************************/
#define ADTS_HEADER_SIZE        7

/* fixed header */
static inline void adts_set_sync(uint8_t *p_adts)
{
    p_adts[0] = 0xff;
    p_adts[1] = 0xf9; /* protection absent */
    p_adts[2] = 0x0;
    p_adts[3] = 0x0;
    p_adts[4] = 0x0;
    p_adts[5] = 0x0;
    p_adts[6] = 0x0;
}

static inline void adts_set_profile(uint8_t *p_adts, uint8_t i_profile)
{
    p_adts[2] &= ~0xc0;
    p_adts[2] |= i_profile << 6;
}

static inline void adts_set_index(uint8_t *p_adts, uint8_t i_index)
{
    p_adts[2] &= ~0x3c;
    p_adts[2] |= (i_index & 0xf) << 2;
}

static inline void adts_set_channels(uint8_t *p_adts, uint8_t i_channels)
{
    p_adts[2] &= ~0x03; /* also clear out the private bit */
    p_adts[2] |= (i_channels & 0x7) >> 2;
    p_adts[3] &= ~0xc0;
    p_adts[3] |= (i_channels & 0x7) << 6;
}

static inline void adts_set_copy(uint8_t *p_adts, bool b_copy)
{
    if (!b_copy)
        p_adts[3] &= ~0x20;
    else
        p_adts[3] |= 0x20;
}

static inline void adts_set_home(uint8_t *p_adts, bool b_home)
{
    if (!b_home)
        p_adts[3] &= ~0x10;
    else
        p_adts[3] |= 0x10;
}

/* variable header */
static inline void adts_set_cp_id(uint8_t *p_adts, bool b_bit, bool b_start)
{
    p_adts[3] &= ~0x0c;
    if (b_bit)
        p_adts[3] |= 0x08;
    if (b_start)
        p_adts[3] |= 0x04;
}

static inline void adts_set_length(uint8_t *p_adts, uint16_t i_length)
{
    p_adts[3] &= ~0x03;
    p_adts[3] |= (i_length >> 11) & 0x03;
    p_adts[4] = i_length >> 3;
    p_adts[5] &= ~0xe0;
    p_adts[5] |= (i_length << 5) & 0xe0;
}

static inline void adts_set_fullness(uint8_t *p_adts, uint16_t i_fullness)
{
    p_adts[5] &= ~0x1f;
    p_adts[5] |= (i_fullness >> 6) & 0x1f;
    p_adts[6] &= ~0xfc;
    p_adts[6] |= (i_fullness << 2) & 0xfc;
}

/* i_blocks == number of blocks - 1 */
static inline void adts_set_num_blocks(uint8_t *p_adts, uint8_t i_blocks)
{
    p_adts[6] &= ~0x03;
    p_adts[6] |= i_blocks & 0x03;
}

#ifdef __cplusplus
}
#endif

#endif