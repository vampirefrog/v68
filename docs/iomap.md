```

        - I/O MAP MANUAL -

==============================================================================

・グラフィック VRAM

  G-VRAM は実画面サイズ及び色数によって構造が異なり、合計 512KB の VRAM が分割
されて割り当てられている. ドットの表現は垂直型で、どのモードでも 1 ドットは必
ず 1 ワードで表し、16/256 色表示モードではそのうち下位の 4/8 ビットのみが有効
となる.

        X = 通常の設定モードでの X 座標
        Y = 〃                    Y 座標
        P = プレーン番号
        ADR = 65536 色表示モードで (X,Y) 点をアクセスする時のアドレス
        BIT = 〃                                      ビット位置

実画面512×512ドット、65536色表示:
        有効ビット   下位 16 ビット
        1 ライン   1024 バイト(512 ワード)
        ページ 0   $c00000〜$c7ffff

実画面512×512ドット、256色表示:
        有効ビット   下位 8 ビット
        1 ライン   1024 バイト(512 ワード)
        ページ 0   $c00000〜$c7ffff
        ページ 1   $c80000〜$cfffff

        ADR = $c00000＋Y×1024＋X×2
        BIT = P×8 〜 P×8＋7 の 8 ビット

実画面512×512ドット、16色表示:
        有効ビット   下位 4 ビット
        1 ライン   1024 バイト(512 ワード)
        ページ 0   $c00000〜$c7ffff
        ページ 1   $c80000〜$cfffff
        ページ 2   $d00000〜$d7ffff
        ページ 3   $d80000〜$dfffff

        ADR = $c00000＋Y×1024＋X×2
        BIT = P×4 〜 P×4＋3 の 4 ビット

実画面1024×1024ドット、16色表示:
        有効ビット   下位 4 ビット
        1 ライン   2048 バイト(1024 ワード)
        ページ 0   $c00000〜$dfffff

        ADR = $c00000＋(Y＆511)×1024＋(X＆511)×2
        BIT = (Y≧512)×8＋(X≧512)×4 〜
              (Y≧512)×8＋(X≧512)×4＋3 の 4 ビット

==============================================================================

・テキスト VRAM

  T-VRAM は実画面サイズ1024×1024、16 色表示に固定で 4 プレーン合計 512KB の
VRAM が割り当てられている. ドットの表現は水平型で、1 ワードは横 16 ドットの各
プレーンにおけるパレットコードを表わす.

        1 ライン   128 バイト(64 ワード)
        プレーン 0  $e00000〜$e1ffff
        プレーン 1  $e20000〜$e3ffff
        プレーン 2  $e40000〜$e5ffff
        プレーン 3  $e60000〜$e7ffff

==============================================================================

・CRT コントローラ

  基本的にはワードサイズのポートだが、バイトサイズでのアクセスも可能である.


address size    R/W
$e80000 1.w       W     R00     水平トータル            ┐
$e80002 1.w       W     R01     水平同期終了位置        │水平タイミング制御
$e80004 1.w       W     R02     水平表示開始位置        │
$e80006 1.w       W     R03     水平表示終了位置        ┘
$e80008 1.w       W     R04     垂直トータル            ┐
$e8000a 1.w       W     R05     垂直同期終了位置        │垂直タイミング制御
$e8000c 1.w       W     R06     垂直表示開始位置        │
$e8000e 1.w       W     R07     垂直表示終了位置        ┘
$e80010 1.w       W     R08     外部同期水平アジャスト     : 水平位置微調整
$e80012 1.w       W     R09     ラスタ番号           : ラスタ割り込み用
$e80014 1.w       W     R10     X 位置                    ┐テキスト画面
$e80016 1.w       W     R11     Y 位置                    ┘       スクロール
$e80018 1.w       W     R12     X0                      ┐
$e8001a 1.w       W     R13     Y0                      │
$e8001c 1.w       W     R14     X1                      │
$e8001e 1.w       W     R15     Y1                      │グラフィック画面
$e80020 1.w       W     R16     X2                      │      スクロール
$e80022 1.w       W     R17     Y2                      │
$e80024 1.w       W     R18     X3                      │
$e80026 1.w       W     R19     Y3                      ┘
$e80028 1.w     R/W     R20     メモリモード/表示モード制御
$e8002a 1.w     R/W     R21     同時アクセス/ラスタコピー/高速クリアプレーン撰択
$e8002c 1.w       W     R22     ラスタコピー動作用       : ラスタ番号
$e8002e 1.w       W     R23     テキスト画面アクセスマスクパターン
$e80480 1.w     R/W     動作      画像取り込み/高速クリア/ラスタコピー制御


CRTC R00($e80000):
        bit 15                           8   7                           0
          ┌───────────────┬───────────────┐
          │                              │                水平トータル  │
          └───────────────┴───────────────┘
                水平同期期間×水平表示ドット数
        R00 = ───────────────── -1
                データ表示期間×8
        ※最下位ビットは必ず 1 にすること.

CRTC R01($e80002):
        bit 15                           8   7                           0
          ┌───────────────┬───────────────┐
          │                              │            水平同期終了位置  │
          └───────────────┴───────────────┘
                水平同期パルス幅×水平表示ドット数
        R01 = ─────────────────── -1
                データ表示期間×8

CRTC R02($e80004):
        bit 15                           8   7                           0
          ┌───────────────┬───────────────┐
          │                              │            水平表示開始位置  │
          └───────────────┴───────────────┘
                (水平同期パルス幅＋水平バックポーチ)×水平表示ドット数
        R02 = ───────────────────────────── -5
                データ表示期間×8

CRTC R03($e80006):
        bit 15                           8   7                           0
          ┌───────────────┬───────────────┐
          │                              │            水平表示終了位置  │
          └───────────────┴───────────────┘
                (水平同期パルス幅−水平フロントポーチ)×水平表示ドット数
        R03 = ────────────────────────────── -5
                データ表示期間×8

CRTC R04($e80008):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                        垂直トータル  │
          └───────────┴───────────────────┘
                垂直同期期間
        R04 = ──────── -1
                水平同期期間

CRTC R05($e8000a):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                    垂直同期終了位置  │
          └───────────┴───────────────────┘
                垂直同期パルス幅
        R05 = ────────── -1
                水平同期期間

CRTC R06($e8000c):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                    垂直表示開始位置  │
          └───────────┴───────────────────┘
                垂直同期パルス幅＋垂直バックポーチ
        R06 = ─────────────────── -1
                水平同期期間

CRTC R07($e8000e):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                    垂直表示終了位置  │
          └───────────┴───────────────────┘
                垂直同期期間−垂直フロントポーチ
        R07 = ────────────────── -1
                水平同期期間

CRTC R08($e80010):
        bit 15                           8   7                           0
          ┌───────────────┬───────────────┐
          │                              │      外部同期水平アジャスト  │
          └───────────────┴───────────────┘

CRTC R09($e80012):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                          ラスタ番号  │
          └───────────┴───────────────────┘
                垂直同期パルス幅＋垂直バックポーチ
        R09 = ─────────────────── ＋ラスタ番号
                水平同期期間
            = R06 +1 ＋ラスタ番号

CRTC R10($e80014):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                               X位置  │
          └───────────┴───────────────────┘

CRTC R11($e80016):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                               Y位置  │
          └───────────┴───────────────────┘

CRTC R12($e80018):
        bit 15                  10   9   8                               0
          ┌───────────┬───────────────────┐
          │                      │  │                              X0  │
          └───────────┴───────────────────┘
        ※実画面512×512ドットのモードでは bit 8〜0 までが有効.

CRTC R13($e8001a):
        bit 15                  10   9   8                               0
          ┌───────────┬───────────────────┐
          │                      │  │                              Y0  │
          └───────────┴───────────────────┘
        ※実画面512×512ドットのモードでは bit 8〜0 までが有効.

CRTC R14($e8001c):
        bit 15                           8                               0
          ┌─────────────┬─────────────────┐
          │                          │                              X1  │
          └─────────────┴─────────────────┘
        ※実画面1024×1024ドットのモードでは使用されない.

CRTC R15($e8001e):
        bit 15                           8                               0
          ┌─────────────┬─────────────────┐
          │                          │                              Y1  │
          └─────────────┴─────────────────┘
        ※実画面1024×1024ドットのモードでは使用されない.

CRTC R16($e80020):
        bit 15                           8                               0
          ┌─────────────┬─────────────────┐
          │                          │                              X2  │
          └─────────────┴─────────────────┘
        ※実画面1024×1024ドットのモードでは使用されない.

CRTC R17($e80022):
        bit 15                           8                               0
          ┌─────────────┬─────────────────┐
          │                          │                              Y2  │
          └─────────────┴─────────────────┘
        ※実画面1024×1024ドットのモードでは使用されない.

CRTC R18($e80024):
        bit 15                           8                               0
          ┌─────────────┬─────────────────┐
          │                          │                              X3  │
          └─────────────┴─────────────────┘
        ※実画面1024×1024ドットのモードでは使用されない.

CRTC R19($e80026):
        bit 15                           8                               0
          ┌─────────────┬─────────────────┐
          │                          │                              Y3  │
          └─────────────┴─────────────────┘
        ※実画面1024×1024ドットのモードでは使用されない.

CRTC R20($e80028):
        bit 15      13  12  11  10   9   8   7       5   4   3   2   1   0
          ┌─────┬─┬─┬─┬───┬─────┬─┬───┬───┐
          │          │  │  │  │  COL │          │HF│  VD  │  HD  │
          └─────┴─┴─┴─┴───┴─────┴─┴───┴───┘
                        │  │  └ SIZE
                        │ G-MEM
                        └ T-MEM

        bit 12   T-MEM  T-VRAM使用モード
                        %0  : 表示用
                        %1  : バッファ用
        bit 11   G-MEM  G-VRAM使用モード
                        %0  : 表示用
                        %1  : バッファ用(bit 10〜8 は無効)
                              (G-VRAM が 65536 色表示時と同じ構造になる)
        bit 10   SIZE   実画面サイズ
                        %0  :  512× 512ドット
                        %1  : 1024×1024ドット
        bit 9〜8 COL     色数モード
                        %00 :    16色
                        %01 :   256色
                        %10 : 未定義
                        %11 : 65536色
        bit 4    HF     水平偏向周波数
                        %0  : 15.98kHz
                        %1  : 31.50kHz
        bit 3〜2 VD      垂直ドット数
                        %00 :  256ドット
                        %01 :  512ドット
                        %10 : 1024ドット・インタレース(HF=%1の時のみ)
                        %11 : 〃
        bit 1〜0 HD      水平ドット数
                        %00 : 256ドット
                        %01 : 512ドット
                        %10 : 768ドット
                        %11 : クロック 50MHz モード(Compact XVI 以降のみ)

CRTC R21($e8002a):
        bit 15                  10   9   8   7   6   5   4   3   2   1   0
          ┌───────────┬─┬─┬───────┬───────┐
          │                      │  │  │  │  │  │  │  │  │  │  │
          └───────────┴─┴─┴───────┴───────┘
                                    │  │      AP3〜AP0 CP3〜CP0
                                    MEN SA

        bit 9   MEN     テキスト画面アクセスマスク
                        %0 : 無効
                        %1 : 有効
        bit 8   SA      テキスト画面同時アクセス
                        %0 : 無効
                        %1 : 有効
        bit 7〜4 AP3〜AP0
                        テキスト画面同時アクセス対象プレーン選択
                        %0 : 非選択
                        %1 : 選択
        bit 3〜0 CP3〜CP0
                        ┌ テキスト画面ラスタコピー対象プレーン選択
                        └ グラフィック画面高速クリア対象ページ選択
                        %0 : 非選択
                        %1 : 選択

CRTC R22($e8002c):
        bit 15                           8   7                           0
          ┌───────────────┬───────────────┐
          │                ソースラスタ  │  ディスティネーションラスタ  │
          └───────────────┴───────────────┘
                                (転送元)                   (転送先)

CRTC R23($e8002e):
        bit 15                                                           0
          ┌───────────────────────────────┐
          │                                                マスクパタン  │
          └───────────────────────────────┘
        %0 : データが変更される
        %1 : データが変更されない

CRTC 動作ポート($e80480):
        bit 15                                           4   3   2   1   0
          ┌───────────────────────┬─┬─┬─┬─┐
          │                                              │RC│ 0│FC│VI│
          └───────────────────────┴─┴─┴─┴─┘

        bit 3   RC      テキスト画面ラスタコピー開始
                        %0 : 停止
                        %1 : 開始
        bit 1   FC      グラフィック画面高速クリア開始
                        %0 : 停止
                        %1 : 開始
                        ※高速クリア動作が終了すると自動的に 0 になる.
        bit 0   VI      画像取り込み開始
                        %0 : 停止
                        %1 : 開始

==============================================================================

・ビデオコントローラ

address size
$e82000 256.w   --      グラフィックパレット
$e82200 256.w   --      テキストパレット、スプライト＋BG パレット
$e82400 1.w     R0      画面モード
$e82500 1.w     R1      プライオリティ制御
$e82600 1.w     R2      ON/OFF 制御/特殊プライオリティ


グラフィックパレット($e82000):
          グラフィックパレットは 16/256色表示モードと 65536色表示モードでは構
        造が異なる.

           16色表示:
                  先頭 16 ワードが使用される. パレットから読み出された値はその
                ままカラーコードとして出力される.
          256色表示:
                  先頭 256 ワードが使用される. パレットから読み出された値はそ
                のままカラーコードとして出力される.
        65536色表示:
                  テキストパレットや 16/256色表示モードでのグラフィックパレッ
                トは 1 ワード単位のカラーコードが色数だけ並んでいるが、65536
                色表示モードでのグラフィックパレットはカラーコードを上位/下位
                のバイト単位に分割し、それが 256 個飛び飛びに並んだものが二組
                (上位/下位)ある.
                  カラーコード出力時には G-VRAM のデータが上位バイトと下位バイ
                トに分割され、それぞれ独立して決定されたのち、合成される.

                PH = G-VRAM のデータの上位バイト
                PL = 〃                下位〃
                CH = 出力されるカラーコードの上位バイト
                CL = 〃                       下位〃

                CH = *(unsigned char)($e82002＋PH×2−PH＆1)
                CL = *(unsigned char)($e82000＋PL×2−PL＆1)

                address size    CH              CL
                $e82000 1.b     --              PL = $00
                $e82001 1.b     --              PL = $01
                $e82002 1.b     PH = $00        --
                $e82003 1.b     PH = $01        --
                $e82004 1.b     --              PL = $02
                $e82005 1.b     --              PL = $03
                $e82006 1.b     PH = $02        --
                $e82007 1.b     PH = $03        --
                                ……
                                ……
                $e821f8 1.b     --              PL = $fc
                $e821f9 1.b     --              PL = $fd
                $e821fa 1.b     PH = $fc        --
                $e821fb 1.b     PH = $fd        --
                $e821fc 1.b     --              PL = $fe
                $e821fd 1.b     --              PL = $ff
                $e821fe 1.b     PH = $fe        --
                $e821ff 1.b     PH = $ff        --
                ($e82000 は PL = $00 の時の CL の値、と読むこと)

テキストパレット($e82200):
          テキストパレットとスプライト＋BG パレットは 16 ワードのパレットブロ
        ックが 16 個並び、一部共用となっている.
          テキストパレットはこのうち先頭のパレットブロック 0 だけが使用され、
        パレットから読み出された値はそのままカラーコードとして出力される.

スプライト＋BG パレット($e82200):
          テキストパレットと同様だが、パレットブロック 0〜15 の全てを使用でき
        る. 使用するパレット番号は、PCG のデータが下位 4 ビットの値となり、ス
        プライトスクロールレジスタや BG データエリアのデータが上位 4 ビットの
        値となって決定される.

カラーコード
        bit 15              11  10               6   5               1   0
          ┌─────────┬─────────┬─────────┬─┐
          │            Green │              Red │             Blue │ I│
          └─────────┴─────────┴─────────┴─┘

        bit 15〜11 Green 緑成分(0〜31)
        bit 10〜 6 Red           赤〃
        bit  5〜 1 Blue          青〃
        bit  0     I            輝度(0〜1)
                                1 にすると G、R、B それぞれ +0.5.

VC R0($e82400):
        bit 15                                               3   2   1   0
          ┌─────────────────────────┬─┬───┐
          │                                                  │  │ COL  │
          └─────────────────────────┴─┴───┘
                                                                SIZ

        bit 2    SIZ    実画面サイズ
                        %0  :  512× 512ドット
                        %1  : 1024×1024ドット
        bit 1〜0 COL     色モード
                        %00 :    16色
                        %01 :   256色
                        %10 : 未定義
                        %11 : 65536色
        ※CRTC R20 のビット 10〜8 と同じ値を設定すること.

VC R1($e82500):
        bit 15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
          ┌───┬───┬───┬───┬───┬───┬───┬───┐
          │      │  SP  │  TX  │  GR  │ GP3  │ GP2  │ GP1  │ GP0  │
          └───┴───┴───┴───┴───┴───┴───┴───┘

        bit 13〜12 SP    スプライト画面の優先順位
        bit 11〜10 TX    テキスト    〃
        bit  9〜 8 GR    グラフィック  〃
        bit  7〜 6 GP3   四番目に優先順位の高いグラフィック画面のページ番号
        bit  5〜 4 GP2   三番目             〃
        bit  3〜 2 GP1   二番目             〃
        bit  1〜 0 GP0   最も優先順位の高いグラフィック画面のページ番号

        ※SP/TX/GR は、優先順位を高くする画面から %00/%01/%10 を設定する.
        ※グラフィック画面が 1 ページの場合、GP3〜GP0 は一組として扱う.
          また、2 ページの場合は GP3〜GP2/GP1〜GP0 をそれぞれ一組として扱う.
        ※GP3〜GP0 設定値
          65536色表示 : %1110_0100
            256色表示 : %1110_0100(ページ 1 < ページ 0)
                        %0100_1110(ページ 0 < ページ 1)
             16色表示 : %1110_0100(ページ 3 < ページ 2 < ページ 1 < ページ 0)
                            …
                        %0001_1011(ページ 0 < ページ 1 < ページ 2 < ページ 3)

VC R2($e82600):
        bit 15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┐
          │YS│AH│  │  │  │  │  │  │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┘
                   VHT EXON H/P B/P G/G G/T │  SON TON GS4 GS3 GS2 GS1 GS0
                                           BCON

        bit 15  YS      ビデオカット
                        (スーパーインポーズ時でもビデオ画像を表示しない)
        bit 14  AH      半透明 : テキストパレット 0
                        ※EXON や H/P 等と無関係に強制的に半透明モードになる.
        bit 13  VHT     半透明 : ビデオ画像(カラーイメージユニットで利用)
        bit 12  EXON    特殊プライオリティ/半透明モード有効
        bit 11  H/P     %0 : 特殊プライオリティ選択
                        %1 : 半透明モード選択
        bit 10  B/P     %0 : シャープ予約
                        %1 : グラフィック画面の最下位ビットで領域指定
        bit  9  G/G     半透明 : グラフィック画面
        bit  8  G/T     半透明 : テキスト/スプライト画面
                        ※グラフィック画面のプライオリティが高い場合のみ有効)
        bit  7  BCON    ボーダーカラー表示 ON(%1)/OFF(%0) 制御
        bit  6  SON     スプライト画面 ON(%1)/OFF(%0) 制御
        bit  5  TON     テキスト画面 ON(%1)/OFF(%0) 制御
        bit  4  GS4     グラフィック画面 ON(%1)/OFF(%0) 制御
                                                (実画面1024×1024ドット時)
        bit  3〜0 GS3〜GS0
                        グラフィック画面 ON(%1)/OFF(%0) 制御
                                                (実画面 512× 512ドット時)

        ※グラフィック画面が 1 ページの場合、GS3〜GS0 の各ビットには全て同じ
          値を設定する. また、2 ページの場合は GS3〜GS2/GS1〜GS0 をそれぞれ
          一組として扱い同じ組の各ビットには全て同じ値を設定する.

==============================================================================


・DMAC

The HD63450 has four channels, and at X680x0, channels # 0, # 1, and # 3 are assigned to FD, HD, and ADPCM, respectively.
Channel # 2 is released to the user, so REQ (DMA transfer request signal), ACK (response signal), PCL (general purpose input signal) etc. are wired in the expansion slot, so transfer between memory and memory or expansion board It is available.

Registers of the DMAC exist for each channel from $e84000 to $40 bytes at a time.
However, GCR (General Control Register) is only in channel # 3.


address channel
$e84000 #0      FDD
$e84040 #1      SASI/SCSI
$e84080 #2      IOCS
$e840c0 #3      ADPCM


offset  size    R/W
+$00    1.b     R/W     CSR     Channel status register
+$01    1.b     R       CER     Channel error register
+$02    1.w     ---
+$04    1.b     R/W     DCR     Device control register
+$05    1.b     R/W     OCR     Operation control register
+$06    1.b     R/W     SCR     Sequence control register
+$07    1.b     R/W     CCR     Channel control register
+$08    1.w     ---
+$0a    1.w     R/W     MTC     Memory transfer counter
+$0c    1.l     R/W     MAR     Memory address register
+$10    1.l     ---
+$14    1.l     R/W     DAR     Device address register
+$18    1.w     ---
+$1a    1.w     R/W     BTC     Base transfer counter
+$1c    1.l     R/W     BAR     Base address register
+$20    1.l     ---
+$24    1.b     ---
+$25    1.b     R/W     NIV     Normal interrupt vector
+$26    1.b     ---
+$27    1.b     R/W     EIV     Error interrupt vector
+$28    1.b     ---
+$29    1.b     R/W     MFC     Memory function code
+$2a    1.w     ---
+$2c    1.b     ---
+$2d    1.b     R/W     CPR     Channel priority register
+$2e    1.w     ---
+$30    1.b     ---
+$31    1.b     R/W     DFC     Device function code
+$32    3.w     ---
+$38    1.b     ---
+$39    1.b     R/W     BFC     Base function code
+$3a    1.l     ---
+$3e    1.b     ---
+$3f    1.b     R/W     GCR     General control register


DMAC CSR(+$00):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            COC BTC NDT ERR ACT DIT PCT PCS

        bit 7   COC     Channel operation completion
                        %0 : Channel operation incomplete
                        %1 : 〃             complete
        bit 6   BTC     Block transfer completion
                        %0 : Block transfer operation incomplete
                        %1 : 〃               complete
        bit 5   NDT     Normal device termination
                        %0 : It is not a device stop due to the DONE signal
                        %1 : Normal device stop by DONE signal
        bit 4   ERR     Error bit
                        %0 : No error
                        %1 : Error occurred (ERROR CODE bit contains error contents)
        bit 3   ACT     Channel active
                        %0 : Channel inactive
                        %1 : Channel active (active)
        bit 2   DIT     DONE input transition
                        %0 : There is no DONE input
                        %1 : There was a DONE input when the BTD bit of OCR was %1
                        ___
        bit 1   PCT     PCL transition
                        %0 : There is no trailing edge of PCL (change from High to Low)
                        %1 : 〃                                   There has occurred
                        ___
        bit 0   PCS     PCL line status
                        %0 : PCL = Low
                        %1 : PCL = High

        When a bit other than ACT and PCS becomes% 1 once, it remains %1 until %1 is written to that bit or reset is applied.
        Especially when the COC, BTC, NDT, ERR, ACT bit is set to %1, the next transfer operation can not be performed, so it is necessary to clear it before use.

DMAC CER(+$01):
        bit  7       5   4               0
          ┌─────┬─────────┐
          │     0    │      ERROR CODE  │
          └─────┴─────────┘

        bit 4〜0 ERROR CODE
                         %00001: Configuration error
                         %00010: Operation timing error
                         %00011: (not used)
                         %001rr: Address error
                         %010rr: bus error
                         %011rr: Count error
                         %10000: External forced stop
                         %10001: software forced stop
DMAC DCR(+$04):
        bit  7   6   5   4   3   2   1   0
          ┌───┬───┬─┬─┬───┐
          │ XRM  │ DTYP │  │ 0│ PCL  │
          └───┴───┴─┴─┴───┘
                            DPS

        bit 7〜6 XRM     External Request Mode
                        %00 : Burst transfer mode
                        %01 : (undefined)
                        %10 : Hold-free cycle steal mode
                        %11 : With hold 〃
        bit 5〜4 DTYP    device type
                        %00 : 68000 Bus type             ┐ Dual address mode
                        %01 : 6800  Bus type             ┘
                                          ___
                        %10 : Device with ACK                ┐
                                           ___     _____     │ Single address mode
                        %11 : Devices with ACK and READY     ┘
                        * Use in dual address mode (normally% 00).
        bit 3 DPS device port size
                        %0: 8 bit port
                        %1: 16 〃
        bit 1 to 0 PCL peripheral control line
                        %00: Status input
                        %01: Status input with interrupt
                        %10: 1/8 start pulse
                        %11: ABORT input (forced termination)

DMAC OCR(+$05):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬───┬───┬───┐
          │  │  │ SIZE │ CHAIN│ REQG │
          └─┴─┴───┴───┴───┘
            DIR BTD

        bit 7 DIR Direction
                        % 0: Memory → device (MAR -> DAR)
                        % 1: Device → Memory (DAR → MAR)
        bit 6 BTD Multiple block transfer with DONE
                        % 0: Normal operation
                        % 1: If there is a DONE input, forcibly transfer the next block
                              To do
        bit 5 to 4 SIZE operand size
                        % 00: Byte (8 bits)
                        % 01: Word (16 bit)
                        % 10: Longword (32 bits)
                        % 11: No pack size Port size 8 bit byte (8 bit) transfer
        bit 3 to 2 CHAIN ​​chaining operation
                        % 00: No chain operation
                        % 01: (not used)
                        % 10: Array chaining
                        % 11: Link array chaining
        bit 1 to 0 REQG request generation method
                        % 00: Auto request limit speed
                        % 01: 〃 Maximum speed
                        % 10: External request transfer (by REQ line)
                        % 11: The first transfer is an auto request
                              External request transfer from the second onward
                        * Channel # 0, # 1, # 3 should be used for external request transfer.

DMAC SCR(+$06):
        bit  7           4   3   2   1   0
          ┌───────┬───┬───┐
          │       0      │ MAC  │ DAC  │
          └───────┴───┴───┘

        bit 3〜2 MAC     Memory address count
                        %00 : The memory address register is not changed
                        %01 : Increase memory address register each time transfer is done
                        %10 : 〃                                   Decrease
                        %11 : (unused)
        bit 1〜0 DAC     Device address count
                        %00 : Device address register is not changed
                        %01 : Increase device address register each time transfer is done
                        %10 : 〃                                     Decrease
                        %11 : (unused)

DMAC CCR(+$07):
        bit  7   6   5   4   3   2       0
          ┌─┬─┬─┬─┬─┬─────┐
          │  │  │  │  │  │    0     │
          └─┴─┴─┴─┴─┴─────┘
            STR CNT HLT SAB INT

         bit 7 STR Start Operation
                         % 1: Start operation
         bit 6 CNT Continue Operation
                         % 0: No continuous operation
                         % 1: 〃 There
         bit 5 HLT halt operation
                         % 0: Cancel pause
                         % 1: Channel operation pause
         bit 4 SAB software abort
                         % 1: Stopping channel operation
                         * It is always 0 when reading.
         bit 3 INT Interrupt Enable
                         % 0: Disable interrupt generation
                         % 1: 〃 permission

DMAC MTC(+$0a):
        Specify the number of transfer operands.

DMAC MAR(+$0c):
        Specify the memory address to be transferred.

DMAC DAR(+$14):
        Specify the device address to be transferred.

DMAC BTC(+$1a):
        Specify the number of transfer blocks (the number of transfer information tables) in array chain mode.

DMAC BAR(+$1c):
        Specify the start address of the transfer information table in the array chain / link array chain mode.

DMAC NIV(+$25):
        Specify the vector number to be used for the interrupt generated when CSR ERR is %0.

DMAC EIV(+$27):
        Specify the vector number to be used for the interrupt generated when CSR ERR is %1.

DMAC MFC(+$29):
        bit  7               3   2   1   0
          ┌─────────┬─────┐
          │         0        │  │  │  │
          └─────────┴─────┘
                                FC2 FC1 FC0

        bit 2〜0 FC2〜FC0
                         Memory function code
                         % 000: (not used)
                         % 001: User data
                         % 010: User program
                         % 011: (not used)
                         % 100: (not used)
                         % 101: Supervisor data
                         % 110: Supervisor program
                         % 111: interrupt acknowledge

DMAC CPR(+$2d):
        bit  7                   2   1   0
          ┌───────────┬───┐
          │           0          │  CP  │
          └───────────┴───┘
        bit 1 to 0 CP      channel priority
                         %00: Highest priority
                         %01: Second highest priority
                         %10: 〃
                         %11: Lowest priority

DMAC DFC(+$31):
        (Like MFC)

DMAC BFC(+$39):
        (Like MFC)

DMAC GCR(+$3f):
        bit  7           4   3   2   1   0
          ┌───────┬───┬───┐
          │       0      │  BT  │  BR  │
          └───────┴───┴───┘

        bit 3 to 2 BT burst time
                         (Number of DMA clock cycles per burst)
                         % 00: 16 clocks
                         % 01: 32 〃
                         % 10: 64 "
                         % 11: 128 〃
        bit 1 to 0 BR Bandwidth Threshold (band occupancy)
                         % 00: 50.00%
                         % 01: 25.00%
                         % 10: 12.50%
                         % 11: 6.25%
        *Only affects the channel of the limited speed auto request mode.


· Array chain table structure

offset size
  0    4 Memory address # 1
  4    2 Number of transferred bytes # 1
  6    4 Memory address # 2
 10    2 Number of transferred bytes # 2
... ... ...
An arbitrary number of above contents are arranged.


· Structure of link array chain table
offset size
  0    4 Memory address # 1
  4    2 Number of transferred bytes # 1
  6    4 Address of the next table # 1
                        │
        ┌───────┘
        ↓
offset size
  0    4 Memory address # 2
  4    2 Number of transferred bytes # 2
  6    4 Address of the next table # 2
                        │
…       …               …
  An arbitrary number of contents above are arranged. If the address of the next table is 0, the table ends.



==============================================================================

・エリアセット

  Register for selecting user area / supervisor area of main memory.
  All byte size write-only port, can not read.

address size    R/W
$e86001 1.b       W     エリアセットレジスタ
$eaff81 1.b       W     拡張エリアセットレジスタ 0($200000〜$3fffff に対応)
$eaff83 1.b       W     〃                        1($400000〜$5fffff 〃)
$eaff85 1.b       W     〃                        2($600000〜$7fffff 〃)
$eaff87 1.b       W     〃                        3($800000〜$9fffff 〃)
$eaff89 1.b       W     〃                        4($a00000〜$bfffff 〃)


エリアセットレジスタ($e86001):
          0 番地からどれだけのメモリ領域をスーパバイザ領域にするかを 8KB 単位
        で指定する. 0 番地から、((設定値+1)×8192-1)までのアドレスがスーパバ
        イザ領域となり、最大で 2MB(($ff+1)×8192-1)の範囲が指定できる.
          本体リセット時、このレジスタには 0 が設定される.

拡張エリアセットレジスタ:
          $200000 番地から 256KB 単位でスーパバイザ領域にするかどうかを指定す
        る. レジスタ #0 の bit 0 から bit 7、…、レジスタ #4 の bit 0 から bit
        7 の順に各ビットがそれぞれ $200000〜$23ffff、$240000〜$27ffff、…、
        $bc0000〜$bfffff というように 256KB 単位で対応する.
          本体リセット時、このレジスタには全て 0 が設定される.

          拡張エリアセットレジスタ番号を R、ビット番号を N とすると、対応する
        アドレスは (R＋1)×$200000＋N×$40000 からの 256KB 分となる.

        ※X68030 で追加されたレジスタであるが、X68000 用の増設メモリでも対応し
          ているものがある(CZ-6BEn、PIO-6BEn 等). また、対応していても本体リセ
          ット時に設定が初期化されないものも存在する. Xsimm10 はコスト削減のた
          め対応していない.
        ※Human68k では常に $e86001 しか使われず、保護すべきメモリが 2MB を超
          えた場合でも無視されてユーザ領域のままとなる.

==============================================================================

・MFP

  マルチファンクションペリフェラル MC68901 は 4 個のタイマ、1 チャンネルのシリ
アルポート(USART)、8 ビット分の汎用 I/O ポート(GPIP)を持っている.
  X680x0/Human68k での割り当ては以下のようになっている.

        タイマ     A       V-DISP(垂直表示期間)信号による割り込み
        〃       B       シリアルポートのクロック生成
        〃       C       カーソル点滅/FDD 制御等の割り込み
        〃       D       BG プロセス切り換え用の割り込み
        シリアルポート キーボード入出力
        汎用 I/O ポート      各種信号による割り込み


address size    R/W
$e88001 1.b     R       GPIP    汎用 I/O レジスタ             ┐
$e88003 1.b     R/W     AER     アクティブエッジレジスタ    │GPIP 制御
$e88005 1.b     R/W     DDR     データ方向レジスタ               ┘
$e88007 1.b     R/W     IERA    割り込みイネーブルレジスタ A ┐
$e88009 1.b     R/W     IERB    〃                          B    │
$e8800b 1.b     R/W     IPRA    割り込みペンディングレジスタ A        │
$e8800d 1.b     R/W     IPRB    〃                            B  │
$e8800f 1.b     R/W     ISRA    割り込みインサービスレジスタ A        │割り込み制御
$e88011 1.b     R/W     ISRB    〃                            B  │
$e88013 1.b     R/W     IMRA    割り込みマスクレジスタ A   │
$e88015 1.b     R/W     IMRB    〃                      B        │
$e88017 1.b     R/W     VR      ベクタレジスタ                 ┘
$e88019 1.b     R/W     TACR    タイマ A コントロールレジスタ        ┐
$e8801b 1.b     R/W     TBCR    タイマ B 〃                 │
$e8801d 1.b     R/W     TCDCR   タイマ C&D コントロールレジスタ      │
$e8801f 1.b     R/W     TADR    タイマ A データレジスタ           │タイマ制御
$e88021 1.b     R/W     TBDR    タイマ B 〃                 │
$e88023 1.b     R/W     TCDR    タイマ C 〃                 │
$e88025 1.b     R/W     TDDR    タイマ D 〃                 ┘
$e88027 1.b     R/W     SCR     SYNC キャラクタレジスタ          ┐
$e88029 1.b     R/W     UCR     USART コントロールレジスタ        │
$e8802b 1.b     R/W     RSR     レシーバステータスレジスタ   │USART 制御
$e8802d 1.b     R/W     TSR     トランスミッタステータスレジスタ│
$e8802f 1.b     R/W     UDR     USART データレジスタ           ┘


MFP GPIP($e88001):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │  │  │  │  │  └ ALARM
            │  │  │  │  │  │  └ EXPON
            │  │  │  │  │  └ POW SW
            │  │  │  │  └ FMIRQ
            │  │  │  └ V-DISP
            │  │  └ 常に %1
            │  └ CIRQ
            └ H-SYNC

        bit 7〜0 に GPIP 7〜0 の各信号の状態がそのまま入力される.

        bit 7 H-SYNC    CRTC の H-SYNC 信号
                        %0 : 'L'(水平帰線期間)
                        %1 : 'H'( 〃 同期 〃 )
        bit 6 CIRQ      CRTC の割り込み要求信号(ラスタ割り込み)
                        %0 : 割り込み要求中
                        %1 : 〃           なし
        bit 5 (未使用)     常に %1
        bit 4 V-DISP    CRTC の V-DISP 信号
                        %0 : 'L'(垂直帰線期間)
                        %1 : 'H'( 〃 表示 〃 )
        bit 3 FMIRQ     FM 音源 IC の割り込み要求信号
                        %0 : 割り込み要求中
                        %1 : 〃           なし
        bit 2 POW SW    本体前面の電源スイッチの状態
                        %0 : 電源スイッチ ON(通常)
                        %1 : 〃            OFF
        bit 1 EXPON     EXPON 信号
                        %0 : 'L'
                        %1 : 'H'(通常)
        bit 0 ALARM     RTC の ALARM 信号
                        %0 : 'L'
                        %1 : 'H'(通常)

MFP AER($e88003):
        (GPIP と同様)

        各信号ごとに、割り込みを発生させる変化方向を設定する.
        %0 : %0 → %1 の変化で割り込み発生
        %1 : %1 → %0 〃

MFP DDR($e88005):
        (GPIP と同様)

        各信号ごとに、入力/出力のいずれで使うのかを設定する.
        %0 : 入力
        %1 : 出力
        ※X680x0 では全て入力として使用する.

MFP IERA($e88007):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │  │  │  │  │  └ タイマ B
            │  │  │  │  │  │  └ 送信エラー
            │  │  │  │  │  └ 送信バッファエンプティ
            │  │  │  │  └ 受信エラー
            │  │  │  └ 受信バッファフル
            │  │  └ タイマ A
            │  └ CIRQ(GPIP 6)
            └ H-SYNC(GPIP 7)

        bit 7   CRTC H-SYNC(水平同期信号)
        bit 6   CRTC ラスタ割り込み
        bit 5   タイマ A
        bit 4   MPSC 受信バッファフル(受信データ読み取り要求)
        bit 3   〃   受信エラー
        bit 2   〃   送信バッファエンプティ(送信データ書き込み要求)
        bit 1   〃   送信エラー
        bit 0   タイマ B

        各割り込みの発生の許可/禁止を設定する.
        %0 : 割り込み発生禁止
        %1 : 〃           許可

MFP IERB($e88009):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │  │  │  │  │  └ ALARM(GPIP 0)
            │  │  │  │  │  │  └ EXPON(GPIP 1)
            │  │  │  │  │  └ POW SW(GPIP 2)
            │  │  │  │  └ FMIRQ(GPIP 3)
            │  │  │  └ タイマ D
            │  │  └ タイマ C
            │  └ V-DISP(GPIP 4)
            └ (GPIP 5)

        (IERA と同様)

MFP IPRA($e8800b):
        (IERA と同様)

          割り込み要求がペンディング(保留)されていることを示す. 割り込み要求が
        発生すれば %1 になり、割り込み要求が受け取られたら %0 に戻る.
        %0 : 割り込み要求はペンディングされていない
        %1 : 〃                         されている
        ※IERA/IERB によって割り込みが禁止されるか、IPRA/IPRB に %0 を書き込む
          事によっても %0 に戻る.

MFP IPRB($e8800d):
        (IERB と同様)
        (IPRA と同様)

MFP ISRA($e8800f):
        (IERA と同様)

          割り込みがインサービス(処理中)であるとを示す.
          ソフトウェア EOI(End Of interrupt = サービス終了通知)モードでは、MFP
        から MPU に割り込みが伝えられると %1 になり、%0 を ISRA/ISRB に書き込
        むと %0 に戻る.
          自動 EOI モードでは、割り込みを伝えた時点で自動的に %0 に戻る.
        %0 : 割り込み要求は処理中ではない
        %1 : 〃                   である

MFP ISRB($e88011):
        (IERB と同様)
        (ISRA と同様)

MFP IMRA($e88013):
        (IERA と同様)

          割り込みのマスク制御を設定する.
        %0 : 割り込み要求をマスクする  (割り込み発生不可)
        %1 : 〃                   しない(〃      可  )
        ※IERA/IERB は割り込みの発生を抑制するが、IMRA/IMRB は割り込み要因が発
          生しても割り込み要求を出力しないだけである.

MFP IMRB($e88015):
        (IERB と同様)
        (IMRA と同様)

MFP VR($e88017):
        bit  7   6   5   4   3   2   1   0
          ┌───────┬─┬─────┐
          │  ベクタ番号  │ S│          │
          └───────┴─┴─────┘

        bit 7〜4 割り込みベクタの上位 4 ビット
                        MFP による割り込み時に出力するベクタ番号の上位 4 ビッ
                        トの値を設定する. 下位 4 ビットは割り込み要因によって
                        決定される(優先度の高い順から %1111〜%0000).
        bit 3           インサービスレジスタイネーブル
                        %0 : オート EOI モード            (インサービスレジスタ無効)
                        %1 : ソフトウェア EOI モード(〃                    有効)

MFP TACR($e88019):
        bit  7       5   4   3   2   1   0
          ┌─────┬─┬───────┐
          │          │  │  │  │  │  │
          └─────┴─┴───────┘
                        │      AC3〜AC0
                    リセット TAO

        bit 4    リセット TAO
                        タイマ出力(TAO)リセット
                        %0 : 通常動作
                        %1 : タイマ出力ピンの状態を'L'に設定する
        bit 3〜0 AC3〜AC0
                        タイマの動作モード
                        %0000 : タイマストップ
                        %0001 : ディレイモード         (÷  4 プリスケーラ)
                        %0010 : 〃                       (÷ 10 〃    )
                        %0011 : 〃                       (÷ 16 〃    )
                        %0100 : 〃                       (÷ 50 〃    )
                        %0101 : 〃                       (÷ 64 〃    )
                        %0110 : 〃                       (÷100 〃    )
                        %0111 : 〃                       (÷200 〃    )
                        %1000 : イベントカウントモード
                        %1001 : パルス幅測定モード       (÷  4 プリスケーラ)
                        %1010 : 〃                       (÷ 10 〃    )
                        %1011 : 〃                       (÷ 16 〃    )
                        %1100 : 〃                       (÷ 50 〃    )
                        %1101 : 〃                       (÷ 64 〃    )
                        %1110 : 〃                       (÷100 〃    )
                        %1111 : 〃                       (÷200 〃    )

MFP TBCR($e8801b):
        bit  7       5   4   3   2   1   0
          ┌─────┬─┬───────┐
          │          │  │  │  │  │  │
          └─────┴─┴───────┘
                        │      BC3〜BC0
                    リセット TBO

        (TACR と同様)

MFP TCDCR($e8801d):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─────┬─┬─────┐
          │  │  │  │  │  │  │  │  │
          └─┴─────┴─┴─────┘
                CC2〜CC0 DC2〜DC0

        bit 6〜4 CC2〜CC0
                        タイマ C 動作モード
                        %000 : タイマストップ
                        %001 : ディレイモード  (÷  4 プリスケーラ)
                        %010 : 〃                (÷ 10 〃    )
                        %011 : 〃                (÷ 16 〃    )
                        %100 : 〃                (÷ 50 〃    )
                        %101 : 〃                (÷ 64 〃    )
                        %110 : 〃                (÷100 〃    )
                        %111 : 〃                (÷200 〃    )
        bit 2〜0 DC2〜DC0
                        タイマ D 動作モード
                        (CC2〜CC0 と同様)

MFP TADR($e8801f):
        タイマ A の値.

MFP TBDR($e88021):
        タイマ B の値.

MFP TCDR($e88023):
        タイマ C の値.

MFP TDDR($e88025):
        タイマ D の値.

MFP SCR($e88027):
        同期転送モードで使用する.

MFP UCR($e88029):
        bit  7   6   5   4   3   2   1   0
          ┌─┬───┬───┬─┬─┬─┐
          │  │  │  │  │  │PE│  │  │
          └─┴───┴───┴─┴─┴─┘
            CLK WL1 WL0 ST1 ST0     E/O

        bit 7    CLK    送受信速度
                        %0  : 入力クロック周波数と同一
                        %1  : 〃         の 1/16
        bit 6〜5 WL1〜WL0
                        データ長
                        %00 : 8bit
                        %01 : 7bit
                        %10 : 6bit
                        %11 : 5bit
        bit 4〜3 ST1〜ST0
                        同期モード/スタートビット長/ストップビット長
                                        スタートビット ストップビット
                        %00 : 同期        0               0
                        %01 : 〃 1               1
                        %10 : 〃 1               1.5
                        %11 : 非同期       1               2
        bit 2    PE     パリティ有効/無効
                        %0  : パリティ無効
                        %1  : 〃      有効
        bit 1    E/O    パリティ偶数/奇数
                        %0  : 奇数パリティ
                        %1  : 偶数〃

        ※X680x0 ではキーボードとの通信に使用されているため、非同期 1/16 クロ
          ックモード、データ長 8 ビット、ストップビット 1、パリティ無しで
          2400bps の設定にしなければならない.

MFP RSR($e8802b):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │BF│OE│PE│FE│  │  │SS│RE│
          └─┴─┴─┴─┴─┴─┴─┴─┘
                            │  └ M/CIP
                            └ F/S or B

        bit 7   BF      バッファフル
                        %0 : 受信バッファは空
                        %1 : 〃           にデータが入っている
        bit 6   OE      オーバーランエラー
                        %0 : 正常動作
                        %1 : オーバーランエラーが発生した
        bit 5   PE      パリティエラー
                        %0 : 正常動作
                        %1 : パリティエラーが発生した
        bit 4   FE      フレーミングエラー
                        %0 : 正常動作
                        %1 : フレーミングエラーが発生した
                             (ストップビットが見つからない)
        bit 3   F/S or B
                        ファウンド/サーチまたはブレーク
                          同期モード時:
                                %0 を書き込むとワードワーチモードになり、SYNC
                                キャラクタと同じデータが受信されると %1 になる.
                        非同期モード時:
                                %0 : ブレークを検出した
                                %1 : ブレーク状態ではない
        bit 2   M/CIP   マッチ/文字処理中
                          同期モード時:
                                %0 : 受信ワードは SCR と一致していない
                                %1 : 〃                      している
        bit 1   SS      シンクロナスストップ
                        %0 : SCR と一致するキャラクタは取り込まない
                        %1 : 〃                       も取り込む
        bit 0   RE      レシーバイネーブル
                        %0 : レシーバディセーブル
                        %1 : 〃      イネーブル

MFP TSR($e8802d):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬───┬─┐
          │BE│UE│AT│  │ B│ H│ L│TE│
          └─┴─┴─┴─┴─┴───┴─┘
                        END

        bit 7    BE     バッファエンプティ
                        %0 : 送信バッファにデータが入っている
                        %1 : 〃           は空
        bit 6    UE     アンダーランエラー
                        %0 : 正常動作
                        %1 : アンダーランが発生した
        bit 5    AT     オートターンアラウンド
                        %0 : 通常動作
                        %1 : トランスミッタがディセーブルになると自動的にレシー
                             バをイネーブルする(レシーバイネーブル後、自動的に
                             クリアされる)
        bit 4    END
                        %0 : トランスミッタはイネーブルになっている
                        %1 : 〃               ディセーブルされている
        bit 3    B      ブレーク
                        %0 : 通常動作
                        %1 : SO 端子(送信データ端子)をブレーク状態にする
                        ※非同期モード時のみ有効.
        bit 2〜1 H〜L     High/Low
                        %00 : TE = %0 の時、SO 端子 = ハイインピーダンス
                        %01 : 〃             = Low
                        %10 : 〃             = High
                        %11 : ループバックモード
        bit 0    TE     トランスミッタイネーブル
                        %0 : トランスミッタディセーブル
                        %1 : 〃             イネーブル

MFP UDR read($e8802f):
        bit  7   6                       0
          ┌─┬─────────────┐
          │  │                Key Code  │
          └─┴─────────────┘
            └ Make/Break

        最後に押されたキー若しくは最後に離されたキーのキーコードが読み出される.
        一度読んだ後も次の入力が発生するまでは、最後の入力キーコードを保持する.

        bit 7   Make/Break
                %0 : キーが押された
                %1 : 〃    離された

MFP UDR write($e8802f):
        bit  7   6   5   4   3   2   1   0
          ┌───────────────┐
          │D7│D6│D5│D4│D3│D2│D1│D0│
          └───────────────┘

        キーボードへの制御コマンドを書き込む.


※スプリアス割り込み
  MFP のレジスタを操作して割り込みを禁止する時、MPU の割り込みマスクレベルが禁
止しようとしている割り込みを受け付ける設定であった場合、レジスタを操作した瞬間
に割り込みが発生すると、割り込みを正しく処理できずにスプリアス割り込みが発生す
る. これを防ぐには MPU の割り込みマスクを設定して割り込みを受け付けないように
するか、スプリアス割り込みが呼び出されても何もせずに戻るルーチンに置き換えれば
よい.

==============================================================================

・キーボード制御コマンド

        bit  7   6   5   4               0
          ┌─┬─┬─┬─────────┐
          │ 0│ 0│  │    TV CTRL Code  │
          └─┴─┴─┴─────────┘

        ディスプレイ制御信号を発生させる.

        bit 4〜0 TV CTRL Code
                コード      名称             機能                              キー
                %00000 : ---            (無効)
                %00001 : Vol.up         音量 up                           [↑]
                %00010 : Vol.down       〃   down                        [↓]
                %00011 : Vol.normal     〃   標準                  [,]
                %00100 : Call           チャンネルコール                [CLR]
                %00101 : CS down        テレビ画面(初期化)
                %00110 : Mute           消音                              [0]
                %00111 : CH 16          (無効)
                %01000 : BR up          テレビ/コンピュータ画面切り替え(トグル)
                                                                        [.]
                %01001 : BR down        テレビ/外部入力切り替え(トグル)       [=]
                %01010 : BR 1/2         コントラストノーマル
                %01011 : CH up          チャンネル up                        [→]
                %01100 : CH down        〃          down                 [←]
                %01101 : ---            (無効)
                %01110 : Power ON/OFF   電源 ON/OFF(トグル)
                %01111 : CS 1/2         スーパーインポーズ ON/OFF(トグル)
                                        コントラストダウン               [+]
                %10000 : CH 1           チャンネル 1                 [1]
                %10001 : CH 2           〃          2                    [2]
                %10010 : CH 3           〃          3                    [3]
                %10011 : CH 4           〃          4                    [4]
                %10100 : CH 5           〃          5                    [5]
                %10101 : CH 6           〃          6                    [6]
                %10110 : CH 7           〃          7                    [7]
                %10111 : CH 8           〃          8                    [8]
                %11000 : CH 9           〃          9                    [9]
                %11001 : CH 10          〃          10                   [/]
                %11010 : CH 11          〃          11                   [*]
                %11011 : CH 12          〃          12                   [-]
                %11100 : CH 13          テレビ画面                   [=]
                %11101 : CH 14          コンピュータ画面                [.]
                %11110 : CH 15          スーパーインポーズ ON/OFF(トグル)
                                        コントラストダウン               [+]
                %11111 : ---            〃
                                        コントラストノーマル
                ※X1 コンパチモードの = . + は %11100 〜 %11110 の動作.

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬───┬─┐
          │ 0│ 1│ 0│ 0│ 0│      │  │
          └─┴─┴─┴─┴─┴───┴─┘
                                        └ MSCTRL

        マウスコントロール信号を制御する.

        bit 0   MSCTRL
                %0 : MSCTRL を 'Low' にする
                %1 : 〃        'High' 〃

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬───┬─┐
          │ 0│ 1│ 0│ 0│ 1│      │  │
          └─┴─┴─┴─┴─┴───┴─┘
                                        └ KEY EN

        キーデータの送出を許可/禁止する.
        禁止中もキースキャンは行うのでディスプレイ制御は可能.

        bit 0   KEY EN
                %0 : キーデータ送出禁止
                %1 : 〃             許可

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │ 0│ 1│ 0│ 1│ 0│ 0│  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘     __
                                        └ X68K/X1

        ディスプレイコントロールキーモードを選択する.

                     __
        bit 0   X68K/X1
                %0 : X1 コンパチモード
                %1 : X68000 モード

        キー      X68000 モード                              X1 コンパチモード
        +       スーパーインポーズ ON/OFF(トグル)   スーパーインポーズ
        =       TV/外部入力切り替え(トグル)                TV
        .       TV/コンピュータ切り替え(トグル)              コンピュータ

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬───┐
          │ 0│ 1│ 0│ 1│ 0│ 1│BRIGHT│
          └─┴─┴─┴─┴─┴─┴───┘

        キーボード上の LED の明るさを選択する.

        bit 1〜0 BRIGHT
                %00 : 明るい
                %01 : やや明るい
                %10 : やや暗い
                %11 : 暗い

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │ 0│ 1│ 0│ 1│ 1│ 0│  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
                                        └ CTRL EN

        本体からキーボードに要求するディスプレイ制御コマンドを受け付けるか否か
        を設定する.

        bit 0   CTRL EN
                %0 : 本体からのディスプレイ制御無効
                %1 : 〃                         有効

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │ 0│ 1│ 0│ 1│ 1│ 1│  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
                                        └ OPT.2 EN

        OPT.2 キーによるディスプレイ制御を許可/禁止する.

        bit 0   OPT.2 EN
                %0 : OPT.2 キーによるディスプレイ制御禁止
                %1 : 〃                               許可

        bit  7   6   5   4   3           0
          ┌─┬─┬─┬─┬───────┐
          │ 0│ 1│ 1│ 0│  REP. DELAY  │
          └─┴─┴─┴─┴───────┘

        キーリピートが開始されるまでの時間を設定する.
        キーリピート開始までの時間は (REP. DELAY)×100＋200ms で、リセット時は
        500ms に初期化される.

        bit  7   6   5   4   3           0
          ┌─┬─┬─┬─┬───────┐
          │ 0│ 1│ 1│ 1│  REP. TIME   │
          └─┴─┴─┴─┴───────┘

        キーリピートの間隔を設定する.
        キーリピート間隔は (REP. TIME)^2×5＋30ms で、リセット時は 110ms に初
        期化される.

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │ 1│  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
                │  │  │  │  │  │  └ かな
                │  │  │  │  │  └ ローマ字
                │  │  │  │  └ コード入力
                │  │  │  └ CAPS
                │  │  └ INS
                │  └ ひらがな
                └ 全角

        キーボード上の LED の点灯/消灯を制御する. 各ビットが %0 で点灯、%1 で
        消灯.

        bit 6   全角
        bit 5   ひらがな
        bit 4   INS
        bit 3   CAPS
        bit 2   コード入力
        bit 1   ローマ字
        bit 0   かな


※以下のコマンドは Compact キーボード用(未確認).

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │ 0│ 1│ 0│ 0│ 0│ 1│ 1│ 1│
          └─┴─┴─┴─┴─┴─┴─┴─┘

        キーボードの種類を判別する. このコマンドを出力後に $fe が入力されれば
        Compact のキーボードである.

        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │ 0│ 1│ 0│ 0│ 0│ 1│ 0│  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
                                        └ NUM LED

        Compact キーボード上の NUM キー LED の点灯/消灯を制御する.

        bit 0   NUM LED
                %0 : 消灯
                %1 : 点灯

==============================================================================

・RTC

  RTC(リアルタイムクロック RP5C15)のレジスタは二つのバンクが同一アドレスに割り
当てられており、どちらのバンクをアクセスするかは MODE レジスタのビット 0 で設
定する. なお、時間を設定するレジスタは全て BCD フォーマットとなっている.


address size    BANK 0                  BANK 1
$e8a001 1.b      1 秒カウンタ                CLKOUT セレクトレジスタ
$e8a003 1.b     10 秒    〃               Adjust
$e8a005 1.b      1 分    〃               アラーム  1 分レジスタ
$e8a007 1.b     10 分    〃               〃        10 分   〃
$e8a009 1.b      1 時間   〃               〃         1 時間〃
$e8a00b 1.b     10 時間   〃               〃        10 時間〃
$e8a00d 1.b     曜日      〃               〃        曜日     〃
$e8a00f 1.b      1 日    〃               〃         1 日   〃
$e8a011 1.b     10 日    〃               〃        10 日   〃
$e8a013 1.b      1 月    〃               (未使用)
$e8a015 1.b     10 月    〃               12/24 時間セレクタ
$e8a017 1.b      1 年    〃               閏年カウンタ
$e8a019 1.b     10 年    〃               (未使用)
$e8a01b 1.b     MODE レジスタ               MODE レジスタ
$e8a01d 1.b     TEST 〃                  TEST 〃
$e8a01f 1.b     RESET コントローラ    RESET コントローラ


RTC CLKOUT セレクトレジスタ($e8a001,BANK 1):
        bit  7               3   2       0
          ┌─────────┬─────┐
          │                  │  CLKOUT  │
          └─────────┴─────┘

        bit 2〜0 CLKOUT
                        CLKOUT 端子の出力波形選択
                        %000 : ハイインピーダンス('H')(点灯)
                        %001 : 16384Hz(点滅)
                        %010 :  1024Hz( 〃 )
                        %011 :   128Hz( 〃 )
                        %100 :    16Hz( 〃 )
                        %101 :     1Hz( 〃 )
                        %110 :  1/60Hz( 〃 )
                        %111 : 'L' レベル固定(消灯)
                        ※%101 : 立ち上がりで秒カウンタが進む
                          %110 : 〃           分〃

RTC アジャストレジスタ($e8a003,BANK 1):
        bit  7                       1   0
          ┌─────────────┬─┐
          │                          │  │
          └─────────────┴─┘
                                        Adjust

        bit 0   Adjust  秒カウンタアジャスト
                        %0 : アジャスト OFF
                        %1 : アジャスト ON
        ※アジャストした場合、カウンタは 0 に戻るが、秒が 30 以上の場合は次の
          分へ進む.

RTC 12/24 時間セレクタ($e8a015,BANK 1):
        bit  7                       1   0
          ┌─────────────┬─┐
          │                          │  │
          └─────────────┴─┘
                                        12/24

        bit 0   12/24   12 時間計/24 時間計選択
                        %0 : 12 時間計
                        %1 : 24 〃
        ※12 時間計の場合、10 時間レジスタのビット 1 が午前/午後を表わす.
          %0 : 午前
          %1 : 午後

RTC 閏年カウンタ($e8a017,BANK 1):
        bit  7                   2   1   0
          ┌───────────┬───┐
          │                      │ Leap │
          └───────────┴───┘

        bit 1〜0 閏年から経過した年数
                        %00 : 今年が閏年
                        %01 : 三年後が閏年
                        %10 : 二年〃
                        %11 : 来年が閏年
        ※西暦を 4 で割った余りを設定すればよい.

RTC MODE レジスタ($e8a01b):
        bit  7           4   3   2   1   0
          ┌───────┬─┬─┬─┬─┐
          │              │  │  │  │  │
          └───────┴─┴─┴─┴─┘
                            │  │      └ BANK 1/0
                            │  └ アラーム EN
                            └─── タイマ EN

        bit 3   タイマ EN
                        タイマ動作許可/禁止
                        %0 : タイマ動作禁止(秒以降のカウンタは停止)
                        %1 : 〃        許可(通常動作)
        bit 2   アラーム EN
                        アラーム動作許可/禁止
                        %0 : アラーム動作禁止
                        %1 : 〃           許可
                        ※実際にはどちらに設定してもアラーム割り込みは発生し、
                          また %1 を書き込むと逆に割り込みが発生しない機種も存
                          在するようである.

        bit 0   BANK 1/0
                        レジスタバンク選択
                        %0 : BANK 0
                        %1 : BANK 1

RTC TEST レジスタ($e8a01d):
        bit  7           4   3   2   1   0
          ┌───────┬───────┐
          │              │  │  │  │  │
          └───────┴───────┘
                             TEST3〜TEST0

        ※通常は全て %0 を設定すること.

RTC RESET コントローラ($e8a01f):
        bit  7           4   3   2   1   0
          ┌───────┬─┬─┬─┬─┐
          │              │  │  │  │  │
          └───────┴─┴─┴─┴─┘
                            │  │  │  └ アラーム RESET
                            │  │  └─── タイマ RESET
                            │  └ 16Hz ON
                            └──  1Hz ON

        bit 3    1Hz ON
                        Alarm 端子からの 1Hz パルス出力制御
                        %0 : 出力 ON
                        %1 : 〃   OFF
        bit 2   16Hz ON
                        Alarm 端子からの 16Hz パルス出力制御
                        %0 : 出力 ON
                        %1 : 〃   OFF
        bit 1   タイマ RESET
                        秒未満の桁のカウンタのリセット/解除
                        %0 : 通常動作
                        %1 : カウンタリセット
        bit 0   アラーム RESET
                        %0 : アラームリセット解除
                        %1 : 〃               する

        ※このレジスタは書き込み専用で、読み込んだ場合は全てのビットが 0 にな
          った値が読み出されるので注意すること.

==============================================================================

・プリンタ

address size    R/W
$e8c001 1.b       W     プリンタデータ
$e8c003 1.b       W     プリンタストローブ


プリンタデータ($e8c001):
        プリンタに出力するデータを書き込む.

プリンタストローブ($e8c003):
        bit  7                       1   0
          ┌─────────────┬─┐
          │                          │  │
          └─────────────┴─┘
                                        STR0

        bit 0   STR0    STROBE 信号
                        %0 : STROBE 信号を Low  レベルにする
                        %1 : 〃             High 〃

==============================================================================

・システムポート

address size    R/W
$e8e001 1.b     R/W     #1      コンピュータ画面コントラスト
$e8e003 1.b     R/W     #2      ディスプレイ/3D スコープ制御
$e8e005 1.b       W     #3      カラーイメージユニット制御
$e8e007 1.b     R/W     #4      キーボード/NMI/ドットクロック制御
$e8e009 1.b       W     #5      ROM/DRAM ウェイト数制御
$e8e00b 1.b     R       #6      MPU 種別/動作クロック読み出し
$e8e00d 1.b       W     #7      SRAM 書き込み制御
$e8e00f 1.b       W     #8      本体電源 OFF 制御


System Port #1($e8e001):
        bit  7           4   3           0
          ┌───────┬───────┐
          │              │    CONTRAST  │
          └───────┴───────┘

        bit 3〜0 CONTRAST
                        コンピュータ画面コントラスト
                        %0000 : 最も暗い
                            …
                        %1111 : 最も明るい

System Port #2($e8e003):
        bit  7           4   3   2   1   0
          ┌───────┬─┬─┬─┬─┐
          │              │  │  │  │  │
          └───────┴─┴─┴─┴─┘
                            │      │  └ 3D-R
                         TV CTRL    └── 3D-L

        bit 3   TV CTRL
                        (書き込み時)
                        %0 : ディスプレイ制御信号を 0 にする
                        %1 : 〃                      1 〃
                        (読み込み時)
                        %0 : ディスプレイの電源は ON
                        %1 : 〃                    OFF
        bit 1   3D-L
                        %0 : 3D スコープ左シャッター CLOSE
                        %1 : 〃                       OPEN
        bit 0   3D-R
                        %0 : 3D スコープ右シャッター CLOSE
                        %1 : 〃                       OPEN

System Port #3($e8e005):
        bit  7       5   4               0
          ┌─────┬─────────┐
          │          │                  │
          └─────┴─────────┘
                        カラーイメージユニット制御

        bit 4〜0 カラーイメージユニット制御
                        書き込んだ値は、そのまま IMAGE IN 端子の 17〜21 ピンに
                        出力される.

System Port #4($e8e007):
        bit  7           4   3   2   1   0
          ┌───────┬─┬─┬─┬─┐
          │              │  │  │  │  │
          └───────┴─┴─┴─┴─┘
                            │  │  └ HRL
                            │  └ NMI RESET
                            └ KEY CTRL

        bit 3   KEY CTRL
                        (書き込み時)
                        %0 : キーデータ送信不可
                        %1 : 〃             可
                        (読み込み時)
                        %0 : キーボードコネクタが抜かれている
                        %1 : 〃                   差し込まれている
        bit 2   NMI RESET
                        %0 : NMI リセットしない
                        %1 : 〃           する
                        ※NMI の処理が終了した時点で %1 を書き込む
        bit 1   HRL     高解像度モード時のドットクロック切り替え
                        %0 : 分周比を 1/2、1/3、1/6 にする(通常)
                        %1 : 〃       1/2、1/4、1/8 〃

System Port #5($e8e009):
        bit  7           4   3           0
          ┌───────┬───────┐
          │    ROM Wait  │   DRAM Wait  │
          └───────┴───────┘

        bit 7〜4 ROM Wait
                        ROM アクセス時のウェイト数
                        設定値 +2 のウェイトがかかる.
        bit 3〜0 DRAM Wait
                        DRAM アクセス時のウェイト数
                        設定値 +2 のウェイトがかかる.
                        ただし、$0 の場合スタティックカラム動作中は 0 ウェイト、
                        ページ間を跨ぐ時は 4 ウェイト.
        ※X68030 で追加されたポート.
        ※$4(6 ウェイト)で 16MHz 相当、$a(12 ウェイト)で 10MHz 相当モード.

System Port #6($e8e00b):
        bit  7           4   3           0
          ┌───────┬───────┐
          │    CPU Type  │   CPU Clock  │
          └───────┴───────┘

        bit 7〜4 CPU Type
                        %1100 : 68040
                        %1101 : 68030
                        %1110 : 68020
                        %1111 : 68000
        bit 3〜0 CPU Clock
                        %1001 : 50MHz
                        %1010 : 40〃
                        %1011 : 33〃
                        %1100 : 25〃
                        %1101 : 20〃
                        %1110 : 16〃
                        %1111 : 10〃
        ※X68000 XVI で追加されたポート. それ以前の機種では $ff が読み出される.
          また、X68030 では 16/10MHz 相当モードであっても $dc である.

System Port #7($e8e00d):
        bit  7   6   5   4   3   2   1   0
          ┌───────────────┐
          │   SRAM Write Enable Control  │
          └───────────────┘

        bit 7〜0 SRAM Write Enable Control
                        $31 を書き込むと SRAM への書き込みが許可になり、それ以
                        外の値を書き込むと禁止になる.

System Port #8($e8e00f):
        bit  7   6   5   4   3   2   1   0
          ┌───────┬───────┐
          │              │              │
          └───────┴───────┘
                            Power Off Control

        bit 3〜0 Power Off Control
                        $00、$0f、$0f を順に書き込むと本体の電源が OFF になる.

==============================================================================

・OPM

  OPM(Fm Operator Type-m YM2151)のレジスタへの書き込みは、レジスタ番号設定ポー
トにアクセスするレジスタ番号を設定してからデータポートにデータを書き込む手順で
行う. レジスタからの読み込むは出来ず、常にステータスレジスタが読み出される.


address size    R/W
$e90001 1.b       W     レジスタ番号設定ポート
$e90003 1.b     R       ステータスレジスタ
〃       〃         W     データポート


レジスタ番号設定ポート($e90001):
        bit  7                           0
          ┌───────────────┐
          │                レジスタ番号  │
          └───────────────┘

        次に書き込むレジスタ番号を設定する.

データポート($e90003):
        bit  7                           0
          ┌───────────────┐
          │                      データ  │
          └───────────────┘

        レジスタへ書き込む値を設定する.

ステータスレジスタ($e90003):
        bit  7   6               2   1   0
          ┌─┬─────────┬─┬─┐
          │ B│                  │  │  │
          └─┴─────────┴─┴─┘
                                    │  └ 1ST(A)
                                    └── 1ST(B)

        bit 7   B       書き込み BUSY フラグ
                        %0 : 通常動作            (次のデータは書き込み可  )
                        %1 : データ書き込み中(〃           不可)
        bit 1   1ST(B)
                        %0 : タイマ B はオーバーフローしていない
                        %1 : 〃       オーバーフロー発生
        bit 0   1ST(A)
                        %0 : タイマ A はオーバーフローしていない
                        %1 : 〃       オーバーフロー発生


OPM レジスタ一覧

レジスタ番号  内容
$00             ---
$01             テストレジスタ
$02〜$07 ---
$08             KON レジスタ
$09〜$0e ---
$0f             ノイズジェネレータ制御レジスタ
$10             タイマ A 設定レジスタ(上位)
$11             〃                    (下位)
$12             タイマ B 設定レジスタ
$13             ---
$14             タイマ制御レジスタ
$15〜$17 ---
$18             LFO 周波数設定レジスタ
$19             PMD/AMD 設定レジスタ
$1a             ---
$1b             汎用出力/LFO 出力波形制御レジスタ
$1c〜$1f ---
$20〜$27 チャンネル構成/出力制御レジスタ        ┐
$28〜$2f KC(キーコード)レジスタ           │1 チャンネルごとに設定
$30〜$37 KF(キーフラクション)レジスタ        │
$38〜$3f PMS/AMS 設定レジスタ          ┘
$40〜$5f DT1/MUL 設定レジスタ          ┐
$60〜$7f TL(トータルレベル)設定レジスタ       │
$80〜$7f KS/AR 設定レジスタ            │1 スロットごとに設定
$a0〜$bf AMS-EN/D1R 設定レジスタ               │
$c0〜$df DT2/D2R 設定レジスタ          │
$e0〜$ff D1L/RR 設定レジスタ           ┘


$01(テストレジスタ):
        bit  7                   2   1   0
          ┌───────────┬─┬─┐
          │                TEST  │  │  │
          └───────────┴─┴─┘
                                └─│─┘
                                LFO RESET

        bit 7〜2,0       TEST
                        テスト用の為全て 0 にする.
        bit 1           LFO RESET
                        %0 : LFO 出力をスタートさせる
                        %1 : LFO をリセットする

$08(KON レジスタ):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─────┐
          │  │C2│C1│M2│M1│  CH No.  │
          └─┴─┴─┴─┴─┴─────┘

        bit 6           C2
                        %0 : C2 出力 OFF
                        %1 : 〃      ON
        bit 5           C1
                        %0 : C1 出力 OFF
                        %1 : 〃      ON
        bit 4           M2
                        %0 : M2 出力 OFF
                        %1 : 〃      ON
        bit 3           M1
                        %0 : M1 出力 OFF
                        %1 : 〃      ON
        bit 2〜0 CH No.
                        チャンネル番号選択
                        %000 : チャンネル 1
                                …
                        %111 : チャンネル 8

$0f(ノイズジェネレータ制御レジスタ):
        bit  7   6   5   4               0
          ┌─┬───┬─────────┐
          │NE│      │            NFRQ  │
          └─┴───┴─────────┘

        bit 7    NE     ノイズイネーブル
                        %0 : ノイズ出力 OFF
                        %1 : 〃          ON
        bit 4〜0 NFRQ    ノイズ周波数
                                                  4000
                        ノイズ周波数 Ｆnoise = ───── (kHz)
                                                32×NFRQ
                                                    2^17-1
                        ノイズ周期   Ｔnoise = ──────── (Hz)
                                                Ｆnoise×(10^3)

$10(タイマ A 設定レジスタ):
        bit  7                           0
          ┌───────────────┐
          │                  CLKA(上位)  │
          └───────────────┘

$11(タイマ A 設定レジスタ):
        bit  7                   2   1   0
          ┌───────────┬───┐
          │                      │      │
          └───────────┴───┘
                                    CLKA(下位)

        CLKA = CLKA(上位)<<2 ＋ CLKA(下位)

                          64×(1024−CLKA)
        タイマ時間 Ｔa = ───────── (ms)
                                4000

$12(タイマ B 設定レジスタ):
        bit  7                           0
          ┌───────────────┐
          │                        CLKB  │
          └───────────────┘

                          1024×(256−CLKB)
        タイマ時間 Ｔb = ───────── (ms)
                                4000

$14(タイマ制御レジスタ):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬───┬───┬───┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴───┴───┴───┘
            │      │  │  │  │  │  └ LOAD(A)
           CSM      │  │  │  │  └── LOAD(B)
                    │  │  │  └ IRQEN(A)
                    │  │  └── IRQEN(B)
                    │  └ F-RESET(A)
                    └── F-RESET(B)

        bit 7   CSM
                        %0 : 通常動作
                        %1 : タイマ A のオーバーフロー時に全スロット ON
        bit 5   F-RESET(B)
                        %0 : 通常動作
                        %1 : タイマ B オーバーフローフラグリセット
        bit 4   F-RESET(A)
                        %0 : 通常動作
                        %1 : タイマ A オーバーフローフラグリセット
        bit 3   IRQEN(B)
                        %0 : タイマ B 割り込みディセーブル
                        %1 : 〃                イネーブル
        bit 2   IRQEN(A)
                        %0 : タイマ A 割り込みディセーブル
                        %1 : 〃                イネーブル
        bit 1   LOAD(B)
                        %0 : タイマ B 動作停止
                        %1 : 〃            開始
        bit 0   LOAD(A)
                        %0 : タイマ A 動作停止
                        %1 : 〃            開始

$18(LFO 周波数設定レジスタ):
        bit  7                           0
          ┌───────────────┐
          │                        LFRQ  │
          └───────────────┘

        LFO の発振周波数を設定する.

$19(PMD/AMD 設定レジスタ):
        bit  7   6                       0
          ┌─┬─────────────┐
          │ F│                 PMD/AMD  │
          └─┴─────────────┘

        bit 7    F      下位 7bit を PMD/AMD のどちらに設定するかの選択
                        %0 : AMD
                        %1 : PMD
        bit 6〜0 PMD/AMD
                        LFO の出力レベル設定

$1b(汎用出力/LFO 出力波形制御レジスタ):
        bit  7   6   5           2   1   0
          ┌─┬─┬───────┬───┐
          │  │  │              │   W  │
          └─┴─┴───────┴───┘
            CT1 CT2

        bit 7    CT1    ADPCM クロック切り替え
                        %0 : 8MHz
                        %1 : 4〃
        bit 6    CT2    FDC READY 端子制御
                        %0 : 通常動作
                        %1 : 強制的に READY 状態にする
        bit 1〜0 W       LFO 出力波形選択
                        %00 : 鋸波
                        %01 : 矩形波
                        %10 : 三角波
                        %11 : ノイズ
        * OPM registers can not be read, so partial changes to ADPCM / FDC / LFO are saved in IOCS work $9da.b.
        The setting that does not change is to use the work value, and if you change the value, update the work as well.

$20〜$27(チャンネル構成/出力制御レジスタ):
        bit  7   6   5       3   2       0
          ┌─┬─┬─────┬─────┐
          │  │  │      FL  │     CON  │
          └─┴─┴─────┴─────┘
            │  └ L-CH EN
            └── R-CH EN

        bit 7    R-CH EN
                        %0 : 右チャンネルへの出力 OFF
                        %1 : 〃                    ON
        bit 6    L-CH EN
                        %0 : 左チャンネルへの出力 OFF
                        %1 : 〃                    ON
        bit 5〜3 FL      M1 スロットのフィードバック率
                        %000 : 0(フィードバック OFF)
                        %001 :  π/16
                        %010 :  π/8
                        %011 :  π/4
                        %100 :  π/2
                        %101 :  π
                        %110 : 2π
                        %111 : 4π
        bit 2〜0 CON     Connection(スロットの接続方法)
                        %000 :  M1 → C1 → M2 → C2 → OUT
                        %001 :  C1 ┐
                                M1 ┴→ M2 → C2 → OUT
                        %010 :        M1 ┐
                                C1 → M2 ┴→ C2 → OUT
                        %011 :        M2 ┐
                                M1 → C1 ┴→ C2 → OUT
                        %100 :  M1 → C1 ┐
                                M2 → C2 ┴→ OUT
                        %101 :     ┌→ C1 ┐
                                M1 ┼→ M2 ┼→ OUT
                                   └→ C2 ┘
                        %110 :  M1 → C1 ┐
                                      M2 ┼→ OUT
                                      C2 ┘
                        %111 :  M1 ┐
                                C1 ┼→ OUT
                                M2 ┤
                                C2 ┘

$28〜$2f(KC レジスタ):
        bit  7   6       4   3           0
          ┌─┬─────┬───────┐
          │  │     OCT  │        NOTE  │
          └─┴─────┴───────┘

        bit 6〜4 OCT     オクターブ指定
        bit 3〜0 NOTE    NOTE 指定
                        %0000 : D#
                        %0001 : E (ミ)
                        %0010 : F (ファ)
                        %0100 : F#
                        %0101 : G (ソ)
                        %0110 : G#
                        %1000 : A (ラ)
                        %1001 : A#
                        %1010 : B (シ)
                        %1100 : C (ド)
                        %1101 : C#
                        %1110 : D (レ)
        ※X680x0 では OPM に本来のクロック 3.579545MHz ではなく、4MHz を与えて
          いるので、約 1 音(192.27 セント)高い音が出る.

$30〜$37(KF レジスタ):
        bit  7                   2   1   0
          ┌───────────┬───┐
          │                  KF  │      │
          └───────────┴───┘

        bit 7〜2 KF      キーフラクション
                        1 音(100 セント)間を 1.6 セント(=100/64)単位で音程の微
                        調整を行う.

$38〜$3f(PMS/AMS 設定レジスタ):
        bit  7   6       4   3   2   1   0
          ┌─┬─────┬───┬───┐
          │  │     PMS  │      │ AMS  │
          └─┴─────┴───┴───┘

        bit 6〜4 PMS     LFO による周波数変調度設定
                        %000 :     0 セント
                        %001 : ±  5 〃
                        %010 : ± 10 〃
                        %011 : ± 20 〃
                        %100 : ± 50 〃
                        %101 : ±100 〃
                        %110 : ±400 〃
                        %111 : ±700 〃
        bit 1〜0 AMS     LFO による出力レベル変調度設定
                        %00 : 0dB
                        %01 : 23.90625dB
                        %10 : 47.8125 〃
                        %11 : 95.625  〃
        ※数値はいずれも LFO の出力レベルが最大の時の値.

$40〜$5f(DT1/MUL 設定レジスタ):
        bit  7   6       4   3           0
          ┌─┬─────┬───────┐
          │  │     DT1  │         MUL  │
          └─┴─────┴───────┘

        bit 6〜4 DT1
                        KC、KF で与えた周波数をさらにずらす量の設定(微調整)
        bit 3〜0 MUL
                        KC、KF で与えた周波数の倍率の設定(0 なら 0.5 倍)

$60〜$7f(TL 設定レジスタ):
        bit  7   6                       0
          ┌─┬─────────────┐
          │  │                      TL  │
          └─┴─────────────┘

        bit 6〜0 TL      トータルレベル
                        各スロットの出力レベルを 0.75dB 単位で設定する. TL = 0
                        の時が最も出力信号レベルが大きく、TL = $7f の時が最も
                        小さい.
                        Ｌmax = 最大出力レベル
                        出力レベル Ｌ = Ｌmax×10^(−1.75/20×TL)

$80〜$7f(KS/AR 設定レジスタ):
        bit  7   6   5   4               0
          ┌───┬─┬─────────┐
          │  KS  │  │              AR  │
          └───┴─┴─────────┘

        bit 7〜6 KS
                        KC によるアタック、ファーストディケイ、セカンドディケ
                        イ、リリース時間の変化量の設定. 各設定値のレートに対し、
                        int(int(KC/4)/(2^(3-KS)))を加える.
        bit 4〜0 AR      アタックレート
                        キーオンされてから減衰量が 0dB になるまでの時間の設定.

$a0〜$bf(AMS-EN/D1R 設定レジスタ):
        bit  7   6   5   4               0
          ┌─┬───┬─────────┐
          │  │      │             D1R  │
          └─┴───┴─────────┘
          AMS-EN

        bit 7    AMS-EN
                        LFO による出力レベル変調
                        %0 : 変調をかけない
                        %1 : 〃    かける
        bit 4〜0 D1R     ファーストディケイ時間
                        ファーストディケイからセカンドディケイに移るまでの時間.

$c0〜$df(DT2/D2R 設定レジスタ):
        bit  7   6   5   4               0
          ┌───┬─┬─────────┐
          │ DT2  │  │             D2R  │
          └───┴─┴─────────┘

        bit 7〜6 DT2     KC、KF で与えた周波数をさらにずらす量
                        %00 : +  0 セント(×1.00 倍)
                        %01 : +600 〃     (×1.41 倍)
                        %10 : +781 〃     (×1.57 倍)
                        %11 : +950 〃     (×1.73 倍)
        bit 4〜9 D2R     セカンドディケイ時間
                        セカンドディケイから完全な消音までの時間.

$e0〜$ff(D1L/RR 設定レジスタ):
        bit  7   6   5   4   3   2   1   0
          ┌───────┬───────┐
          │         D1L  │          RR  │
          └───────┴───────┘

        bit 7〜4 D1L
                        ファーストディケイからセカンドディケイに移るときのレベ
                        ル Ｌd1 を決定する.
                        0 ≦ D1L ≦ $e のとき、 Ｌd1 = -3×D1L (dB)
                        D1L = $f のとき、   Ｌd1 = -3×D1L -48 (dB)
        bit 3〜0 RR      リリース時間
                        キーオフしてから音が消えるまでの時間.

==============================================================================

・ADPCM

address size    R/W
$e92001 1.b     R       ADPCM status
〃       〃         W     ADPCM command
$e92003 1.b     R/W     Data input / output
$e9a005 1.b     R/W     ADPCM sample rate / output control


ADPCM ステータス($e92001):
        bit  7   6   5                   0
          ┌─┬─┬───────────┐
          │  │ 1│                   0  │
          └─┴─┴───────────┘
          PLAY/REC

        bit 7   PLAY/REC
                %0 : 再生中
                %1 : 録音中/スタンバイ中

ADPCM コマンド($e92001):
        bit  7               3   2   1   0
          ┌─────────┬─┬─┬─┐
          │               0  │  │  │SP│
          └─────────┴─┴─┴─┘
                                │  └ PLAY ST
                                └ REC ST

        bit 2   REC ST
                %0 : 通常動作
                %1 : ADPCM 録音開始
        bit 1   PLAY ST
                %0 : 通常動作
                %1 : ADPCM playback start
        bit 0   SP
                %0 : 通常動作
                %1 : 録音/再生動作停止

データ入出力($e92003):
        bit  7           4   3           0
          ┌───────┬───────┐
          │    Data n+1  │      Data n  │
          └───────┴───────┘

        ADPCM データの入出力を行うレジスタ. 4 ビットのサンプリングデータを 2
        個一組で入出力する. サンプリングデータは最上位ビットが符号で、残りが絶
        対値となっている.

ADPCM Sample rate / output control ($e9a005):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬───┬───┐
          │  │  │  │  │      │      │
          └─┴─┴─┴─┴───┴───┘
            │  │  PC5 PC4 │      └ PCM PAN
            │  │          └ Sampling Rate
            │  └ IOA5
            └ IOA6

        bit 7    IOA6   ジョイスティック #1 トリガ B
                        %0 : 通常動作
                        %1 : B ボタンが押された状態にする
        bit 6    IOA5   ジョイスティック #1 トリガ A
                        %0 : 通常動作
                        %1 : A ボタンが押された状態にする
        bit 5    PC5    ジョイスティック #2 コントロール
                        %0 : 通常動作
                        %1 : 操作禁止
        bit 4    PC4    ジョイスティック #1 コントロール
                        %0 : 通常動作
                        %1 : 操作禁止
        bit 3〜2 Sampling Rate
                        ADPCM サンプリングレート切り替え
                                                         4MHz    8MHz
                        %00 : 基本クロックの 1/1024    (3.9k    7.8k)
                        %01 : 〃      1/768      (5.2k   10.4k)
                        %10 : 〃      1/512      (7.8k   15.6k)
                        %11 : (未使用)
        bit 1〜0 PCM PAN
                        ADPCM 出力制御
                        %00 : Both left and right ON
                        %01 : 右のみ ON
                        %10 : 左のみ ON
                        %11 : 左右とも OFF
        ※ Select the input clock by bit 7 of OPM register $1b

==============================================================================

・FDC

address size    R/W
$e94001 1.b     R       FDC ステータスレジスタ
〃       〃         W     FDC コマンドレジスタ
$e94003 1.b     R       FDC データレジスタ
〃       〃         W     FDC コマンドレジスタ
$e94005 1.b     R       ドライブステータス
〃       〃         W     ドライブオプション信号制御
$e94007 1.b       W     アクセスドライブセレクト等


FDC ステータスレジスタ($e94001):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            RQM DIO NDM CB  D3B D2B D1B D0B

        bit 7    RQM    Request for Master
                        %0 : FDC のデータ転送準備が出来ていない
                        %1 : 〃                           いる
        bit 6    DIO    Data Input/Output
                        %0 : ホストから FDC への転送
                        %1 : FDC からホストへの転送
        bit 5    NDM    Non-DMA Mode
                        %0 : Non-DMA モードでのデータ転送中でない
                        %1 : 〃                             である
                        ※C-PHASE、R-PHASE では常に %0.
        bit 4    CB     FDC Busy
                        %0 : FDC 動作中でない
                        %1 : FDC 動作中
                        ※シーク/キャリブレートの E-PHASE ではセットされない.
        bit 3〜0 D3B〜D0B
                        FDn Busy
                        %0 : ドライブはシーク中/シーク完了割り込み保留中でない
                        %1 : 〃                                          である

FDC コマンドレジスタ($e94001):
        初期化等のコマンド(SET STANDBY、RESET STANDBY、SOFTWARE RESET の指定を
        行うレジスタ.

FDC データレジスタ($e94003):
        データの入出力を行うレジスタ.

FDC コマンドレジスタ($e94003):
        通常のコマンドの指定を行うレジスタ.

IOC ドライブステータス($e94005):
        bit  7   6   5                   0
          ┌─┬─┬───────────┐
          │  │  │                   0  │
          └─┴─┴───────────┘
            │  └ ERROR DISK
            └ DISK IN

        bit 7   DISK IN
                %0 : ディスク非挿入状態
                %1 : 〃      挿入状態
        bit 6   ERROR DISK
                %0 : ディスク誤挿入ではない
                %1 : ディスク誤挿入(bit 7 も %1 になる)

IOC ドライブオプション信号制御($e94005):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │ 0│  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │      │  │  │  └ DRIVE #0
            │  │  │      │  │  └── DRIVE #1
            │  │  │      │  └──── DRIVE #2
            │  │  │      └────── DRIVE #3
            │  │  └ EJECT ON/OFF
            │  └ EJECT MASK
            └ LED CTRL

        bit 7   LED CTRL
                %0 : FD アクセスランプを消灯する
                %1 : 〃                  点滅する
                ※メディア非挿入状態のときのみ有効.
        bit 6   EJECT MASK
                %0 : イジェクトボタン有効(イジェクトボタン上の LED が 点灯)
                %1 : 〃               無効(〃                             消灯)
        bit 5   EJECT ON/OFF
                %0 : メディアをイジェクトしない
                %1 : 〃                   する
        bit 3〜0
                DRIVE #3〜#0
                %1 → %0 の変化時、bit 7〜5 で選択したオプション機能が働く.

IOC アクセスドライブセレクト等($e94007):
        bit  7   6   5   4   3   2   1   0
          ┌─┬───┬─┬───┬───┐
          │  │   0  │  │   0  │      │
          └─┴───┴─┴───┴───┘
            │          │              └ ACCESS DRIVE
            │          └ 2HD/2DD
            └ MOTOR ON

        bit 7   MOTOR ON
                %0 : ドライブセレクトディセーブル/モータ OFF
                                                (アクセスランプは緑色になる)
                %1 : 〃               イネーブル  /モータ ON
                                                (〃               赤色〃    )
        bit 4   2HD/2DD
                %0 : 2HD ディスク
                %1 : 2DD/2D ディスク
                ※X680x0 内蔵ドライブでは 2HD のみ使用可能.
        bit 1〜0
                ACCESS DRIVE
                アクセスするドライブ番号を設定する.

==============================================================================

・SASI

address size    R/W
$e96001 1.b     R/W     SASI データ入出力
$e96003 1.b     R       SASI ステータス入力
〃       〃         W     SEL 信号 H
$e96005 1.b       W     データ任意
$e96007 1.b       W     SEL 信号 L


SASI データ入出力($e96001):
          データの入出力ポート. このポートにアクセスすると自動的に REQ-ACK ハ
        ンドシェークが行われる.

SASI ステータス入力($e96003):
        bit  7       5   4   3   2   1   0
          ┌─────┬─┬─┬─┬─┬─┐
          │          │  │  │  │  │  │
          └─────┴─┴─┴─┴─┴─┘
                        MSG C/D I/O BSY REQ

        bit 4   MSG     Message
                        %0 : DATA ラインの内容はメッセージバイトではない
                        %1 : 〃                                  である
        bit 3   C/D     Command/Data
                        %0 : DATA ラインの内容はデータ
                        %1 : 〃                  コマンド/ステータス
        bit 2   I/O     Input/Output
                        %0 : ホスト→コントローラ(Output)
                        %1 : コントローラ→ホスト(Input)
        bit 1   BSY     BUSY
                        %0 : BUSY 状態ではない
                        %1 : 〃       である
        bit 0   REQ     Request
                        %0 : 通常動作
                        %1 : コントローラがホストにデータ転送を要求している

SEL 信号 H($e96003):
        書き込まれたデータを SASI バスに出力し、SEL 信号を '0'(H レベル)にする.

データ任意($e96005):
        RESET 信号を約 300μs 間 '1'(L レベル)にする.

SEL 信号 H($e96007):
        書き込まれたデータを SASI バスに出力し、SEL 信号を '1'(L レベル)にする.

==============================================================================

・SPC

address size    R/W
$e96021 1.b     R       BDID (Bus Device ID)
〃       〃         W     〃
$e96023 1.b     R/W     SCTL (SPC Control)
$e96025 1.b     R/W     SCMD (SPC Command)
$e96029 1.b     R       INTS (Interrupt Sense)
〃       〃         W     〃
$e9602b 1.b     R       PSNS (Phase Sense)
〃       〃         W     SDGC (SPC Diag Control)
$e9602d 1.b     R       SSTS (SPC Status)
$e9602f 1.b     R       SERR (SPC Error Status)
$e96031 1.b     R/W     PCTL (Phase Control)
$e96033 1.b     R       MBC  (Modified Byte Counter)
$e96035 1.b     R/W     DREG (Data Register)
$e96037 1.b     R       TEMP (Temporary Register)
〃       〃         W     〃
$e96039 1.b     R/W     TCH  (Transfer Counter High)
$e9603b 1.b     R/W     TCM  (Transfer Counter Mid)
$e9603d 1.b     R/W     TCL  (Transfer Counter Low)

※SCSI ボードの場合は $ea0001〜$ea001d.


BDID read($e96021):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │#7│#6│#5│#4│#3│#2│#1│#0│
          └─┴─┴─┴─┴─┴─┴─┴─┘
            ID

        設定した ID 番号のビットが %1 になったデータが読み出される.

BDID write($e96021):
        bit  7   6   5   4   3   2   1   0
          ┌─────────┬─────┐
          │                  │      ID  │
          └─────────┴─────┘

        自分自身の ID 番号を設定する.

SCTL($e96023):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │  │  │  │  │  └ Interrupt Enable
            │  │  │  │  │  │  └ Reselect Enable
            │  │  │  │  │  └ Select Enable
            │  │  │  │  └ Parity Enable
            │  │  │  └ Arbitration Enable
            │  │  └ Diag Mode
            │  └ Control Reset
            └ Reset&Disable

        bit 7   Reset&Disable
                        SPC 内レジスタ及び制御回路のリセット
                        %0 : 通常動作
                        %1 : リセットする
        bit 6   Control Reset
                        SPC のデータ転送制御回路のリセット
                        %0 : 通常動作
                        %1 : リセットする
        bit 5   Diag Mode
                        %0 : 通常動作
                        %1 : SPC を自己診断モードにする
        bit 4   Arbitration Enable
                        アービトレーションフェーズの実行可否制御
                        %0 : アービトレーションフェーズは行わない
                        %1 : セレクション/リセレクションフェーズの前にアービト
                             レーションフェーズを行う
        bit 3   Parity Enable
                        SCSI バスのデータラインのパリティチェック
                        %0 : パリティチェックをしない
                        %1 : 〃                 する
        bit 2   Select Enable
                        セレクションへの応答制御
                        %0 : セレクションフェーズに応答しない
                        %1 : 〃                         する
        bit 1   Reselect Enable
                        リセレクションへの応答制御
                        %0 : リセレクションフェーズに応答しない
                        %1 : 〃                           する
        bit 0   Interrupt Enable
                        SPC 割り込み制御
                        %0 : 割り込み発生禁止
                        %1 : 〃           許可
                        ※割り込み禁止状態でも、割り込み要因は INTS レジスタに
                          反映される.

SCMD($e96025):
        bit  7       5   4   3   2   1   0
          ┌─────┬─┬─┬─────┐
          │          │  │  │  │  │  │
          └─────┴─┴─┴─────┘
                │      │  │      └ Transfer Modifire
                │      │  └ Intercept Transfer
                │      └ RST Out
                └ Command Code

        bit 7〜5 Command Code
                        SPC への動作指示
                        %000 : Bus Release
                        %001 : Select
                        %010 : Reset ATN
                        %011 : Set ATN
                        %100 : Transfer
                        %101 : Transfer Pause
                        %110 : Reset ACK/REQ
                        %111 : Set ACK/REQ
        bit 4    RST Out
                        SCSI バスの RST(リセット)信号制御
                        %0 : RTS を %0(High レベル)にする(通常動作)
                        %1 : 〃     %1(Low  〃    )にする
        bit 3    Intercept Transfer
                        マニュアル転送時のデータバッファ保持
                        %0 : バッファ内容は保持されない
                        %1 : 〃                 される
        bit 2    Program Transfer
                        %0 : DMA で転送を行う(DREQ は出力される  )
                        %1 : CPU 〃           (〃  されない)
        bit 1    (未使用)
                        常に %0 にする.
        bit 0    Termination Mode
                        イニシエータ時:
                        %0 : バイトカウント = 0 で転送停止
                        %1 : バイトカウント = 0 でも REQ に応答する
                        ターゲット時:
                        %0 : バイトカウント = 0 になるまで転送を続行する
                        %1 : パリティエラー発生時に転送を停止する

INTS read($e96029):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │  │  │  │  │  └ Reset Condition
            │  │  │  │  │  │  └ SPC Hard Error
            │  │  │  │  │  └ Time Out
            │  │  │  │  └ Service Required
            │  │  │  └ Command Complete
            │  │  └ Disconnected
            │  └ Reselected
            └ Selected

        bit 7   Selected
                %0 : 通常動作
                %1 : SCSI 上の他のデバイスからセレクションフェーズによって選択
                     された
        bit 6   Reselected
                %0 : 通常動作
                %1 : SCSI 上の他のデバイスからリセレクションフェーズによって選
                     択された
        bit 5   Disconnected
                %0 : 通常動作
                %1 : SCSI 上でバスフリーフェーズが検出された(SCTL bit 7=1 の時).
        bit 4   Command Complete
                %0 : 通常動作
                %1 : Select コマンドや Transfer コマンドの実行が完了した
        bit 3   Service Required
                %0 : 通常動作
                %1 : イニシエータとして動作中にバス上のフェーズと PCTL レジス
                     タで指定されるフェーズが一致しないため、Transfer コマンド
                     を実行できない(できなくなった)
        bit 2   Time Out
                %0 : 通常動作
                %1 : セレクション/リセレクション実行後一定期間たっても相手から
                     の応答がない
        bit 1   SPC Hard Error
                %0 : 通常動作
                %1 : SERR レジスタに示されるエラーのうち、
                        ・TC Parity Error
                        ・Short Transfer Period
                     が検出された
        bit 0   Reset Condition
                %0 : 通常動作
                %1 : SCSI バスが Reset された

INTS write($e96029):
        (INTS read と同様)

        %1 を書き込むと、INTS のそのビットの値が %0 に戻る.

PSNS($e9602b):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            REQ ACK ATN SEL BSY MSG C/D I/O

        bit 7   REQ
                %0 : REQ 信号は '0'(High レベル)
                %1 : 〃          '1'(Low  〃    )
        bit 6   ACK
                %0 : ACK 信号は '0'(High レベル)
                %1 : 〃          '1'(Low  〃    )
        bit 5   ATN
                %0 : ATN 信号は '0'(High レベル)
                %1 : 〃          '1'(Low  〃    )
        bit 4   SEL
                %0 : SEL 信号は '0'(High レベル)
                %1 : 〃          '1'(Low  〃    )
        bit 3   BSY
                %0 : BSY 信号は '0'(High レベル)
                %1 : 〃          '1'(Low  〃    )
        bit 2   MSG
                %0 : MSG 信号は '0'(High レベル)
                %1 : 〃          '1'(Low  〃    )
        bit 1   C/D
                %0 : C/D 信号は '0'(High レベル)
                %1 : 〃          '1'(Low  〃    )
        bit 0   I/O
                %0 : I/O 信号は '0'(High レベル)
                %1 : 〃          '1'(Low  〃    )

SDGC($e9602b):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │      │  │  │  └ Diag I/O
            │  │  │      │  │  └ Diag C/D
            │  │  │      │  └ Diag MSG
            │  │  │      └ Diag BSY
            │  │  └ Xfer Enable
            │  └ Diag ACK
            └ Diag REQ

        bit 7〜6,3〜0
                診断モード時に SCSI バスの状態を設定する.
        bit 5   Xfer Enable
                プログラム転送時
                %0 : Data Request のかわりに割り込みを発生しない
                %1 : 〃                                    する

SSTS($e9602d):
        bit  7   6   5   4   3   2   1   0
          ┌───┬─┬─┬─┬─┬───┐
          │      │  │  │  │  │      │
          └───┴─┴─┴─┴─┴───┘
            │  │  │  │  │  │  │  └ DREG status Empty
            │  │  │  │  │  │  └── 〃              Full
            │  │  │  │  │  └ TC = 0
            │  │  │  │  └ SCSI ResetIn
            │  │  │  └ Transfer in progress
            │  │  └ SPC Busy
            │  └ Connected TARG
            └── 〃            INIT

        bit 7〜6 Connected
                        SPC と SCSI の結合状態
                        %00 : 非結合中
                        %01 : ターゲットとして結合中
                        %10 : イニシエータとして結合中
                        %11 : (未定義)
        bit 5    SPC Busy
                        %0 : コマンド実行中/実行待ち状態ではない
                        %1 : 〃                          である
        bit 4    Transfer in progress
                        %0 : 通常動作
                        %1 : ハード転送実行中/SCSI で転送フェーズが要求されて
                             いる
        bit 3    SCSI ResetIn
                        SCSI バス上の RST 信号の状態
                        %0 : RST 信号は '0'(High レベル)
                        %1 : 〃          '1'(Low  〃    )
        bit 2    TC = 0
                        %0 : 転送バイトカウンタの内容がゼロではない
                        %1 : 〃                             である
        bit 1〜0 DREG status
                        内部データバッファレジスタの状態
                        %00 : 1〜7 バイトのデータがある
                        %01 : バッファは空
                        %10 : 8 バイトのデータがある
                        %11 : (未使用)

SERR($e9602f):
        bit  7   6   5   4   3   2   1   0
          ┌───┬─┬─┬─┬─┬─┬─┐
          │      │  │ 0│  │ 0│  │ 0│
          └───┴─┴─┴─┴─┴─┴─┘
            │  │  │      │      └ Short Transfer Period
            │  │  │      └ TC Parity Error
            │  │  └ Xfer Out
            │  └ Data Error SPC
            └── 〃             SCSI

        bit 7〜6 Data Error
                        %00 : パリティエラーは検出されなかった
                        %01 : アウトプット動作時、SCSI バスに出力しようとした
                              データにパリティエラーを検出した
                        %10 : (未定義)
                        %11 : インプット動作時、SCSI バスに出力しようとしたデ
                              ータにパリティエラーを検出した
        bit 5    Xfer Out
                        %0 : 通常動作
                        %1 : Data Request 中(SDGC bit 5=1 の時に使用)
        bit 3    TC Parity Error
                        %0 : 正常
                        %1 : 転送バイトカウンタの減算動作でパリティエラーが発
                             生した
        bit 1    Short Transfer Period
                        %0 : 正常
                        %1 : REQ、または ACK 信号が SPC が追従できないほど速い
                             周期で入力された

PCTL($e96031):
        bit  7   6           3   2   1   0
          ┌─┬───────┬─────┐
          │  │           0  │          │
          └─┴───────┴─────┘
            │                  │  │  └ Transfer Phase MSG
           Busfree INT Enable   │  └── 〃                 C/D
                                └──── 〃           I/O

        bit 7    Busfree INT Enable
                %0 : 通常動作
                %1 : バスフリーフェーズを検出すると Disconnected 割り込みを発
                     生する
                ※Select コマンド発行時や Disconnected 割り込みをリセットする
                  時は %0 にしておくこと.
        bit 2〜0 Transfer Phase
                ターゲット  動作時:SCSI で実行するフェーズを指定
                イニシエータ動作時:実行予定のフェーズを指定
                %000 : データアウトフェーズ
                %001 : データイン〃
                %010 : コマンド〃
                %011 : ステータス〃
                %100 : (未使用)
                %101 : (未使用)
                %110 : メッセージアウト〃
                %111 : メッセージイン〃

MBC($e96033):
        bit  7           4   3           0
          ┌───────┬───────┐
          │           0  │         MBC  │
          └───────┴───────┘

        bit 3〜0 MBC
                SPC 内部のバッファと CPU とのデータ転送数を制御するレジスタ.
                初期値は TCL レジスタの下位 4 ビットが設定される.

DREG($e96035):
        bit  7                           0
          ┌───────────────┐
          │                        Data  │
          └───────────────┘

        データ入出力を行うレジスタ. 8 バイトの FIFO バッファとなっている.

TEMP($e96037):
        bit  7                           0
          ┌───────────────┐
          │              Temporary Data  │
          └───────────────┘

        マニュアル転送モードのときに SCSI とのデータ転送に使用するレジスタ.
        また、アービトレーション/セレクションで出力する ID 設定用としても使用
        される.

TCH($e96039):
        bit  7                           0
          ┌───────────────┐
          │      Transfer Counter(上位)  │
          └───────────────┘

          TCH/TCM/TCL を合せて、3 バイト(24 ビット)の転送バイト数カウンタとし
        て使用される. ハード転送時に SCSI 上で 1 バイトの転送が行われるごとに
        デクリメントされ、残りバイト数を保持する. また、セレクションフェーズで
        のタイムアウト時間設定用としても使用される.

TCM($e9603b):
        bit  7                           0
          ┌───────────────┐
          │      Transfer Counter(中位)  │
          └───────────────┘

TCL($e9603d):
        bit  7                           0
          ┌───────────────┐
          │      Transfer Counter(下位)  │
          └───────────────┘

==============================================================================

・SCC

  SCC(シリアルコミュニケーションコントローラ Z8530)のレジスタへのアクセスは、
コマンドポートにこれからアクセスするレジスタ番号を書き込み、次にコマンドポート
へ値を書き込むか読み込めばよい. レジスタ番号を設定していない時は WR0/RR0 にア
クセスされる. レジスタ番号を設定後、一度アクセスするとレジスタの指定が解除され
次回指定しなければ WR0/RR0 へのアクセスに戻る.


address size
$e98001 1.b     チャンネル B コマンドポート
$e98003 1.b     〃            データポート
$e98005 1.b     チャンネル A コマンドポート
$e98007 1.b     〃            データポート


WR0     CRC 初期化/SCC 初期化/アクセスレジスタ選択
WR1     送受信割り込み設定
WR2     割り込みベクタ設定
WR3     受信動作パラメータ設定
WR4     送受信動作関係のパラメータ設定
WR5     送信動作パラメータ設定
WR6     同期キャラクタ/SDLC アドレス設定
WR7     〃             /SDLC フラグ設定
WR8     送信バッファ($e98003/$e98007 と同一)
WR9     割り込み発生制御/SCC リセット(チャンネル A/B で共通)
WR10    トランスミッタ/レシーバ制御
WR11    クロックモード制御
WR12    ボーレートジェネレータ(下位)
WR13    〃                     (上位)
WR14    DPLL 動作モード設定
WR15    外部/ステータス割り込み発生許可/禁止制御

RR0     送受信バッファ/制御端子ステータス
RR1     スペシャル Rx コンディションステータス/端数コード読み出し
RR2     割り込みベクタ
RR3     ペンディングされている割り込み要因(チャンネル A にのみ存在)
RR8     受信バッファ($e98003/$e98007 と同一)
RR10    FM モードでの Missing Clock/SDLC での動作ステータス
RR12    WR12 設定値
RR13    WR13 〃
RR15    WR15 〃


WR0:
        bit  7   6   5       3   2       0
          ┌───┬─────┬─────┐
          │      │          │          │
          └───┴─────┴─────┘
            │          │          └ Register Select
            │          └ Command Code
            └ CRC Reset Command

        bit 7〜6 CRC Reset Command
                        %00 : ヌルコード(何もしない)
                        %01 : 受信 CRC チェッカリセット
                        %10 : 送信 CRC ジェネレータリセット
                        %11 : 送信アンダーラン/EOM ラッチリセット
        bit 5〜3 Command Code
                        SCC 動作コマンド
                        %000 : ヌルコード(何もしない)
                        %001 : 上位レジスタ(WR8/RR8〜15)選択
                        %010 : 外部ステータス変化割り込みリセット
                        %011 : アボート送出
                        %100 : 次の受信割り込みイネーブル
                        %101 : 送信割り込みペンディングビットリセット
                        %110 : エラーリセット
                        %111 : 最上位 IUS リセット
        bit 2〜0 Register Select
                        次にアクセスするレジスタ番号を設定する.

WR1:
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬───┬─┬─┬─┐
          │  │  │  │      │  │  │  │
          └─┴─┴─┴───┴─┴─┴─┘
            │  │  │  │      │  │  └ E/S INT Enable
            │  │  │  │      │  └ Tx INT Enable
            │  │  │  │      └ Parity is Spec. Condition
            │  │  │  └ Rx INT Mode
            │  │  └ Wait/DREQ on Rx/Tx
            │  └ Wait/DREQ Function
            └ Wait/DREQ Enable

        bit 7    Wait/DREQ Enable
                        %0 : W/REQ 機能ディセーブル
                        %1 : 〃        イネーブル
                        ※X680x0 では %0 に設定する.
        bit 6    Wait/DREQ Function
                        %0 : W/REQ 端子は Wait 信号として動作する
                        %1 : 〃            DMA 要求信号として動作する
        bit 5    Wait/DREQ on Rx/Tx
                        %0 : 送信動作で W/REQ 信号を使う
                        %1 : 受信〃
        bit 4〜3 Rx INT Mode
                        受信割り込みモード
                        %00 : 受信割り込み禁止
                        %01 : 最初のキャラクタのみ割り込み発生
                        %10 : 全ての受信キャラクタで割り込み発生
                        %11 : スペシャル Rx コンディション時のみ割り込み発生
                        ※%01、%10 はスペシャル Rx コンディション割り込みも可.
        bit 2    Parity is Spec. Condition
                        パリティエラー
                        %0 : スペシャル Rx コンディションにしない
                        %1 : 〃                             する
        bit 1    Tx INT Enable
                        %0 : 送信割り込み禁止
                        %1 : 〃           許可
        bit 0    E/S INT Enable
                        %0 : 外部ステータス割り込み禁止
                        %1 : 〃                     許可

WR2:
        bit  7                           0
          ┌───────────────┐
          │               Vector Number  │
          └───────────────┘

          割り込み発生時に使用するベクタ番号を設定する. WR9 VIS が %1 の時、設
        定した値のビット 4〜6 又は 3〜1 を割り込み要因によって変化させる.

WR3:
        bit  7   6   5   4   3   2   1   0
          ┌───┬─┬─┬─┬─┬─┬─┐
          │      │  │  │  │  │  │  │
          └───┴─┴─┴─┴─┴─┴─┘
            │      │  │  │  │  │  └ Rx Enable
            │      │  │  │  │  └ SYNC Char. Load INH
            │      │  │  │  └ Address Search Mode
            │      │  │  └ Rx CRC Enable
            │      │  └ Enter Hunt Mode
            │      └ Auto Enable
            └ Rx bit/char.

        bit 7〜6 Rx bit/char.
                        受信キャラクタのビット長を指定する.
                        %00 : 5 ビット/キャラクタ
                        %01 : 7 〃
                        %10 : 6 〃
                        %11 : 8 〃
        bit 5    Auto Enable
                        %0 : 通常モード   ___                      ___
                        %1 : オートモード(DCD = 'L' で受信可、CTS = 'L' で送信
                             可となる)
        bit 4    Enter Hunt Mode
                        %0 : 同期が確率した
                        %1 : 同期確率モードとなる(同期キャラクタ/フラグと一致
                             するキャラクタが入力されるまで待つ)
        bit 3    Rx CRC Enable
                        %0 : 受信キャラクタの CRC 計算を行わない
                        %1 : 〃                          行う
        bit 2    Address Search Mode
                        SDLC モード時:
                        %0 : 通常動作
                        %1 : WR6 に書き込んだアドレスと一致しないメッセージを
                             無視する
        bit 1    SYNC Char. Load INH
                        SDLC モード時:
                        %0 : 通常動作
                        %1 : アドレスの上位 4 ビットだけを比較する
                        SDLC モード以外の同期モード時:
                        %0 : 通常動作
                        %1 : WR6 と一致するキャラクタは受信しない
        bit 0    Rx Enable
                        %0 : 受信動作禁止
                        %1 : 〃      許可

WR4:
        bit  7   6   5   4   3   2   1   0
          ┌───┬───┬───┬─┬─┐
          │      │      │      │  │  │
          └───┴───┴───┴─┴─┘
            │      │      │      │  └ Parity Enable
            │      │      │      └ Parity Even/Odd
            │      │      └ STOP Bit
            │      └ SYNC Mode
            └ Clock Mode

        bit 7〜6 Clock Mode
                %00 : × 1 クロックモード
                %01 : ×16 〃
                %10 : ×32 〃
                %11 : ×64 〃
        bit 5〜4 SYNC Mode
                %00 :  8 ビット同期キャラクタ(Monosync)
                %01 : 16 〃                   (Bisync)
                %10 : SDLC モード('01111110' がフラグ)
                %11 : 外部同期モード(External Sync、X680x0 では使用不可)
                ※STOP Bit = %00 の時のみ有効.
        bit 3〜2 STOP Bit
                %00 : ストップビット なし(同期モード)
                %01 : 〃      1   ビット(非同期モード)
                %10 : 〃      1.5 〃    (〃            )
                %11 : 〃      2   〃    (〃            )
        bit 1    Parity Even/Odd
                %0 : 奇数
                %1 : 偶数
        bit 0    Parity Enable
                %0 : パリティなし
                %1 : 〃      あり

        ※X680x0 ではチャンネル B はマウスとの通信に使用されているため、非同期
          1/16 クロックモード、データ長 8 ビット、ストップビット 2、パリティ無
          しで 4800bps の設定にしなければならない.

WR5:
        bit  7   6   5   4   3   2   1   0
          ┌─┬───┬─┬─┬─┬─┬─┐
          │  │      │  │  │  │  │  │
          └─┴───┴─┴─┴─┴─┴─┘
            │  │      │  │  │  │  └ Tx CRC Enable
            │  │      │  │  │  └ RTS
            │  │      │  │  └ SDLC/CRC-16
            │  │      │  └ Tx Enable
            │  │      └ Send Break
            │  └ Tx Bit/char.
            └ DTR

        bit 7    DTR
                        %0 : DTR 信号を 'H' にする
                        %1 : 〃          'L' 〃
        bit 6〜5 Tx Bit/char.
                        送信キャラクタのビット長を指定する.
                        %00 : 5 ビット/キャラクタ
                        %01 : 7 〃
                        %10 : 6 〃
                        %11 : 8 〃
        bit 4    Send Break
                        %0 : 通常動作
                        %1 : ブレークを送出する(TxD を '0' のままにする)
        bit 3    Tx Enable
                        %0 : 送信動作禁止
                        %1 : 〃      許可
        bit 2    SDLC/CRC-16
                        使用する CRC 生成多項式を選択する
                        %0 : CRC-CCITT
                        %1 : CRC-16
                        ※SDLC 使用時は %0 にすること.
        bit 1    RTS         ___
                        %0 : RTS 端子を 'H' にする
                        %1 : 〃          'L' 〃
        bit 0    Tx CRC Enable
                        %0 : 送信キャラクタの CRC 計算を行わない
                        %1 : 〃                          行う

WR6:
        bit  7                           0
          ┌───────────────┐
          │          SYHC Char./Address  │
          └───────────────┘

        Monosync モード時 : 同期キャラクタを設定する.
        Bisync モード時   : 同期キャラクタの下位バイトを設定する.
        SDLC モード時         : 自局のアドレスを設定する.

WR7:
        bit  7                           0
          ┌───────────────┐
          │                  SYHC Char.  │
          └───────────────┘

        Monosync モード時 : 同期キャラクタを設定する.
        Bisync モード時   : 同期キャラクタの上位バイトを設定する.
        SDLC モード時         : フラグキャラクタ('01111110')を設定する.

WR8:
        bit  7                           0
          ┌───────────────┐
          │                送信バッファ  │
          └───────────────┘

        $e98003(チャンネル B)/$e98007(チャンネル A)と同一.

WR9:
        bit  7   6   5   4   3   2   1   0
          ┌───┬─┬─┬─┬─┬─┬─┐
          │      │ 0│  │  │  │  │  │
          └───┴─┴─┴─┴─┴─┴─┘
            │          │  │  │  │  └ VIS
            │          │  │  │  └ NV
            │          │  │  └ DLC
            │          │  └ MIE
            │          └ Status High/Status Low
            └ Reset Command

        bit 7〜6 Reset Command
                        %00 : リセットしない
                        %01 : チャンネルリセット B
                        %10 : チャンネルリセット A
                        %11 : 強制ハードウェアリセット
        bit 4    Status High/Status Low
                        割り込み要因によって変化させるベクタのビット位置
                        %0 : bit 3〜1
                        %1 : bit 4〜6
        bit 3    MIE
                        %0 : SCC の割り込み発生禁止
                        %1 : 〃                 許可
        bit 2    DLC
                        %0 : 下位チェーン禁止
                        %1 : 〃           許可
        bit 1    NV
                        %0 : 割り込み応答時、ベクタを出力する
                        %1 : 〃                           しない
        bit 0    VIS
                        %0 : ベクタ番号は固定
                        %1 : 割り込み要因によってベクタを変化させる

WR10:
        bit  7   6   5   4   3   2   1   0
          ┌─┬───┬─┬─┬─┬─┬─┐
          │  │      │  │  │  │  │  │
          └─┴───┴─┴─┴─┴─┴─┘
            │  │      │  │  │  │  └ 6bit/8bit Sync
            │  │      │  │  │  └ Loop Mode
            │  │      │  │  └ Abort/Flag on Underrun
            │  │      │  └ Mark/Flag Idle
            │  │      └ Go Active on Poll
            │  └ Data Encording
            └ CRC Preset 1/0

        bit 7    CRC Preset 1/0
                        %0 : CRC ジェネレータ、CRC チェッカの初期値は All'0'
                        %1 : 〃                                           '1'
        bit 6〜5 Data Encording
                        TxD の変調モード
                        %00 : NRZ
                        %01 : NRZI
                        %10 : FM(遷移'1')
                        %11 : FM(遷移'0')
        bit 4    Go Active on Poll
                        SDLC のループモード時
                        %0 : フラグ送信終了後、1 ビット遅延モードになる
                        %1 : フラグ送信後、フラグがデータを送信する
        bit 3    Mark/Flag Idle
                        SDLC モード時
                        %0 : アイドル時、フラグ(フラグアイドル)を送信する
                        %1 : 〃           TxD を '1'(マーク状態)にする
        bit 2    Abort/Flag on Underrun
                        SDLC モード時
                        %0 : 送信アンダーラン発生時、CRC を送る
                        %1 : 〃                       アボートとフラグを送る
        bit 1    Loop Mode
                        SDLC モード、同期モード時
                        %0 : 通常動作
                        %1 : ループモード(TxD と RxD を内部的に結線する)にする
        bit 0    6bit/8bit Sync
                        Monosync モード時:
                        %0 : 同期キャラクタは 8bit
                        %1 : 〃                6bit
                        Bisync モード時:
                        %0 : 受信同期キャラクタは 16bit
                        %1 : 〃                    12bit

WR11:
        bit  7   6   5   4   3   2   1   0
          ┌─┬───┬───┬─┬───┐
          │  │      │      │  │      │
          └─┴───┴───┴─┴───┘
            │  │      │      │  └ TRxC Output
            │  │      │      └ TRxc Out/In
            │  │      └ Transmit Clock
            │  └ Receive Clock
            └ RTxC XTAL/NO XTAL

        bit 7    RTxC XTAL/NO XTAL
                             ____
                        %0 : RTxC はクロック入力端子となる
                             ____    ____
                        %1 : RTxC と SYNC 端子で水晶発振回路を構成する
                        ※X680x0 では %0 に設定する.
        bit 6〜5 Receive Clock
                        受信クロック選択
                              ____
                        %00 : RTxC 端子と同一
                              ____
                        %01 : TRxC 端子と同一
                        %10 : ボーレートジェネレータ出力
                        %11 : DPLL 出力
        bit 4〜3 Transmit Clock
                        送信クロック選択(Receive Clock と同様)
        bit 2    TRxc Out/In ____
                        %0 : TRxC 端子は入力端子
                        %1 : 〃          出力端子
        bit 1〜0 TRxC Output
                        ____
                        TRxC 端子から出力される信号源
                        %00 : 水晶発振回路出力
                        %01 : 送信クロック
                        %10 : ボーレートジェネレータ出力
                        %11 : DPLL 出力

WR12:
        bit  7                           0
          ┌───────────────┐
          │                    TC(下位)  │
          └───────────────┘

        ボーレートジェネレータの出力周波数を指定する.
                      ボーレートジェネレータへの入力クロック周波数
        出力周波数 = ───────────────────────
                                      2×(TC＋2)
        ※X680x0 の場合、出力周波数(bps) = 156250/(TC+2)
                         時定数(TC)         = 156250/bps−2
        ※TC を変更する場合はボーレートジェネレータの動作を停止しておくこと.

WR13:
        bit  7                           0
          ┌───────────────┐
          │                    TC(上位)  │
          └───────────────┘

WR14:
        bit  7       5   4   3   2   1   0
          ┌─────┬─┬─┬─┬─┬─┐
          │          │  │  │  │  │  │
          └─────┴─┴─┴─┴─┴─┘
            │          │  │  │  │  └ BRG Enable
            │          │  │  │  └ BRG Source
            │          │  │  └ DTR/REQ Function
            │          │  └ Auto Echo
            │          └ Local Loopback
            └ DPLL Command

        bit 7〜5 DPLL Command
                        DPLL 動作モード選択
                        %000 : Null Command
                        %001 : Enter Search Mode
                        %010 : Reset Missing Clock
                        %011 : Disable DPLL
                        %100 : Set Source = BRG
                        %101 : Set Source = RTxC
                        %110 : Set FM Mode
                        %111 : Set NRZI Mode
        bit 4    Local Loopback
                        %0 : 通常動作
                        %1 : RxD は外部と切り離され、TxD と内部接続される
        bit 3    Auto Echo
                        %0 : 通常動作
                        %1 : TxD は外部と切り離され、RxD と内部接続される
        bit 2    DTR/REQ Function
                        %0 : DTR 端子は WR5 の DTR ビットで制御される
                        %1 : 送信バッファ空/同期モードで CRC 送出時 DTR = 'L'
        bit 1    BRG Source
                        ボーレートジェネレータのクロック選択源
                        %0 : RTxC 端子と同一(外部供給)
                        %1 : SCC の PCLK(X680x0 では 5MHz)
        bit 0    BRG Enable
                        %0 : ボーレートジェネレータ停止
                        %1 : 〃                     動作

WR15:
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │ 0│  │ 0│
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │  │  │      └ Zero Count IE
            │  │  │  │  └ DCD IE
            │  │  │  └ Sync/Hunt IE
            │  │  └ CTS IE
            │  └ Tx Underrun/EOM IE
            └ Break/Abort IE

        bit 7   Break/Abort IE
                %0 : ブレーク/アボート状態検出で割り込みを発生しない
                %1 : 〃                                        する
        bit 6   Tx Underrun/EOM IE
                %0 : 送信アンダーラン/送信終了で割り込みを発生しない
                %1 : 〃                                        する
        bit 5   CTS IE
                     ___
                %0 : CTS 端子の変化で割り込みを発生しない
                %1 : 〃                             する
        bit 4   Sync/Hunt IE
                非同期モード時:
                     ____
                %0 : SYNC 端子の変化で割り込みを発生しない
                %1 : 〃                              する
                同期モード/SDLC モード時:
                %0 : 同期の確立/喪失の変化で割り込みを発生しない
                %1 : 〃                                    する
        bit 3   DCD IE
                     ___
                %0 : DCD 端子の状態変化で割り込みを発生しない
                %1 : 〃                                 する
        bit 1   Zero Count IE
                %0 : ゼロカウント割り込みは発生しない
                %1 : ボーレートジェネレータのカウント値 =0 で外部ステータス割
                     り込み発生


RR0:
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │  │  │  │  │  └ Rx character Available
            │  │  │  │  │  │  └ Zero Count
            │  │  │  │  │  └ Tx buffer Empty
            │  │  │  │  └ DCD
            │  │  │  └ SYNC/Hunt
            │  │  └ CTS
            │  └ Tx Underrun/EOM
            └ Break/Abort

        bit 7   Break/Abort
                        非同期モード時:
                        %0 : ブレーク状態から回復した/通常動作
                        %1 : 〃           を検出した
                        SDLC モード時:
                        %0 : アボートシーケンスが終了した/通常動作
                        %1 : アボートシーケンス(7 個以上 '1' が連続する)を
                             検出した
        bit 6   Tx Underrun/EOM
                        %0 : 通常動作
                        %1 : 送信アンダーラン(バッファ内のキャラクタを全て送信
                             したが、新しいキャラクタの書き込みが行われなかっ
                             た)、または送信終了
        bit 5   CTS     CTS 端子の状態を示す
                             ___
                        %0 : CTS 端子は 'H' レベル(送信禁止状態)
                        %1 : 〃          'L' 〃     (〃  許可〃  )
        bit 4   SYNC/Hunt
                        非同期モード時:
                             ____
                        %0 : SYNC 端子は 'H' レベル
                        %1 : 〃           'L' 〃
                        同期モード/SDLC モード時:
                        %0 : 同期が確立した
                        %1 : 同期は確立していない(Hunt モード)
        bit 3   DCD     DCD 端子の状態を示す
                             ___
                        %0 : DCD 端子は 'H' レベル(キャリア検出していない)
                        %1 : 〃          'L' 〃     (〃       検出した  )
        bit 2   Tx buffer Empty
                        %0 : 送信バッファにキャラクタがある
                        %1 : 〃           が空になった
        bit 1   Zero Count
                        %0 : 通常動作
                        %1 : ボーレートジェネレータのカウント値が 0 になった
                        ※WR15 の bit 1 = %1 の時のみ有効.
        bit 0   Rx character Available
                        受信バッファにキャラクタ(データ)が入っているかを示す
                        %0 : バッファは空
                        %1 : キャラクタがある

RR1:
        bit  7   6   5   4   3       1   0
          ┌─┬─┬─┬─┬─────┬─┐
          │  │  │  │  │          │  │
          └─┴─┴─┴─┴─────┴─┘
            │  │  │  │  │          └ All Sent
            │  │  │  │  └ Residue Code 0〜2
            │  │  │  └ Parity Error
            │  │  └ Rx Overrun Error
            │  └ CRC/Framing Error
            └ End-of-Frame

        bit 7    End-of-Frame
                        SDLC モード時
                        %0 : 通常動作
                        %1 : 正常な終了フラグを受け取った/
                             CRC エラービットと端数ビットが確定した
        bit 6    CRC/Framing Error
                        非同期モード時:
                        %0 : 正常動作
                        %1 : フレーミングエラー(ストップビットが見つからない)
                             を検出した
                        同期モード/SDLC モード時:
                        %0 : 正常動作
                        %1 : CRC エラーを検出した
        bit 5    Rx Overrun Error
                        %0 : 正常動作
                        %1 : 受信バッファがオーバーフローした
        bit 4    Parity Error
                        %0 : 正常動作
                        %1 : 受信データ中にパリティエラーを検出した
        bit 3〜1 Residue Code 0〜2
                        SDLC モード時、最後に受信したキャラクタの有効ビット数
                        を示す.
                        %111 : 1 ビット
                        %000 : 2 〃
                        %100 : 3 〃
                        %010 : 4 〃
                        %110 : 5 〃
                        %001 : 6 〃
                        %101 : 7 〃
                        %011 : 8 〃
        bit 0    All Sent
                        非同期モード時
                        %0 : キャラクタ送出中
                        %1 : キャラクタの送出が完了した

RR2(チャンネル A):
        bit  7                           0
          ┌───────────────┐
          │               Vector Number  │
          └───────────────┘

        WR2 への設定値がそのまま読み出せる.

RR2(チャンネル B):
        bit  7           4   3       1   0
          ┌───────┬─────┬─┐
          │              │          │  │
          └───────┴─────┴─┘
                ├───────│───┘
            設定したベクタ番号   割り込み要因

        bit 7〜4,0
                設定したベクタ番号
        bit 3〜1
                割り込み要因
                %000 : チャンネル B 送信バッファ空
                %001 : 〃            外部ステータス変化
                %010 : 〃            受信キャラクタ有効
                %011 : 〃            スペシャル Rx コンディション
                %100 : チャンネル B 送信バッファ空
                %101 : 〃            外部ステータス変化
                %110 : 〃            受信キャラクタ有効
                %111 : 〃            スペシャル Rx コンディション
        ※WR9 VIS が %1 の時は bit 3〜1 ではなく bit 4〜6 が変化する.

RR3:
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │ 0│ 0│  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
                    │  │  │  │  │  └ ChannelB EXT/STAT IP
                    │  │  │  │  └── ChannelB Tx IP
                    │  │  │  └──── ChannelB Rx IP
                    │  │  └ ChannelA EXT/STAT IP
                    │  └── ChannelA Tx IP
                    └──── ChannelA Rx IP

        割り込みがペンディング中に %1 になる.

        bit 5   ChannelA Rx IP
                        チャンネル A 受信割り込み
        bit 4   ChannelA Tx IP
                        チャンネル A 送信割り込み
        bit 3   ChannelA EXT/STAT IP
                        チャンネル A 外部ステータス変化割り込み
        bit 2   ChannelB Rx IP
                        チャンネル B 受信割り込み
        bit 1   ChannelB Tx IP
                        チャンネル B 送信割り込み
        bit 0   ChannelB EXT/STAT IP
                        チャンネル B 外部ステータス変化割り込み

RR8:
        bit  7                           0
          ┌───────────────┐
          │                受信バッファ  │
          └───────────────┘

        $e98003(チャンネル B)/$e98007(チャンネル A)と同一.

RR10:
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬───┬─┬─┐
          │  │  │ 0│  │   0  │  │ 0│
          └─┴─┴─┴─┴───┴─┴─┘
            │  │      │          └ On Loop
            │  │      └ Loop Sending
            │  └ Two Clocks Missing
            └── One Clock  Missing

        bit 7   One Clock  Missing
                        %0 : その他の状態
                        %1 : FM モード時、RxD で '1' がある筈の期間中にクロッ
                             クエッジが見つからなかった
        bit 6   Two Clocks Missing
                        %0 : その他の状態
                        %1 : FM モード時、連続二回の試みでクロックエッジが見つ
                             からなかった
        bit 4   Loop Sending
                        SDLC モード時
                        %0 : それ以外の状態
                        %1 : ループモードで、SCC がループ上に送信動作をしてい
                             る
        bit 1   On Loop
                        SDLC モード時:
                        %0 : それ以外の状態
                        %1 : ループモードで、SCC が実際にオンループにある
                        Monosync 時:
                        %0 : それ以外の状態
                        %1 : ループモードで、トランスミッタがアクティブである

RR12:
        (WR12 と同様)
        WR12 設定値がそのまま読み出される.

RR13:
        (WR13 と同様)
        WR13 設定値がそのまま読み出される.

RR15:
        (WR15 と同様)
        WR15 設定値がそのまま読み出される.

==============================================================================

・Joystick interface

address size    R/W
$e9a001 1.b     R       8255 ポート A(ジョイスティック #1)
$e9a003 1.b     R       8255 ポート B(ジョイスティック #2)
$e9a005 1.b     R/W     8255 Port C (Joystick control)
$e9a007 1.b       W     8255 コントロールワード(動作モード/ビット操作)


ジョイスティック #1($e9a001):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
                │  │      │  │  │  └ FORWARD
                │  │      │  │  └ BACK
                │  │      │  └ LEFT
                │  │      └ RIGHT
                │  └ TRG A
                └ TRG B

        bit 6   TRG B
                %0 : トリガ B ボタンは押されている
                %1 : 〃                押されていない
        bit 5   TRG A
                %0 : トリガ A ボタンは押されている
                %1 : 〃                押されていない
        bit 3   RIGHT
                %0 : 右スイッチは押されている
                %1 : 〃           押されていない
        bit 2   LEFT
                %0 : 左スイッチは押されている
                %1 : 〃           押されていない
        bit 1   BACK
                %0 : 下スイッチは押されている
                %1 : 〃           押されていない
        bit 0   FORWARD
                %0 : 上スイッチは押されている
                %1 : 〃           押されていない

ジョイスティック #2($e9a003):
        (ジョイスティック #1 と同様)

ジョイスティックコントロール($e9a005):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬───┬───┐
          │  │  │  │  │      │      │
          └─┴─┴─┴─┴───┴───┘
            │  │  │  │  │      └ PCM PAN
            │  │  │  │  └ Sampling Rate
            │  │  │  └ IOC4
            │  │  └ IOC5
            │  └ IOC6
            └ IOC7

        bit 7    IOC7
                        %0 : 通常動作
                        %1 : ジョイスティック #1 オプション機能 B 動作
        bit 6    IOC6
                        %0 : 通常動作
                        %1 : ジョイスティック #1 オプション機能 A 動作
        bit 5    IOC5
                        %0 : 通常動作
                        %1 : ジョイスティック #2 操作無効
        bit 4    IOC4
                        %0 : 通常動作
                        %1 : ジョイスティック #1 操作無効
        bit 3〜2 Sampling Rate
                        ADPCM sampling rate switching
        bit 1〜0 PCM PAN
                        ADPCM output switching

コントロールワード ビットセット/リセット($e9a007):
        bit  7   6       4   3       1   0
          ┌─┬─────┬─────┬─┐
          │ 0│          │  BITSEL  │  │
          └─┴─────┴─────┴─┘
                                        DATA

        bit 3〜1 BITSEL
                        操作するビット位置(7〜0)を指定する.
        bit 0    DATA
                        セットするデータを指定する.
        ※最上位ビットが %0 の値を書き込むと、ポート C の任意のビットを操作す
          る(コントロールワードのビットではない).

コントロールワード モード設定($e9a007):
        bit  7   6   5   4   3   2   1   0
          ┌─┬───┬─┬─┬─┬─┬─┐
          │ 1│      │  │  │  │  │  │
          └─┴───┴─┴─┴─┴─┴─┘
                │      │  │  │  │  └ PORT C(Low) IN/OUT
                │      │  │  │  └ PORT B IN/OUT
                │      │  │  └ Group B Mode
                │      │  └ PORT C(High) IN/OUT
                │      └ PORT A IN/OUT
                └ Group A Mode

        bit 6〜5 Group A Mode
                        グループ A(ポート A とポート C 上位)の動作モード
                        %00 : モード 0
                        %01 : 〃     1
                        %10 : 〃     2
                        %11 : 〃     2
        bit 4    PORT A IN/OUT
                        %0 : ポート A は出力
                        %1 : 〃          入力
        bit 3    PORT C(High) IN/OUT
                        %0 : ポート C(上位) は出力
                        %1 : 〃                入力
        bit 2    Group B Mode
                        グループ B(ポート B とポート C 下位)の動作モード
                        %0 : モード 0
                        %1 : 〃     1
        bit 1    PORT B IN/OUT
                        %0 : ポート B は出力
                        %1 : 〃          入力
        bit 0    PORT C(Low) IN/OUT
                        %0 : ポート C(下位) は出力
                        %1 : 〃                入力
        ※最上位ビットが %1 の値を書き込むと、動作モード設定を行う.

==============================================================================

・I/O コントローラ

address size    R/W
$e9c001 1.b     R       割り込みステータス
〃       〃         W     割り込みマスク
$e9c003 1.b       W     割り込みベクタ


IOC 割り込みステータス($e9c001):
        bit  7   6   5   4   3   2   1   0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
          │  │  │  │  │  │  │  │  │
          └─┴─┴─┴─┴─┴─┴─┴─┘
            │  │  │  │  │  │  │  └ PRTI EN
            │  │  │  │  │  │  └ FDDI EN
            │  │  │  │  │  └ FDCI EN
            │  │  │  │  └ HDDI EN
            │  │  │  └ HDD INT
            │  │  └ PRT INT
            │  └ FDD INT
            └ FDC INT

        bit 7   FDC INT
                        %0 : FDC 割り込みは発生していない
                        %1 : 〃                 している
        bit 6   FDD INT
                        %0 : FDD 割り込みは発生していない
                        %1 : 〃                 している
        bit 5   PRT INT
                        %0 : プリンタ BUSY 信号 = High(BUSY 状態)
                        %1 : 〃                  = Low (READY 状態)
        bit 4   HDD INT
                        %0 : HDD 割り込みは発生していない
                        %1 : 〃                 している
        bit 3   HDDI EN
                        %0 : HDD 割り込み禁止中
                        %1 : 〃           許可中
        bit 2   FDCI EN
                        %0 : FDC 割り込み禁止中
                        %1 : 〃           許可中
        bit 1   FDDI EN
                        %0 : FDD 割り込み禁止中
                        %1 : 〃           許可中
        bit 0   PRTI EN
                        %0 : プリンタ割り込み禁止中
                        %1 : 〃               許可中

IOC 割り込みマスク($e9c001):
        bit  7           4   3   2   1   0
          ┌───────┬─┬─┬─┬─┐
          │              │  │  │  │  │
          └───────┴─┴─┴─┴─┘
                            │  │  │  └ PRTI EN
                            │  │  └ FDDI EN
                            │  └ FDCI EN
                            └ HDDI EN

        bit 3   HDDI EN
                        %0 : HDD 割り込み禁止
                        %1 : 〃           許可
        bit 2   FDCI EN
                        %0 : FDC 割り込み禁止
                        %1 : 〃           許可
        bit 1   FDDI EN
                        %0 : FDD 割り込み禁止
                        %1 : 〃           許可
        bit 0   PRTI EN
                        %0 : プリンタ割り込み禁止
                        %1 : 〃               許可

IOC 割り込みベクタ($e9c003):
        bit  7                   2   1   0
          ┌───────────┬───┐
          │                Vect  │DEVICE│
          └───────────┴───┘

        bit 7〜2 Vect    ベクタ番号
                        割り込み発生時に使用するベクタ番号を設定する.
                        下位 2 ビットは割り込み要因によって自動的に決定される.
        bit 1〜0 DEVICE  割り込み要因
                        %00 : FDC 割り込み
                        %01 : FDD 〃
                        %10 : HDD 〃
                        %11 : プリンタ割り込み

==============================================================================

・コプロセッサ I/O

address size    R/W
$e9e000 1.w     R       応答 CIR
$e9e002 1.w       W     コントロール CIR
$e9e004 1.w     R       セーブ CIR
$e9e006 1.w     R/W     リストア CIR
$e9e008 1.w     ---     (オペレーションワード CIR)
$e9e00a 1.w       W     コマンド CIR
$e9e00c 1.w     ---
$e9e00e 1.w       W     コンディション CIR
$e9e010 1.l     R/W     オペランド CIR
$e9e014 1.w     R       レジスタ選択 CIR
$e9e016 1.w     ---
$e9e018 1.l       W     命令アドレス CIR
$e9e01c 1.l     ---     (オペランドアドレス CIR)

※第二アドレスの場合は $e9e080〜$e9e09f.


応答 CIR($e9e000):
        bit 15  14  13  12               8   7                           0
          ┌─┬─┬─┬─────────┬───────────────┐
          │CA│PC│DR│            機能  │                  パラメータ  │
          └─┴─┴─┴─────────┴───────────────┘

        bit 15  CA      カムアゲイン
                        %1 : CPU による何らかのサービス要求あり
        bit 14  PC      プログラムカウンタ受け取り要求
                        %1 : 要求あり
        bit 13  DR      データ(オペランド)の転送方向
                        %0 : ホスト CPU → 68881
                        %1 : 68881 → ホスト CPU
                        ※機能ビットが %00100(ヌルプリミティブ)、%11100(命令前
                          例外取得プリミティブ)、%11101(命令中例外取得プリミテ
                          ィブ)の時は %0.
        bit 12〜8        機能
                        %00100 : ヌルプリミティブ
                        %10xxx : 実行アドレス評価/データ転送プリミティブ
                        %01100 : 単一メインプロセッサレジスタ転送プリミティブ
                        %00001 : 複数コプロセッサレジスタ転送プリミティブ
                        %11100 : 命令前例外取得プリミティブ
                        %11101 : 命令中例外取得プリミティブ
        bit  7〜0        パラメータ

コントロール CIR($e9e002):
          このレジスタに書き込みが行われると、68881 は実行中の処理を直ちに中止
        し、ペンディングされている例外を全てクリアした後、アイドル状態に復帰す
        る.

セーブ CIR($e9e004):
          このレジスタを読み出すと、68881 は現在の処理動作を中断し、動作状態ス
        テータスを返す.

リストア CIR($e9e006):
          このレジスタに書き込みが行われると、68881 は動作を中断しリストア動作
        を開始する.

オペレーションワード CIR($e9e008):
        68881 では使用されない.

コマンド CIR($e9e00a):
          68881 に対して命令を書き込むレジスタ.

コンディション CIR($e9e00e):
          条件チェックを行う為に使用する.

オペランド CIR($e9e010):
          データ転送に使用する.

レジスタ選択 CIR($e9e014):
          レジスタ転送命令実行時に、68881 から CPU にレジスタマスクを渡す為に
        使用される.

命令アドレス CIR($e9e018):
          PC を渡す為に使用する.

オペランドアドレス CIR($e9e01c):
          68881 では使用されない.

==============================================================================

・スプライト/BG

  スプライト/BG のメモリは画面モードが 512×512 若しくは 256×256 の時以外は切
り離されてアクセスできない. また、スプライトは同一水平線上には最大 32 個までし
か表示できない.


address size
$eb0000 512.w   スプライトスクロールレジスタ

$eb0800 1.w     BG0 スクロールレジスタ X
$eb0802 1.w     〃                      Y
$eb0804 1.w     BG1 スクロールレジスタ X
$eb0806 1.w     〃                      Y
$eb0808 1.w     BG コントロール
$eb080a 1.w     水平トータル ┐
$eb080c 1.w     水平表示位置 │画面モードレジスタ
$eb080e 1.w     垂直表示位置 │
$eb0810 1.w     解像度設定   ┘

$eb8000 8192.b  PCG エリア(16×16ドット/8×8ドット共用)
$eba000 8192.b  〃         (16×16ドット専用)
$ebc000 8192.b  BG データエリア(BG0) 若しくは PCG(16×16ドット専用)
$ebe000 8192.b  〃              (BG1) 〃


スプライトスクロールレジスタ($eb0000):
        スプライト #0〜#127 の順に、4 ワードずつのレジスタの組が合計 512 ワー
        ド並ぶ.

+0:
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                                XPOS  │
          └───────────┴───────────────────┘

        bit 9〜0 XPOS    X 座標
                        ※スプライト画面は他の画面より 16 ドットだけ左上方向に
                          ずれているので、(16,16) が見かけ上の原点となる.

+2:
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                                YPOS  │
          └───────────┴───────────────────┘

        bit 9〜0 YPOS    Y 座標
                        ※スプライト画面は他の画面より 16 ドットだけ左上方向に
                          ずれているので、(16,16) が見かけ上の原点となる.

+4:
        bit 15  14  13  12  11           8   7                           0
          ┌─┬─┬───┬───────┬───────────────┐
          │VR│HR│      │       COLOR  │                       SPAT#  │
          └─┴─┴───┴───────┴───────────────┘

        bit 15          VR      垂直方向反転
        bit 14          HR      水平方向反転
        bit 11〜8        COLOR   色コードの上位 4 ビット
                        ※下位 4 ビットは PCG エリアで指定する.
        bit  7〜0        SPAT#   スプライトパターン番号

+6:
        bit 15                                                   2   1   0
          ┌───────────────────────────┬───┐
          │                                                      │ PRW  │
          └───────────────────────────┴───┘

        bit 1〜0 PRW     プライオリティ
                        %00 : 表示オフ
                        %01 : SP < BG1 < BG0
                        %10 : BG1 < SP < BG0
                        %11 : BG1 < BG0 < SP

BG スクロールレジスタ X($eb0800,$eb0804):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                                XPOS  │
          └───────────┴───────────────────┘

BG スクロールレジスタ Y($eb0802,$eb0806):
        bit 15                  10   9                                   0
          ┌───────────┬───────────────────┐
          │                      │                                YPOS  │
          └───────────┴───────────────────┘

BG コントロール($eb0808):
        bit 15                  10   9   8       6   5   4   3   2   1   0
          ┌───────────┬─┬─────┬───┬─┬───┬─┐
          │                      │  │          │ TXSEL│ON│ TXSEL│ON│
          └───────────┴─┴─────┴───┴─┴───┴─┘
                                    D/C             └─ BG1 ┘ └─ BG0 ┘

        bit 9    DISP/CPU
                        %0 : スプライト/BG 表示オフ
                        %1 : 〃                 オン
                        ※表示をオフにすると PCG やレジスタへのアクセスが速く
                          なります.
        bit 5〜4 BG1 TXSEL
                        %00 : BG データエリア 0 を使用
                        %01 : 〃       1 〃
                        %10 : (未定義)
                        %11 : (  〃  )
        bit 3    BG1 ON
                        %0 : BG1 表示オフ
                        %1 : BG1 表示オン
        bit 2〜1 BG0 TXSEL
                        %00 : BG データエリア 0 を使用
                        %01 : 〃       1 〃
                        %10 : (未定義)
                        %11 : (  〃  )
        bit 0    BG0 ON
                        %0 : BG0 表示オフ
                        %1 : BG0 表示オン
        ※BG0 と BG1 で同一の BG データエリアを使用することも可能.

水平トータル($eb080a):
        bit 15                           8   7                           0
          ┌───────────────┬───────────────┐
          │                              │                     H-TOTAL  │
          └───────────────┴───────────────┘

        bit 7〜0 H-TOTAL
                低解像度 256×256 ドットモードの時は CRTC R00 と同じ値を、それ
                以外の時は $ff を設定する.
        ※最下位ビットは必ず 1 にすること.

水平表示位置($eb080c):
        bit 15                                   6   5                   0
          ┌───────────────────┬───────────┐
          │                                      │              H-DISP  │
          └───────────────────┴───────────┘

        bit 7〜0 H-DISP
                CRTC R02 の設定値＋4 を設定する.

垂直表示位置($eb080e):
        bit 15                           8   7                           0
          ┌───────────────┬───────────────┐
          │                              │                      V-DISP  │
          └───────────────┴───────────────┘

        bit 7〜0 V-DISP
                CRTC R06 と同じ値を設定する.

解像度設定($eb0810):
        bit 15                                       5   4   3   2   1   0
          ┌─────────────────────┬─┬───┬───┐
          │                                          │  │ V-RES│ H-RES│
          └─────────────────────┴─┴───┴───┘
                                                        └ L/H freq

        bit 4    L/H freq
                        %0 : 水平同期周波数 15.98kHz
                        %1 :    〃           31.5kHz
        bit 3〜2 V-RES
                        %00 : 垂直 256 ラインモード
                        %01 : 〃   512 〃
                        %10 : 未定義
                        %11 : 未定義
        bit 1〜0 H-RES
                        %00 : 水平 256 ドットモード
                        %01 : 〃   512 〃
                        %10 : 未定義
                        %11 : 未定義

PCG エリア($eb8000):
        8×8ドットのパターンを指定するメモリが 8 ロングワードずつ並ぶ. 16×16
        ドットのパターンは、8×8ドットのパターン(n×4)〜(n×4+3)を左上、左下、
        右上、右下の順に四パターン一組で組み合せたものが使用される. BG のパター
        ンサイズは画面モードによって違うが、スプライトパターンとして使用される
        場合は 16×16 ドットに固定されている.

        1 ドットの色は 4 ビット垂直型で指定し、8 ドット(X 方向の 1 ライン)で 1
        ロングワードとなる.

        PCG エリアへの書き込みは必ずワードサイズでなければならず、バイトサイズ
        で書き込んだ場合はデータが破壊されてしまう. IOCS _DMAMOVE、_DMAMOV_A、
        _DMAMOV_L では必ずバイトサイズで転送が行われるので注意すること.

        OFS = スプライト N の (X,Y) 点をアクセスする時のオフセット
        BIT = 〃                                 ビット位置

        OFS = $eb8000＋Y×4 (ロングワード単位)
        BIT = (28−X×4) 〜 (31−X×4) の 4 ビット

BG データエリア($ebc000):
        BG は画面モードに関係なく 64×64 パターン分の領域を持ち、パターンのサ
        イズを切り替えることで実画面と同じサイズにしている.

        BG の (X,Y) 点のレジスタのオフセットは (Y×128＋X) で求められる.

        bit 15  14  13  12  11           8   7                           0
          ┌─┬─┬───┬───────┬───────────────┐
          │VR│HR│      │       COLOR  │                        PCG#  │
          └─┴─┴───┴───────┴───────────────┘

        bit 15          VR      垂直方向反転
        bit 14          HR      水平方向反転
        bit 11〜8        COLOR   色コードの上位 4 ビット
                        ※下位 4 ビットは PCG エリアで指定する.
        bit  7〜0        PCG#    PCG パターン番号

==============================================================================
```