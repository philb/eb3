module top (mclk, netclk, txdata, rxdata, txen, irq, no_clock, idle, sck, mosi, miso, ss, usart0, usart1, usart2, usart3, reset);
   input mclk;
   input netclk;
   output txdata;
   input  rxdata;
   output txen;
   output irq;
   output no_clock;
   output idle;
   input  ss;
   input sck;
   input mosi;
   output miso;
   input  usart0;
   input  usart1;
   input  usart2;
   input  usart3;
   input  reset;		// FPGA2_SCK

   wire   flag;

   wire [7:0] rx_byte;
   wire       rx_byte_strobe;

   rx_top RX(netclk, rxdata, reset, mclk, frame_complete, abort, idle, frame_valid, no_clock);

   assign miso = frame_complete;

endmodule // sdlc
