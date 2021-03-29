/*---------------------------------------------------------------------------*
  Project:  Dolphin Performance Spy
  File:     perf.h

  Copyright 2000 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: perf.h,v $
  Revision 1.2  2006/02/04 11:56:44  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    11    2002/06/02 3:59p Tian
    Added PERFSetDrawFrames
    
    10    2002/01/25 4:58p Tian
    Added PERFShutDown
    
    9     2000/07/24 4:26p Tian
    Added PERFSetDrawSyncCallback
    
    8     2000/7/18 3:49p Tian
    Draw options for graphs.  Added pre- and post- draw functions
    
    7     2000/07/05 2:07p Tian
    Added event coloring
    
    6     2000/6/30 6:17p Tian
    Comments, CPU IPC/cachemiss graph
    
    5     2000/6/29 8:54p Tian
    Sampling interval now in floating point for higher resolution.
    
    4     2000/06/29 5:56p Tian
    Cache miss cycle counting
    
    3     2000/06/29 11:59a Tian
    Cleanup.  Token accessors.
    
    2     2000/06/28 4:08p Tian
    Cleanup.  Added color bar key.
    
    1     2000/06/27 8:51p Tian
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __PERF_H__
#define __PERF_H__

#include <revolution/types.h>
#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
  Do not install your own token callback if you are using this API.
  Token ids range from 0xE000 - 0xFFFF.  You may use any other token
  ids.  However, note that the more tokens you use, the more likely
  that tokens will be missed by the PERF library.
 *---------------------------------------------------------------------------*/
/*===========================================================================*
  Types
 *===========================================================================*/
typedef u8  PERFId;


// CPU events show up as red bars 
// CPU-GP events show up as red bars for the CPU component.  The corresponding
// GP time is indicated as a blue bar at the top.  There are connecting lines
// between the CPU bar and the corresponding GP sub-bar.
// GP events show up only as blue bars
typedef enum 
{
    PERF_CPU_EVENT,
    PERF_CPU_GP_EVENT,
    PERF_GP_EVENT
} PerfType;


// Access to the memory allocator of choice
typedef void*(*PERFAllocator)   ( u32 size      );
typedef void (*PERFDeallocator) ( void* block   );

// Use this callback to restore GP draw state to whatever is needed
// The PERF API only uses VTXFMT0.  It changes Zmode,
// texgens, texcoord gen, blend mode.  It restores projection matrix
typedef void (*PERFDrawCallback)( void );

/*===========================================================================*
  Initialization
 *===========================================================================*/

/*---------------------------------------------------------------------------*
    Name:           PERFInit
    
    Description:    Initializes performance spy.
                    
    Arguments:      numSamples:  	total number of samples per "frame"
                                If PERF runs out of samples, the very last one
                                will be used over and over
                    numFrames:   	total number frames worth of data to remember
                    numTypes:    	maximum number of types of events
                    allocator:   	pointer to your allocator of choice
                    deallocator: 	pointer to your deallocator of choice
                    initDraw:    	pointer to function that re-initializes gp
                                for your program to render.  If NULL,
                                performance graphs won't render.
    
    Returns:        returns heap footprint size
*----------------------------------------------------------------------------*/
u32     PERFInit( u32               numSamples,         
                  u32               numFramesHistory,   
                  u32               numTypes,           
                  PERFAllocator     allocator,
                  PERFDeallocator   deallocator,
                  PERFDrawCallback  initDraw);  

/*---------------------------------------------------------------------------*
  Name:         PERFShutDown

  Description:  Disables PERF autosampling, frees all memory, and sets
                the GXDrawSync callback to the user defined one (set
                via PERFSetDrawSyncCallback).  If there is no
                user-defined callback, the token callback will be set
                to NULL (i.e. no token callbacks will be processed).

  Arguments:    None.
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PERFShutDown( void );
    

/*---------------------------------------------------------------------------*
  Name:         PERFSetDrawSyncCallback

  Description:  allows the program to have their own callback called for their
                tokens.  Note that program tokens must be less than 0xE000,
                otherwise they will interfere with profiling.

  Arguments:    cb:   	token callback

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PERFSetDrawSyncCallback(GXDrawSyncCallback cb);
    

/*---------------------------------------------------------------------------*
  Name:         PERFSetEvent

  Description:  Initializes an event type
                
  Arguments:    id:      	the index into the event array
                name:    	constant string describing the event
                type:    	CPU, GP, CPU-GP event
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PERFSetEvent        ( PERFId    id, 
                              char*     name, 
                              PerfType  type );
    
// color only affects CPU events
void    PERFSetEventColor   ( PERFId    id, 
                              GXColor   color );

/*===========================================================================*
  Measurement
 *===========================================================================*/

// if sampling is used, do not use the decrementer on the CPU
void    PERFStartAutoSampling ( f32 msInterval );
void    PERFStopAutoSampling  ( void );

/*---------------------------------------------------------------------------*
  Name:         PERFStartFrame

  Description:  Call at the beginning of every measurement frame.  A 
                measurement frame can encompass multiple video frames.
                Be careful not to call PERFDumpScreen until you have
                called PERFEndFrame.

                Resets sample counts and counters.
                
  Arguments:    None.
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PERFStartFrame  ( void );

/*---------------------------------------------------------------------------*
  Name:         PERFEndFrame

  Description:  Call at the end of every measurement frame.  A 
                measurement frame can encompass multiple video frames.
                Be careful not to call PERFDumpScreen until you have
                called PERFEndFrame.

                Closes any outstanding samples, and takes some last 
                measurements.
                
  Arguments:    None.
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PERFEndFrame    ( void );

/*---------------------------------------------------------------------------*
  Name:         PERFEventStart

  Description:  Indicates an event has begun, and starts measuring.
                This event will not be rendered until PERFEventEnd has
                been called on this event.  If this event is a CPU-GP or GP
                event, then a token will be written to the pipe.

                You cannot call PERFEventStart on an event until 
                PERFEventEnd has been called on it.  You may make multiple
                measurements per frame.

                CPU events can overlap one another, but CPU-GP and GP events
                cannot
                
  Arguments:    id:      	index into event table.
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PERFEventStart  ( PERFId id );

/*---------------------------------------------------------------------------*
  Name:         PERFEventEnd

  Description:  Stops measurements for the current event.
                
  Arguments:    id:      	index into event table.
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PERFEventEnd    ( PERFId id );    


/*===========================================================================*
  Display
 *===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         PERFDumpScreen

  Description:  Draws fancy graphs on the screen:
                1. Saves projection matrix.
                   Sets blend mode, z compare location, numchans = 1,
                   chanctrl, tevorder, tevop, texgens, tevstages, z mode.
                   Invalidates all vertex descriptors.

                2. Renders graphs

                3. Restores projection matrix
                
                4. Calls application-specified draw init function 
                
  Arguments:    None.
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PERFDumpScreen      ( void );
void    PERFPreDraw         ( void );
void    PERFPostDraw        ( void );

void    PERFSetDrawBWBarKey ( BOOL tf );    // enable drawing of on-screen color key
void    PERFSetDrawBWBar    ( BOOL tf );    
void    PERFSetDrawCPUBar   ( BOOL tf );    
void    PERFSetDrawXFBars   ( BOOL tf );    
void    PERFSetDrawRASBar   ( BOOL tf );    

void    PERFSetDrawFrames   ( u32 frames ); // zero means auto scale
    

void    PERFToggleDrawBWBarKey ( void );
void    PERFToggleDrawBWBar    ( void );
void    PERFToggleDrawCPUBar   ( void );
void    PERFToggleDrawXFBars   ( void );
void    PERFToggleDrawRASBar   ( void );

// to be implemented/TBD
//void    PERFInitHostIO  ( void );
//void    PERFDumpHost    ( void );
//void    PERFQueryHost   ( void ); // pull settings from host


//void    PERFMenu        ( void ); 


#ifdef __cplusplus
}
#endif

#endif  // __PERF_H__
