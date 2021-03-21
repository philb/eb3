module serializer (clk, data_in, strobe, ready, reset, data_out, use_stuffing);
   input       clk;
   input [7:0] data_in;
   input       strobe;
   output      ready;
   input       reset;
   output      data_out;
   input       use_stuffing;

   reg [12:0]   bits;
   reg         ready;
   reg [2:0]   count;
   reg 	       stuff;

   wire        needstuff;

   assign needstuff = use_stuffing && (bits[12:8] == 5'b11111);
   assign data_out = bits[7] && !needstuff;

   always @(negedge clk or posedge reset)
     begin
	if (reset)
	  begin
	     bits[7:0] <= 8'hff;
	     ready <= 1'b1;
	     count <= 3'b000;
	     stuff <= 1'b0;
	  end
	else
	  if (strobe && ready)
	    begin
	       bits[12:0] <= { 5'b00000, data_in[7:0] };
	       ready <= 1'b0;
	       count <= 3'b000;
	    end
	  else if (ready)
	    begin
	       bits[7:0] = 8'b11111111;
	    end
	  else
	    begin
	       if (needstuff)
		 begin
		    stuff <= 1'b1;
		    bits[12:0] <= { 5'b00000, bits[7:0] };
		 end
	       else
		 begin
		    bits[12:0] <= { bits[11:0], 1'b1 };
		    count <= count + 1;
		    if (count == 6)
		      begin
			 ready <= 1'b1;
		      end
		 end
	    end
     end

endmodule // serializer
