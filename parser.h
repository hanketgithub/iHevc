

#ifndef ___I_HEVC_PARSER_H___
#define ___I_HEVC_PARSER_H___



extern void ParseVPS(InputBitstream_t &bitstream, VPS_t &pVPS);

extern void ParseSPS(InputBitstream_t &bitstream, SPS_t SPSs[], HevcInfo_t *pHevcInfo);

extern void ParsePPS(InputBitstream_t &bitstream, PPS_t PPSs[]);

extern void ParseAUD(InputBitstream_t &bitstream);

extern void ParseSliceHeader
(
    InputBitstream_t &bitstream, 
    Slice_t &slice,
    SPS_t SPSs[],
    PPS_t PPSs[],
    NalUnitType nal_unit_type,
    std::string &message
);

extern uint32_t ParseSEI
(
    InputBitstream_t &bitstream,
    SeiType     *payloadType,
    uint8_t     *payloadSize,
    PicStruct   *picStruct
);


#endif

