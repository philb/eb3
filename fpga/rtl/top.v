module top (mclk, netclk, txdata, rxdata, txen, irq, no_clock, idle, sck, mosi, miso, ss, sck_m, mosi_m, miso_m, ss_m, usart0, usart1, usart2, usart3, reset, flag_fill, tx_error);
   input       mclk;
   input       netclk;
   output      txdata;
   input       rxdata;
   output      txen;
   output      irq;
   output      no_clock;
   output      idle;
   input       ss;
   input       sck;
   input       mosi;
   output      miso;

   output      sck_m;
   output      mosi_m;
   input       miso_m;
   output      ss_m;

   input       usart0;
   input       usart1;
   input       usart2;
   input       usart3;
   input       reset;		// FPGA2_SCK

   input       flag_fill;

   output      tx_error;

   wire [15:0] spi_rx_data;
   wire [15:0] spi_tx_data;

   wire        spi_rx_strobe;
   wire        spi_rx_accept;
   wire        spi_tx_request;
   wire        spi_tx_strobe;

   wire        tx_go;
   wire        tx_overflow;
   wire        tx_underflow;

   assign tx_error = (tx_overflow || tx_underflow);

   rx_top RX(netclk, rxdata, reset, mclk, idle, no_clock, spi_rx_data, spi_rx_strobe, spi_rx_accept);
   tx_top TX(mclk, netclk, reset, txdata, flag_fill, spi_tx_data, spi_tx_request, spi_tx_strobe, tx_go,
	     tx_underflow, tx_overflow);

   spi_master SPI(mclk, reset, spi_rx_data, spi_rx_strobe, spi_rx_accept,
		  spi_tx_request, spi_tx_data, spi_tx_strobe,
		  ss_m, sck_m, mosi_m, miso_m);

endmodule // sdlc
