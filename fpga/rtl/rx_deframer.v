module rx_deframer(netclk, reset, rxdata, frame_abort, idle, frame_complete, frame_valid, byte_ready, dout);
   input 	netclk;
   input 	reset;
   input 	rxdata;
   output 	frame_abort;
   output 	idle;
   output 	frame_complete;
   output       frame_valid;
   output       byte_ready;
   output [7:0] dout;

   reg [2:0] 	state;

   wire 	is_flag;
   wire 	is_abort;
   wire 	is_stuffing;
   wire [15:0] 	new_crc;
   reg [15:0] 	lfsr;
   reg [7:0] 	rx_shift;
   reg [7:0] 	byte;
   reg [2:0] 	bit;
   reg 		byte_ready;
   reg 		frame_abort;
   reg 		frame_complete;
   reg 		frame_valid;
   wire 	idle;

   reg [7:0] 	rx_latch;

   wire 	good_fcs;

   // state definitions
   parameter HUNT = 3'b001;
   parameter START_FRAME = 3'b010;
   parameter IN_FRAME = 3'b100;

   assign dout[7:0] = rx_latch[7:0];

   assign is_flag = ({rxdata, rx_shift[7:1]} == 8'b01111110);
   assign is_abort = ({rxdata, rx_shift[7:2]} == 7'b1111111);
   assign is_stuffing = ({ rxdata, rx_shift[7:3] } == 6'b011111);
   assign idle = ({rxdata, rx_shift[7:1]} == 8'b11111111);

   assign new_crc[0]  = rx_shift[7] ^ lfsr[15];
   assign new_crc[1]  = lfsr[0];
   assign new_crc[2]  = lfsr[1];
   assign new_crc[3]  = lfsr[2];
   assign new_crc[4]  = lfsr[3];
   assign new_crc[5]  = lfsr[4] ^ rx_shift[7] ^ lfsr[15];
   assign new_crc[6]  = lfsr[5];
   assign new_crc[7]  = lfsr[6];
   assign new_crc[8]  = lfsr[7];
   assign new_crc[9]  = lfsr[8];
   assign new_crc[10] = lfsr[9];
   assign new_crc[11] = lfsr[10];
   assign new_crc[12] = lfsr[11] ^ rx_shift[7] ^ lfsr[15];
   assign new_crc[13] = lfsr[12];
   assign new_crc[14] = lfsr[13];
   assign new_crc[15] = lfsr[14];

   assign good_fcs = (new_crc[15:0] == 16'h1d0f);

   wire [7:0] 	new_byte;
   assign new_byte = { rxdata, byte[7:1] };

   always @(posedge netclk or posedge reset)
     begin
	if (reset)
	  begin
	     state <= HUNT;
	     bit <= 3'b000;
	     rx_shift <= 8'b11111111;
	     frame_complete <= 1'b0;
	     frame_valid <= 1'b0;
	     frame_abort <= 1'b0;
	     rx_latch <= 8'hff;
	  end
	else
	  begin
	     rx_shift[7:0] <= { rxdata, rx_shift[7:1] };

	     case (state)
	       HUNT:
		 if (is_flag)
		   begin
		      lfsr <= 16'hffff;
		      bit <= 3'b000;
		      state <= START_FRAME;
		      byte_ready <= 1'b0;
		      frame_complete <= 1'b0;
		      frame_valid <= 1'b0;
		      frame_abort <= 1'b0;
		   end
		 else
		   begin
		      frame_abort <= 1'b0;
		      state <= HUNT;
		   end

	       START_FRAME:
		 if (is_abort)
		   begin
		      state <= HUNT;		// Early abort doesn't set aborted flag
		   end
		 else if (is_flag)
		   begin
		      lfsr <= 16'hffff;
		      bit <= 3'b000;
		      frame_complete <= 1'b0;
		      frame_valid <= 1'b0;
		   end
		 else if (!is_stuffing)
		   begin
		      byte <= new_byte;
		      lfsr <= new_crc;
		      if (bit == 3'h7)
			begin
			   frame_complete <= 1'b0;
			   frame_valid <= 1'b0;
			   state <= IN_FRAME;
			   bit <= 3'h0;
			   rx_latch <= new_byte;
			   byte_ready <= 1'b1;
			end
		      else
			begin
			   bit  <= bit + 1;
			   byte_ready <= 1'b0;
			end
		   end // if (!is_stuffing)

	       IN_FRAME:
		 if (is_abort)
		   begin
		      state <= HUNT;
		      frame_abort <= 1'b1;
		   end
		 else if (is_flag)
		   begin
		      frame_complete <= 1'b1;
		      bit <= 3'h0;
		      state <= START_FRAME;	// Maintain character sync
		   end
		 else if (!is_stuffing)
		   begin
		      byte <= new_byte;
		      lfsr <= new_crc;
		      if (bit == 3'h7)
			begin
			   bit <= 3'h0;
			   byte_ready <= 1'b1;
			   rx_latch <= new_byte;
			   frame_valid <= good_fcs;
			end
		      else
			begin
			   bit <= bit + 1;
			   byte_ready <= 1'b0;
			end
		   end // if (!is_stuffing)
	     endcase // case (state)
	  end
     end // always @ (posedge netclk or posedge reset)

endmodule // rx_deframer
