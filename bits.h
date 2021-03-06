//
//  bits.h
//  iHevc
//
//  Created by hank on 2015/8/31.
//  Copyright (c) 2015�~ hank. All rights reserved.
//

#ifndef ___I_HEVC_BITS_H___
#define ___I_HEVC_BITS_H___

typedef struct
{
    uint32_t m_num_held_bits;
    uint8_t  m_held_bits;
    uint32_t m_numBitsRead;

    uint8_t *m_fifo;
    uint32_t m_fifo_idx;
    uint32_t m_fifo_size;
} InputBitstream_t;


extern InputBitstream_t m_pcBitstream;


uint32_t READ_CODE
(
    uint32_t length, 
    char    *name
);


bool READ_FLAG
(
    char *name
);


uint32_t READ_UVLC
(
    char *name
);


int32_t READ_SVLC
(
    char *name
);



bool MORE_RBSP_DATA
(
    void
);

#endif
