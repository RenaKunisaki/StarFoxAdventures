/*---------------------------------------------------------------------------*
  Project:  Host I/O API for EXI-USB
  File:     hio2.h

  (C)2005 HUDSON SOFT

  $Header: /home/cvsroot/SDK/include/revolution/hio2.h,v 1.2 2006/03/09 12:28:43 yasuh-to Exp $

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	__HIO2_H__
#define	__HIO2_H__

#ifdef	WIN32	// PC include here

#   include <windows.h>
#   include <win32/dolphin.types.h>

#	ifdef HIO2API_DLL_EXPORTS
#		define	HIO2API __declspec(dllexport)
#	else
#		define	HIO2API __declspec(dllimport)
#	endif

#else	// WIN32

#   include <dolphin/types.h>

#	define	HIO2API 

#endif	// WIN32

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// define

//---- EXI-USB shared memory size
#define	HIO2_BUFFER_SIZE	(8 * 1024)

//---- status 
#define HIO2_STATUS_TX	0x00000001
#define HIO2_STATUS_RX	0x00000002

//---- EXI channel number
enum
{
	HIO2_CHAN_INVALID = -1,
	HIO2_CHAN_0,		// EXI0
	HIO2_CHAN_1,		// EXI1
	HIO2_CHAN_MAX
};

//---- invalid handle
#define	HIO2_INVALID_HANDLE_VALUE	((HIO2Handle)HIO2_CHAN_INVALID)

//-----------------------------------------------------------------------------
// type definition

#ifdef	WIN32

//---- HIO2 handle
typedef LPVOID	HIO2Handle;

//----- HIO2 device (PC only)
#ifndef	HW0
typedef	LPCSTR	HIO2DevicePath;
#else	// HW0
typedef int		HIO2DevicePath;
#	endif	// HW0

#else	// WIN32
//---- HIO2 handle
typedef s32		HIO2Handle;
#endif	// WIN32

//---- HIO2 device type (NNGC only)
typedef enum
{
	HIO2_DEVICE_INVALID = -1,
	HIO2_DEVICE_EXI2USB_0,		// EXI-USB EXI0
	HIO2_DEVICE_EXI2USB_1,		// EXI-USB EXI1
	HIO2_DEVICE_MrEXI			// Mr.EXI-USB
} HIO2DeviceType;

//---- notify event (PC only)
typedef enum
{
	HIO2_NOTIFY_INIT_REQUEST = 1,
	HIO2_NOTIFY_EXIT_DONE
} HIO2NotifyEvent;

//----- error code
typedef enum
{
	HIO2_ERROR_NONE = 0,			// no error
	HIO2_ERROR_UNINITIALIZED,		// HIO2 is not initialized
	HIO2_ERROR_INVALID_HANDLE,		// invalid HIO2 handle
	HIO2_ERROR_INVALID_PARAMETER,	// invalid function parameter
	HIO2_ERROR_CHANNEL_BUSY,		// channel already used
	HIO2_ERROR_CHANNEL_FULL,		// all channels are used
	HIO2_ERROR_CANNOT_USE,			// this console cannot use HIO2
	HIO2_ERROR_NOT_FIND,			// EXI-USB not find
	HIO2_ERROR_INTERNAL				// internal error (EXI error)
} HIO2Error;

//---- callback prototypes

// enum EXI device
#ifdef	WIN32
typedef BOOL	(*HIO2EnumCallback)( HIO2DevicePath path, void* param );
#else	// WIN32
typedef BOOL	(*HIO2EnumCallback)( HIO2DeviceType type );
#endif	// WIN32

// receive mail
typedef void	(*HIO2ReceiveCallback)( HIO2Handle h );

// ReadAsync/WriteAsync done
typedef void	(*HIO2DmaCallback)( HIO2Handle h );

// disconnect EXI device (NNGC only)
typedef void	(*HIO2DisconnectCallback)( HIO2Handle h );

// notify (PC only)
typedef int		(*HIO2NotifyCallback)( HIO2NotifyEvent event, void* param );

//-----------------------------------------------------------------------------
// function prototypes

HIO2API BOOL		HIO2Init			( void );

#ifdef	WIN32

HIO2API s32			HIO2EnumDevices		( HIO2EnumCallback callback, void* param );
HIO2API HIO2Handle	HIO2Open			( HIO2DevicePath pathName, HIO2ReceiveCallback callback, HIO2NotifyCallback notify, void* param );
#else	// WIN32

BOOL			HIO2EnumDevices			( HIO2EnumCallback callback );
HIO2Handle		HIO2Open				( HIO2DeviceType type, HIO2ReceiveCallback callback, HIO2DisconnectCallback disconnect );
HIO2DeviceType	HIO2GetDeviceType		( HIO2Handle h );

#endif	// WIN32

HIO2API BOOL		HIO2Close			( HIO2Handle h );
HIO2API BOOL		HIO2ReadMailbox		( HIO2Handle h, u32* mail );
HIO2API BOOL		HIO2WriteMailbox	( HIO2Handle h, u32  mail );
HIO2API BOOL		HIO2Read			( HIO2Handle h, u32 addr, void* buffer, s32 size );
HIO2API BOOL		HIO2Write			( HIO2Handle h, u32 addr, const void* buffer, s32 size );
HIO2API BOOL		HIO2ReadAsync		( HIO2Handle h, u32 addr, void* buffer, s32 size, HIO2DmaCallback callback );
HIO2API BOOL		HIO2WriteAsync		( HIO2Handle h, u32 addr, const void* buffer, s32 size,
										  HIO2DmaCallback callback );
HIO2API BOOL		HIO2ReadStatus		( HIO2Handle h, u32* status );
HIO2API void		HIO2Exit			( void );
HIO2API HIO2Error	HIO2GetLastError	( void );

#ifdef	WIN32

//-----------------------------------------------------------------------------
// EXPORT function (for hio2[D].dll)

//---- type definition
typedef BOOL		(*HIO2InitType)			( void );
typedef s32			(*HIO2EnumDevicesType)	( HIO2EnumCallback callback, void* param );
typedef HIO2Handle	(*HIO2OpenType)			( HIO2DevicePath pathName, HIO2ReceiveCallback callback, HIO2NotifyCallback notify, void* param);
typedef BOOL		(*HIO2CloseType)		( HIO2Handle h );
typedef BOOL		(*HIO2ReadMailboxType)	( HIO2Handle h, u32* mail );
typedef BOOL		(*HIO2WriteMailboxType)	( HIO2Handle h, u32  mail );
typedef BOOL		(*HIO2ReadType)			( HIO2Handle h, u32 addr, void* buffer, s32 size );
typedef BOOL		(*HIO2WriteType)		( HIO2Handle h, u32 addr, const void* buffer, s32 size );
typedef BOOL		(*HIO2ReadAsyncType)	( HIO2Handle h, u32 addr, void* buffer, s32 size, HIO2DmaCallback callback );
typedef BOOL		(*HIO2WriteAsyncType)	( HIO2Handle h, u32 addr, const void* buffer, s32 size, HIO2DmaCallback callback );
typedef BOOL		(*HIO2ReadStatusType)	( HIO2Handle h, u32* status );
typedef void		(*HIO2ExitType)			( void );
typedef HIO2Error	(*HIO2GetLastErrorType)	( void );

//---- string (EXPORT function name) definition
#define HIO2InitStr			"HIO2Init"
#define HIO2EnumDevicesStr	"HIO2EnumDevices"
#define HIO2OpenStr			"HIO2Open"
#define HIO2CloseStr		"HIO2Close"
#define HIO2ReadMailboxStr	"HIO2ReadMailbox"
#define HIO2WriteMailboxStr	"HIO2WriteMailbox"
#define HIO2ReadStr			"HIO2Read"
#define HIO2WriteStr		"HIO2Write"
#define HIO2ReadAsyncStr	"HIO2ReadAsync"
#define HIO2WriteAsyncStr	"HIO2WriteAsync"
#define HIO2ReadStatusStr	"HIO2ReadStatus"
#define HIO2ExitStr			"HIO2Exit"
#define HIO2GetLastErrorStr	"HIO2GetLastError"

#endif	// WIN32

#ifdef __cplusplus
}
#endif

#endif	// __HIO2_H__
