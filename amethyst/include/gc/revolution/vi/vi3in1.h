/*---------------------------------------------------------------------------*
  Project:  Revolution 3in1
  File:     vi3in1.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: vi3in1.h,v $
  Revision 1.2  04/19/2006 08:39:43  urata
  Modified a value of the gamma define.

  Revision 1.1  2006/02/09 03:01:57  urata
  Initial checkin.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef _VI3IN1_H_
#define _VI3IN1_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/vi/vi3in1types.h>

typedef enum _VIGamma
{
    VI_GM_0_1=1,
    VI_GM_0_2,
    VI_GM_0_3,
    VI_GM_0_4,
    VI_GM_0_5,
    VI_GM_0_6,
    VI_GM_0_7,
    VI_GM_0_8,
    VI_GM_0_9,
    VI_GM_1_0,
    VI_GM_1_1,
    VI_GM_1_2,
    VI_GM_1_3,
    VI_GM_1_4,
    VI_GM_1_5,
    VI_GM_1_6,
    VI_GM_1_7,
    VI_GM_1_8,
    VI_GM_1_9,
    VI_GM_2_0,
    VI_GM_2_1,
    VI_GM_2_2,
    VI_GM_2_3,
    VI_GM_2_4,
    VI_GM_2_5,
    VI_GM_2_6,
    VI_GM_2_7,
    VI_GM_2_8,
    VI_GM_2_9,
    VI_GM_3_0
} VIGamma;


// for the game developer
void VISetGamma     ( VIGamma gamma );
void VISetTrapFilter( VIBool filter );

#ifdef __cplusplus
}
#endif

#endif // _VI3IN1_H_

