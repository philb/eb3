module test_mclk (clk);
   output clk;
   reg 	  clk;

   initial begin
      clk <= 1'b0;
   end

   always #1 begin
      clk <= !clk;
   end

endmodule // test_clock
