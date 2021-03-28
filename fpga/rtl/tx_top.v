module tx_top(netclk, mclk, reset, txdata, flag_fill, data_in[7:0], data_available, data_consumed, eop);

   input netclk;
   input mclk;
   input reset;
   input flag_fill;

   output txdata;
   output        data_consumed;

   reg [2:0] state;

   parameter IDLE = 3'b000;
   parameter OPENING_FLAG = 3'b001;
   parameter IN_FRAME = 3'b010;
   parameter FCS = 3'b011;
   parameter CLOSING_FLAG = 3'b100;

   wire [15:0] new_crc;
   reg [15:0]  lfsr;

   wire        need_zero_insert;
   wire        txdata;

   assign new_crc[0]  = txdata ^ lfsr[15];
   assign new_crc[1]  = lfsr[0];
   assign new_crc[2]  = lfsr[1];
   assign new_crc[3]  = lfsr[2];
   assign new_crc[4]  = lfsr[3];
   assign new_crc[5]  = lfsr[4] ^ txdata ^ lfsr[15];
   assign new_crc[6]  = lfsr[5];
   assign new_crc[7]  = lfsr[6];
   assign new_crc[8]  = lfsr[7];
   assign new_crc[9]  = lfsr[8];
   assign new_crc[10] = lfsr[9];
   assign new_crc[11] = lfsr[10];
   assign new_crc[12] = lfsr[11] ^ txdata ^ lfsr[15];
   assign new_crc[13] = lfsr[12];
   assign new_crc[14] = lfsr[13];
   assign new_crc[15] = lfsr[14];

   wire    [15:0]    not_crc;
   assign not_crc[15:0] = lfsr[15:0] ^ 16'hffff;

   reg [7:0]   data;
   reg [4:0]   bitn;
   reg [4:0]   out_bits;

   input [7:0]   data_in;
   input 	 data_available;
   reg 		 data_consumed;
   input 	 eop;

   assign need_zero_insert = (state == IN_FRAME) && (out_bits[4:0] == 5'b11111);
   assign txdata = need_zero_insert ? 1'b0 : ((state == IDLE) ? 1'b1 : ((state == FCS) ? !lfsr[15] : data[0]));

   always @(negedge netclk or posedge reset)
     begin
	if (reset)
	  begin
	     state <= IDLE;
	  end
	else
	  begin
	     case (state)
	       IDLE:
		 begin
		    data <= 8'h7E;
		    bitn <= 0;

		    if (flag_fill)
		      begin
			 state <= CLOSING_FLAG;
		      end
		    else if (data_available)
		      begin
			 state <= OPENING_FLAG;
		      end
		    else
		      begin
			 state <= IDLE;
		      end
		 end

	       OPENING_FLAG:
		 begin
		    if (bitn == 7)
		      begin
			 bitn <= 0;
			 out_bits <= 5'b00000;
			 lfsr <= 16'hffff;
			 state <= IN_FRAME;
			 data <= data_in;
			 data_consumed <= 1'b1;
		      end
		    else
		      begin
			 bitn <= bitn + 1;
			 data <= { 1'b1, data[7:1] };
		      end
		 end

	       IN_FRAME:
		 begin
		    out_bits <= { txdata, out_bits[4:1] };

		    if (!need_zero_insert)
		      begin
			 lfsr[15:0] <= new_crc[15:0];
		      end

		    if (bitn == 7)
		      begin
			 bitn <= 0;
			 if (!eop && data_available)
			   begin
			      data <= data_in;
			      data_consumed <= 1'b1;
			   end
			 else if (!eop)
			   begin
			      state <= CLOSING_FLAG;
			      data <= 8'hff;
			   end
			 else
			   begin
			      state <= FCS;
			   end
		      end
		    else
		      begin
			 if (!need_zero_insert)
			   begin
			      bitn <= bitn + 1;
			      data <= { 1'b1, data[7:1] };
			   end
		      end
		 end // case: IN_FRAME

	       FCS:
		 begin
		    if (bitn == 15)
		      begin
			 bitn <= 0;
			 state <= CLOSING_FLAG;
			 data <= 8'h7e;
		      end
		    else
		      begin
			 bitn <= bitn + 1;
			 lfsr <= { lfsr[14:0], 1'b1 };
		      end
		 end

	       CLOSING_FLAG:
		 begin
		    data <= { 1'b1, data[7:1] };

		    if (bitn == 7)
		      begin
			 bitn <= 0;
			 data <= 8'h7e;
			 state <= flag_fill ? CLOSING_FLAG : IDLE;
		      end
		    else
		      begin
			 bitn <= bitn + 1;
		      end
		 end
 	     endcase // case (state)
	  end // else: !if(reset)
     end // always @ (posedge netclk or posedge reset)

endmodule // tx_top
