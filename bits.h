//
//  bits.h
//  iHevc
//
//  Created by Hank Lee on 2015/8/31.
//  Copyright (c) 2015 hank. All rights reserved.
//

#ifndef ___I_HEVC_BITS_H___
#define ___I_HEVC_BITS_H___


typedef struct
{
    uint32_t m_num_held_bits;
    uint8_t  m_held_bits;
    uint32_t m_numBitsRead;

    uint8_t *m_fifo;
    //std::vector<uint8_t> m_fifo;
    uint32_t m_fifo_idx;
    uint32_t m_fifo_size;
} InputBitstream_t;


typedef struct
{
    uint32_t m_num_held_bits;
    uint8_t  m_held_bits;

    std::vector<uint8_t> m_fifo;
    uint32_t m_fifo_idx;
    uint32_t m_fifo_size;
} OutputBitstream_t;


uint32_t READ_CODE
(
    InputBitstream_t &bitstream,
    uint32_t length, 
    const char *name
);


bool READ_FLAG
(
    InputBitstream_t &bitstream,
    const char *name
);


uint32_t READ_UVLC
(
    InputBitstream_t &bitstream,
    const char *name
);


int32_t READ_SVLC
(
    InputBitstream_t &bitstream,
    const char *name
);


bool MORE_RBSP_DATA
(
    InputBitstream_t &bitstream
);


void WRITE_CODE
(
    OutputBitstream_t &bitstream,
    uint32_t uiCode,
    uint32_t uiLength,
    const char *name
);


void WRITE_FLAG
(
    OutputBitstream_t &bitstream,
    bool flag,
    const char *name
);


void WRITE_UVLC
(
    OutputBitstream_t &bitstream,
    uint32_t uiCode
);


void WRITE_SVLC
(
    OutputBitstream_t &bitstream,
    int32_t iCode
);

#endif
