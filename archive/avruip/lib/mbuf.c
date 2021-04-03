
#include <string.h>

#include "mbuf.h"


#define POOL_SIZE	128 				// amount of memory buffers to allocate. (default 128, max 256)


static struct mbuf mbuf_pool[POOL_SIZE];	// array of memory buffers
static struct mbuf *next_free;		// pointer to next free mbuf address

/*********************************************************

Initialise the memory buffers

Entry: Nothing
Returns: Nothing

*********************************************************/

void mbuf_init(void)
{
  uint8_t i;

  for (i = 0; i < POOL_SIZE; i++)		// loop through the memory buffers
  {
    struct mbuf *m = &mbuf_pool[i]; 	// set m as a pointer to the next memory buffer
    mbuf_free(m);		    			// flag the memory buffer as free
  }
}

/*********************************************************

Allocate a memory buffer.
Returns the next free mbuf

Entry: Passes a pointer to a memory buffer
Returns: pointer to mbuf or NULL

*********************************************************/

struct mbuf *mbuf_alloc(void) __attribute__ ((noinline));

struct mbuf *mbuf_alloc(void)
{
  struct mbuf *m = next_free;			// get the next free mbuf

  if (m)						// set the next free mbuf
  {
    struct mbuf *next = m->next;		// set up a temp pointer to hold the next mbuf
    if (next)
      next->prev = NULL;			// if valid set the previous to NULL

    next_free = next;				// set the next free
    m->next = NULL;				// set the next in the chain to NULL
  }
  return m;
}


/*********************************************************
*
* Free a memory buffer.
* Sets this mbuf as the next free. 
* 
* Entry: Passes a pointer to a memory buffer
* Returns: Nothing
*
*********************************************************/

void mbuf_free(struct mbuf *m) __attribute__ ((noinline));

void mbuf_free(struct mbuf *m)
{
  m->next = next_free;				// set to the next free
  m->prev = NULL;					// set previous to NULL
  if (next_free)					// if free list not null (ie. not the first)
    next_free->prev = m;			// this now becomes the preceding buffer

  m->length = 0;					// set length = 0
  next_free = m;					// next free is now this mbuf
}


/*********************************************************

Frees a chain of memory buffers.

Entry: Passes a pointer to a memory buffer
Returns: Nothing

*********************************************************/

void mbuf_free_chain(struct mbuf *m)
{
  while (m)
  {
    struct mbuf *next = m->next;		// save the next mbuf address
    mbuf_free (m);				// clear the mbuf
    m = next;					// set as the next in the chain
  }
}

/*********************************************************

Copy data into memory buffers.

Entry: data in buffer, length of data
Returns: pointer to the first mbuf in the chain, or NULL

*********************************************************/

struct mbuf *copy_to_mbufs(uint8_t *buffer, uint16_t length)
{
  struct mbuf *first = NULL, *last = NULL;// set up pointers to an mbuf for the first and last
							// mbufs in the chain

  while (length)					// while there is still data to process
  {
    struct mbuf *m = mbuf_alloc ();		// allocate an mbuf

    if (m == NULL)				// if no more mbufs are available whilst copying
    {							// clear the chain
      // Out of memory
      mbuf_free_chain (first);
			// sends chain to be cleared

      return NULL;
    }

    if (first == NULL)				// first mbuf in chain?
    {
      first = m;
    }
    else
    {
      m->prev = last;				// set the previous pointer to the previous mbuf
      last->next = m;				// set the previous mbuf next pointer
    }

    last = m;					// set this as the last one in the chain

    uint8_t to_copy = MBUF_SIZE;		// set length to the max data length
    if (to_copy > length)			// is more data than 1 buffer?
      to_copy = length;				// yes, then set to data length

    memcpy (m->data, buffer, to_copy);	// copy the data in the buffer to the mbuf
    m->length = to_copy;			// set the length of data in the mbuf
    buffer += to_copy;				// increase the buffer pointer start by amount copied
    length -= to_copy;				// set the amount of data still to copy
  }
  return first;
}
