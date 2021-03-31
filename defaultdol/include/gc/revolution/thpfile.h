/*---------------------------------------------------------------------------*
  Project:  THP file format
  File:     thpfile.h

  Copyright (C)2002-2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: thpfile.h,v $
  Revision 1.2  02/08/2006 06:46:27  aka
  Removed including revolution.h.

  Revision 1.1  02/03/2006 10:06:49  aka
  Imported from Dolphin tree.
    
    3     03/09/15 9:38a Akagi
    
    2     03/07/01 3:12p Akagi
    Modified to divide old THPConv.exe into 2 LIBs and 1 EXE by
    Ohki-san@NTSC.
    
    1     02/05/14 11:54a Suzuki
    Initial check-in.
    
  $NoKeywords: $  

 *---------------------------------------------------------------------------*/
 
#ifndef __THPFILE_H__
#define __THPFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define THP_VERSION  (0x11000)  // Version Infomation
#define THP_COMP_MAX (16)       // Max component num

// Component number 
typedef enum
{
    THP_VIDEO_COMP,
    THP_AUDIO_COMP,
    THP_NOCOMP_COMP = 0xFF
} THPComponent;

// VideoData Type
typedef enum
{
    THP_VIDEO_NON_INTERLACE,
    THP_VIDEO_ODD_INTERLACE,
    THP_VIDEO_EVEN_INTERLACE
} THPVideoType;

//--------------------------------------
// THPHeader
//--------------------------------------
typedef struct
{
    char    magic[4];               // "THP\0"
    u32     version;                // version number
    u32     bufSize;                // max frame size for buffer computation
    u32     audioMaxSamples;        // max samples of audio data
    f32     frameRate;              // frame per seconds
    u32     numFrames;
    u32     firstFrameSize;         // how much to load
    u32     movieDataSize;           
    u32     compInfoDataOffsets;    // offset to component infomation data
    u32     offsetDataOffsets;      // offset to array of frame offsets
    u32     movieDataOffsets;       // offset to first frame (start of movie data) 
    u32     finalFrameDataOffsets;  // offset to final frame
} THPHeader;

//--------------------------------------
// THPFrameCompInfo
//--------------------------------------
typedef struct
{
    u32     numComponents;            // a number of Components in a frame
    u8      frameComp[THP_COMP_MAX];  // kind of Components
} THPFrameCompInfo;

//--------------------------------------
// THPVideoInfo
//--------------------------------------
typedef struct
{
    u32     xSize;      // width  of video
    u32     ySize;      // height of video
    u32     videoType;
} THPVideoInfo;

// OLD Structer (Ver 1.0000)
typedef struct
{
    u32     xSize;
    u32     ySize;
} THPVideoInfoOld;

 //--------------------------------------
// THPAudioInfo
//--------------------------------------
typedef struct
{
    u32     sndChannels;
    u32     sndFrequency;
    u32     sndNumSamples;
    u32     sndNumTracks;   // number of Tracks
} THPAudioInfo;

// OLD Structer
typedef struct
{
    u32     sndChannels;
    u32     sndFrequency;
    u32     sndNumSamples;
} THPAudioInfoOld;

//--------------------------------------
// THPFileHeader
//--------------------------------------
typedef struct
{
    THPHeader           header;
    THPFrameCompInfo    frameCompInfo;  
    THPVideoInfo        videoInfo;      // THP_COMP_VIDEO
    THPAudioInfo        audioInfo;      // THP_COMP_AUDIO
} THPFileHeader;

//--------------------------------------
// THPFrameHeader
//--------------------------------------
typedef struct
{
    u32     frameSizeNext;
    u32     frameSizePrevious;
    u32     comp[THP_COMP_MAX];
} THPFrameHeader;

#ifdef __cplusplus
}
#endif

#endif // __THPFILE_H__
