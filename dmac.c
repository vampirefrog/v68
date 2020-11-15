// Code copied from XM6 2.05
// and translated to C
// some code commented out, as it wasn't needed for v68

#include <string.h>

#include "v68.h"
#include "musashi/m68k.h"
#include "dmac.h"

#define ASSERT(...)

static uint32_t dmac_channel_read(struct dmac *dmac, int ch, uint32_t addr);
static void     dmac_channel_write(struct dmac *dmac, int ch, uint32_t addr, uint32_t data);
static void     dmac_channel_set_dcr(struct dmac *dmac, int ch, uint32_t data);
static uint32_t dmac_channel_get_dcr(struct dmac *dmac, int ch);
static void     dmac_channel_set_ocr(struct dmac *dmac, int ch, uint32_t data);
static uint32_t dmac_channel_get_ocr(struct dmac *dmac, int ch);
static void     dmac_channel_set_scr(struct dmac *dmac, int ch, uint32_t data);
static uint32_t dmac_channel_get_scr(struct dmac *dmac, int ch);
static void     dmac_channel_set_ccr(struct dmac *dmac, int ch, uint32_t data);
static uint32_t dmac_channel_get_ccr(struct dmac *dmac, int ch);
static void     dmac_channel_set_csr(struct dmac *dmac, int ch, uint32_t data);
static uint32_t dmac_channel_get_csr(struct dmac *dmac, int ch);
static void     dmac_channel_set_gcr(struct dmac *dmac, uint32_t data);
static void     dmac_channel_reset(struct dmac *dmac, int ch);
static void     dmac_channel_start(struct dmac *dmac, int ch);
static void     dmac_channel_continue(struct dmac *dmac, int ch);
static void     dmac_channel_abort(struct dmac *dmac, int ch);
static void     dmac_channel_load(struct dmac *dmac, int ch);
static void     dmac_channel_set_error(struct dmac *dmac, int ch, uint32_t code);
static void     dmac_interrupt(struct dmac *dmac);
static int      dmac_channel_transfer(struct dmac *dmac, int ch);

static int mem_diff_table[8][4] = {
	{ 0, 1, -1, 0},		// 8bit, バイト
	{ 0, 2, -2, 0},		// 8bit, ワード
	{ 0, 4, -4, 0},		// 8bit, ロングワード
	{ 0, 1, -1, 0},		// 8bit, パックバイト
	{ 0, 1, -1, 0},		// 16bit, バイト
	{ 0, 2, -2, 0},		// 16bit, ワード
	{ 0, 4, -4, 0},		// 16bit, ロングワード
	{ 0, 1, -1, 0}		// 16bit, パックバイト
};

static int dev_diff_table[8][4] = {
	{ 0, 2, -2, 0},		// 8bit, バイト
	{ 0, 4, -4, 0},		// 8bit, ワード
	{ 0, 8, -8, 0},		// 8bit, ロングワード
	{ 0, 2, -2, 0},		// 8bit, パックバイト
	{ 0, 1, -1, 0},		// 16bit, バイト
	{ 0, 2, -2, 0},		// 16bit, ワード
	{ 0, 4, -4, 0},		// 16bit, ロングワード
	{ 0, 1, -1, 0}		// 16bit, パックバイト
};

int dmac_init(struct dmac *dmac) {
	for(int ch = 0; ch < 4; ch++) {
		memset(&dmac->channels[ch], 0, sizeof(dmac->channels[ch]));
	}

	return 1;
}

void dmac_deinit(struct dmac *dmac) {
}

void dmac_reset(struct dmac *dmac) {
	int ch;

	// グローバル
	dmac->control.transfer = 0;
	dmac->control.load = 0;
	dmac->control.exec = 0;
	dmac->control.current_ch = 0;
	dmac->control.cpu_cycle = 0;
	dmac->control.vector = -1;

	// DMACチャネルを回る
	for (ch=0; ch<4; ch++) {
		dmac_channel_reset(dmac, ch);
	}
}

uint32_t dmac_read_8(struct dmac *dmac, uint32_t addr) {
	ASSERT(dmac);
	ASSERT((addr >= memdev.first) && (addr <= memdev.last));

	// ウェイト
	//scheduler->Wait(7);

	// チャネルに割り当て
	int ch = (int)(addr >> 6);
	ch &= 3;
	addr &= 0x3f;

	// チャネル単位で行う
	return dmac_channel_read(dmac, ch, addr);
}

uint32_t dmac_read_16(struct dmac *dmac, uint32_t addr) {
	ASSERT((addr >= memdev.first) && (addr <= memdev.last));
	ASSERT((addr & 1) == 0);

	// ウェイト
	//scheduler->Wait(7);

	// チャネルに割り当て
	int ch = (int)(addr >> 6);
	ch &= 3;
	addr &= 0x3f;

	// チャネル単位で行う
	return ((dmac_channel_read(dmac, ch, addr) << 8) | dmac_channel_read(dmac, ch, addr + 1));
}

void dmac_write_8(struct dmac *dmac, uint32_t addr, uint32_t data) {
	ASSERT((addr >= memdev.first) && (addr <= memdev.last));
	ASSERT(data < 0x100);

	// ウェイト
	// scheduler->Wait(7);

	// チャネルに割り当て
	int ch = (int)(addr >> 6);
	ch &= 3;
	addr &= 0x3f;

	// チャネル単位で行う
	dmac_channel_write(dmac, ch, addr, data);
}

void dmac_write_16(struct dmac *dmac, uint32_t addr, uint32_t data) {
	ASSERT((addr >= memdev.first) && (addr <= memdev.last));
	ASSERT((addr & 1) == 0);
	ASSERT(data < 0x10000);

	// ウェイト
	// scheduler->Wait(7);

	// チャネルに割り当て
	int ch = (int)(addr >> 6);
	ch &= 3;
	addr &= 0x3f;

	// チャネル単位で行う
	dmac_channel_write(dmac, ch, addr, (uint8_t)(data >> 8));
	dmac_channel_write(dmac, ch, addr + 1, (uint8_t)data);
}

static uint32_t dmac_channel_read(struct dmac *dmac, int ch, uint32_t addr) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(addr <= 0x3f);

	switch (addr) {
		// CSR
		case 0x00:
			return dmac_channel_get_csr(dmac, ch);

		// CER
		case 0x01:
			return dmac->channels[ch].ecode;

		// DCR
		case 0x04:
			return dmac_channel_get_dcr(dmac, ch);

		// OCR
		case 0x05:
			return dmac_channel_get_ocr(dmac, ch);

		// SCR
		case 0x06:
			return dmac_channel_get_scr(dmac, ch);

		// CCR
		case 0x07:
			return dmac_channel_get_ccr(dmac, ch);

		// MTC
		case 0x0a:
			return (uint8_t)(dmac->channels[ch].mtc >> 8);
		case 0x0b:
			return (uint8_t)dmac->channels[ch].mtc;

		// MAR
		case 0x0c:
			return 0;
		case 0x0d:
			return (uint8_t)(dmac->channels[ch].mar >> 16);
		case 0x0e:
			return (uint8_t)(dmac->channels[ch].mar >> 8);
		case 0x0f:
			return (uint8_t)dmac->channels[ch].mar;

		// DAR
		case 0x14:
			return 0;
		case 0x15:
			return (uint8_t)(dmac->channels[ch].dar >> 16);
		case 0x16:
			return (uint8_t)(dmac->channels[ch].dar >> 8);
		case 0x17:
			return (uint8_t)dmac->channels[ch].dar;

		// BTC
		case 0x1a:
			return (uint8_t)(dmac->channels[ch].btc >> 8);
		case 0x1b:
			return (uint8_t)dmac->channels[ch].btc;

		// BAR
		case 0x1c:
			return 0;
		case 0x1d:
			return (uint8_t)(dmac->channels[ch].bar >> 16);
		case 0x1e:
			return (uint8_t)(dmac->channels[ch].bar >> 8);
		case 0x1f:
			return (uint8_t)dmac->channels[ch].bar;

		// NIV
		case 0x25:
			return dmac->channels[ch].niv;

		// EIV
		case 0x27:
			return dmac->channels[ch].eiv;

		// MFC
		case 0x29:
			return dmac->channels[ch].mfc;

		// CPR
		case 0x2d:
			return dmac->channels[ch].cp;

		// DFC
		case 0x31:
			return dmac->channels[ch].dfc;

		// BFC
		case 0x39:
			return dmac->channels[ch].bfc;

		// GCR
		case 0x3f:
			if (ch == 3) {
				// チャネル3のみバースト転送情報を返す
				ASSERT(dmac->channels[ch].bt <= 3);
				ASSERT(dmac->channels[ch].br <= 3);

				uint32_t data = dmac->channels[ch].bt;
				data <<= 2;
				data |= dmac->channels[ch].br;
				return data;
			}
			return 0xff;
	}

	return 0xff;
}

static void dmac_channel_write(struct dmac *dmac, int ch, uint32_t addr, uint32_t data) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(addr <= 0x3f);
	ASSERT(data < 0x100);

	switch (addr) {
		// CSR
		case 0x00:
			dmac_channel_set_csr(dmac, ch, data);
			return;

		// CER(Read Only)
		case 0x01:
			return;

		// DCR
		case 0x04:
			dmac_channel_set_dcr(dmac, ch, data);
			return;

		// OCR
		case 0x05:
			dmac_channel_set_ocr(dmac, ch, data);
			return;

		// SCR
		case 0x06:
			dmac_channel_set_scr(dmac, ch, data);
			return;

		// CCR
		case 0x07:
			dmac_channel_set_ccr(dmac, ch, data);
			return;

		// MTC
		case 0x0a:
			dmac->channels[ch].mtc &= 0x00ff;
			dmac->channels[ch].mtc |= (data << 8);
			return;
		case 0x0b:
			dmac->channels[ch].mtc &= 0xff00;
			dmac->channels[ch].mtc |= data;
			return;

		// MAR
		case 0x0c:
			return;
		case 0x0d:
			dmac->channels[ch].mar &= 0x0000ffff;
			dmac->channels[ch].mar |= (data << 16);
			return;
		case 0x0e:
			dmac->channels[ch].mar &= 0x00ff00ff;
			dmac->channels[ch].mar |= (data << 8);
			return;
		case 0x0f:
			dmac->channels[ch].mar &= 0x00ffff00;
			dmac->channels[ch].mar |= data;
			return;

		// DAR
		case 0x14:
			return;
		case 0x15:
			dmac->channels[ch].dar &= 0x0000ffff;
			dmac->channels[ch].dar |= (data << 16);
			return;
		case 0x16:
			dmac->channels[ch].dar &= 0x00ff00ff;
			dmac->channels[ch].dar |= (data << 8);
			return;
		case 0x17:
			dmac->channels[ch].dar &= 0x00ffff00;
			dmac->channels[ch].dar |= data;
			return;

		// BTC
		case 0x1a:
			dmac->channels[ch].btc &= 0x00ff;
			dmac->channels[ch].btc |= (data << 8);
			return;
		case 0x1b:
			dmac->channels[ch].btc &= 0xff00;
			dmac->channels[ch].btc |= data;
			return;

		// BAR
		case 0x1c:
			return;
		case 0x1d:
			dmac->channels[ch].bar &= 0x0000ffff;
			dmac->channels[ch].bar |=(data << 16);
			return;
		case 0x1e:
			dmac->channels[ch].bar &= 0x00ff00ff;
			dmac->channels[ch].bar |= (data << 8);
			return;
		case 0x1f:
			dmac->channels[ch].bar &= 0x00ffff00;
			dmac->channels[ch].bar |= data;
			return;

		// NIV
		case 0x25:
			dmac->channels[ch].niv = data;
			return;

		// EIV
		case 0x27:
			dmac->channels[ch].eiv = data;
			return;

		// MFC
		case 0x29:
			dmac->channels[ch].mfc = data;
			return;

		// CPR
		case 0x2d:
			dmac->channels[ch].cp = data & 0x03;
			return;

		// DFC
		case 0x31:
			dmac->channels[ch].dfc = data;
			return;

		// BFC
		case 0x39:
			dmac->channels[ch].bfc = data;
			return;

		// GCR
		case 0x3f:
			if (ch == 3) {
				// チャネル3のみ
				dmac_channel_set_gcr(dmac, data);
			}
			return;
	}
}

static void dmac_channel_set_dcr(struct dmac *dmac, int ch, uint32_t data) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(data < 0x100);

	// ACTが上がっていればタイミングエラー
	if (dmac->channels[ch].act) {
		printf("setting error 0x02\n");
		dmac_channel_set_error(dmac, ch, 0x02);
		return;
	}

	// XRM
	dmac->channels[ch].xrm = data >> 6;

	// DTYP
	dmac->channels[ch].dtyp = (data >> 4) & 0x03;

	// DPS
	if (data & 0x08) {
		dmac->channels[ch].dps = 1;
	}
	else {
		dmac->channels[ch].dps = 0;
	}

	// PCL
	dmac->channels[ch].pcl = (data & 0x03);

	// 割り込みチェック
	dmac_interrupt(dmac);
}

static uint32_t dmac_channel_get_dcr(struct dmac *dmac, int ch) {
	uint32_t data;

	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(dmac->channels[ch].xrm <= 3);
	ASSERT(dmac->channels[ch].dtyp <= 3);
	ASSERT(dmac->channels[ch].pcl <= 3);

	// データ作成
	data = dmac->channels[ch].xrm;
	data <<= 2;
	data |= dmac->channels[ch].dtyp;
	data <<= 1;
	if (dmac->channels[ch].dps) {
		data |= 0x01;
	}
	data <<= 3;
	data |= dmac->channels[ch].pcl;

	return data;
}

static void dmac_channel_set_ocr(struct dmac *dmac, int ch, uint32_t data) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(data < 0x100);

	// ACTが上がっていればタイミングエラー
	if (dmac->channels[ch].act) {
		printf("ocr setting error 0x02 on ch %d\n", ch);
		dmac_channel_set_error(dmac, ch, 0x02);
		return;
	}

	// DIR
	if (data & 0x80) {
		dmac->channels[ch].dir = 1;
	}
	else {
		dmac->channels[ch].dir = 0;
	}

	// BTD
	if (data & 0x40) {
		dmac->channels[ch].btd = 1;
	}
	else {
		dmac->channels[ch].btd = 0;
	}

	// SIZE
	dmac->channels[ch].size = (data >> 4) & 0x03;

	// CHAIN
	dmac->channels[ch].chain = (data >> 2) & 0x03;

	// REQG
	dmac->channels[ch].reqg = (data & 0x03);
}

static uint32_t dmac_channel_get_ocr(struct dmac *dmac, int ch) {
	uint32_t data;

	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(dmac->channels[ch].size <= 3);
	ASSERT(dmac->channels[ch].chain <= 3);
	ASSERT(dmac->channels[ch].reqg <= 3);

	// データ作成
	data = 0;
	if (dmac->channels[ch].dir) {
		data |= 0x02;
	}
	if (dmac->channels[ch].btd) {
		data |= 0x01;
	}
	data <<= 2;
	data |= dmac->channels[ch].size;
	data <<= 2;
	data |= dmac->channels[ch].chain;
	data <<= 2;
	data |= dmac->channels[ch].reqg;

	return data;
}

static void dmac_channel_set_scr(struct dmac *dmac, int ch, uint32_t data) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(data < 0x100);

	// ACTが上がっていればタイミングエラー
	if (dmac->channels[ch].act) {
		dmac_channel_set_error(dmac, ch, 0x02);
		return;
	}

	dmac->channels[ch].mac = (data >> 2) & 0x03;
	dmac->channels[ch].dac = (data & 0x03);
}

static uint32_t dmac_channel_get_scr(struct dmac *dmac, int ch) {
	uint32_t data;

	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(dmac->channels[ch].mac <= 3);
	ASSERT(dmac->channels[ch].dac <= 3);

	// データ作成
	data = dmac->channels[ch].mac;
	data <<= 2;
	data |= dmac->channels[ch].dac;

	return data;
}

static void dmac_channel_set_ccr(struct dmac *dmac, int ch, uint32_t data) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(data < 0x100);

	// INT
	if (data & 0x08) {
		dmac->channels[ch].intr = 1;
	} else {
		dmac->channels[ch].intr = 0;
	}

	// HLT
	if (data & 0x20) {
		dmac->channels[ch].hlt = 1;
	} else {
		dmac->channels[ch].hlt = 0;
	}

	// STR
	if (data & 0x80) {
		dmac->channels[ch].str = 1;
		dmac_channel_start(dmac, ch);
	}

	// CNT
	if (data & 0x40) {
		dmac->channels[ch].cnt = 1;
		dmac_channel_continue(dmac, ch);
	}

	// SAB
	if (data & 0x10) {
		dmac->channels[ch].sab = 1;
		dmac_channel_abort(dmac, ch);
	}
}

static uint32_t dmac_channel_get_ccr(struct dmac *dmac, int ch) {
	uint32_t data;

	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));

	// INT,HLT,STR,CNTだけ返す
	data = 0;
	if (dmac->channels[ch].intr) {
		data |= 0x08;
	}
	if (dmac->channels[ch].hlt) {
		data |= 0x20;
	}
	if (dmac->channels[ch].str) {
		data |= 0x80;
	}
	if (dmac->channels[ch].cnt) {
		data |= 0x40;
	}

	return data;
}

static void dmac_channel_set_csr(struct dmac *dmac, int ch, uint32_t data) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(data < 0x100);

	// ACT,PCS以外は1を書き込むことによりクリアできる
	if (data & 0x80) {
		dmac->channels[ch].coc = 0;
	}
	if (data & 0x40) {
		dmac->channels[ch].boc = 0;
	}
	if (data & 0x20) {
		dmac->channels[ch].ndt = 0;
	}
	if (data & 0x10) {
		dmac->channels[ch].err = 0;
	}
	if (data & 0x04) {
		dmac->channels[ch].dit = 0;
	}
	if (data & 0x02) {
		dmac->channels[ch].pct = 0;
	}

	// 割り込み処理
	dmac_interrupt(dmac);
}

static uint32_t dmac_channel_get_csr(struct dmac *dmac, int ch) {
	uint32_t data;

	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));

	// データ作成
	data = 0;
	if (dmac->channels[ch].coc) {
		data |= 0x80;
	}
	if (dmac->channels[ch].boc) {
		data |= 0x40;
	}
	if (dmac->channels[ch].ndt) {
		data |= 0x20;
	}
	if (dmac->channels[ch].err) {
		data |= 0x10;
	}
	if (dmac->channels[ch].act) {
		data |= 0x08;
	}
	if (dmac->channels[ch].dit) {
		data |= 0x04;
	}
	if (dmac->channels[ch].pct) {
		data |= 0x02;
	}
	if (dmac->channels[ch].pcs) {
		data |= 0x01;
	}

	return data;
}

static void dmac_channel_set_gcr(struct dmac *dmac, uint32_t data) {
	int ch;
	uint32_t bt;
	uint32_t br;

	ASSERT(dmac);
	ASSERT(data < 0x100);

	// データ分離
	bt = (data >> 2) & 0x03;
	br = data & 0x03;

	// 全チャネルに設定
	for (ch=0; ch<4; ch++) {
		dmac->channels[ch].bt = bt;
		dmac->channels[ch].br = br;
	}
}

static void dmac_channel_reset(struct dmac *dmac, int ch) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));

	// GCR初期化
	dmac->channels[ch].bt = 0;
	dmac->channels[ch].br = 0;

	// DCR初期化
	dmac->channels[ch].xrm = 0;
	dmac->channels[ch].dtyp = 0;
	dmac->channels[ch].dps = 0;
	dmac->channels[ch].pcl = 0;

	// OCR初期化
	dmac->channels[ch].dir = 0;
	dmac->channels[ch].btd = 0;
	dmac->channels[ch].size = 0;
	dmac->channels[ch].chain = 0;
	dmac->channels[ch].reqg = 0;

	// SCR初期化
	dmac->channels[ch].mac = 0;
	dmac->channels[ch].dac = 0;

	// CCR初期化
	dmac->channels[ch].str = 0;
	dmac->channels[ch].cnt = 0;
	dmac->channels[ch].hlt = 0;
	dmac->channels[ch].sab = 0;
	dmac->channels[ch].intr = 0;

	// CSR初期化
	dmac->channels[ch].coc = 0;
	dmac->channels[ch].boc = 0;
	dmac->channels[ch].ndt = 0;
	dmac->channels[ch].err = 0;
	dmac->channels[ch].act = 0;
	dmac->channels[ch].dit = 0;
	dmac->channels[ch].pct = 0;
	if (ch == 0) {
		// FDCは'L'
		dmac->channels[ch].pcs = 0;
	} else {
		// それ以外は'H'
		dmac->channels[ch].pcs = 1;
	}

	// CPR初期化
	dmac->channels[ch].cp = 0;

	// CER初期化
	dmac->channels[ch].ecode = 0;

	// 割り込みベクタ初期化
	dmac->channels[ch].niv = 0x0f;
	dmac->channels[ch].eiv = 0x0f;

	// アドレス及びカウンタはリセットしない(データシートによる)
	dmac->channels[ch].mar &= 0x00ffffff;
	dmac->channels[ch].dar &= 0x00ffffff;
	dmac->channels[ch].bar &= 0x00ffffff;
	dmac->channels[ch].mtc &= 0x0000ffff;
	dmac->channels[ch].btc &= 0x0000ffff;

	// 転送タイプ、カウンタ初期化
	dmac->channels[ch].type = 0;
	dmac->channels[ch].startcnt = 0;
	dmac->channels[ch].errorcnt = 0;
}

static void dmac_channel_start(struct dmac *dmac, int ch) {
	int c;

	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));

	// ACT,COC,BOC,NDT,ERRが上がっていればタイミングエラー
	if (dmac->channels[ch].act || dmac->channels[ch].coc || dmac->channels[ch].boc || dmac->channels[ch].ndt || dmac->channels[ch].err) {
		dmac_channel_set_error(dmac, ch, 0x02);
		return;
	}

	// チェインなしの場合は、MTC=0ならメモリカウントエラー
	if (dmac->channels[ch].chain == 0) {
		if (dmac->channels[ch].mtc == 0) {
			dmac_channel_set_error(dmac, ch, 0x0d);
			return;
		}
	}

	// アレイチェインの場合は、BTC=0ならベースカウントエラー
	if (dmac->channels[ch].chain == 0x02) {
		if (dmac->channels[ch].btc == 0) {
			dmac_channel_set_error(dmac, ch, 0x0f);
			return;
		}
	}

	// コンフィギュレーションエラーチェック
	if ((dmac->channels[ch].xrm == 0x01) || (dmac->channels[ch].mac == 0x03) || (dmac->channels[ch].dac == 0x03)
			|| (dmac->channels[ch].chain == 0x01)) {
		dmac_channel_set_error(dmac, ch, 0x01);
		return;
	}

	// 転送タイプ作成
	dmac->channels[ch].type = 0;
	if (dmac->channels[ch].dps) {
		dmac->channels[ch].type += 4;
	}
	dmac->channels[ch].type += dmac->channels[ch].size;

	// ワーク初期化
	dmac->channels[ch].str = 0;
	dmac->channels[ch].act = 1;
	dmac->channels[ch].cnt = 0;
	dmac->channels[ch].sab = 0;

	// カウントアップ
	dmac->channels[ch].startcnt++;

	// アレイチェインまたはリンクアレイチェインは、最初のブロックをロード
	if (dmac->channels[ch].chain != 0) {
		dmac_channel_load(dmac, ch);
		// ロード時にアドレスエラーまたはバスエラーが起きたら、エラーフラグが上がる
		if (dmac->channels[ch].err) {
			return;
		}
	}

	// CPUサイクルをクリアして、モード別
	dmac->control.cpu_cycle = 0;
	switch (dmac->channels[ch].reqg) {
		// オートリクエスト限定
		case 0:
		// オートリクエスト最大
		case 1:
			// 現在の残りだけDMAを動かして、CPUを止める
			dmac->control.current_ch = ch;
			dmac->control.cpu_cycle = 0;
			dmac->control.exec = 1;
			// scheduler->dma_active = 1;
			c = dmac_channel_auto(dmac, 0/*cpu->Get_IOCycle()*/);
			if (c == 0) {
				//cpu->Release();
			}
			break;

		// 外部要求転送
		case 2:
			break;

		// オートリクエスト＋外部要求転送
		case 3:
			// 現在の残りだけDMAを動かして、CPUを止める
			dmac->control.current_ch = ch;
			dmac->control.cpu_cycle = 0;
			dmac->control.exec = 1;
			// scheduler->dma_active= 1;
			dmac->channels[ch].reqg = 1;
			c = dmac_channel_auto(dmac, 0/*cpu->Get_IOCycle()*/);
			if (c == 0) {
				//cpu->Release();
			}
			dmac->channels[ch].reqg = 3;
			break;

		default:
			ASSERT(0);
	}
}

static void dmac_channel_continue(struct dmac *dmac, int ch) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));

	// ACTが上がっていないと動作タイミングエラー
	if (!dmac->channels[ch].act) {
		dmac_channel_set_error(dmac, ch, 0x02);
		return;
	}
	// チェインモードの場合はコンフィグエラー
	if (dmac->channels[ch].chain != 0) {
		dmac_channel_set_error(dmac, ch, 0x01);
	}
}

static void dmac_channel_abort(struct dmac *dmac, int ch) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));

	// 非アクティブならエラー処理を行わない(Marianne.pan)
	if (!dmac->channels[ch].act) {
		// さらにCOCを落とす(バラデューク)
		dmac->channels[ch].coc = 0;
		return;
	}

	// 転送完了、非アクティブ
	dmac->channels[ch].coc = 1;
	dmac->channels[ch].act = 0;

	// ソフトウェアアボートでエラー発生
	dmac_channel_set_error(dmac, ch, 0x11);
}

static void dmac_channel_load(struct dmac *dmac, int ch) {
	uint32_t base;

	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(dmac->control.load == 0);

	// ロード中(read_16でのアドレスエラー、バスエラーに注意)
	dmac->control.load = (ch + 1);

	if (dmac->channels[ch].bar & 1) {
		// BARアドレスエラー
		dmac_addr_err(dmac, dmac->channels[ch].bar, 1);

		dmac->control.load = 0;
		return;
	}

	// MAR読み込み
	dmac->channels[ch].bar &= 0xfffffe;
	dmac->channels[ch].mar = (m68k_read_memory_16(dmac->channels[ch].bar) & 0x00ff);
	dmac->channels[ch].bar += 2;
	dmac->channels[ch].mar <<= 16;
	dmac->channels[ch].bar &= 0xfffffe;
	dmac->channels[ch].mar |= (m68k_read_memory_16(dmac->channels[ch].bar) & 0xffff);
	dmac->channels[ch].bar += 2;

	// MTC読み込み
	dmac->channels[ch].bar &= 0xfffffe;
	dmac->channels[ch].mtc = (m68k_read_memory_16(dmac->channels[ch].bar) & 0xffff);
	dmac->channels[ch].bar += 2;

	if (dmac->channels[ch].err) {
		// MAR,MTC読み込みエラー
		dmac->control.load = 0;
		return;
	}

	// アレイチェインではここまで
	if (dmac->channels[ch].chain == 0x02) {
		dmac->channels[ch].btc--;
		dmac->control.load = 0;
		return;
	}

	// リンクアレイチェイン(では次のリンクアドレスをBARへロード
	dmac->channels[ch].bar &= 0xfffffe;
	base = (m68k_read_memory_16(dmac->channels[ch].bar) & 0x00ff);
	dmac->channels[ch].bar += 2;
	base <<= 16;
	dmac->channels[ch].bar &= 0xfffffe;
	base |= (m68k_read_memory_16(dmac->channels[ch].bar) & 0xffff);
	dmac->channels[ch].bar = base;

	// ロード終了
	dmac->control.load = 0;
}

static void dmac_channel_set_error(struct dmac *dmac, int ch, uint32_t code) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT((code >= 0x01) && (code <= 17));

	printf("setting error %02x on channel %d\n", code, ch);

	// ACTを降ろす(ファランクス ADPCM)
	dmac->channels[ch].act = 0;

	// エラーコードを書き込む
	dmac->channels[ch].ecode = code;

	// エラーフラグを立てる
	dmac->channels[ch].err = 1;

	// カウントアップ
	dmac->channels[ch].errorcnt++;

	// 割り込み処理
	dmac_interrupt(dmac);
}

static void dmac_interrupt(struct dmac *dmac) {
	uint32_t cp;
	int ch;

	ASSERT(dmac);

	// DMAと同じ優先度で処理(データシートより)
	for (cp=0; cp<=3; cp++) {
		for (ch=0; ch<=3; ch++) {
			// CPチェック
			if (cp != dmac->channels[ch].cp) {
				continue;
			}

			// インタラプトイネーブルをチェック
			if (!dmac->channels[ch].intr) {
				continue;
			}

			// ERRによりEIVで出力
			if (dmac->channels[ch].err) {
				if (dmac->control.vector != (int)dmac->channels[ch].eiv) {
					// 別の割り込みを要求していれば、一旦キャンセル
					if (dmac->control.vector >= 0) {
						// cpu->Int_Cancel(3);
					}
					v68.int_vec = dmac->channels[ch].eiv;
					m68k_set_irq(3);
					dmac->control.vector = (int)dmac->channels[ch].eiv;
				}
				return;
			}

			// COC,BOC,NDT,PCT(割り込みラインの場合)にNIVで出力
			if (dmac->channels[ch].coc || dmac->channels[ch].boc || dmac->channels[ch].ndt) {
				if (dmac->control.vector != (int)dmac->channels[ch].niv) {
					// 別の割り込みを要求していれば、一旦キャンセル
					if (dmac->control.vector >= 0) {
						// cpu->Int_Cancel(3);
					}
					v68.int_vec = dmac->channels[ch].niv;
					m68k_set_irq(3);
					dmac->control.vector = (int)dmac->channels[ch].niv;
				}
				return;
			}

			if ((dmac->channels[ch].pcl == 0x01) && dmac->channels[ch].pct) {
				if (dmac->control.vector != (int)dmac->channels[ch].niv) {
					// 別の割り込みを要求していれば、一旦キャンセル
					if (dmac->control.vector >= 0) {
						// cpu->Int_Cancel(3);
					}
					v68.int_vec = dmac->channels[ch].niv;
					m68k_set_irq(3);
					// cpu->Interrupt(3, (uint8_t)dmac->channels[ch].niv);
					dmac->control.vector = (int)dmac->channels[ch].niv;
				}
				return;
			}
		}
	}

	// 要求中の割り込みはない
	if (dmac->control.vector >= 0) {
		// cpu->Int_Cancel(3);
		dmac->control.vector = -1;
	}
}

void dmac_int_ack(struct dmac *dmac) {
	ASSERT(dmac);

	// リセット直後に、CPUから割り込みが間違って入る場合がある
	if (dmac->control.vector < 0) {
		// LOG0(Log::Warning, "要求していない割り込み");
		return;
	}

	// 要求中ベクタなし
	dmac->control.vector = -1;
}

int dmac_is_act(struct dmac *dmac, int ch) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));

	// ACTでないか、ERRか、HLTなら転送できない
	if (!dmac->channels[ch].act || dmac->channels[ch].err || dmac->channels[ch].hlt) {
		return 0;
	}

	// 転送できる
	return 1;
}

void dmac_bus_err(struct dmac *dmac, uint32_t addr, int read) {
	// ロード中のエラーか
	if (dmac->control.load != 0) {
		// メモリ・デバイス・ベースの区別は考慮していない
		ASSERT((dmac->control.load >= 1) && (dmac->control.load <= 4));
		dmac_channel_set_error(dmac, dmac->control.load - 1, 0x08);
		return;
	}

	// メモリ・デバイス・ベースの区別は考慮していない
	ASSERT((dmac->control.transfer >= 1) && (dmac->control.transfer <= 4));
	dmac_channel_set_error(dmac, dmac->control.transfer - 1, 0x08);
}

void dmac_addr_err(struct dmac *dmac, uint32_t addr, int read) {
	ASSERT(dmac);
	ASSERT(addr <= 0xffffff);

	if (read) {
		// LOG1(Log::Warning, "DMAアドレスエラー(読み込み) $%06X", addr);
	}
	else {
		// LOG1(Log::Warning, "DMAアドレスエラー(書き込み) $%06X", addr);
	}

	// ロード中のエラーか
	if (dmac->control.load != 0) {
		// メモリ・デバイス・ベースの区別は考慮していない
		ASSERT((dmac->control.load >= 1) && (dmac->control.load <= 4));
		dmac_channel_set_error(dmac, dmac->control.load - 1, 0x0c);
		return;
	}

	// メモリ・デバイス・ベースの区別は考慮していない
	ASSERT((dmac->control.transfer >= 1) && (dmac->control.transfer <= 4));
	dmac_channel_set_error(dmac, dmac->control.transfer - 1, 0x0c);
}

uint32_t dmac_get_vector(struct dmac *dmac, int type) {
	ASSERT(dmac);
	ASSERT((type >= 0) && (type < 8));

	// ノーマル・エラーのベクタを交互に出力
	if (type & 1) {
		// エラー
		return dmac->channels[type >> 1].eiv;
	}
	else {
		// ノーマル
		return dmac->channels[type >> 1].niv;
	}
}

int dmac_channel_request(struct dmac *dmac, int ch) {
	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));

	// ACTでないか、ERRか、HLTなら何もしない
	if (!dmac->channels[ch].act || dmac->channels[ch].err || dmac->channels[ch].hlt) {
		return 0;
	}

	// DMA転送
	dmac_channel_transfer(dmac, ch);
	return 1;
}

uint32_t dmac_channel_auto(struct dmac *dmac, uint32_t cycle) {
#if 0
	int i;
	int ch;
	int mul;
	int remain;
	int used;
	uint32_t backup;
	int flag;

	ASSERT(dmac);

	// パラメータ記憶
	remain = (int)cycle;

	// 実行フラグが上がっていなければオートリクエストは無し
	if (!dmac->control.exec) {
		return cycle;
	}

	// 実行継続フラグをリセット
	flag = 0;

	// 最大速度オートリクエストのチャネルを先に処理
	for (i=0; i<4; i++) {
		// 見るべきチャネルを決定
		ch = (dmac->control.current_ch + i) & 3;

		// ACT, ERR, HLTのチェック
		if (!dmac->channels[ch].act || dmac->channels[ch].err || dmac->channels[ch].hlt) {
			continue;
		}

		// 最大速度オートリクエストか
		if (dmac->channels[ch].reqg != 1) {
			continue;
		}

		// 加算して、最低でも10サイクルは必要。
		dmac->control.cpu_cycle += cycle;
		if (dmac->control.cpu_cycle < 10) {
			// CPUは実行できない。DMA継続
			return 0;
		}

		// 2回以上加算させない、フラグUP
		cycle = 0;
		flag = 1;

		// スケジューラのcycle(オーバーサイクル計算)を保持し、リセット
		backup = scheduler->Get_CPUCycle();
		scheduler->Set_CPUCycle(0);

		// cpu_cycleがマイナスになるまで実行。消費時間はスケジューラより得る
		while (scheduler->Get_CPUCycle() < dmac->control.cpu_cycle) {
			// ACT, ERR, HLTのチェック
			if (!dmac->channels[ch].act || dmac->channels[ch].err || dmac->channels[ch].hlt) {
				break;
			}

			// scheulder->Get_CPUCycle()を使い、DMAC消費サイクル数を取得する
			dmac_channel_transfer(dmac, ch);
		}

		// 消費サイクルを削り、復元
		dmac->control.cpu_cycle -= scheduler->Get_CPUCycle();
		remain -= scheduler->Get_CPUCycle();
		scheduler->Set_CPUCycle(backup);

		// チャネルを次へ(ラウンドロビン)
		dmac->control.current_ch = (dmac->control.current_ch + 1) & 3;

		// すべて時間を使い切ったか
		if (dmac->control.cpu_cycle <= 0) {
			// CPUは実行できない
			// ここで全チャネルを完了した場合、次のAudo_DMAでフラグ落とす
			return 0;
		}
	}

	// 最大速度オートリクエストがなかったか、完了して時間が余った
	// 限定速度オートリクエストのチャネルを処理
	for (i=0; i<4; i++) {
		// 見るべきチャネルを決定
		ch = (dmac->control.current_ch + i) & 3;

		// ACT, ERR, HLTのチェック
		if (!dmac->channels[ch].act || dmac->channels[ch].err || dmac->channels[ch].hlt) {
			continue;
		}

		// 最大速度オートリクエストはありえない(上の部分で必ず処理)
		ASSERT(dmac->channels[ch].reqg != 1);

		// 限定速度オートリクエストか
		if (dmac->channels[ch].reqg != 0) {
			continue;
		}

		// 加算して、最低でも10サイクルは必要。
		dmac->control.cpu_cycle += cycle;
		if (dmac->control.cpu_cycle < 10) {
			// CPUは実行できない。DMA継続
			return 0;
		}

		// 2回以上加算させない、フラグUP
		cycle = 0;
		flag = 1;

		// スケジューラのcycle(オーバーサイクル計算)を保持し、リセット
		backup = scheduler->Get_CPUCycle();
		scheduler->Set_CPUCycle(0);

		// バス占有率倍率を計算(BT=0なら2倍など)
		mul = (dmac->channels[ch].bt + 1);

		// cpu_cycleがバス占有率を考慮した値を超えているか
		while ((scheduler->Get_CPUCycle() << mul) < dmac->control.cpu_cycle) {
			// ACT, ERR, HLTのチェック
			if (!dmac->channels[ch].act || dmac->channels[ch].err || dmac->channels[ch].hlt) {
				break;
			}

			// 転送
			dmac_channel_transfer(dmac, ch);
		}

		// 使用サイクルを記憶(後で使うため)
		used = scheduler->Get_CPUCycle();
		scheduler->Set_CPUCycle(backup);

		// チャネルを次へ(ラウンドロビン)
		dmac->control.current_ch = (dmac->control.current_ch + 1) & 3;

		// ここで終了か
		if (dmac->control.cpu_cycle < (used << mul)) {
			// 予定されていたバス占有率を使い切った。残りはCPUに返却
			dmac->control.cpu_cycle -= used;
			if (used < remain) {
				// 十分余りがある
				return (remain - used);
			}
			else {
				// なぜか、使いすぎた。CPUは0
				return 0;
			}
		}

		// まだバスの使用が許されるので、他チャネルをまわる
		remain -= used;
	}

	if (!flag) {
		// DMAは使わなかった。dmac->control.execを降ろす
		dmac->control.exec = 0;
		scheduler->dma_active = 0;
	}

#endif
	return cycle;
}

static int dmac_channel_transfer(struct dmac *dmac, int ch) {
	uint32_t data;

	ASSERT(dmac);
	ASSERT((ch >= 0) && (ch <= 3));
	ASSERT(dmac->control.transfer == 0);

	// 転送フラグON
	dmac->control.transfer = ch + 1;

	// タイプ、ディレクションに応じて転送
	switch (dmac->channels[ch].type) {
		// 8bit, Packバイト, 8bitポート
		case 0:
		// 8bit, Unpackバイト, 8bitポート
		case 3:
		// 8bit, Unpackバイト, 16bitポート
		case 7:
			// SCSIディスク ベンチマーク(dskbench.x)より
			if (dmac->channels[ch].dir) {
				m68k_write_memory_8(dmac->channels[ch].mar, (uint8_t)(m68k_read_memory_8(dmac->channels[ch].dar)));
				// scheduler->Wait(11);
			}
			else {
				m68k_write_memory_8(dmac->channels[ch].dar, (uint8_t)(m68k_read_memory_8(dmac->channels[ch].mar)));
				// scheduler->Wait(11);
			}
			break;

		// 8bit, Packバイト, 16bitポート(Unpackより速くする:パロディウスだ!)
		// Wait12:パロディウスだ!、Wait?:Moon Fighter
		case 4:
			if (dmac->channels[ch].dir) {
				m68k_write_memory_8(dmac->channels[ch].mar, (uint8_t)(m68k_read_memory_8(dmac->channels[ch].dar)));
				// scheduler->Wait(10);
			}
			else {
				m68k_write_memory_8(dmac->channels[ch].dar, (uint8_t)(m68k_read_memory_8(dmac->channels[ch].mar)));
				// scheduler->Wait(10);
			}
			break;

		// 8bit, ワード
		case 1:
			if (dmac->channels[ch].dir) {
				data = (uint8_t)(m68k_read_memory_8(dmac->channels[ch].dar));
				data <<= 8;
				data |= (uint8_t)(m68k_read_memory_8(dmac->channels[ch].dar + 2));
				m68k_write_memory_16(dmac->channels[ch].mar, data);
				// scheduler->Wait(19);
			}
			else {
				data = m68k_read_memory_16(dmac->channels[ch].mar);
				m68k_write_memory_8(dmac->channels[ch].dar, (uint8_t)(data >> 8));
				m68k_write_memory_8(dmac->channels[ch].dar + 2, (uint8_t)data);
				// scheduler->Wait(19);
			}
			break;

		// 8bit, ロングワード
		case 2:
			if (dmac->channels[ch].dir) {
				data = (uint8_t)(m68k_read_memory_8(dmac->channels[ch].dar));
				data <<= 8;
				data |= (uint8_t)(m68k_read_memory_8(dmac->channels[ch].dar + 2));
				data <<= 8;
				data |= (uint8_t)(m68k_read_memory_8(dmac->channels[ch].dar + 4));
				data <<= 8;
				data |= (uint8_t)(m68k_read_memory_8(dmac->channels[ch].dar + 6));
				m68k_write_memory_16(dmac->channels[ch].mar, (uint16_t)(data >> 16));
				m68k_write_memory_16(dmac->channels[ch].mar + 2, (uint16_t)data);
				// scheduler->Wait(38);
			}
			else {
				data = m68k_read_memory_16(dmac->channels[ch].mar);
				data <<= 16;
				data |= (uint16_t)(m68k_read_memory_16(dmac->channels[ch].mar + 2));
				m68k_write_memory_8(dmac->channels[ch].dar, (uint8_t)(data >> 24));
				m68k_write_memory_8(dmac->channels[ch].dar + 2, (uint8_t)(data >> 16));
				m68k_write_memory_8(dmac->channels[ch].dar + 4, (uint8_t)(data >> 8));
				m68k_write_memory_8(dmac->channels[ch].dar + 6, (uint8_t)data);
				// scheduler->Wait(38);
			}
			break;

		// 16bit, ワード
		case 5:
			// あまり遅くするとFM音源割り込みがひきずられる(グラディウスII)
			if (dmac->channels[ch].dir) {
				data = m68k_read_memory_16(dmac->channels[ch].dar);
				m68k_write_memory_16(dmac->channels[ch].mar, (uint16_t)data);
				// scheduler->Wait(10);
			}
			else {
				data = m68k_read_memory_16(dmac->channels[ch].mar);
				m68k_write_memory_16(dmac->channels[ch].dar, (uint16_t)data);
				// scheduler->Wait(10);
			}
			break;

		// 16bit, ロングワード
		case 6:
			if (dmac->channels[ch].dir) {
				data = m68k_read_memory_16(dmac->channels[ch].dar);
				data <<= 16;
				data |= (uint16_t)(m68k_read_memory_16(dmac->channels[ch].dar + 2));
				m68k_write_memory_16(dmac->channels[ch].mar, (uint16_t)(data >> 16));
				m68k_write_memory_16(dmac->channels[ch].mar + 2, (uint16_t)data);
				// scheduler->Wait(20);
			}
			else {
				data = m68k_read_memory_16(dmac->channels[ch].mar);
				data <<= 16;
				data |= (uint16_t)m68k_read_memory_16(dmac->channels[ch].mar + 2);
				m68k_write_memory_16(dmac->channels[ch].dar, (uint16_t)(data >> 16));
				m68k_write_memory_16(dmac->channels[ch].dar + 2, (uint16_t)data);
				// scheduler->Wait(20);
			}
			break;

		// それ以外
		default:
			ASSERT(0);
	}

	// 転送フラグOFF
	dmac->control.transfer = 0;

	// 転送エラーのチェック(バスエラー及びアドレスエラー)
	if (dmac->channels[ch].err) {
		// アドレス更新前に抜ける(データシートによる)
		return 0;
	}

	// アドレス更新(12bitに制限:Racing Champ)
	dmac->channels[ch].mar += mem_diff_table[ dmac->channels[ch].type ][ dmac->channels[ch].mac ];
	dmac->channels[ch].mar &= 0xffffff;
	dmac->channels[ch].dar += dev_diff_table[ dmac->channels[ch].type ][ dmac->channels[ch].dac ];
	dmac->channels[ch].dar &= 0xffffff;

	// メモリカウントをデクリメント
	dmac->channels[ch].mtc--;
	if (dmac->channels[ch].mtc > 0) {
		return 1;
	}

	// コンティニューの処理
	if (dmac->channels[ch].cnt) {
		// BAR,BFC,BTCをMAR,MFC,MTCに転送
		dmac->channels[ch].mar = dmac->channels[ch].bar;
		dmac->channels[ch].mfc = dmac->channels[ch].bfc;
		dmac->channels[ch].mtc = dmac->channels[ch].btc;

		// BOCを上げる
		dmac->channels[ch].boc = 1;
		dmac_interrupt(dmac);
		return 1;
	}

	// アレイチェインの処理
	if (dmac->channels[ch].chain == 0x02) {
		if (dmac->channels[ch].btc > 0) {
			// 次のブロックがある
			dmac_channel_load(dmac, ch);
			return 1;
		}
	}

	// リンクアレイチェインの処理
	if (dmac->channels[ch].chain == 0x03) {
		if (dmac->channels[ch].bar != 0) {
			// 次のブロックがある
			dmac_channel_load(dmac, ch);
			return 1;
		}
	}

	// DMA完了
	// フラグ設定、割り込み
	dmac->channels[ch].act = 0;
	dmac->channels[ch].coc = 1;
	dmac_interrupt(dmac);

	return 0;
}
