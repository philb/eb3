module rx_top(netclk, rxdata, reset, clk, frame_complete_out, abort_out, idle, frame_valid_out, no_clock);

   input netclk;
   input rxdata;
   input reset;
   input clk;

   output frame_complete_out;
   output abort_out;
   output idle;
   output frame_valid_out;
   output no_clock;

   clock_detect clock_detect_(netclk, clk, reset, no_clock);
   rx_deframer rx_deframer_(netclk, reset, rxdata, abort, idle, frame_complete, frame_valid);
   
   reg 	  frame_complete_out;
   reg 	  frame_valid_out;
   reg 	  abort_out;

   reg 	  frame_complete_1;
   reg 	  abort_1;

   always @(posedge clk or posedge reset)
     begin
	if (reset)
	  begin
	     frame_complete_out <= 1'b0;
	     frame_valid_out <= 1'b0;
	     abort_out <= 1'b0;

	     abort_1 <= 1'b0;
	     frame_complete_1 <= 1'b0;
	  end // if (reset)
	else
	  begin
	     abort_1 <= abort;
	     frame_complete_1 <= frame_complete;
	     frame_valid_out <= frame_valid;

	     frame_complete_out <= (frame_complete && !frame_complete_1);
	     abort_out <= (abort && !abort_1);
	  end // else: !if(reset)
     end

endmodule // rx_top
