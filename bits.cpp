//
//  bits.cpp
//  iHevc
//
//  Created by Hank Lee on 2023/8/10.
//  Copyright (c) 2023 hank. All rights reserved.
//

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <vector>

#include "bits.h"


using namespace std;


int dbg = 1;

#define min(a, b) (((a) < (b)) ? (a) : (b)) 

#define TRACE(fmt, name, val)   \
    if (dbg > 0)                \
        fprintf(stdout, fmt, name, val);

#define TRACE_4(fmt, name, len, val)   \
    if (dbg > 0)                \
        fprintf(stdout, fmt, name, len, val);


static int32_t  read_svlc(InputBitstream_t &bitstream);

static uint32_t read_uvlc(InputBitstream_t &bitstream);

static uint32_t read_bits(InputBitstream_t &bitstream, uint32_t uiNumberOfBits);

static uint32_t get_num_bits_left(InputBitstream_t &bitstream);

static uint32_t peek_bits
(
    InputBitstream_t &bitstream,
    uint32_t uiBits
);

static uint32_t pseudo_read
(
    InputBitstream_t &bitstream,
    uint32_t uiNumberOfBits
);

static void write_bits
(
    OutputBitstream_t &bitstream,
    uint32_t uiBits,
    uint32_t uiNumberOfBits
);


static uint32_t ConvertToUInt(int iValue)
{
    return (iValue <= 0) ? -iValue << 1 : (iValue << 1) - 1;
}

static int32_t read_svlc(InputBitstream_t &bitstream)
{
    uint32_t leadingZeroBits = (uint32_t) -1;
    uint32_t kplus1 = 0;        // k is the codeNum in uvlc
    int32_t  codeNum;           // codeNum in svlc
    bool     b;
    
    for (b = 0; !b; leadingZeroBits++)
    {
        b = (bool) (read_bits(bitstream, 1) & 0x01);
    }
    
    kplus1 = (1 << leadingZeroBits) + read_bits(bitstream, leadingZeroBits);

    codeNum = (kplus1 & 1) ? - (int32_t) (kplus1 >> 1) : (int32_t) (kplus1 >> 1);
    
    return codeNum;    
}

static uint32_t read_uvlc(InputBitstream_t &bitstream)
{
    /* coding according to 9-1 */
    uint32_t leadingZeroBits = (uint32_t) -1;
    uint32_t codeNum = 0;
    bool     b;
    
    for (b = 0; !b; leadingZeroBits++)
    {
        b = (bool) (read_bits(bitstream, 1) & 0x01);
    }
    
    codeNum = (1 << leadingZeroBits) - 1 + read_bits(bitstream, leadingZeroBits);
    
    return codeNum;
}


/**
 * TComInputBitstream::read() in HM
 *
 * read_bits(n) in H.265 spec
 */
static uint32_t read_bits(InputBitstream_t &bitstream, uint32_t uiNumberOfBits)
{
    //assert(uiNumberOfBits <= 32);
    
    bitstream.m_numBitsRead += uiNumberOfBits;
    
    /* NB, bits are extracted from the MSB of each byte. */
    uint32_t retval = 0;
    
    if (uiNumberOfBits <= bitstream.m_num_held_bits)
    {
        /* n=1, len(H)=7:   -VHH HHHH, shift_down=6, mask=0xfe
         * n=3, len(H)=7:   -VVV HHHH, shift_down=4, mask=0xf8
         */
        retval = bitstream.m_held_bits >> (bitstream.m_num_held_bits - uiNumberOfBits);
        retval &= ~(0xff << uiNumberOfBits);
        bitstream.m_num_held_bits -= uiNumberOfBits;
        
        return retval;
    }
    
    /* all num_held_bits will go into retval
     *   => need to mask leftover bits from previous extractions
     *   => align retval with top of extracted word */
    /* n=5, len(H)=3: ---- -VVV, mask=0x07, shift_up=5-3=2,
     * n=9, len(H)=3: ---- -VVV, mask=0x07, shift_up=9-3=6 */
    uiNumberOfBits -= bitstream.m_num_held_bits;
    retval = bitstream.m_held_bits & ~(0xff << bitstream.m_num_held_bits);
    retval <<= uiNumberOfBits;
    
    /* number of whole bytes that need to be loaded to form retval */
    /* n=32, len(H)=0, load 4bytes, shift_down=0
     * n=32, len(H)=1, load 4bytes, shift_down=1
     * n=31, len(H)=1, load 4bytes, shift_down=1+1
     * n=8,  len(H)=0, load 1byte,  shift_down=0
     * n=8,  len(H)=3, load 1byte,  shift_down=3
     * n=5,  len(H)=1, load 1byte,  shift_down=1+3
     */
    uint32_t aligned_word = 0;
    uint32_t num_bytes_to_load = (uiNumberOfBits - 1) >> 3;
    
    //assert(m_fifo_idx + num_bytes_to_load < m_fifo->size());
    
    switch (num_bytes_to_load)
    {
        case 3: aligned_word  = (bitstream.m_fifo)[bitstream.m_fifo_idx++] << 24;
        case 2: aligned_word |= (bitstream.m_fifo)[bitstream.m_fifo_idx++] << 16;
        case 1: aligned_word |= (bitstream.m_fifo)[bitstream.m_fifo_idx++] << 8;
        case 0: aligned_word |= (bitstream.m_fifo)[bitstream.m_fifo_idx++];
    }
    
    /* resolve remainder bits */
    uint32_t next_num_held_bits = (32 - uiNumberOfBits) % 8;
    
    /* copy required part of aligned_word into retval */
    retval |= aligned_word >> next_num_held_bits;
    
    /* store held bits */
    bitstream.m_num_held_bits = next_num_held_bits;
    bitstream.m_held_bits = (uint8_t) (aligned_word & 0xFF);
    
    return retval;
}


static uint32_t get_num_bits_left(InputBitstream_t &bitstream) 
{ 
    return 8 * (bitstream.m_fifo_size - bitstream.m_fifo_idx) + bitstream.m_num_held_bits;
}


static uint32_t peek_bits
(
    InputBitstream_t &bitstream,
    uint32_t uiBits
)
{
    return pseudo_read(bitstream, uiBits);
}


/**
 * TComInputBitstream::pseudoRead() in HM
 *
 */
static uint32_t pseudo_read
(
    InputBitstream_t &bitstream,
    uint32_t uiNumberOfBits
)
{
    uint32_t saved_num_held_bits;
    uint8_t  saved_held_bits;
    uint32_t saved_fifo_idx;

    uint32_t retVal;
    
    uint32_t num_bits_to_read = min(uiNumberOfBits, get_num_bits_left(bitstream));


    saved_num_held_bits = bitstream.m_num_held_bits;
    saved_held_bits     = bitstream.m_held_bits;
    saved_fifo_idx      = bitstream.m_fifo_idx;
    
    retVal = read_bits(bitstream, num_bits_to_read);

    retVal <<= (uiNumberOfBits - num_bits_to_read);
    
    bitstream.m_fifo_idx      = saved_fifo_idx;
    bitstream.m_held_bits     = saved_held_bits;
    bitstream.m_num_held_bits = saved_num_held_bits;

    return retVal;
}


uint32_t READ_CODE
(
    InputBitstream_t &bitstream,
    uint32_t length, 
    const char *name
)
{
    uint32_t ret;
    
    ret = read_bits(bitstream, length);

    TRACE_4("%-50s u(%d)  : %u\n", name, length, ret);

    return ret;
}


bool READ_FLAG
(
    InputBitstream_t &bitstream,
    const char *name
)
{
    bool ret;

    ret = (bool) (read_bits(bitstream, 1) & 0x01);
    
    TRACE("%-50s u(1)  : %u\n", name, ret);

    return ret;
}


uint32_t READ_UVLC
(
    InputBitstream_t &bitstream,
    const char *name
)
{
    uint32_t ret;

    ret = read_uvlc(bitstream);

    TRACE("%-50s ue(v) : %u\n", name, ret);

    return ret;
}


int32_t READ_SVLC
(
    InputBitstream_t &bitstream,
    const char *name
)
{
    int32_t ret;
    
    ret = read_svlc(bitstream);
    
    TRACE("%-50s se(v) : %d\n", name, ret);
    
    return ret;
}


bool MORE_RBSP_DATA(InputBitstream_t &bitstream)
{ 
    int bitsLeft = get_num_bits_left(bitstream);
    
    // if there are more than 8 bits, it cannot be rbsp_trailing_bits
    if (bitsLeft > 8)
    {
        return true;
    }
    
    uint8_t lastByte = peek_bits(bitstream, bitsLeft);
    int cnt = bitsLeft;
    
    // remove trailing bits equal to zero
    while ((cnt > 0) && ((lastByte & 1) == 0))
    {
        lastByte >>= 1;
        cnt--;
    }

    // remove bit equal to one
    cnt--;
    
    // we should not have a negative number of bits
    assert(cnt >= 0);
    
    // we have more data, if cnt is not zero
    return (cnt > 0);
}

/**
 * TComOutputBitstream::write() in HM
 *
 * Append uiNumberOfBits least significant bits of uiBits to the current bitstream
 #
 */
static void write_bits(OutputBitstream_t &bitstream, uint32_t uiBits, uint32_t uiNumberOfBits)
{
    /* any modulo 8 remainder of num_total_bits cannot be written this time,
     * and will be held until next time. */
    uint32_t num_total_bits = uiNumberOfBits + bitstream.m_num_held_bits;
    uint32_t next_num_held_bits = num_total_bits % 8;
    
    /* form a byte aligned word (write_bits), by concatenating any held bits
     * with the new bits, discarding the bits that will form the next_held_bits.
     * eg: H = held bits, V = n new bits        /---- next_held_bits
     * len(H)=7, len(V)=1: ... ---- HHHH HHHV . 0000 0000, next_num_held_bits=0
     * len(H)=7, len(V)=2: ... ---- HHHH HHHV . V000 0000, next_num_held_bits=1
     * if total_bits < 8, the value of v_ is not used */
    uint8_t next_held_bits = uiBits << (8 - next_num_held_bits);
    
    if (!(num_total_bits >> 3))
    {
      /* insufficient bits accumulated to write out, append new_held_bits to
       * current held_bits */
      /* NB, this requires that v only contains 0 in bit positions {31..n} */
      bitstream.m_held_bits |= next_held_bits;
      bitstream.m_num_held_bits = next_num_held_bits;
    
      return;
    }
    
    /* topword serves to justify held_bits to align with the msb of uiBits */
    uint32_t topword = (uiNumberOfBits - next_num_held_bits) & ~((1 << 3) -1);
    uint32_t write_value = (bitstream.m_held_bits << topword) | (uiBits >> next_num_held_bits);
    
    switch (num_total_bits >> 3)
    {
      case 4: bitstream.m_fifo.push_back(write_value >> 24);
      case 3: bitstream.m_fifo.push_back(write_value >> 16);
      case 2: bitstream.m_fifo.push_back(write_value >> 8);
      case 1: bitstream.m_fifo.push_back(write_value);
    }

    bitstream.m_held_bits = next_held_bits;
    bitstream.m_num_held_bits = next_num_held_bits;
}


void WRITE_CODE
(
    OutputBitstream_t &bitstream,
    uint32_t uiCode,
    uint32_t uiLength,
    const char *name
)
{
    write_bits(bitstream, uiCode, uiLength);

    TRACE_4("%-50s u(%d)  : %d\n", name, uiLength, uiCode);
}


void WRITE_FLAG
(
    OutputBitstream_t &bitstream,
    bool flag,
    const char *name
)
{
    write_bits(bitstream, flag, 1);

    TRACE("%-50s u(1)  : %d\n", name, flag);
}


void WRITE_UVLC
(
    OutputBitstream_t &bitstream,
    uint32_t uiCode
)
{
    uint32_t uiLength = 1;
    uint32_t uiTemp = ++uiCode;

    while (1 != uiTemp)
    {
        uiTemp >>= 1;
        uiLength += 2;
    }

    write_bits(bitstream, 0, uiLength >> 1);
    write_bits(bitstream, uiCode, (uiLength+1) >> 1);
}


void WRITE_SVLC
(
    OutputBitstream_t &bitstream,
    int32_t iCode
)
{
    uint32_t uiCode;

    uiCode = ConvertToUInt(iCode);

    WRITE_UVLC(bitstream, uiCode);
}


