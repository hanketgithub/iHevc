//
//  common.h
//  iHevc
//
//  Created by Hank Lee on 2015/8/31.
//  Copyright (c) 2015 hank. All rights reserved.
//

#ifndef ___I_HEVC_COMMON_H___
#define ___I_HEVC_COMMON_H___


#define BIT7            0x80
#define BIT6            0x40
#define BIT5            0x20
#define BIT4            0x10
#define BIT3            0x08
#define BIT2            0x04
#define BIT1            0x02
#define BIT0            0x01

#define MAX_NUM_REF_PICS    16
#define MAX_CPB_CNT         32
#define MAX_TLAYER          7   ///< Explicit temporal layer QP offset - max number of temporal layer

#define MAX_NUM_LONG_TERM_REF_PICS  33


typedef enum
{
    NAL_UNIT_CODED_SLICE_TRAIL_N = 0, // 0
    NAL_UNIT_CODED_SLICE_TRAIL_R,     // 1
    
    NAL_UNIT_CODED_SLICE_TSA_N,       // 2
    NAL_UNIT_CODED_SLICE_TSA_R,       // 3
    
    NAL_UNIT_CODED_SLICE_STSA_N,      // 4
    NAL_UNIT_CODED_SLICE_STSA_R,      // 5
    
    NAL_UNIT_CODED_SLICE_RADL_N,      // 6
    NAL_UNIT_CODED_SLICE_RADL_R,      // 7
    
    NAL_UNIT_CODED_SLICE_RASL_N,      // 8
    NAL_UNIT_CODED_SLICE_RASL_R,      // 9
    
    NAL_UNIT_RESERVED_VCL_N10,
    NAL_UNIT_RESERVED_VCL_R11,
    NAL_UNIT_RESERVED_VCL_N12,
    NAL_UNIT_RESERVED_VCL_R13,
    NAL_UNIT_RESERVED_VCL_N14,
    NAL_UNIT_RESERVED_VCL_R15,
    
    NAL_UNIT_CODED_SLICE_BLA_W_LP,    // 16
    NAL_UNIT_CODED_SLICE_BLA_W_RADL,  // 17
    NAL_UNIT_CODED_SLICE_BLA_N_LP,    // 18
    NAL_UNIT_CODED_SLICE_IDR_W_RADL,  // 19
    NAL_UNIT_CODED_SLICE_IDR_N_LP,    // 20
    NAL_UNIT_CODED_SLICE_CRA,         // 21
    NAL_UNIT_RESERVED_IRAP_VCL22,
    NAL_UNIT_RESERVED_IRAP_VCL23,
    
    NAL_UNIT_RESERVED_VCL24,
    NAL_UNIT_RESERVED_VCL25,
    NAL_UNIT_RESERVED_VCL26,
    NAL_UNIT_RESERVED_VCL27,
    NAL_UNIT_RESERVED_VCL28,
    NAL_UNIT_RESERVED_VCL29,
    NAL_UNIT_RESERVED_VCL30,
    NAL_UNIT_RESERVED_VCL31,
    
    NAL_UNIT_VPS,                     // 32
    NAL_UNIT_SPS,                     // 33
    NAL_UNIT_PPS,                     // 34
    NAL_UNIT_ACCESS_UNIT_DELIMITER,   // 35
    NAL_UNIT_EOS,                     // 36
    NAL_UNIT_EOB,                     // 37
    NAL_UNIT_FILLER_DATA,             // 38
    NAL_UNIT_PREFIX_SEI,              // 39
    NAL_UNIT_SUFFIX_SEI,              // 40
    
    NAL_UNIT_RESERVED_NVCL41,
    NAL_UNIT_RESERVED_NVCL42,
    NAL_UNIT_RESERVED_NVCL43,
    NAL_UNIT_RESERVED_NVCL44,
    NAL_UNIT_RESERVED_NVCL45,
    NAL_UNIT_RESERVED_NVCL46,
    NAL_UNIT_RESERVED_NVCL47,
    NAL_UNIT_UNSPECIFIED_48,
    NAL_UNIT_UNSPECIFIED_49,
    NAL_UNIT_UNSPECIFIED_50,
    NAL_UNIT_UNSPECIFIED_51,
    NAL_UNIT_UNSPECIFIED_52,
    NAL_UNIT_UNSPECIFIED_53,
    NAL_UNIT_UNSPECIFIED_54,
    NAL_UNIT_UNSPECIFIED_55,
    NAL_UNIT_UNSPECIFIED_56,
    NAL_UNIT_UNSPECIFIED_57,
    NAL_UNIT_UNSPECIFIED_58,
    NAL_UNIT_UNSPECIFIED_59,
    NAL_UNIT_UNSPECIFIED_60,
    NAL_UNIT_UNSPECIFIED_61,
    NAL_UNIT_UNSPECIFIED_62,
    NAL_UNIT_UNSPECIFIED_63,
    NAL_UNIT_INVALID,
} NalUnitType;


typedef enum
{
    ASPECT_RATIO_UNSPECIFIED    = 0,
    ASPECT_RATIO_1_1,
    ASPECT_RATIO_12_11,
    ASPECT_RATIO_10_11,
    ASPECT_RATIO_16_11,
    ASPECT_RATIO_40_33,
    ASPECT_RATIO_24_11,
    ASPECT_RATIO_20_11,
    ASPECT_RATIO_32_11,
    ASPECT_RATIO_80_33,
    ASPECT_RATIO_18_11,
    ASPECT_RATIO_15_11,
    ASPECT_RATIO_64_33,
    ASPECT_RATIO_160_99,
    ASPECT_RATIO_4_3,
    ASPECT_RATIO_3_2,
    ASPECT_RATIO_2_1,
    ASPECT_RATIO_EXTENDED_SAR   = 255,
} AspectRatioIdc;


typedef enum
{
    SEI_BUFFERING_PERIOD                     = 0,
    SEI_PICTURE_TIMING                       = 1,
    SEI_PAN_SCAN_RECT                        = 2,
    SEI_FILLER_PAYLOAD                       = 3,
    SEI_USER_DATA_REGISTERED_ITU_T_T35       = 4,
    SEI_USER_DATA_UNREGISTERED               = 5,
    SEI_RECOVERY_POINT                       = 6,
    SEI_SCENE_INFO                           = 9,
    SEI_FULL_FRAME_SNAPSHOT                  = 15,
    SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START = 16,
    SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END   = 17,
    SEI_FILM_GRAIN_CHARACTERISTICS           = 19,
    SEI_POST_FILTER_HINT                     = 22,
    SEI_TONE_MAPPING_INFO                    = 23,
    SEI_FRAME_PACKING                        = 45,
    SEI_DISPLAY_ORIENTATION                  = 47,
    SEI_SOP_DESCRIPTION                      = 128,
    SEI_ACTIVE_PARAMETER_SETS                = 129,
    SEI_DECODING_UNIT_INFO                   = 130,
    SEI_TEMPORAL_LEVEL0_INDEX                = 131,
    SEI_DECODED_PICTURE_HASH                 = 132,
    SEI_SCALABLE_NESTING                     = 133,
    SEI_REGION_REFRESH_INFO                  = 134,
    SEI_TIME_CODE                            = 136,
} SeiType;


typedef enum
{
    B_SLICE,
    P_SLICE,
    I_SLICE,
} SliceType;


typedef enum
{
    PIC_STRUCT_FRAME,
    PIC_STRUCT_FIELD_TOP,
    PIC_STRUCT_FIELD_BOT,
} PicStruct;



typedef enum
{
    CHANNEL_TYPE_LUMA    = 0,
    CHANNEL_TYPE_CHROMA  = 1,
    MAX_NUM_CHANNEL_TYPE = 2
} ChannelType;


/// chroma formats (according to semantics of chroma_format_idc)
typedef enum
{
    CHROMA_400        = 0,
    CHROMA_420        = 1,
    CHROMA_422        = 2,
    CHROMA_444        = 3,
    NUM_CHROMA_FORMAT = 4
} ChromaFormat;


typedef struct
{
    uint32_t u32Width;
    uint32_t u32Height;
} HevcInfo_t;


typedef struct
{
    uint32_t    m_bRefPicListModificationFlagL0;  
    uint32_t    m_bRefPicListModificationFlagL1;  
    uint32_t    m_RefPicSetIdxL0[32];
    uint32_t    m_RefPicSetIdxL1[32];
} RefPicListModification_t;


typedef struct
{
    uint32_t    m_numberOfPictures;
    uint32_t    m_numberOfNegativePictures;
    uint32_t    m_numberOfPositivePictures;
    uint32_t    m_numberOfLongtermPictures;

    bool        m_used[MAX_NUM_REF_PICS];
    
} ReferencePictureSet_t;


typedef struct
{
    uint32_t                m_numberOfReferencePictureSets;
    
    ReferencePictureSet_t  *m_referencePictureSets;
} RPSList_t;


typedef struct
{
    bool        fixedPicRateFlag;
    bool        fixedPicRateWithinCvsFlag;
    uint32_t    picDurationInTcMinus1;
    bool        lowDelayHrdFlag;
    uint32_t    cpbCntMinus1;
    uint32_t    bitRateValueMinus1 [MAX_CPB_CNT][2];
    uint32_t    cpbSizeValue       [MAX_CPB_CNT][2];
    uint32_t    ducpbSizeValue     [MAX_CPB_CNT][2];
    bool        cbrFlag            [MAX_CPB_CNT][2];
    uint32_t    duBitRateValue     [MAX_CPB_CNT][2];
} HrdSubLayerInfo_t;


typedef struct
{
    bool        m_nalHrdParametersPresentFlag;
    bool        m_vclHrdParametersPresentFlag;
    bool        m_subPicCpbParamsPresentFlag;
    uint32_t    m_tickDivisorMinus2;
    uint32_t    m_duCpbRemovalDelayLengthMinus1;
    bool        m_subPicCpbParamsInPicTimingSEIFlag;
    uint32_t    m_dpbOutputDelayDuLengthMinus1;
    uint32_t    m_bitRateScale;
    uint32_t    m_cpbSizeScale;
    uint32_t    m_ducpbSizeScale;
    uint32_t    m_initialCpbRemovalDelayLengthMinus1;
    uint32_t    m_cpbRemovalDelayLengthMinus1;
    uint32_t    m_dpbOutputDelayLengthMinus1;
    HrdSubLayerInfo_t m_HRD[MAX_TLAYER];
} HRD_t;


typedef struct
{
    int32_t                 m_VPSId;
    uint32_t                m_uiMaxTLayers;
    uint32_t                m_uiMaxLayers;
    bool                    m_bTemporalIdNestingFlag;

    uint32_t                m_numReorderPics[MAX_TLAYER];
    uint32_t                m_uiMaxDecPicBuffering[MAX_TLAYER];
    uint32_t                m_uiMaxLatencyIncrease[MAX_TLAYER]; // Really max latency increase plus 1 (value 0 expresses no limit)

    uint32_t                m_numHrdParameters;
    uint32_t                m_maxNuhReservedZeroLayerId;
    std::vector<HRD_t>      m_hrdParameters;
    std::vector<uint32_t>   m_hrdOpSetIdx;
    std::vector<bool>       m_cprmsPresentFlag;
    uint32_t                m_numOpSets;
//    bool                    m_layerIdIncludedFlag[MAX_VPS_OP_SETS_PLUS1][MAX_VPS_NUH_RESERVED_ZERO_LAYER_ID_PLUS1];

//    TComPTL                 m_pcPTL;
//    TimingInfo              m_timingInfo;
} VPS_t;


typedef struct 
{
    bool        m_aspectRatioInfoPresentFlag;
    int32_t     m_aspectRatioIdc;
    int32_t     m_sarWidth;
    int32_t     m_sarHeight;
    bool        m_overscanInfoPresentFlag;
    bool        m_overscanAppropriateFlag;
    bool        m_videoSignalTypePresentFlag;
    int32_t     m_videoFormat;
    bool        m_videoFullRangeFlag;
    bool        m_colourDescriptionPresentFlag;
    int32_t     m_colourPrimaries;
    int32_t     m_transferCharacteristics;
    int32_t     m_matrixCoefficients;
    bool        m_chromaLocInfoPresentFlag;
    int32_t     m_chromaSampleLocTypeTopField;
    int32_t     m_chromaSampleLocTypeBottomField;
    bool        m_neutralChromaIndicationFlag;
    bool        m_fieldSeqFlag;
    //Window      m_defaultDisplayWindow;
    bool        m_frameFieldInfoPresentFlag;
    bool        m_hrdParametersPresentFlag;
    bool        m_bitstreamRestrictionFlag;
    bool        m_tilesFixedStructureFlag;
    bool        m_motionVectorsOverPicBoundariesFlag;
    bool        m_restrictedRefPicListsFlag;
    int32_t     m_minSpatialSegmentationIdc;
    int32_t     m_maxBytesPerPicDenom;
    int32_t     m_maxBitsPerMinCuDenom;
    int32_t     m_log2MaxMvLengthHorizontal;
    int32_t     m_log2MaxMvLengthVertical;
    HRD_t       m_hrdParameters;
    //TimingInfo  m_timingInfo;
} VUI_t;


typedef struct
{
    int32_t             m_SPSId;
    int32_t             m_VPSId;
    ChromaFormat        m_chromaFormatIdc;
    bool                m_separateColourPlaneFlag;

    uint32_t            m_uiMaxTLayers;           // maximum number of temporal layers
    
    // Structure
    uint32_t             m_picWidthInLumaSamples;
    uint32_t             m_picHeightInLumaSamples;
    
    int32_t              m_log2MinCodingBlockSize;
    int32_t              m_log2DiffMaxMinCodingBlockSize;
    uint32_t             m_uiMaxCUWidth;
    uint32_t             m_uiMaxCUHeight;
    uint32_t             m_uiMaxTotalCUDepth; ///< Total CU depth, relative to the smallest possible transform block size.
    
    //Window           m_conformanceWindow;
    
    RPSList_t       m_RPSList;
    bool            m_bLongTermRefsPresent;
    bool            m_SPSTemporalMVPEnabledFlag;
    int32_t         m_numReorderPics[MAX_TLAYER];
    
    // Tool list
    uint32_t        m_uiQuadtreeTULog2MaxSize;
    uint32_t        m_uiQuadtreeTULog2MinSize;
    uint32_t        m_uiQuadtreeTUMaxDepthInter;
    uint32_t        m_uiQuadtreeTUMaxDepthIntra;
    bool            m_usePCM;
    uint32_t        m_pcmLog2MaxSize;
    uint32_t        m_uiPCMLog2MinSize;
    bool            m_useAMP;
    
    // Parameter
    //BitDepths       m_bitDepths;
    int32_t         m_qpBDOffset[MAX_NUM_CHANNEL_TYPE];
    int32_t         m_pcmBitDepths[MAX_NUM_CHANNEL_TYPE];
    bool            m_bPCMFilterDisableFlag;
    
    uint32_t        m_uiBitsForPOC;
    uint32_t        m_numLongTermRefPicSPS;
    uint32_t        m_ltRefPicPocLsbSps[MAX_NUM_LONG_TERM_REF_PICS];
    bool            m_usedByCurrPicLtSPSFlag[MAX_NUM_LONG_TERM_REF_PICS];
    // Max physical transform size
    uint32_t        m_uiMaxTrSize;
    
    bool            m_bUseSAO;
    
    bool            m_bTemporalIdNestingFlag; // temporal_id_nesting_flag
    
    bool            m_scalingListEnabledFlag;
    bool            m_scalingListPresentFlag;
    //ScalingList_t   m_scalingList;
    uint32_t        m_uiMaxDecPicBuffering[MAX_TLAYER];
    uint32_t        m_uiMaxLatencyIncreasePlus1[MAX_TLAYER];
    
    bool            m_useStrongIntraSmoothing;
    
    bool            m_vuiParametersPresentFlag;
    VUI_t           m_vuiParameters;
    
    //TComSPSRExt      m_spsRangeExtension;
    
    //static const int32_t m_winUnitX[NUM_CHROMA_FORMAT];
    //static const int32_t m_winUnitY[NUM_CHROMA_FORMAT];
    //TComPTL          m_pcPTL;
} SPS_t;


typedef struct
{
    uint32_t    m_PPSId;
    uint32_t    m_SPSId;
    int32_t     m_picInitQPMinus26;
    bool        m_bSliceChromaQpFlag;

    uint32_t    m_numRefIdxL0DefaultActive;
    uint32_t    m_numRefIdxL1DefaultActive;
    
    bool        m_dependentSliceSegmentsEnabledFlag;
    bool        m_tilesEnabledFlag;
    bool        m_entropyCodingSyncEnabledFlag;

    bool        m_bUseWeightPred;           // Use of Weighting Prediction (P_SLICE)
    bool        m_useWeightedBiPred;        // Use of Weighting Bi-Prediction (B_SLICE)
    bool        m_OutputFlagPresentFlag;

    uint32_t    m_numExtraSliceHeaderBits;

    bool        m_cabacInitPresentFlag;

    bool        m_sliceHeaderExtensionPresentFlag;
    
    bool        m_loopFilterAcrossSlicesEnabledFlag;
    bool        m_deblockingFilterControlPresentFlag;
    bool        m_deblockingFilterOverrideEnabledFlag;
    bool        m_picDisableDeblockingFilterFlag;
    int32_t     m_deblockingFilterBetaOffsetDiv2;    //< beta offset for deblocking filter
    int32_t     m_deblockingFilterTcOffsetDiv2;      //< tc offset for deblocking filter
    
    bool        m_listsModificationPresentFlag;
} PPS_t;


typedef struct
{
    bool        m_saoEnabledFlag;
    bool        m_saoEnabledFlagChroma;
    uint32_t    m_iPPSId;
    bool        m_PicOutputFlag;
    uint32_t    m_iPOC;

    ReferencePictureSet_t m_pcRPS;
    ReferencePictureSet_t m_LocalRPS;

    RefPicListModification_t    m_RefPicListModification;
                
    SliceType   m_eSliceType;
    int32_t     m_iSliceQp;
    bool        m_dependentSliceSegmentFlag;

    bool        m_deblockingFilterDisable;
    bool        m_deblockingFilterOverrideFlag;
    int32_t     m_deblockingFilterBetaOffsetDiv2;
    int32_t     m_deblockingFilterTcOffsetDiv2;

    uint32_t    m_aiNumRefIdx[2];

    int32_t     m_iSliceQpDelta;
    int32_t     m_iSliceQpDeltaCb;
    int32_t     m_iSliceQpDeltaCr;

    bool        m_cabacInitFlag;

    bool        m_bLMvdL1Zero;
    uint32_t    m_numEntryPointOffsets;

    bool        m_enableTMVPFlag;
} Slice_t;


#endif
