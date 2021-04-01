// Move data between the CPU and transmit/receive blocks using SPI
//
// Port naming in this module is from the perspective of the CPU, 
// so "rx_data" is data coming out of the receiver and going to
// the CPU.
//
// The receiver (rx_top) places 16 bits of parallel data on rx_data[]
// and then drives rx_strobe high to indicate it has something to
// send.  When the SPI block is ready to transfer, it latches rx_data[]
// into its internal shift register and drives rx_accept high to
// indicate that the data has been latched.  rx_accept will stay high
// for one SCK cycle.  We do not wait for the receiver to deassert
// rx_strobe, we assume it will do so at some point during the SPI
// transmit cycle.
//
// The transmitter (tx_top) drives tx_request high to indicate it
// wants input data from the CPU.  SPI moves 16 bits from the CPU into
// the tx shift register then asserts tx_strobe to indicate that data
// is ready.  It then blocks waiting to see tx_request deasserted
// before it will accept another request from either transmitter or
// receiver.  If the transmitter wants to start another transfer then
// it must deassert tx_request, wait for tx_strobe to go low, and then
// assert tx_request again for the next operation.
//
// We use SPI mode 0: data changes on falling edge of SCK, latched on
// rising edge of SCK, first data bit moves on the first rising edge
// of SCK following SS# asserted.
//
// We will always deassert SS for at least two SCK cycles between
// transfers so a transfer takes a minimum of 18 SCK cycles (= 36 MCLK
// cycles).  The 16-bit payload is 8 bits of data plus 8 bits of
// status so we actually end up moving about 1 payload bit per 5 MCLK
// cycles on average.  This is still fine because the 25MHz MCLK is
// at least an order of magnitude faster than the ~500kHz line clock.

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
   reg [3:0] 	 bit;

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
