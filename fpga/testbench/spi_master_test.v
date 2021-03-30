module top();

   wire clk;
   reg 	reset;
   reg  [15:0] rx_data;
   reg rx_strobe;
   wire 	rx_accept;
   reg 	tx_request;
   wire [15:0] tx_data;
   wire        tx_strobe;
   wire        ss;
   wire        sck;
   wire        mosi;
   reg 	       miso;   
	   
   test_clock clock(clk);
   
   spi_master spi(clk, reset, rx_data, rx_strobe, rx_accept, tx_request, tx_data, tx_strobe, ss, sck, mosi, miso);
   
   initial begin
      $dumpfile("test.vcd");
      $dumpvars(0,top);
      
      reset <= 1'b1;
      miso <= 1'b1;
      rx_strobe <= 1'b0;
      tx_request <= 1'b0;
      rx_data <= 16'h53cc;      

      #20 reset <= 1'b0;

      #20 tx_request <= 1'b1;

      #500 rx_strobe <= 1'b1;      

      #10 tx_request <= 1'b0;
            
      #50000 $finish;
      
   end
   
endmodule // top
