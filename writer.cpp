//
//  writer.cpp
//  iHevc
//
//  Created by Hank Lee on 2023/8/15.
//  Copyright (c) 2023 Hank Lee. All rights reserved.
//

/******************************
 * include
 */
#include <stdint.h>

#include <vector>

#include "bits.h"


void WriteTimecodeSEI
(
    OutputBitstream_t &bitstream,
    uint8_t num_clock_ts, uint16_t n_frames, uint8_t seconds, uint8_t minutes, uint8_t hours
)
{
    WRITE_CODE(bitstream, num_clock_ts, 2);   // num_clock_ts
    WRITE_FLAG(bitstream, true);   // clock_timestamp_flag
    WRITE_FLAG(bitstream, false);  // units_field_based_flag
    WRITE_CODE(bitstream, 0, 5);   // counting_type
    WRITE_FLAG(bitstream, true);   // full_timestamp_flag
    WRITE_CODE(bitstream, 0, 1);   // discontinuity_flag
    WRITE_FLAG(bitstream, false);  // cnt_dropped_flag
    WRITE_CODE(bitstream, n_frames, 9);   // n_frames
    WRITE_CODE(bitstream, seconds, 6);   // SS
    WRITE_CODE(bitstream, minutes, 6);  // MM
    WRITE_CODE(bitstream, hours, 5);  // HH
    WRITE_CODE(bitstream, 0, 5);   // time_offset_length
    WRITE_CODE(bitstream, 0, 32);  // done
}




