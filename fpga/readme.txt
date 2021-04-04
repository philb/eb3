Bridge3 FPGA
------------

This FPGA implements the bit-oriented parts of the Econet SDLC
protocol that are awkward to do in the CPU with sufficient
performance.

There are no CPU-visible registers in the FPGA.  Control and status
are implemented on dedicated I/Os.  Data transfer is done over SPI
with the FPGA as master.


Receiver operation
------------------

The receiver is enabled whenever RX_ENABLE is high (and RESET is low)
and the transmitter is not active.  It accepts data at RXDATA clocked
on the rising edge of NETCLK.

If NETCLK is lost for more than 2047 cycles of MCLK, the FPGA drives
NO_CLOCK high.  This output returns low as soon as another rising
edge is seen on NETCLK.  The receiver itself is fully static and is
not reset by loss of clock.

When a line idle pattern (eight consecutive "1" bits) is detected,
the FPGA drives IDLE high.  This output goes low as soon as a zero
is received.

When not in a frame, the receiver continuously hunts for the flag
pattern 01111110.  When a flag is detected, the receiver uses this
to establish frame synchronisation and switches to data transfer
mode.  Invalid bit patterns at the receiver will cause loss of IDLE
state but the receiver will remain in flag hunt mode.

Once a frame has been opened, the receiver deserialises incoming
data into a shift register and performs zero deletion as required.
If a flag or abort sequence is detected before the first byte is
completed, the runt frame is ignored, the FPGA returns to
start-of-frame or flag hunt mode as appropriate, and no data is
passed to the CPU.

As soon as a full eight-bit byte has been received it is passed to
the CPU over the SPI master interface.  SPI transfers performed by
the FPGA always move 16 bits per operation, formatted as follows:

Bits [15:12] reserved, always zero in current implementation
Bit 11 abort indication
     1=frame aborted
     0=not aborted
Bits [10:9] frame status:
     11=frame complete, good FCS
     01=frame complete, bad FCS
     x0=frame not complete
Bit 8 payload flag
     1=data byte present in bits [7:0]
     0=no data present
Bits 7-0: data (if present)

The FPGA does not distinguish between address, control, information
and frame check subfields.  No address filtering is performed and the
entire contents of the frame including FCS is transferred to the CPU.

The receiver continuously (one bit at a time) calculates a CRC
over the received data.  For a valid frame the CRC over the whole
frame including the transmitted FCS field should be 0x1D0F for a
valid frame.  If this condition is met then bit 10 is set in the
receive status word.  This bit is not significant except when
end-of-frame is being signalled and should be ignored at other
times.

The SPI clock runs at half the speed of MCLK.  For MCLK=12.5MHz
and NETCLK max 1MHz the receiver is guaranteed never to overrun
inside the FPGA because the SPI interface can always unload the
receiver register before another byte is clocked in.

End-of-frame is identified by the closing flag.  The receiver
needs to see eight bits of data on the line to recognise a flag so
the last data byte (actually the second half of the FCS) will
already have been transferred to the CPU before end-of-frame is
recognised.


Transmitter operation
---------------------

Transmission is initiated by the CPU driving TX_GO high for at
least one MCLK cycle.  TX_GO must return low and then go high
again in order to initiate a subsequent transmission.

Once a transmission is started the FPGA will:

1. Inhibit the receiver.

2. Turn on the output line driver

3. Start sending an opening flag.

4. Perform an SPI master transfer to read transmit data from the
CPU into the transmit holding register.

The opening flag takes 8 NETCLK cycles to transmit.  As described
above for the receiver, this is sufficient time for the FPGA to
fetch the first byte of transmit data from the CPU because the SPI
bus runs much faster than the network.  The CPU must be ready to
provide transmit data over SPI as soon as it asserts TX_GO.

After the opening flag has been sent the transmitter moves the
first data byte from the transmit holding register into the
shift register, commences serializing it onto the wire, and
initiates another SPI master transfer to fetch the next byte
from the CPU.  This pattern repeats until the CPU signals "end
of frame".

Transmit data transfers over the SPI bus are performed 16-bits
at a time in a similar fashion to the receiver:

Bit 15 end-of-frame
  1=frame ends after data (if any) included here
  0=more data follows

Bit 14 data present
  1=bits [7:0] contain transmit data payload
  0=no data in bits [7:0]

Bits 13-8 reserved, should be zero

Bits 7-0 data payload, if any

The transmitter calculates a CRC on a bit-by-bit basis over the
transmitted data.

At the end of the frame the FPGA:

1. Appends the bitwise inverse of the FCS to the transmitted
frame.

2. Transmits a closing flag.

3. Turns off the line driver and de-inhibits the receiver.

4. Returns to the idle state.

It is not possible for the transmitter to underrun in the FPGA.
However, if the CPU cannot load its SPI transmit register quickly
enough it is possible that the transmit operation may underrun at
the CPU side and the transmitter will load garbage.  If the CPU
detects an underrun, collision or any other erroneous condition
it can abort the transmission by taking TX_ABORT high for at least
one MCLK cycle.  If this happens the transmitter:

1. Immediately transmits at least eight consecutive "1" bits to
signal an abort condition.

2. Turns off the line driver and de-inhibits the receiver.

3. Returns to the idle state.

TX_ABORT should be returned low before any further transmit
operation is attempted.

The transmitter counts the number of bytes transmitted onto the
wire between opening and closing flags.  If this exceeds 65535
then the transmitter jabber protection logic will generate an
abort and terminate the transmission as if TX_ABORT had been
taken high.  The FPGA drives TX_JABBER high to indicate that
this has happened.  The CPU can clear this status by driving
TX_ABORT high.

If the CPU takes FLAG_FILL high then the transmitter will:

1. Complete any active transmission

2. Turn on the line driver

3. Transmit continuous flags on the line

If FLAG_FILL goes low before TX_GO goes high the transmitter will:

1. Finish sending the current flag

2. Turn off the line driver

If TX_GO goes high before FLAG_FILL goes low the transmitter will:

1. Finish sending the current flag.

2. Initiate a transmission as normal.  An additional opening flag
will be sent because the transmitter needs time to fetch the first
data byte over SPI.
