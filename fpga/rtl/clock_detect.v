module clock_detect(netclk, clk, reset, no_clock);
   input netclk;
   input clk;
   input reset;
   output no_clock;
   
   reg 	       clk_detect_ff;
   reg [10:0]  clk_detect_count;
   reg 	       no_clock;

   always @(posedge clk or posedge reset)
     begin
	if (reset)
	  begin
	     clk_detect_ff <= 1'b0;
	     clk_detect_count <= 11'd0;
	     no_clock <= 1'b0;
	  end
	else
	  begin
	     clk_detect_ff <= netclk;

	     if (netclk && !clk_detect_ff)
	       begin
		  no_clock <= 1'b0;
		  clk_detect_count <= 11'd0;
	       end
	     else
	       begin
		  clk_detect_count <= clk_detect_count + 1;
		  if (clk_detect_count == 11'd2047)
		    begin
		       no_clock <= 1'b1;
		    end
	       end // else: !if(netclk && !clk_detect_ff)
	  end // else: !if(reset)
     end // always @ (posedge clk or posedge reset)

endmodule // clock_detect
