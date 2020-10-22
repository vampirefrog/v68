/* ＤＭＡＣ ◆ DMA controller */

#include "v68.h"
#include "musashi/m68k.h"
#include "dmac.h"

static void dmac_transfer_start(int chan);
static void dmac_transfer_abort(int chan);
static void dmac_transfer_halt(int chan);
static void dmac_transfer_continue(int chan);

void dmac_init() {
	for (int x = 0; x < 4; x++)
	{
		v68.dmac.channels[x].niv = 0x0f;
		v68.dmac.channels[x].eiv = 0x0f;
		v68.dmac.channels[x].cpr = 0;
		v68.dmac.channels[x].dcr = 0;
		v68.dmac.channels[x].ocr = 0;
		v68.dmac.channels[x].scr = 0;
		v68.dmac.channels[x].ccr = 0;
		v68.dmac.channels[x].csr &= 0xfe;
		v68.dmac.channels[x].cer = 0;
		v68.dmac.channels[x].gcr = 0;

		// m_timer[x]->adjust(attotime::never);
		v68.dmac.halted[x] = 0;
	}
}

static inline int dma_in_progress(int chan) {
	return (v68.dmac.channels[chan].csr & 0x08) != 0;
}

void dmac_tick(int chan) {
	int data;
	int datasize = 1;

	verbose1("dmac_tick chan=%d in_progress=%d\n", chan, dma_in_progress(chan));

	if (!dma_in_progress(chan))  // DMA in progress in channel x
		return;

	uint32_t mar = v68.dmac.channels[chan].mar & 0x00ffffff;
	uint32_t dar = v68.dmac.channels[chan].dar & 0x00ffffff;

	if (v68.dmac.channels[chan].ocr & 0x80)  // direction: 1 = device -> memory
	{
		switch(v68.dmac.channels[chan].ocr & 0x30)  // operation size
		{
		case 0x00:  // 8 bit
			data = m68k_read_memory_8(dar);  // read from device address
			m68k_write_memory_8(mar, data);  // write to memory address
			datasize = 1;
			break;
		case 0x10:  // 16 bit
			data = m68k_read_memory_16(dar);  // read from device address
			m68k_write_memory_16(mar, data);  // write to memory address
			datasize = 2;
			break;
		case 0x20:  // 32 bit
			data = m68k_read_memory_16(dar) << 16;  // read from device address
			data |= m68k_read_memory_16(dar+2);
			m68k_write_memory_16(mar, (data & 0xffff0000) >> 16);  // write to memory address
			m68k_write_memory_16(mar+2, data & 0x0000ffff);
			datasize = 4;
			break;
		case 0x30:  // 8 bit packed (?)
			data = m68k_read_memory_8(dar);  // read from device address
			m68k_write_memory_8(mar, data);  // write to memory address
			datasize = 1;
			break;
		}
		verbose2("dmac_tick device->memory dar=0x%08x mar=0x%08x data=0x%02x\n", dar, mar, data);
	}
	else  // memory -> device
	{
		switch(v68.dmac.channels[chan].ocr & 0x30)  // operation size
		{
		case 0x00:  // 8 bit
			data = m68k_read_memory_8(mar);  // read from memory address
			m68k_write_memory_8(dar, data);  // write to device address
			datasize = 1;
			break;
		case 0x10:  // 16 bit
			data = m68k_read_memory_16(mar);  // read from memory address
			m68k_write_memory_16(dar, data);  // write to device address
			datasize = 2;
			break;
		case 0x20:  // 32 bit
			data = m68k_read_memory_16(mar) << 16;  // read from memory address
			data |= m68k_read_memory_16(2);  // read from memory address
			m68k_write_memory_16(dar, (data & 0xffff0000) >> 16);  // write to device address
			m68k_write_memory_16(dar+2, data & 0x0000ffff);  // write to device address
			datasize = 4;
			break;
		case 0x30:  // 8 bit packed (?)
			data = m68k_read_memory_8(mar);  // read from memory address
			m68k_write_memory_8(dar, data);  // write to device address
			verbose2("dmac_tick 0x%08x -> 0x%08x = 0x%02x\n", mar, dar, data);
			datasize = 1;
			break;
		}
		verbose2("dmac_tick memory->device mar=0x%08x dar=0x%08x data=0x%02x\n", v68.dmac.channels[chan].mar, v68.dmac.channels[chan].dar, data);
	}

	// decrease memory transfer counter
	if (v68.dmac.channels[chan].mtc > 0)
		v68.dmac.channels[chan].mtc--;

	// handle change of memory and device addresses
	if ((v68.dmac.channels[chan].scr & 0x03) == 0x01)
		v68.dmac.channels[chan].dar+=datasize;
	else if ((v68.dmac.channels[chan].scr & 0x03) == 0x02)
		v68.dmac.channels[chan].dar-=datasize;

	if ((v68.dmac.channels[chan].scr & 0x0c) == 0x04)
		v68.dmac.channels[chan].mar+=datasize;
	else if ((v68.dmac.channels[chan].scr & 0x0c) == 0x08)
		v68.dmac.channels[chan].mar-=datasize;

	if (v68.dmac.channels[chan].mtc <= 0) {
		// End of transfer
		verbose2("dmac_tick End of transfer\n");
		if ((v68.dmac.channels[chan].ocr & 0x0c) != 0 && v68.dmac.channels[chan].btc > 0)
		{
			v68.dmac.channels[chan].btc--;
			v68.dmac.channels[chan].bar+=6;
			v68.dmac.channels[chan].mar = m68k_read_memory_16(v68.dmac.channels[chan].bar) << 16;
			v68.dmac.channels[chan].mar |= m68k_read_memory_16(v68.dmac.channels[chan].bar+2);
			v68.dmac.channels[chan].mtc = m68k_read_memory_16(v68.dmac.channels[chan].bar+4);
			return;
		}
		// m_timer[x]->adjust(attotime::never);
		v68.dmac.channels[chan].csr |= 0xe0;  // channel operation complete, block transfer complete
		v68.dmac.channels[chan].csr &= ~0x08;  // channel no longer active
		v68.dmac.channels[chan].ccr &= ~0xc0;

		// Burst transfer
		if ((v68.dmac.channels[chan].dcr & 0xc0) == 0x00)
		{
			verbose2("dmac_tick burst transfer\n");
			// m_cpu->set_input_line(INPUT_LINE_HALT, CLEAR_LINE);
		}

		v68.int_vec = 0x6a;
		m68k_set_irq(3);
	}
}

static void dmac_transfer_start(int chan) {
	verbose1("dmac_transfer_start chan=%d\n", chan);
	v68.dmac.channels[chan].csr &= ~0xe0;
	v68.dmac.channels[chan].csr |= 0x08;  // Channel active
	v68.dmac.channels[chan].csr &= ~0x30;  // Reset Error and Normal termination bits
	if ((v68.dmac.channels[chan].ocr & 0x0c) != 0x00)  // Array chain or Link array chain
	{
		v68.dmac.channels[chan].mar = m68k_read_memory_16(v68.dmac.channels[chan].bar) << 16;
		v68.dmac.channels[chan].mar |= m68k_read_memory_16(v68.dmac.channels[chan].bar+2);
		v68.dmac.channels[chan].mtc = m68k_read_memory_16(v68.dmac.channels[chan].bar+4);
		if (v68.dmac.channels[chan].btc > 0)
			v68.dmac.channels[chan].btc--;
	}

	// Burst transfers will halt the CPU until the transfer is complete
	if ((v68.dmac.channels[chan].dcr & 0xc0) == 0x00)  // Burst transfer
	{
		verbose2("dmac_transfer_start  complete\n");
		// m_cpu->set_input_line(INPUT_LINE_HALT, ASSERT_LINE);
		// m_timer[channel]->adjust(attotime::zero, channel, m_burst_clock[channel]);
	} else if (!(v68.dmac.channels[chan].ocr & 2)) {
		verbose2("dmac_transfer_start  & 2\n");
		// m_timer[channel]->adjust(attotime::from_usec(500), channel, m_our_clock[channel]);
	} else if ((v68.dmac.channels[chan].ocr & 3) == 3) {
		verbose2("dmac_transfer_start  & 3 == 3\n");
		// m_timer[channel]->adjust(attotime::from_usec(500), channel, attotime::never);
	} else if ((v68.dmac.channels[chan].ocr & 3) == 2) {
		verbose2("dmac_transfer_start  & 3 == 2\n");
		// m_timer[channel]->adjust(attotime::never, channel, attotime::never);
	}

	v68.dmac.transfer_size[chan] = v68.dmac.channels[chan].mtc;

	verbose2("DMA: Transfer begins: size=0x%08x\n",v68.dmac.transfer_size[chan]);
}

static void dmac_transfer_abort(int chan) {
	if (!dma_in_progress(chan))
		return;

	verbose3("DMA#%i: Transfer aborted\n",chan);
	// m_timer[channel]->adjust(attotime::never);
	v68.dmac.channels[chan].csr |= 0x90;  // channel error
	v68.dmac.channels[chan].csr &= ~0x08;  // channel no longer active
	v68.dmac.channels[chan].cer = 0x11;
	v68.dmac.channels[chan].ccr &= ~0xc0;
	// m_dma_error((offs_t)3, v68.dmac.channels[chan].ccr & 0x08);
	v68.int_vec = 0x6b;
	m68k_set_irq(3);
}

static void dmac_transfer_halt(int chan) {
	v68.dmac.halted[chan] = 1;
	// m_timer[channel]->adjust(attotime::never);
}

static void dmac_transfer_continue(int chan) {
	if (v68.dmac.halted[chan] != 0)
	{
		v68.dmac.halted[chan] = 0;
		// m_timer[channel]->adjust(attotime::zero, channel, m_our_clock[channel]);
	}
}

#define ACCESSING_BITS_0_7              ((mem_mask & 0x000000ffU) != 0)
#define ACCESSING_BITS_8_15             ((mem_mask & 0x0000ff00U) != 0)
void dmac_write_16(uint32_t addr, uint16_t data, uint16_t mem_mask) {
	uint8_t chan = (addr >> 6) & 0x03;
	uint8_t reg = (addr & 0x1f) >> 1;
	verbose2("dmac_write_16 0x%08x = 0x%04x mask=0x%04x chan=%d reg=0x%02x\n", addr, data, mem_mask, chan, reg);
	switch(reg) {
		case 0x00:
			if(ACCESSING_BITS_8_15) {
				v68.dmac.channels[chan].csr &= ~((data & 0xf600) >> 8);
				verbose2("DMA#%i: Channel status write : %02x data=0x%04x data&0xf600=0x%04x (data&0xf600)>>8=0x%04x ~((data&0xf600)>>8)=%04x\n",chan,v68.dmac.channels[chan].csr, data, data&0xf600, (data&0xf600)>>8, ~((data & 0xf600) >> 8));
				// Clearing ERR also resets CER (which is otherwise read-only)
				if ((data & 0x1000) != 0)
					v68.dmac.channels[chan].cer = 0;
			}
			break;
		case 0x02:  // DCR / OCR
			if (ACCESSING_BITS_8_15)
			{
				v68.dmac.channels[chan].dcr = (data & 0xff00) >> 8;
				verbose2("DMA#%i: Device Control write : %02x\n",chan,v68.dmac.channels[chan].dcr);
			}
			if (ACCESSING_BITS_0_7)
			{
				v68.dmac.channels[chan].ocr = data & 0x00ff;
				verbose2("DMA#%i: Operation Control write : %02x\n",chan,v68.dmac.channels[chan].ocr);
			}
			break;
		case 0x03:  // SCR / CCR
			if (ACCESSING_BITS_8_15)
			{
				v68.dmac.channels[chan].scr = (data & 0xff00) >> 8;
				verbose3("DMA#%i: Sequence Control write : %02x\n",chan,v68.dmac.channels[chan].scr);
			}
			if (ACCESSING_BITS_0_7)
			{
				v68.dmac.channels[chan].ccr = data & 0x00ff;
				if ((data & 0x0080))
					dmac_transfer_start(chan);
				if (data & 0x0010)  // software abort
					dmac_transfer_abort(chan);
				if (data & 0x0020)  // halt operation
					dmac_transfer_halt(chan);
				if (data & 0x0040)  // continure operation
					dmac_transfer_continue(chan);
				verbose3("DMA#%i: Channel Control write : %02x\n",chan,v68.dmac.channels[chan].ccr);
			}
			break;
		case 0x05:  // MTC
			v68.dmac.channels[chan].mtc = data;
			verbose2("DMA#%i:  Memory Transfer Counter write : 0x%04x\n",chan,v68.dmac.channels[chan].mtc);
			break;
		case 0x06:  // MAR (high)
			v68.dmac.channels[chan].mar = (v68.dmac.channels[chan].mar & 0x0000ffff) | (data << 16);
			verbose2("DMA#%i:  Memory Address write : %08x\n",chan,v68.dmac.channels[chan].mar);
			break;
		case 0x07:  // MAR (low)
			v68.dmac.channels[chan].mar = (v68.dmac.channels[chan].mar & 0xffff0000) | (data & 0x0000ffff);
			verbose2("DMA#%i:  Memory Address write : %08x\n",chan,v68.dmac.channels[chan].mar);
			break;
		case 0x0a:  // DAR (high)
			v68.dmac.channels[chan].dar = (v68.dmac.channels[chan].dar & 0x0000ffff) | (data << 16);
			verbose2("DMA#%i:  Device Address write : %08x\n",chan,v68.dmac.channels[chan].dar);
			break;
		case 0x0b:  // DAR (low)
			v68.dmac.channels[chan].dar = (v68.dmac.channels[chan].dar & 0xffff0000) | (data & 0x0000ffff);
			verbose2("DMA#%i:  Device Address write : %08x\n",chan,v68.dmac.channels[chan].dar);
			break;
		case 0x0d:  // BTC
			v68.dmac.channels[chan].btc = data;
			verbose3("DMA#%i:  Base Transfer Counter write : %04x\n",chan,v68.dmac.channels[chan].btc);
			break;
		case 0x0e:  // BAR (high)
			v68.dmac.channels[chan].bar = (v68.dmac.channels[chan].bar & 0x0000ffff) | (data << 16);
			verbose3("DMA#%i:  Base Address write : %08x\n",chan,v68.dmac.channels[chan].bar);
			break;
		case 0x0f:  // BAR (low)
			v68.dmac.channels[chan].bar = (v68.dmac.channels[chan].bar & 0xffff0000) | (data & 0x0000ffff);
			verbose3("DMA#%i:  Base Address write : %08x\n",chan,v68.dmac.channels[chan].bar);
			break;
		case 0x12:  // NIV
			v68.dmac.channels[chan].niv = data & 0xff;
			verbose3("DMA#%i:  Normal IRQ Vector write : %02x\n",chan,v68.dmac.channels[chan].niv);
			break;
		case 0x13:  // EIV
			v68.dmac.channels[chan].eiv = data & 0xff;
			verbose3("DMA#%i:  Error IRQ Vector write : %02x\n",chan,v68.dmac.channels[chan].eiv);
			break;
		case 0x14:  // MFC
			v68.dmac.channels[chan].mfc = data & 0xff;
			verbose3("DMA#%i:  Memory Function Code write : %02x\n",chan,v68.dmac.channels[chan].mfc);
			break;
		case 0x16:  // CPR
			v68.dmac.channels[chan].cpr = data & 0xff;
			verbose3("DMA#%i:  Channel Priority write : %02x\n",chan,v68.dmac.channels[chan].cpr);
			break;
		case 0x18:  // DFC
			v68.dmac.channels[chan].dfc = data & 0xff;
			verbose3("DMA#%i:  Device Function Code write : %02x\n",chan,v68.dmac.channels[chan].dfc);
			break;
		case 0x1c:  // BFC
			v68.dmac.channels[chan].bfc = data & 0xff;
			verbose3("DMA#%i:  Base Function Code write : %02x\n",chan,v68.dmac.channels[chan].bfc);
			break;
		case 0x1f:
			v68.dmac.channels[chan].gcr = data & 0xff;
			verbose3("DMA#%i:  General Control write : %02x\n",chan,v68.dmac.channels[chan].gcr);
			break;
	}
}

uint16_t dmac_read_16(uint32_t offset, uint16_t mem_mask) {
	int chan = (offset >> 6) & 0x03;
	int reg = (offset & 0x1f) >> 1;

	verbose3("dmac_read_16 offset=0x%08x mem_mask=0x%04x chan=0x%02x reg=0x%02x\n", offset, mem_mask, chan, reg);

	switch(reg)
	{
	case 0x00:  // CSR / CER
		return (v68.dmac.channels[chan].csr << 8) | v68.dmac.channels[chan].cer;
	case 0x02:  // DCR / OCR
		return (v68.dmac.channels[chan].dcr << 8) | v68.dmac.channels[chan].ocr;
	case 0x03:  // SCR / CCR
		return (v68.dmac.channels[chan].scr << 8) | v68.dmac.channels[chan].ccr;
	case 0x05:  // MTC
		return v68.dmac.channels[chan].mtc;
	case 0x06:  // MAR (high)
		return (v68.dmac.channels[chan].mar & 0xffff0000) >> 16;
	case 0x07:  // MAR (low)
		return (v68.dmac.channels[chan].mar & 0x0000ffff);
	case 0x0a:  // DAR (high)
		return (v68.dmac.channels[chan].dar & 0xffff0000) >> 16;
	case 0x0b:  // DAR (low)
		return (v68.dmac.channels[chan].dar & 0x0000ffff);
	case 0x0d:  // BTC
		return v68.dmac.channels[chan].btc;
	case 0x0e:  // BAR (high)
		return (v68.dmac.channels[chan].bar & 0xffff0000) >> 16;
	case 0x0f:  // BAR (low)
		return (v68.dmac.channels[chan].bar & 0x0000ffff);
	case 0x12:  // NIV
		return v68.dmac.channels[chan].niv;
	case 0x13:  // EIV
		return v68.dmac.channels[chan].eiv;
	case 0x14:  // MFC
		return v68.dmac.channels[chan].mfc;
	case 0x16:  // CPR
		return v68.dmac.channels[chan].cpr;
	case 0x18:  // DFC
		return v68.dmac.channels[chan].dfc;
	case 0x1c:  // BFC
		return v68.dmac.channels[chan].bfc;
	case 0x1f:  // GCR
		return v68.dmac.channels[chan].gcr;
	}
	return 0xff;
}
