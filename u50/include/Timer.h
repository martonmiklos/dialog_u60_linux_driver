//
// Timer.h
//
// Copyright (c) 2004 Echelon Corporation.  All Rights Reserved.
//                          ** ECHELON CONFIDENTIAL **
//
// This file contains definitions used for timers.
//

#ifndef _TIMER_H
#define _TIMER_H

#include "platform.h"
// Time related but not related to timers below
typedef struct
{
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	BYTE second;
} LTimeDate;

// The timer macros hide some implementation specific aspects
// of using the timer directly.  A timer is started and then can
// be checked against various time out values until it is explicitly stopped.
// A timer has a fundamental limit of 10.9 minutes (65,536/100 seconds).
//
// For example,
//
//		// Define a timer.
//		Timer myTimer;
//
//		// Start the timer.
//		TMR_Start(myTimer);
//
//		// Check if the timer is still running
//		if (TMR_Running(myTimer))
//		{
//			// Do something while timer is running.
//		}
//
//		// Check if the timer has been running for at least 2 seconds.
//		// Note that expiration does not cause the timer to stop running.
//		// You just stop it explicitly.
//		if (TMR_Elapsed(myTimer, 2000))
//		{
//			// Do something based on timer running for 2 seconds.
//
//			// And now stop the timer.
//			TMR_Stop(myTimer);
//		}
//
typedef struct
{
	BOOL  bRunning;
	unsigned long startValue;
} Timer;

#define TMR_Current (OsalGetTickCount())

// Use this to start milliseconds "timer"
#define TMR_Start(timer)		\
	timer.bRunning = TRUE; timer.startValue = TMR_Current;

#define TMR_Stop(timer) 		\
	timer.bRunning = FALSE;

#define TMR_Running(timer) 	\
	timer.bRunning

// Use this to check "timer" for "duration" milliseconds having elapsed since starting
#define TMR_Expired(timer, duration) \
	(!timer.bRunning || (TMR_Current - timer.startValue) > duration)

// Same as TMR_Expired but also makes sure the timer is running.
#define TMR_Elapsed(timer, duration)	\
	(timer.bRunning && TMR_Expired(timer, duration))

// Replaced with TimerRemaining to fix the issue where there was a tiny window  
// where the TMR_Current would change during the logic of the original macro	
#define TMR_Remaining(timer, duration)	TimerRemaining(timer, duration)

#endif
