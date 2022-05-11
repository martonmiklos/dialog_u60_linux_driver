/*
 *
 */

#pragma once

#include <linux/spinlock.h>

#include <linux/netdevice.h>
#include <linux/tty.h>
#include "U61Link.h"

#define U61_PRIV_BUF_SIZE 150			// N.A. never referenced
struct u61_priv {
	struct U61LinkState		state;
	spinlock_t 				lock;
	struct tty_struct 		*tty;
	struct net_device* 		dev;
	unsigned char 			*rbuff;
	int 					rcount;
	unsigned char 			*xbuff;
	unsigned char 			*xhead;
	int			 			xleft;		// why was this 8 bits?
//	unsigned char			xleft;
	int 					buffsize;
};

void u61_bump(struct u61_priv *priv, uint8_t *buf, int count);

#define get_priv(state) container_of(state, struct u61_priv, state)
