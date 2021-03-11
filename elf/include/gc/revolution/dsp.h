/*---------------------------------------------------------------------------*
  Project:  Revolution OS - DSP Driver and API
  File:     dsp.h

  Copyright (C)1999-2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dsp.h,v $
  Revision 1.4  03/20/2006 11:22:57  hiratsu
  Fixed unsafe macro.

  Revision 1.3  02/09/2006 04:46:42  aka
  Changed copyright.

  Revision 1.2  2006/02/04 11:56:44  hashida
  (none)
 
  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.
 
  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Imported from Dolphin tree.
    
    10    4/28/01 9:14p Eugene
    Added volatile keyword to state variables in DSPTaskInfo structure, for
    safety.
    
    9     4/28/01 7:24p Eugene
    Added: 
    - DSPAssertTask()
    - DSPCheckInit()
    - DSP version/timestamp for debug builds
    - Some debugging APIs
    - Priority-based task sorting.
    
    8     4/26/01 9:57p Eugene
    Interim check-in for DSP task switching.
    ================
    1. Added "NOYIELD" CPU-to-DSP directive for task resumption without
    "RESUME" interrupt from DSP. This is an optimization to reduce the
    number of DSP interrupts in general.
    2. Added "DSPAssertTask()" so that a higher-priority task can bully a
    lower-priority task into yielding the DSP. 
    
    7     4/15/01 1:59a Eugene
    Support for task switching.
    
    5     3/26/01 9:00p Eugene
    DSPRegisterCallback() now returns old callback.
    
    4     3/26/01 8:34p Eugene
    Added DSPGetDMAStatus()
    
    3     4/10/00 9:01p Eugene
    
    2     3/20/00 3:41p Eugene
    Added public function prototypes.
    
    1     3/18/00 11:07p Eugene
    Initial check-in for DSP driver header file.
    
    2     3/08/00 2:54p Tian
    Added stack argument to AIInit
    
    1     3/07/00 7:08p Eugene
    Public header file for AI driver/API.
 
  $NoKeywords: $
 
 *---------------------------------------------------------------------------*
 
  Description
  -----------
  This is the public API definition file for the DSP driver. Applications or
  OS components which need access to the DSP must include this file.
 
 *---------------------------------------------------------------------------*/
  
#ifndef __DSP_H__
#define __DSP_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
 * Includes
 *---------------------------------------------------------------------------*/
#include <revolution/types.h>

/*---------------------------------------------------------------------------*
 * Definitions
 *---------------------------------------------------------------------------*/

#define DSP_TASK_FLAG_CLEARALL  0x00000000  // zero all flags
#define DSP_TASK_FLAG_ATTACHED  0x00000001  // Indicates task is in the list. Asserted by DSPAddTask(). Cleared by __DSP_remove_task().
#define DSP_TASK_FLAG_CANCEL    0x00000002  // Asserted by DSPCancelTask(). Marks a task for cancellation/removal.

#define DSP_TASK_STATE_INIT     0           // task has never been run before    
#define DSP_TASK_STATE_RUN      1           // task is running
#define DSP_TASK_STATE_YIELD    2           // task has yielded control
#define DSP_TASK_STATE_DONE     3           // task is finished and has been culled from list

/*---------------------------------------------------------------------------*
 * Types/Declarations
 *---------------------------------------------------------------------------*/


// All DSP callbacks now pass a pointer to the parent task
typedef void (*DSPCallback)(void *task);

typedef struct STRUCT_DSP_TASK      
{                                   
    volatile u32  state;            // state of task
    volatile u32  priority;                  
    volatile u32  flags;                     
                                    
    u16 *iram_mmem_addr;            // start of IRAM image in main mem
    u32  iram_length;               // length of DRAM image, in bytes
    u32  iram_addr;                 // destination address in DSP (u16 words)
                                    
    u16 *dram_mmem_addr;            // start of DRAM image in main mem
    u32  dram_length;               // length of DRAM image, in bytes
    u32  dram_addr;                 // destination address in DSP (u16 words)
                                    
    u16  dsp_init_vector;           // start vector of task on first execution
    u16  dsp_resume_vector;         // start vector of task when resuming after context switch
                                    
    DSPCallback init_cb;            // callback for init state
    DSPCallback res_cb;             // callback for resume state
    DSPCallback done_cb;            // callback for done state
    DSPCallback req_cb;             // callback for general interrupt-driven requests from DSP

    struct STRUCT_DSP_TASK *next;   // linked list - next task (NULL if last)
    struct STRUCT_DSP_TASK *prev;   // linked list - prev task (NULL if first)

	OSTime t_context;
	OSTime t_task;

} DSPTaskInfo;


// Note that storage for the IRAM and DRAM images must be provided by the 
// task's owner.

 
/*---------------------------------------------------------------------------*
 * Globals
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * Macros
 *---------------------------------------------------------------------------*/

#define DSPGetTaskState(t)          ((t)->state)            // u32 value
#define DSPGetTaskPriority(t)       ((t)->priority)         // u32 value
#define DSPSetTaskPriority(t,x)     ((t)->priority|=(x))    // x is a u32 value

/*---------------------------------------------------------------------------*
 * Function Prototypes
 *---------------------------------------------------------------------------*/

void            DSPInit             (void);
BOOL            DSPCheckInit        (void);
                
void            DSPReset            (void);
void            DSPAssertInt        (void);
                
void            DSPHalt             (void);
void            DSPUnhalt           (void);    
                
u32             DSPReadCPUToDSPMbox (void);             
u32             DSPReadMailFromDSP  (void);             
                
void            DSPSendMailToDSP    (u32 mail);
                
u32             DSPCheckMailToDSP   (void);             
u32             DSPCheckMailFromDSP (void);             
                
u32             DSPGetDMAStatus     (void);

DSPTaskInfo    *DSPAddTask          (DSPTaskInfo *task);
DSPTaskInfo    *DSPCancelTask       (DSPTaskInfo *task);
DSPTaskInfo    *DSPAssertTask       (DSPTaskInfo *task);


// for debugging

DSPTaskInfo     *__DSPGetCurrentTask(void);


#ifdef __cplusplus
}
#endif

#endif // __DSP_H__
