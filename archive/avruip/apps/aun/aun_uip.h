#ifndef AUN_UIP_H
#define AUN_UIP_H

/* Since this file will be included by uip.h, we cannot include uip.h
   here. But we might need to include uipopt.h if we need the u8_t and
   u16_t datatypes. */

struct aun_state {
  struct uip_udp_conn *conn;
  struct uip_udp_conn *wanconn; 
  uint16_t handle;
};

typedef struct aun_state uip_udp_appstate_t;

/* Finally we define the application function to be called by uIP. */
void aun_appcall(void);

#ifndef UIP_UDP_APPCALL
#define UIP_UDP_APPCALL aun_appcall
#endif /* UIP_APPCALL */

//dummy for tcp
#ifndef UIP_APPCALL
#define UIP_APPCALL aun_appcall
#endif /* UIP_APPCALL */

#endif
