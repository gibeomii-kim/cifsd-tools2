/*
 * Cryptographic API.
 *
 * MD4 Message Digest Algorithm (RFC1320).
 *
 * Implementation derived from Andrew Tridgell and Steve French's
 * CIFS MD4 implementation, and the cryptoapi implementation
 * originally based on the public domain implementation written
 * by Colin Plumb in 1993.
 *
 * Copyright (c) Andrew Tridgell 1997-1998.
 * Modified by Steve French (sfrench@us.ibm.com) 2002
 * Modified by Namjae Jeon (namjae.jeon@samsung.com) 2015
 * Copyright (c) Cryptoapi developers.
 * Copyright (c) 2002 David S. Miller (davem@redhat.com)
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __MD4_HASH_H__
#define __MD4_HASH_H__

#define MD4_BLOCK_WORDS 16
#define MD4_HASH_WORDS  4

struct md4_ctx {
	unsigned int	hash[MD4_HASH_WORDS];
	unsigned int	block[MD4_BLOCK_WORDS];
	unsigned long	long byte_count;
};

void md4_init(struct md4_ctx *);
void md4_update(struct md4_ctx *, const unsigned char *, unsigned int);
void md4_final(struct md4_ctx *, unsigned char *);

#endif /* __MD4_HASH_H__ */
