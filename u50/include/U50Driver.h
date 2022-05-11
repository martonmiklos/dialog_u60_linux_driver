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
	U50_OPEN_DEFAULT,		// Affect no change
	U50_OPEN_LAYER5,		// Enforce L5
	U50_OPEN_LAYER2,		// Enforce L2
	U50_OPEN_PA,			// Enforce PA, currently same as L2
	U50_OPEN_UNK = -1
} U50OpenMode;

typedef struct
{
	DWORD Size;         // set to sizeof(U50_Stats);
	DWORD AckTMOs;      // # of Ack timeouts
	DWORD RxTmos;       // # of receive timeouts
	DWORD CsumErrors;   // # of uplink checksum errors
	DWORD CpFails;      // # uplink CpFail rx'd (implies downlink cs error)
	DWORD ULDuplicates; // # of duplicates sensed
	DWORD UlDiscarded;  // # of tossed uplinks
    DWORD MsgRejects;   // # CpMsgReject received
	WORD  MipVersion;
	WORD  MipMode;
    DWORD ReferenceCount;
} U50_Stats, *pU50_Stats;

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

