
#ifndef ___I_HEVC_WRITER_H___
#define ___I_HEVC_WRITER_H___


extern void WriteTimecodeSEI
(
    OutputBitstream_t &bitstream,
    uint8_t num_clock_ts, uint16_t n_frames, uint8_t seconds, uint8_t minutes, uint8_t hours
);


#endif

