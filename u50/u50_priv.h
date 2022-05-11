/*
 *
 */

#pragma once

#include <linux/spinlock.h>

#include <linux/netdevice.h>
#include <linux/tty.h>
#include "U50Link.h"

#define U50_PRIV_BUF_SIZE 150
struct u50_priv {
	struct U50LinkState state;
	spinlock_t lock;
	struct tty_struct *tty;
	struct net_device* dev;
	unsigned char *rbuff;
	int rcount;
	unsigned char *xbuff;
	unsigned char *xhead;
	int			 			xleft;		// why was this 8 bits?
//	unsigned char			xleft;
	int buffsize;
};

void u50_bump(struct u50_priv *priv, uint8_t *buf, int count);

#define get_priv(state) container_of(state, struct u50_priv, state)
