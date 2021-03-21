module crc (data, clk, reset, read, out);
   input data;
   input clk;
   input reset;
   input read;

   output [15:0] out;

   reg [15:0] lfsr;

   assign out[15:0] = lfsr[15:0];

   always @(posedge clk or posedge reset)
     begin
	if (reset)
	  begin
	     lfsr[15:0] <= 16'hffff;
	  end
	else
	  if (read)
	    begin
	       lfsr[15:0] <= { lfsr[14:0], 1'b1 };
	    end
	  else
	    begin
	       lfsr[0]  <= data ^ lfsr[15];
	       lfsr[1]  <= lfsr[0];
	       lfsr[2]  <= lfsr[1];
	       lfsr[3]  <= lfsr[2];
	       lfsr[4]  <= lfsr[3];
	       lfsr[5]  <= lfsr[4] ^ data ^ lfsr[15];
	       lfsr[6]  <= lfsr[5];
	       lfsr[7]  <= lfsr[6];
	       lfsr[8]  <= lfsr[7];
	       lfsr[9]  <= lfsr[8];
	       lfsr[10] <= lfsr[9];
	       lfsr[11] <= lfsr[10];
	       lfsr[12] <= lfsr[11] ^ data ^ lfsr[15];
	       lfsr[13] <= lfsr[12];
	       lfsr[14] <= lfsr[13];
	       lfsr[15] <= lfsr[14];
	    end
	  end
     end // always @ (posedge clk)

endmodule // crc
