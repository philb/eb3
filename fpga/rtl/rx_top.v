module rx_top(netclk, rxdata, reset, clk, idle, no_clock, spi_data, rx_strobe, rx_accept);

   input netclk;
   input rxdata;
   input reset;
   input clk;

   output idle;
   output no_clock;

   output [15:0] spi_data;
   output 	 rx_strobe;
   input 	 rx_accept;

   // bits 0:7 data (if present)
   //      8   data present
   //      9   frame complete
   //      10  frame valid (good FCS)
   //      11  aborted
   //      12  reserved
   //      13  reserved
   //      14  reserved
   //      15  reserved
   reg [15:0] 	 spi_data;
   reg 		 rx_strobe;

   wire   byte_ready;
   wire [7:0] dout;

   clock_detect clock_detect_(netclk, clk, reset, no_clock);
   rx_deframer rx_deframer_(netclk, reset, rxdata, abort, idle, frame_complete, frame_valid, byte_ready, dout);

   // These signals are clocked from netclk which is asynchronous to MCLK so we need to register them here.
   // Also, since netclk is much slower, they will stay asserted for several MCLK cycles.  Use a second set
   // of flip-flops for edge detection.
   reg 	  frame_complete_1;
   reg    frame_valid_1;
   reg 	  abort_1;
   reg    byte_ready_1;

   reg 	  frame_complete_2;
   reg 	  abort_2;
   reg    byte_ready_2;
   // Don't need edge detection on frame_valid because it is just a status bit that
   // goes along with frame_complete.

   wire   frame_complete_i = (frame_complete_1 && !frame_complete_2);
   wire   abort_i = (abort_1 && !abort_2);
   wire   byte_ready_i = (byte_ready_1 && !byte_ready_2);

   always @(posedge clk or posedge reset)
     begin
	if (reset)
	  begin
	     abort_1 <= 1'b0;
	     frame_complete_1 <= 1'b0;
	     frame_valid_1 <= 1'b0;
	     byte_ready_1 <= 1'b0;
	     rx_strobe <= 1'b0;

	     abort_2 <= 1'b0;
	     frame_complete_2 <= 1'b0;
	     byte_ready_2 <= 1'b0;
	  end // if (reset)
	else
	  begin
	     abort_1 <= abort;
	     frame_complete_1 <= frame_complete;
	     frame_valid_1 <= frame_valid;
	     byte_ready_1 <= byte_ready;

	     abort_2 <= abort_1;
	     frame_complete_2 <= frame_complete_1;
	     byte_ready_2 <= byte_ready_1;

	     if (frame_complete_i || abort_i || byte_ready_i)
	       begin
		  spi_data[15:0] <= { 4'b0, abort_i, frame_valid_1, frame_complete_i, byte_ready_i, (byte_ready_i ? dout : 8'h00) };
		  rx_strobe <= 1'b1;
	       end
	     else
	       begin
		  if (rx_accept)
		    begin
		       rx_strobe <= 1'b0;
		    end
	       end
	  end // else: !if(reset)
     end

endmodule // rx_top
