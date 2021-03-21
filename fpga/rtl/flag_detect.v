module flag_detect (clk, data, idle, flag, abort, reset);
   input clk;
   input data;
   input reset;

   output flag;
   output abort;
   output idle;

   reg [7:0] bits;

   assign flag = (bits[7:0] == 8'b01111110) && !reset;
   assign idle = (bits[7:0] == 8'b11111111) && !reset;
   assign abort = (bits[7:0] == 8'b01111111) && !reset;

   always @(posedge clk or posedge reset)
     begin
	if (reset)
	  bits <= 8'b00000000;
	else
	  bits <= { bits[6:0], data };
     end

endmodule // flag_detect
