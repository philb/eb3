module tx_top(clk, netclk, reset, txdata, flag_fill, spi_data[15:0], spi_data_request, spi_data_strobe, go, underrun, overrun, txen, jabber, abort);
   input        clk;
   input 	netclk;
   input 	reset;
   output 	txdata;

   input 	flag_fill;

   input [15:0] spi_data;
   input 	spi_data_strobe;
   output 	spi_data_request;

   input 	go;
   output 	underrun;
   output 	overrun;
   output 	txen;
   output 	jabber;
   input 	abort;

   reg [7:0] 	thr;
   reg 		thr_full;
   wire 	thr_consumed;
   reg 		overrun;

   reg 		eop;

   reg 		was_spi_data_strobe;
   reg 		spi_data_request;

   reg 		was_go;

   reg [19:0] 	jabber_count;

   always @(posedge clk or posedge reset)
     begin
	if (reset)
	  begin
	     thr <= 8'h00;
	     thr_full <= 1'b0;
	     was_spi_data_strobe <= 1'b0;
	     spi_data_request <= 1'b0;
	     overrun <= 1'b0;
	     jabber_count <= 0;
	     was_go <= 1'b0;
	  end
	else
	  begin
	     was_spi_data_strobe <= spi_data_strobe;
	     was_go <= go;

	     if (spi_data_strobe && !was_spi_data_strobe)
	       begin
		  eop <= spi_data[15];
		  if (spi_data[14])
		    begin
		       if (thr_full)
			 begin
			    overrun <= 1'b1;
			 end
		       thr[7:0] <= spi_data[7:0];
		       thr_full <= 1'b1;
		    end
		  if (eop || thr_full)
		    begin
		       spi_data_request <= 1'b0;
		    end
	       end // if (spi_data_strobe && !was_spi_data_strobe)
	     else
	       begin
		  if (thr_consumed)
		    begin
		       thr_full <= 1'b0;
		       if (!eop)
			 begin
			    spi_data_request <= 1'b1;
			 end
		    end
		  else
		    begin
		       if (go && !was_go)
			 begin
			    eop <= 1'b0;
			    spi_data_request <= 1'b1;
			 end
		    end
	       end
	  end
     end

   tx_framer tx_framer_(netclk, reset, txdata, flag_fill, thr, thr_full, thr_consumed, eop, underrun, txen);

endmodule // tx_top
