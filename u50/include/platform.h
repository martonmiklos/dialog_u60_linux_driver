#pragma once
#ifdef WINDOWS
typedef HANDLE PORT_HANDLE;
#define CloseThreadHandle(HANDLE) CloseHandle(HANDLE)
#endif
#include "Osal.h"
#ifdef linux
#define EXCLUDE_STLPORT_HELPERS
#ifdef U50_KERNEL
#include <linux/delay.h>
#else
#include <time.h>
#endif

#define FALSE 0
#define TRUE 1
//#define FALSE false
//#define TRUE true

typedef int PORT_HANDLE;
//Yanked from windows headers
#define far
#define near
#define FAR                 far
#define NEAR                near
#ifndef CONST
#define CONST               const
#endif

#ifdef U50_KERNEL
#include <linux/wait.h>

typedef struct {
	wait_queue_head_t queue;
	uint8_t flag;
} OsalEvent;
#endif

typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef void				*PVOID;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef unsigned int UINT_PTR;
typedef long LONG_PTR;
typedef unsigned long ULONG_PTR;

typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        };

        PVOID Pointer;
    };

    OsalHandle  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

#define ERROR_SUCCESS                    0L
#define ERROR_INVALID_HANDLE             6L
#define ERROR_IO_PENDING                 997L 

#define WAIT_FAILED ((DWORD)0xFFFFFFFF)
#define WAIT_OBJECT_0  0

#define INFINITE            0xFFFFFFFF

#define WINAPI /**/

#define LPSECURITY_ATTRIBUTES void* 
#define CloseHandle close
#define INVALID_HANDLE_VALUE -1
#define sprintf_s(format, size, args ...) sprintf(format, args)
#define OutputDebugString(...)
#ifdef U50_KERNEL
#define Sleep(time) msleep(time)
#else
#define Sleep(time) usleep(time*1000)
#endif
#define CloseThreadHandle(HANDLE)

typedef void *(*LPTHREAD_START_ROUTINE)(void *pParameter);

int GetLastError(void);
#endif
