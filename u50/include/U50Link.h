#ifndef U50LINK_H
#define U50LINK_H
#include "Ldv32.h"
#include "Timer.h"
#include "platform.h"
#include "U50Driver.h"
#include "U50Defs.h"

#define N_PIG_COUNT		5
#define SMP_MSECS_KA		(1000*5)
#define SMP_MSECS_REJECT	(1000*10)

struct U50LinkState {
	PORT_HANDLE		m_hComPort;
	OsalThreadId	m_hThisThread;
	OsalHandle		m_hClientEvent;
	OsalHandle		m_hReadThreadNotifier;
    OsalHandle		m_hReadMACNotifier;
	ThreadState		m_RTState;
	// LLP states:
	BOOL	    	m_bWaitForMAC;
	BOOL	    	m_bHaveMAC;
	DCMPRXSTATE		m_LlpRxState;
	DCMPTXSTATE		m_LlpTxState;
	WORD			m_NiCmdLocal;
	BOOL			m_UlDuplicate;			// this transfer is a duplicate.
	BOOL			m_UplinkTailEscd;
 	WORD			m_RxSequence;
 	WORD			m_TxSequence;
 	BOOL			m_DownlinkAck;
	int				m_StartupIndex;
	// Various LLP counts:
	WORD			m_UplinkLeftovers;
	WORD			m_NextRxCount;
	WORD			m_RxCountRead;
	WORD			m_LlpRxOffset;
	WORD			m_UplinkLength;
	WORD			m_AckTimeout;
	WORD			m_AckTimeoutStartValue;
	WORD			m_TMOCount;
	WORD			m_AckTimeoutPhase;
	//
	OsalTickCount	m_tCountKeepalive;
	//
	U50OpenMode		m_OpenMode;
	U50_Stats		m_Llpstat;
	BYTE			m_neuronMipVersion;
	BYTE			m_supportsExBuffers;
    BYTE            m_partialLen;
	BYTE			m_Challenge[SIZE_CHALLENGE];
	LONVXD_BufferShort m_DlmsgShort;
	// The ack timer:
	Timer			m_AckTimer;
	// The RX timer
	Timer			m_ReadStartTimer;
	// The message reject timer
	Timer			m_RejectTimer;
	// 
	BYTE			m_PartialLengthBuffer[2];
	// The serial reader buffer
	BYTE			m_UplinkBuffer[SIZE_ULB];
	// A pointer to the 'start' of the current packet in <UplinkBuffer>
	BYTE			*m_pULB;
	// A pointer to the current downlink message
	pLONVXD_Buffer	m_pDownlinkMsg;
	// message is built here before queueing.
	LONVXD_Buffer	m_UplinkMsg;
	pLONVXD_Buffer	m_UplinkLvdp;
	WORD			m_ExtUplinkLength;	// extended length
	BYTE			m_UplinkCsBase;		// 0, or extended length fields
	// Current pointer into above:
	void			*m_UplinkReadPtr;
	// 
	BOOL			m_bFreeDownlinkMsg;
	//
	BYTE			m_expectedResponse;
	BYTE			m_downlinkRequest[CpMsgQueueCount];	// 
    BYTE            m_mac_id[MAX_ADDR_LEN];
	// Queues:
	t_UDlist		Ulist;	// Uplink list
	t_UDlist		Dlist;	// Downlink list
	DcmpOptions		m_Options;
    DWORD           m_DevIndex;     // [0] based device instance
#ifdef U50_KERNEL
	void* priv; //pointer to private driver memory - link struct belongs to this chunk
#endif
};

// Access methods
void U50LinkInit(struct U50LinkState *state);
short U50LinkStart(struct U50LinkState *state, PORT_HANDLE hComPort, DWORD baudrate, U50OpenMode mode);
short U50LinkRead(struct U50LinkState *state, pLDV_Message pMsg, int iSize);
short U50LinkWrite(struct U50LinkState *state, const pLDV_Message pMsg);
short U50LinkRegisterEvent(struct U50LinkState *state, OsalHandle hClientEvent);
int	U50LinkGetStats(struct U50LinkState *state, pU50_Stats ps);
void U50LinkShutdown(struct U50LinkState *state);

int CalLowerReadCount(struct U50LinkState *state);
DWORD CalLowerRead(struct U50LinkState *state, PVOID pDest, WORD Count, PWORD pActual);
// Experience shows that ERROR_IO_PENDING is the norm.
void CalLowerWrite(struct U50LinkState *state, PVOID OutputBuffer, ULONG OutputBufferLength);
WORD CalLowerGetOvlr(struct U50LinkState *state);

#endif
