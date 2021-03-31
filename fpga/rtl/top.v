module top (mclk, netclk, txdata, rxdata, txen, irq, no_clock, idle, sck, mosi, miso, ss, usart0, usart1, usart2, usart3, reset, flag_fill);
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

   input       flag_fill;

   wire [15:0] spi_rx_data;
   wire [15:0] spi_tx_data;

   wire        spi_rx_strobe;
   wire        spi_rx_accept;
   wire        spi_tx_request;
   wire        spi_tx_strobe;

   wire        tx_go;
   wire        tx_overflow;
   wire        tx_underflow;


   rx_top RX(netclk, rxdata, reset, mclk, idle, no_clock, spi_rx_data, spi_rx_strobe, spi_rx_accept);
   tx_top TX(mclk, netclk, reset, txdata, flag_fill, spi_tx_data, spi_tx_request, spi_tx_strobe, tx_go,
	     tx_underflow, tx_overflow);

   spi_master SPI(mclk, reset, spi_rx_data, spi_rx_strobe, spi_rx_accept,
		  spi_tx_request, spi_tx_data, spi_tx_strobe,
		  ss, sck, mosi, miso);

endmodule // sdlc
