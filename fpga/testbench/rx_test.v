module top;

   wire clk;
   reg data;

   reg 	reset;   

   test_clock tclk(clk);
   test_mclk tmclk(mclk);

   reg 	clken;
 
   rx_top RX(clk & clken, data, reset, mclk, frame_complete, abort, idle, valid, no_clock);

   initial begin
      $dumpfile("test.vcd");
      $dumpvars(0,top);

      clken <= 1'b0;      
      reset <= 1'b1;
      data <= 1'b1;      
   
      #50 reset <= 1'b0;

      #5000 clken <= 1'b1;

      #50000 $finish;
   end // initial begin

   always @(negedge clk) begin
      #6000
	// flag
	#10 data <= 1'b0;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b0;
	#10 data <= 1'b1;

        // 0xaa
	#10 data <= 1'b0;
	#10 data <= 1'b1;
	#10 data <= 1'b0;
	#10 data <= 1'b1;
	#10 data <= 1'b0;
	#10 data <= 1'b1;
	#10 data <= 1'b0;
	#10 data <= 1'b1;

        // 0x00
	#10 data <= 1'b0;
	#10 data <= 1'b0;
	#10 data <= 1'b0;
	#10 data <= 1'b0;
	#10 data <= 1'b0;
	#10 data <= 1'b0;
	#10 data <= 1'b0;
	#10 data <= 1'b0;

        // 0xff, need zero insertion after five bits
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b0;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;

	// flag
	#10 data <= 1'b0;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b1;
	#10 data <= 1'b0;
	#10 data <= 1'b1;
   end

endmodule // top
