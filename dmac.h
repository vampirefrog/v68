#ifndef DMAC_H_
#define DMAC_H_

#include <stdint.h>

struct dmac_channel {
	uint32_t xrm;      // リクエストモード
	uint32_t dtyp;     // デバイスタイプ
	unsigned dps:1;    // ポートサイズ (TRUEで16bit)
	uint32_t pcl;      // PCLセレクタ
	unsigned dir:1;    // 方向 (TRUEでDAR→メモリ)
	unsigned btd:1;    // DONEで次ブロックへ
	uint32_t size;     // オペランドサイズ
	uint32_t chain;    // チェイン動作
	uint32_t reqg;     // REQ生成モード
	uint32_t mac;      // メモリアドレス更新モード
	uint32_t dac;      // デバイスアドレス更新モード

	// 制御フラグ
	unsigned str:1;    // スタートフラグ
	unsigned cnt:1;    // コンティニューフラグ
	unsigned hlt:1;    // HALTフラグ
	unsigned sab:1;    // ソフトウェアアボートフラグ
	unsigned intr:1;           // 割り込み可能フラグ
	unsigned coc:1;    // チャンネル動作完了フラグ
	unsigned boc:1;    // ブロック動作完了フラグ
	unsigned ndt:1;    // 正常終了フラグ
	unsigned err:1;    // エラーフラグ
	unsigned act:1;    // アクティブフラグ
	unsigned dit:1;    // DONE入力フラグ
	unsigned pct:1;    // PCL negedge検出フラグ
	unsigned pcs:1;    // PCLの状態 (TRUEでHレベル)
	uint32_t ecode;    // エラーコード

	// アドレス、レングス
	uint32_t mar;      // メモリアドレスカウンタ
	uint32_t dar;      // デバイスアドレスレジスタ
	uint32_t bar;      // ベースアドレスレジスタ
	uint32_t mtc;      // メモリトランスファカウンタ
	uint32_t btc;      // ベーストランスファカウンタ
	uint32_t mfc;      // メモリファンクションコード
	uint32_t dfc;      // デバイスファンクションコード
	uint32_t bfc;      // ベースファンクションコード
	uint32_t niv;      // ノーマルインタラプトベクタ
	uint32_t eiv;      // エラーインタラプトベクタ

	// バースト転送
	uint32_t cp;       // プライオリティ
	uint32_t bt;       // バースト転送タイム
	uint32_t br;       // バンド幅
	uint8_t type;      // 転送タイプ

	// 動作カウンタ(デバッグ向け)
	uint32_t startcnt; // スタートカウンタ
	uint32_t errorcnt; // エラーカウンタ
};

struct dmac_control {
	int transfer;      // 転送中フラグ(チャネル兼用)
	int load;          // チェインロードフラグ(チャネル兼用)
	unsigned exec:1;   // オートリクエスト有無フラグ
	int current_ch;    // オートリクエスト処理チャネル
	int cpu_cycle;     // CPUサイクルカウンタ
	int vector;        // 割り込み要求中ベクタ
};

struct dmac {
	struct dmac_channel channels[4];
	struct dmac_control control;
};

int      dmac_init(struct dmac *dmac);
void     dmac_cleanup(struct dmac *dmac);
void     dmac_deinit(struct dmac *dmac);
void     dmac_reset(struct dmac *dmac);
uint32_t dmac_read_8(struct dmac *dmac, uint32_t addr);
uint32_t dmac_read_16(struct dmac *dmac, uint32_t addr);
void     dmac_write_8(struct dmac *dmac, uint32_t addr, uint32_t data);
void     dmac_write_16(struct dmac *dmac, uint32_t addr, uint32_t data);
uint32_t dmac_channel_auto(struct dmac *dmac, uint32_t cycle);
int      dmac_channel_request(struct dmac *dmac, int ch);
uint32_t dmac_get_vector(struct dmac *dmac, int type);
void     dmac_bus_err(struct dmac *dmac, uint32_t addr, int read);
void     dmac_addr_err(struct dmac *dmac, uint32_t addr, int read);
void     dmac_int_ack(struct dmac *dmac);
int      dmac_is_act(struct dmac *dmac, int ch);

#endif /* DMAC_H_ */
