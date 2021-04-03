/*****************************************************************************
*  "A Very Simple Application" from the uIP 0.9 documentation
*****************************************************************************/

#include "app.h"

struct simples_state {
	char *dataptr;
	unsigned int dataleft;
};

void simples_init(void) {
	uip_listen(HTONS(80));
	uip_listen(HTONS(81));
}

void simples_app(void) {
	struct example1_state *s;
	s = (struct example1_state)uip_conn->appstate;

	if(uip_connected()) {
		switch(uip_conn->lport) {
		case HTONS(80):
			s->dataptr = data_port_80;
			s->dataleft = datalen_port_80;
			break;
		case HTONS(81):
			s->dataptr = data_port_81;
			s->dataleft = datalen_port_81;
			break;
		}
		uip_send(s->dataptr, s->dataleft);
		return;
	}

	if(uip_acked()) {
		if(s->dataleft < uip_mss()) {
		uip_close();
		return;
		}
		s->dataptr += uip_conn->len;
		s->dataleft -= uip_conn->len;
		uip_send(s->dataptr, s->dataleft);
	}
}
