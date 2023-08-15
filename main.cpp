//
//  main.cpp
//  iHevc
//
//  Created by Hank Lee on 2023/8/10.
//  Copyright (c) 2023 Hank Lee. All rights reserved.
//

/******************************
 * include
 */
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>
      
#include "common.h"
#include "bits.h"
#include "parser.h"
#include "writer.h"


using namespace std;



/******************************
 * define
 */
#define SIZE_OF_NAL_UNIT_HDR    2
#define ES_BUFFER_SIZE          (3840 * 2160)

static uint8_t u8endCode[] = { 0xFC, 0xFD, 0xFE, 0xFF };

static uint8_t u8EsBuffer[ES_BUFFER_SIZE + sizeof(u8endCode)];

static VPS_t vps;

static HevcInfo_t tHevcInfo;

string message;

/******************************
 * local function
 */
 
static bool has_start_code
(
    uint8_t *addr,
    uint8_t  zeros
)
{
    int i;
    
    for (i = 0; i < zeros; i++)
    {
        if (addr[i]) return false;
    }
    
    return addr[i] == 0x01 ? true : false;
}

static bool has_end_code(uint8_t *p)
{
    if (memcmp(p, u8endCode, sizeof(u8endCode)) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


static bool scan_nal
(
    uint8_t     *start_addr,
    uint8_t     *nal_unit_header,
    uint32_t    *nal_len,
    uint32_t    *prefix_len
)
{
    uint8_t offset = 0;
    uint8_t *p;

    bool ret = true;
    
    p = start_addr;

    if (has_start_code(p, 2))      // short prefix
    {
        offset = 3;
    }
    else if (has_start_code(p, 3)) // long prefix
    {
        offset = 4;
    }

    //printf("prefix offset=%d\n", offset);
    *prefix_len = offset;
    p += offset;

    while (!has_end_code(p) && !has_start_code(p, 2) && !has_start_code(p, 3))
    {
        p++;
    }

    nal_unit_header[0] = start_addr[offset];
    nal_unit_header[1] = start_addr[offset + 1];
    *nal_len  = (uint32_t) (p - start_addr);

    if (has_end_code(p))
    {
        ret = false;
    }

    return ret;
}

static uint32_t EBSPtoRBSP
(
    uint8_t *streamBuffer,
    uint32_t end_bytepos,
    uint32_t begin_bytepos
)
{
    uint32_t i;
    uint32_t j;
    uint32_t count;
    
    count = 0;
    j = begin_bytepos;
    
    for (i = begin_bytepos; i < end_bytepos; i++)
    {
        // in NAL unit, 0x000000, 0x000001, 0x000002 shall not occur at any byte-aligned position
        if (count == 2 && streamBuffer[i] < 0x03)
        {
            return -1;
        }
        
        if (count == 2 && streamBuffer[i] == 0x03)
        {
            // check the 4th byte after 0x000003, except when cabac.....
            if ((i < end_bytepos - 1) && (streamBuffer[i + 1] > 0x03))
            {
                return -1;
            }
            
            if (i == end_bytepos - 1)
            {
                return j;
            }
            
            // escape 0x03 byte!
            i++;
            count = 0;
        }
        
        streamBuffer[j] = streamBuffer[i];
        //printf("[%02u] 0x%02x\n", j, streamBuffer[j]);
        
        if (streamBuffer[i] == 0x00)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        
        j++;
    }
    
    return j;
}


bool fill_es_buffer
(
    uint8_t *pu8NalAddr,
    uint32_t u32NalSize,
    int fd
)
{
    ssize_t rd_sz;
    
    memmove(u8EsBuffer, pu8NalAddr, u32NalSize);

    rd_sz = read(fd, &u8EsBuffer[u32NalSize], ES_BUFFER_SIZE - u32NalSize);
    if (rd_sz == 0) // EOF
    {
        return false;
    }
    else if (rd_sz < (ES_BUFFER_SIZE - u32NalSize))  // last read!  
    {
        printf("last read!\n");
        
        // Append Stop Code at the end of last read
        memcpy(&u8EsBuffer[u32NalSize + rd_sz], u8endCode, sizeof(u8endCode));        
    }
    else
    {
        // Append Stop Code at the end of buffer
        memcpy(&u8EsBuffer[ES_BUFFER_SIZE], u8endCode, sizeof(u8endCode));
    }

    return true;
}

void test_timecode_sei()
{
    OutputBitstream_t bitstream;

    uint8_t num_clock_ts = 1;
    uint16_t n_frames = 0;
    uint8_t seconds = 5;
    uint8_t minutes = 10;
    uint8_t hours =  15;

    WriteTimecodeSEI(&bitstream, num_clock_ts, n_frames, seconds, minutes, hours);
    
    cout << bitstream.m_fifo.size() << endl;

    vector<uint8_t> sei;

    // Write short start code
    sei.insert(sei.end(), {0x00, 0x00, 0x01} );
    // Wrrite SEI NAL header
    sei.insert(sei.end(), {0x4E, 0x01} );
    // Write payload type
    sei.push_back(SEI_TIME_CODE);
    // Wtite payload size
    sei.push_back( bitstream.m_fifo.size() );

    // Write payload data
    for (auto v : bitstream.m_fifo)
    {
        sei.push_back(v);
    }
    
    for (auto v : sei)
    {
        printf("0x%02x\n", v);
    }
}


int main(int argc, const char * argv[])
{
#if 1
    test_timecode_sei();
#else


    int fd;
    ssize_t rd_sz;
        
    if (argc < 2)
    {
        printf("useage: %s [input_file]\n", argv[0]);
        
        return -1;
    }


    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror(argv[1]);
        exit(-1);
    }
    
    uint8_t *ptr = u8EsBuffer;
    uint8_t nal_unit_header[SIZE_OF_NAL_UNIT_HDR];
    
    bool        forbidden_zero_bit;
    NalUnitType nal_unit_type;
    uint8_t     nuh_layer_id;
    uint8_t     nuh_temporal_id_plus1;
    uint32_t    nal_len;
    uint32_t    offset = 0;
    uint32_t    prefix_len = 0;

    fill_es_buffer(u8EsBuffer, 0, fd);
    
    while (1)
    {        
        if (!scan_nal
             (
                ptr,
                nal_unit_header,
                &nal_len,
                &prefix_len
             )
           )
        {
            // fill buffer
            printf("fill buffer!\n");
            
            if (!fill_es_buffer(ptr, nal_len, fd))
            {
                printf("No more data to read!\n");
                break;
            }

            // rewind ptr
            ptr = u8EsBuffer;
            offset = 0;

            // try scan NAL again!
            bool rescan = scan_nal
             (
                ptr,
                nal_unit_header,
                &nal_len,
                &prefix_len
             );

            printf("Try rescan NAL=%s\n", rescan ? "T" : "F");
        }

        printf("offset=%ld\n", ptr - u8EsBuffer);

        nal_unit_type           = (NalUnitType) ((nal_unit_header[0] & (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1)) >> 1);
        
        forbidden_zero_bit      = (nal_unit_header[0] & BIT7) >> 7;
        nuh_layer_id            = (nal_unit_header[0] & BIT0) << 5 | (nal_unit_header[1] & (BIT7 | BIT6 | BIT5 | BIT4 | BIT3)) >> 3;
        nuh_temporal_id_plus1   = nal_unit_header[1] & (BIT2 | BIT1 | BIT0);

        printf("forbidden_zero_bit=%d, nal_unit_type=%02u, nuh_layer_id=%u, nuh_temporal_id_plus1=%u, nal_len=%6u, offset=0x%x\n",
               forbidden_zero_bit,
               nal_unit_type,
               nuh_layer_id,
               nuh_temporal_id_plus1,
               nal_len,
               offset);
        
        
        m_pcBitstream.m_fifo            = &u8EsBuffer[offset + prefix_len + SIZE_OF_NAL_UNIT_HDR];
        m_pcBitstream.m_fifo_size       = nal_len - prefix_len - SIZE_OF_NAL_UNIT_HDR;
        m_pcBitstream.m_fifo_idx        = 0;
        m_pcBitstream.m_num_held_bits   = 0;
        m_pcBitstream.m_held_bits       = 0;
        m_pcBitstream.m_numBitsRead     = 0;
        
        switch (nal_unit_type)
        {
            case NAL_UNIT_VPS:
            {             
                EBSPtoRBSP(&u8EsBuffer[offset + prefix_len], nal_len, 0);
                
                ParseVPS(&vps);
                
                break;
            }
            case NAL_UNIT_SPS:
            {              
                EBSPtoRBSP(&u8EsBuffer[offset + prefix_len], nal_len, 0);
                
                ParseSPS(&tHevcInfo);
                
                break;
            }
            case NAL_UNIT_PPS:
            {
                EBSPtoRBSP(&u8EsBuffer[offset + prefix_len], nal_len, 0);
                
                ParsePPS();
                
                break;
            }
            case NAL_UNIT_ACCESS_UNIT_DELIMITER:
            {
                EBSPtoRBSP(&u8EsBuffer[offset + prefix_len], nal_len, 0);

                ParseAUD();
                
                break;
            }
            case NAL_UNIT_CODED_SLICE_TRAIL_N:
            case NAL_UNIT_CODED_SLICE_TRAIL_R:
            case NAL_UNIT_CODED_SLICE_TSA_N:
            case NAL_UNIT_CODED_SLICE_TSA_R:
            case NAL_UNIT_CODED_SLICE_STSA_N:
            case NAL_UNIT_CODED_SLICE_STSA_R:
            case NAL_UNIT_CODED_SLICE_RADL_N:
            case NAL_UNIT_CODED_SLICE_RADL_R:
            case NAL_UNIT_CODED_SLICE_RASL_N:
            case NAL_UNIT_CODED_SLICE_RASL_R:
            case NAL_UNIT_CODED_SLICE_BLA_W_LP:    // 16
            case NAL_UNIT_CODED_SLICE_BLA_W_RADL:  // 17
            case NAL_UNIT_CODED_SLICE_BLA_N_LP:    // 18
            case NAL_UNIT_CODED_SLICE_IDR_W_RADL:  // 19
            case NAL_UNIT_CODED_SLICE_IDR_N_LP:    // 20
            case NAL_UNIT_CODED_SLICE_CRA:         // 21
            {
                EBSPtoRBSP(&u8EsBuffer[offset + prefix_len], nal_len, 0);

                ParseSliceHeader(nal_unit_type, message);

                break;
            }
            case NAL_UNIT_PREFIX_SEI:
            {
                EBSPtoRBSP(&u8EsBuffer[offset + prefix_len], nal_len, 0);

                SeiType sei_payload_type = SEI_BUFFERING_PERIOD;
                uint8_t sei_payload_size = 0;
                PicStruct pic_struct = PIC_STRUCT_FRAME;

        
                ParseSEI(&sei_payload_type, &sei_payload_size, &pic_struct);
                
                break;
            }
            default:
            {
                break;
            }
        }
        
        offset = offset + nal_len;
        
        ptr += nal_len;        
    }

    fprintf(stderr, "Resolution: %d x %d\n", tHevcInfo.u32Width, tHevcInfo.u32Height);

    cout << message << endl;;
#endif

    return 0;
}

