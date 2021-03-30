module spi_master(clk, reset, rx_data, rx_strobe, rx_accept, tx_request, tx_data, tx_strobe, ss, sck, mosi, miso);

   input clk;
   input reset;
   
   input [15:0]  rx_data;
   input 	 rx_strobe;
   output 	 rx_accept;

   input 	 tx_request;   
   output [15:0] tx_data;
   output 	 tx_strobe;

   output 	 ss;
   output 	 sck;
   output 	 mosi;
   input 	 miso;   

   reg [15:0] 	 rx_reg;
   reg [15:0] 	 tx_reg;   
   
   reg [2:0] 	 state;
   reg [4:0] 	 bit;

   reg [2:0] 	 clk_divider;
   reg 		 sck;
   reg 		 mosi;	
   reg 		 ss;

   reg 		 rx_accept;
   reg 		 tx_strobe;   

   wire 	 mosi_next;
   
   parameter IDLE = 3'b000;
   parameter RUN = 3'b010;
   parameter END = 3'b100;

   assign mosi_next = rx_reg[0];
   assign ss_next = !(state == RUN);
   assign tx_data[15:0] = tx_reg[15:0];   
 
   always @(posedge clk or posedge reset)
     begin
	if (reset)
	  begin
	     sck <= 1'b0;
	  end
	else
	  begin
	     sck <= !sck;	     
	  end
     end   

   always @(negedge sck or posedge reset)
     begin
	if (reset)
	  begin
	     mosi <= 1'b1;
	     ss <= 1'b1;	     
	  end
	else
	  begin
	     mosi <= mosi_next;
	     ss <= ss_next;
	  end
     end

   always @(posedge sck or posedge reset)
     begin
	if (reset)
	  begin
	     bit <= 4'h0;
	     state <= IDLE;
	     rx_reg <= 16'h0000;
	     tx_reg <= 16'h0000;
	     rx_accept <= 1'b0;
	     tx_strobe <= 1'b0;	     
	  end
	else
	  begin
	     case (state)
	       IDLE:
		 if (rx_strobe || tx_request)
		   begin
		      bit <= 4'h0;		      
		      state <= RUN;
		      if (rx_strobe)
			begin
			   rx_reg <= rx_data;
			   rx_accept <= 1'b1;
			end
		      else
			begin
			   rx_reg <= 16'hffff;
			   rx_accept <= 1'b0;			   
			end
		   end // if (rx_strobe || tx_request)

	       RUN:
		 begin
		    rx_accept <= 1'b0;
		    
		    rx_reg <= { 1'b1, rx_reg[15:1] };
		    tx_reg <= { miso, tx_reg[15:1] };
		    if (bit == 4'hf)
		      begin
			 state <= END;
			 if (tx_request)
			   begin
			      tx_strobe <= 1'b1;
			   end		 
		      end
		    else
		      begin
			 bit <= bit + 1;
		      end
		 end
	       	       
	       END:
		 begin
		    if (!tx_request || !tx_strobe)
		      begin
			 tx_strobe <= 1'b0;
			 state <= IDLE;			 
		      end
		 end
	     endcase // case (state)	     
	  end	
     end
   
endmodule // spi_master
