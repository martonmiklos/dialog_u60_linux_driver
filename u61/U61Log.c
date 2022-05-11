


#include <linux/module.h>       /* Needed by all modules */
#include <linux/kernel.h>       /* Needed for KERN_INFO */
#include <linux/init.h>         /* Needed for the macros */


//////////////////////////////////////////////////////////////////////
// Debug output
//////////////////////////////////////////////////////////////////////

// #define LDDebugOut(format,arg...) vprintk(format, ##arg)
#define U50_LOG 0
static const char szcPrepend[] = "U61 ";

void LDDebugOut(const char *cLevel, const char* fmt, va_list args)
{
	char szBuffer[128];
	size_t preSize;
	strcpy(szBuffer, cLevel);
	strcat(szBuffer, szcPrepend);
	preSize = strlen(szBuffer);
	vsnprintf(&szBuffer[preSize], sizeof(szBuffer)-preSize-2, fmt, args);
	strcat(szBuffer, "\n");
	printk(szBuffer);
}

void LDDebugError(char* szFormat, ... )
{
	va_list args;
	va_start(args, szFormat);
	LDDebugOut(KERN_ERR, szFormat, args);
	va_end(args);
}

void LDDebugInform(char* szFormat, ... )
{
	va_list args;
	va_start(args, szFormat);
	LDDebugOut(KERN_INFO, szFormat, args);
	va_end(args);
}


void LDDebugTracePacket(char* szType, uint8_t *mp, int length)
{
#if U50_LOG
	print_hex_dump_bytes(szType, DUMP_PREFIX_OFFSET, mp, length);
#endif
}

void LDDebugUplinkMessage(uint8_t bIfUplink, const uint8_t* mp)
{
#if U50_LOG

#endif
}
