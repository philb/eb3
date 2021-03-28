module top;

   wire clk;
   wire txdata;
   
   reg 	reset;
   reg [7:0] data;
   reg 	     data_available;
   wire      data_consumed;
   reg      flag_fill;
   
   test_clock tclk(clk);
   test_mclk tmclk(mclk);

   reg 	clken;
   reg 	eop;
      
   tx_top TX(clk & clken, mclk, reset, txdata, flag_fill, data, data_available, data_consumed, eop);

   wire frame_complete;
   wire abort;
   wire idle;
   wire valid;
   wire no_clock;
   
   rx_top RX(clk & clken, txdata, reset, mclk, frame_complete, abort, idle, valid, no_clock); 
 
   initial begin
      $dumpfile("test.vcd");
      $dumpvars(0,top);

      clken <= 1'b0;      
      reset <= 1'b1;
      flag_fill <= 1'b0;      
      data_available <= 1'b0;
      data <= 8'hff;
      eop <= 1'b0;
       
      #50 reset <= 1'b0;

      #5000 clken <= 1'b1;

      #6000 flag_fill <= 1'b1;

      #7000 data <= 8'h53;
      data_available <= 1'b1;
      flag_fill <= 1'b0;
      eop <= 1'b1;      

      #50000 $finish;
   end // initial begin

endmodule // top
