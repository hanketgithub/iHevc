

#ifndef ___I_HEVC_PARSER_H___
#define ___I_HEVC_PARSER_H___



extern void ParseVPS(VPS_t *pVPS);

extern void ParseSPS(HevcInfo_t *pHevcInfo);

extern void ParsePPS(void);

extern void ParseAUD();

extern void ParseSliceHeader(NalUnitType nal_unit_type, std::string &message);




#endif

