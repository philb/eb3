 /*
 * Copyright (c) 2003, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: shell.c,v 1.11 2010-01-23 13:26:04 markusher Exp $
 *
 */

#include "shell.h"

#include "adlc.h"
#include "eeprom.h"

#include <stdlib.h>
#include <string.h>

struct ptentry {
  char *commandstr;
  void (* pfunc)(char *str);
};

#define SHELL_PROMPT "> "


#define xtod(c) ((c>='0' && c<='9') ? c-'0' : ((c>='A' && c<='F') ? \
                c-'A'+10 : ((c>='a' && c<='f') ? c-'a'+10 : 0))) 
  

/*---------------------------------------------------------------------------*/
static void
parse(register char *str, struct ptentry *t)
{

  struct ptentry *p;

  for(p = t; p->commandstr != NULL; ++p) {
    if(strncmp(p->commandstr, str, strlen(p->commandstr)) == 0) {
      break;
    }
  }

  p->pfunc(str);

}
/*---------------------------------------------------------------------------*/
/*
static void
copy_param(uint8_t *location, char *strvalue, uint8_t MIN, uint8_t MAX)
{

	int value;

	value = atoi(strvalue);
	if ((value <=MAX) && (value >= MIN )) {
	  *location = value;
	  EEPROM_WriteAll();
        }
	else
	{
          shell_output("Invalid value ", "");
        }


}
*/
/*---------------------------------------------------------------------------*/
/*
static void
copy_param_ip(uint8_t *location, char *strvalue)
{
  *(location) = atoi(strtok(strvalue, "."));
  *(location+1) = atoi(strtok(NULL, "."));
  *(location+2) = atoi(strtok(NULL, "."));
  *(location+3) = atoi(strtok(NULL, " "));
  EEPROM_WriteAll();
}
*/

char xtode(char c) 
{ 
  if (c>='0' && c<='9') return c-'0'; 
  if (c>='A' && c<='F') return c-'A'+10; 
  if (c>='a' && c<='f') return c-'a'+10; 
  return c=0;        // not Hex digit 
} 
  
int HextoDec(char *hex, int l) 
{ 
  if (*hex==0) return(l); 
  return HextoDec(hex+1, l*16+xtode(*hex)); // hex+1? 
} 
  
int xstrtoi(char *hex)      // hex string to integer 
{ 
  return HextoDec(hex, 0); 
} 

/*---------------------------------------------------------------------------*/
/*
static void
copy_param_mac(uint8_t *location, char *strvalue)
{

  *(location) = xstrtoi(strtok(strvalue, ":"));

  uint8_t i;
  for (i=1; i<5; i++) {
  *(location+i) = xstrtoi(strtok(NULL, ":"));
  }

  *(location+5) = xstrtoi(strtok(NULL, " "));
  EEPROM_WriteAll();

}
*/
/*---------------------------------------------------------------------------*/
static void
inttostr(char *str, uint8_t i)
{
  str[0] = '0' + i / 100;
  if(str[0] == '0') {
    str[0] = ' ';
  }
  str[1] = '0' + (i / 10) % 10;
  if(str[0] == ' ' && str[1] == '0') {
    str[1] = ' ';
  }
  str[2] = '0' + i % 10;
  str[3] = ' ';
  str[4] = 0;
}

#define hexdigit(x)	(((x) < 10) ? (x) + '0' : (x) + 'a' - 10)

static void
hextostr(char *str, uint8_t i)
{
  str[0] = hexdigit(i >> 4);
  str[1] = hexdigit(i & 0xf);
}

static void
iptostr(uint8_t *ipaddress, char *str)   
{
  char outstring[4];
  char period[1];

  period[0] = '.';

  str[0] = 0;

  itoa(ipaddress[0], outstring, 10);
  strcat(str, outstring);
  itoa(ipaddress[1], outstring, 10);
  strcat(str, period);
  itoa(ipaddress[2], outstring, 10);
  strcat(str, period);
  itoa(ipaddress[3], outstring, 10);
  strcat(str, period);

}
/*---------------------------------------------------------------------------*/
static void
help(char *str)
{
//  shell_output("set clck [0-4]", "");
//  shell_output("\nset sttn [1-254]\r\nset enet [1-127]\r\nset aunn [1-251]\r\nset ipad [a.b.c.d]", "");
//  shell_output("\r\nset snet [a.b.c.d]\r\nset gway [a.b.c.d]\r\nset maca [a:b:c:d:e:f]\r\nset ecip [a.b.c.d]\r\nset ecsb [a.b.c.d]\r\nset ewan [a.b.c.d]\n", "");

//  shell_output("stats   - show network statistics", "");
  shell_output("config  - show configuration\r\nhelp, ? - show help\r\nexit    - exit shell", "");
}
/*---------------------------------------------------------------------------*/
static void
setvalue(char *str)
{
/*
   if(strncmp(str+4, "sttn", 4)==0){
//	strlcpy(strvalue, str+10, strlen(str));
        copy_param(&eeprom.Station, str+9, 1, 254);
  }else if(strncmp(str+4, "clck", 4)==0){
        copy_param(&eeprom.ClockMultiplier, str+9, 0, 4); 
    }else if(strncmp(str+4, "enet", 4)==0){
        copy_param(&eeprom.Econet_Network, str+9, 1, 127);
    }else if(strncmp(str+4, "aunn", 4)==0){
        copy_param(&eeprom.Ethernet_Network, str+9, 1, 251);
    }else if(strncmp(str+4, "ipad", 4)==0){
        copy_param_ip(&eeprom.IPAddr, str+9);
    }else if(strncmp(str+4, "snet", 4)==0){
        copy_param_ip(&eeprom.Subnet, str+9);
    }else if(strncmp(str+4, "gway", 4)==0){
        copy_param_ip(&eeprom.Gateway, str+9);
    }else if(strncmp(str+4, "maca", 4)==0){
        copy_param_mac(&eeprom.MAC, str+9);
    }else if(strncmp(str+4, "ecip", 4)==0){
        copy_param_ip(&eeprom.EconetIP, str+9);
    }else if(strncmp(str+4, "ecsb", 4)==0){
        copy_param_ip(&eeprom.EconetMask, str+9);
    }else if(strncmp(str+4, "ewan", 4)==0){
        copy_param_ip(&eeprom.WANRouter, str+9);
    }else{
      shell_output("Unknown command: ", str+4);
    }
*/
}
/*---------------------------------------------------------------------------*/

static void
stats(char *str)
{
/*
  char outstring[4];
  inttostr(&outstring,0);

  shell_output("BEE Statistics", "\n");
  shell_output("Econet", "");
  shell_output("======", "\n");
  shell_output("Frames in        : ", outstring);
  shell_output("Short scouts     : ", outstring);
  shell_output("Rx Broadcasts    : ", outstring);
  shell_output("Tx Attempts      : ", outstring);
  shell_output("Tx Collisions    : ", outstring);
  shell_output("Tx Not Listening : " , outstring);
  shell_output("Tx Net Error     : " , outstring);
  shell_output("Tx Line Jammed   : " , outstring);
  shell_output("\n\nhelp, ? - show help", "");
  shell_output("exit    - exit shell", "");
*/
}/*---------------------------------------------------------------------------*/
static void
config(char *str)
{
  char outstring[5];
  char addr[18];

  uint8_t i;
  for (i = 0; i < 6; i++) {
    hextostr(addr + (i * 3), eeprom.MAC[i]);
    addr[((i*3)+2)] = ':';
  }
  addr[17] = 0;


  inttostr(outstring,eeprom.Econet_Network);

  shell_output("\nEconet\r\n======\r\n\nNetwork\t\t: ", outstring);

  inttostr(outstring,eeprom.Station);
  shell_output("Station\t\t: ", outstring);
/*
  inttostr(outstring,eeprom.ClockMultiplier);
  shell_output("Clock x\t\t: ", outstring);
*/

  inttostr(outstring,eeprom.Ethernet_Network);
  shell_output("AUN Network\t: ", outstring);

  shell_output("\nEthernet\r\n========\r\n\nMAC Address\t: ", addr);

  iptostr(eeprom.IPAddr, addr);
  shell_output("IP Address\t: ", addr);

  iptostr(eeprom.Subnet, addr);
  shell_output("Subnet\t\t: ", addr);

  iptostr(eeprom.Gateway, addr);
  shell_output("Gateway\t\t: ", addr);

  iptostr(eeprom.EconetIP, addr);
  shell_output("\nEconet WAN\r\n==========\r\n\nIP Address\t: ", addr);

  iptostr(eeprom.EconetMask, addr);
  shell_output("Subnet\t\t: ", addr);

  iptostr(eeprom.WANRouter, addr);
  shell_output("Gateway\t\t: ", addr);

}/*---------------------------------------------------------------------------*/
static void
unknown(char *str)
{
  if(strlen(str) > 0) {
    shell_output("Unknown command: ", str);
  }
}
/*---------------------------------------------------------------------------*/
static struct ptentry parsetab[] =
  {{"config", config},
   {"stats", stats}, 
   {"set", setvalue},
   {"exit", shell_quit},
   {"help", help},
   {"?", help},

   /* Default action */
   {NULL, unknown}};
/*---------------------------------------------------------------------------*/
void
shell_init(void)
{
}
/*---------------------------------------------------------------------------*/
void
shell_start(void)
{
  shell_output("BEE Gateway\r\n'?' for help", "");
  shell_prompt(SHELL_PROMPT);
}
/*---------------------------------------------------------------------------*/
void
shell_input(char *cmd)
{
  parse(cmd, parsetab);
  shell_prompt(SHELL_PROMPT);
}
/*---------------------------------------------------------------------------*/
