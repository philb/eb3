#include <atmel_start.h>
#include "hpl_dma.h"
#include "econet.h"
#include "fpga.h"

#define SERCOM_SPI SERCOM4
#define TX_DMA_CH 0
#define RX_DMA_CH 1

void econet_rx_disable()
{
	fpga_rx_enable(0);
}

void econet_rx_enable()
{
	fpga_rx_enable(1);
}

uint8_t econet_frame_pull_byte(struct econet_frame *f)
{
	uint8_t b = *(f->data++);
	f->length--;
	return b;
}

int econet_tx_frame(struct econet_frame *f)
{
	SercomSpi *spi = &SERCOM_SPI->SPI;
  
	if (f->length == 0) return -EINVAL;
	
	econet_rx_disable();
	
	// Load first byte into shift register
	spi->DATA.reg = econet_frame_pull_byte(f);
	
	// Setup dma to move rest of frame
	struct _dma_resource *resource;
	_dma_get_channel_resource(&resource, TX_DMA_CH);
	_dma_set_destination_address(TX_DMA_CH, (const void *)&spi->DATA);
	_dma_set_source_address(TX_DMA_CH, f->data);
	_dma_set_data_amount(TX_DMA_CH, f->length);
	
	fpga_tx_go(1);
	
	return 0;
}

void econet_tx_frame_complete()
{
	fpga_tx_go(0);
	econet_rx_enable();
}
