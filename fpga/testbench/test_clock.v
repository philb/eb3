module test_clock (clk);
   output clk;
   reg 	  clk;

   initial begin
      clk <= 1'b0;
   end

   always #5 begin
      clk <= !clk;
   end

endmodule // test_clock
