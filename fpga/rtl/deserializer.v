module deserializer (clk, data_in, reset, data_out, strobe);

   input clk;
   input data_in;
   input reset;
   output [7:0] data_out;
   output 	strobe;

   reg [7:0] bits;
   reg [4:0] rawbits;

   reg [2:0] count;

   assign strobe = (count == 3'b111);
   assign data_out[7:0] = bits[7:0];

   always @(posedge clk or posedge reset)
     begin
	if (reset)
	  begin
	     bits <= 8'b00000000;
	     rawbits <= 5'b00000;
	     count <= 3'b000;
	  end
	else
	  begin
	     rawbits <= { rawbits[3:0], data_in };

	     if (rawbits[4:0] != 5'b11111)
	       begin
		  bits <= { bits[6:0], data_in };
		  count <= count + 1;
	       end
	  end
     end

endmodule // deserializer
