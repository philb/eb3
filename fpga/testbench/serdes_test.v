module top;

   wire clk;
   wire data;

   test_clock tclk(clk);

   reg       reset;
   reg [7:0] data_in;
   reg 	     data_strobe;

   reg [2:0] count;
   reg [7:0]  frame_data[0:5];
   reg [3:0]  tx_count;

   initial begin
      count = 0;
      data_in = 8'h65;
      reset = 1;
      data_strobe = 0;
      deser_reset = 1;

      frame_data[0] = 8'h7e;
      frame_data[1] = 8'h01;
      frame_data[2] = 8'h02;
      frame_data[3] = 8'h99;
      frame_data[4] = 8'h00;

      tx_count = 0;
   end

   serializer ser (clk, data_in, data_strobe, ser_ready, reset, data, 1'b1);

   wire [7:0] deser_out;
   wire       deser_strobe;
   reg 	      deser_reset;

   deserializer deser (clk, data, deser_reset, deser_out, deser_strobe);

   flag_detect fdet (clk, data, idle, flag, abort, reset);

   always @(posedge clk)
     begin
	count = count + 1;
	if (count == 2)
	  begin
	     reset = 0;
	  end

	if (!reset && ser_ready && (tx_count < 5))
	  begin
	     data_in <= frame_data[tx_count];
	     data_strobe <= 1;
	     tx_count <= tx_count + 1;
	  end

	if (ser_ready == 0)
	  begin
	     deser_reset <= 0;
	     data_strobe <= 0;
	  end

	if (deser_strobe)
	  $display("deserialized: %h", deser_out);

	if (abort)
	  begin
	     $display("Rx abort");
	     deser_reset <= 1;
	     $finish;
	  end
     end

endmodule // top
