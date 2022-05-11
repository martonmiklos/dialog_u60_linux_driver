#ifndef U50OSAL_H
#define U50OSAL_H
#include "Ldv32.h"
#include "U50Driver.h"
#include "U50Defs.h"
#include "U50Link.h"

pLONVXD_Buffer getUplinkBufferFront(struct U50LinkState *state);
void removeUplinkBufferFront(struct U50LinkState *state);
void addToUplinkBuffer(struct U50LinkState *state, pLONVXD_Buffer data);
size_t getUplinkBufferSize(struct U50LinkState *state);
pLONVXD_Buffer getDownlinkBufferFront(struct U50LinkState *state);
void removeDownlinkBufferFront(struct U50LinkState *state);
void addToDownlinkBuffer(struct U50LinkState *state, pLONVXD_Buffer data);
size_t getDownlinkBufferSize(struct U50LinkState *state);
void* allocateMemory(size_t size);
void freeMemory(void* buf);

#endif

