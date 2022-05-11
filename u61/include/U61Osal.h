/*
 *
 */

#pragma once

#include "Ldv32.h"
#include "U61Driver.h"
#include "U61Defs.h"
#include "U61Link.h"

pLONVXD_Buffer getUplinkBufferFront(struct U61LinkState *state);
void removeUplinkBufferFront(struct U61LinkState *state);
void addToUplinkBuffer(struct U61LinkState *state, pLONVXD_Buffer data);
size_t getUplinkBufferSize(struct U61LinkState *state);
pLONVXD_Buffer getDownlinkBufferFront(struct U61LinkState *state);
void removeDownlinkBufferFront(struct U61LinkState *state);
void addToDownlinkBuffer(struct U61LinkState *state, pLONVXD_Buffer data);
size_t getDownlinkBufferSize(struct U61LinkState *state);
void* allocateMemory(size_t size);
void freeMemory(void* buf);


