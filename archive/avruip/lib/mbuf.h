#ifndef MBUF_H
#define MBUF_H

#include <stdint.h>
#include <stdlib.h>

#define MBUF_SIZE	128		// size of data in a mbuf (default 128, max 256)

struct mbuf
{
  struct mbuf *next, *prev;		// mbuf pointer next, previous
  uint8_t length;			// length of data
  uint8_t pad;				// pad byte
  uint8_t data[MBUF_SIZE];		// data size as declared (default 128)
};

void mbuf_init(void);
extern struct mbuf *mbuf_alloc(void);
extern void mbuf_free(struct mbuf *);
extern void mbuf_free_chain(struct mbuf *);
extern struct mbuf *copy_to_mbufs(uint8_t *data, uint16_t length);

#endif
