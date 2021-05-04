#ifndef FPGA_H
#define FPGA_H

extern void fpga_load(void);
extern void fpga_tx_go(bool e);
extern void fpga_rx_enable(bool e);

#endif
