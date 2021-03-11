/*---------------------------------------------------------------------------*
  Project:  Wii Connect 24
  File:     NWC24Mini.h

  Copyright (C)2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: NWC24Mini.h,v $
  Revision 1.2  2006/12/14 06:12:03  hirose
  Added NWC24ResumeScheduler().

  Revision 1.1  2006/12/12 02:40:40  hirose
  Initial check in.


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __NWC24MINI_H__
#define __NWC24MINI_H__
#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

#ifndef __NWC24SCHEDULE_H__
s32         NWC24SuspendScheduler( void );
s32         NWC24ResumeScheduler( void );
#endif /* __NWC24SCHEDULE_H__ */

/*===========================================================================*/
#ifdef __cplusplus
}
#endif
#endif  /* __NWC24MINI_H__ */
