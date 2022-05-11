///////////////////////////////////////////////////////////////////////////////
//
// $Header: //depot/Software/IotRouter/dev/U50Module/include/U50Driver.h#3 $
//
// � Copyright 2013-2013, Echelon Corp.  All rights reserved.
//
// Prototype definitions for both the DLL and client apps.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Ldv32.h"
#include "platform.h"

//////////////////////////////////////////////////////////////////////
// Open() mode enumerations
typedef enum {
	U61_OPEN_DEFAULT,		// Affect no change
	U61_OPEN_LAYER5,		// Enforce L5
	U61_OPEN_LAYER2,		// Enforce L2
	U61_OPEN_PA,			// Enforce PA, currently same as L2
	U61_OPEN_UNK = -1
} U61OpenMode;

typedef struct
{
	DWORD Size;         // set to sizeof(U61_Stats);
	DWORD		FrameErrors;
	DWORD		TmoErrors;
	DWORD		WriteErrors;
	BOOL		bDeadDevice;
	DWORD		ReferenceCount;		// open() reference count
	int			Txid;				// reported on NI reset
	int			L2L5Mode;			// ditto
} U61_Stats, *pU61_Stats;

//////////////////////////////////////////////////////////////////////
// Extended message alternate structure:
#define EXT_LENGTH	0xFF	// extended message length indicator.
typedef struct LDV_ExtMessage
{
	BYTE NiCmd;				// Network Interface Command
	BYTE ExtFlag;			// will be set to EXT_LENGTH
	WORD ExtLength;
	BYTE ExpAppMessage[1];	// size is based on ExtLength
} LDV_ExtMessage, *pLDV_ExtMessage;

#ifdef windows
#ifdef U61_EXPORTS
#define U61DllExport   extern "C" __declspec( dllexport )
#else
#define U61DllExport   extern "C" __declspec( dllimport )
#endif
#else
#ifdef  __cplusplus
#define U61DllExport extern "C"
#else
#define U61DllExport
#endif
#endif

U61DllExport LDVCode U61Close(IN USHORT iDevIndex);
U61DllExport LDVCode U61Open(IN LPCSTR szComPort, OUT USHORT *piDevIndex, BOOL bFindOnly, IN U61OpenMode mode);
U61DllExport LDVCode U61RegisterEvent(USHORT iDevIndex, HANDLE hEvent);
U61DllExport LDVCode U61Read(IN USHORT iDevIndex, OUT void *pMsg, IN int iSize);
U61DllExport LDVCode U61Write(IN USHORT iDevIndex, IN const void *pMsg);
U61DllExport LPCSTR U61GetVersion(void);
U61DllExport int U61Statistics(IN USHORT iDevIndex, IN OUT pU61_Stats ps);
// U61DllExport LDVCode U61RegisterHwnd(USHORT iDevIndex, HWND hWnd, WPARAM wParam, LPARAM lParam);
