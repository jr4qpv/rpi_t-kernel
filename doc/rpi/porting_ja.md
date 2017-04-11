T-Kernel for Raspberry Pi 移植メモ
======================================
本ファイルは、Markdown記法で書いています。

概要
----
Tronフォーラム（<http://www.tron.org/ja/>）で下記公開のオープンソース T-Kernel2.0 & Extensyon(T2EX)から、[Raspberry Pi](https://ja.wikipedia.org/wiki/Raspberry_Pi) にポーティングしたリアルタイムＯＳ。

1. [T-Kernel 2.02.00 Software Package](http://www.tron.org/download/index.php?route=product/product&product_id=133)
    - `2015/06/08  18:35  396,299 tkernel_source.tar.gz`
2. [T-Kernel 2.0 Extension（T2EX 2.01.00）](http://www.tron.org/download/index.php?route=product/product&product_id=134)
    - `2015/06/09  13:16  419,593 t2ex_source.tar.gz`
    - `2015/03/25  17:12  1,219,629 bsd_source.tar.gz`

#### 【Raspberry Pi 対応機種】
TYPE_RPIは、コンパイル時にRasperry Piの機種を区別しているmake変数。

|モデル        | TYPE_RPI |備考                    |
|:-------------|:--------:|:-----------------------|
|Pi Zero       | 1        |                        |
|Pi 1 Model B+ | 1        |動作未確認              |
|Pi 2 Model B  | 2        |シングルコア動作        |
|Pi 3 Model B  | 3        |32bit,シングルコア動作  |

ビルド環境
----------
作者が確認した環境。GCCのバージョンは多少異なっても大丈夫と思う。インストール手順は参考リンク[1.][2.][3.]を参照。
#### 【Windows】
* [msys2](https://msys2.github.io/)
* [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded)  5-2016-q2-update

#### 【macOS】
* macOS Sierra v10.12.4
* [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded)  5-2016-q2-update

#### 【Linux】
* [Ubuntu 16.04 LTS Server](https://www.ubuntulinux.jp/home)
* [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded)  5-2016-q2-update

コンパイル手順
--------------
以下、Windowsのmsys2環境での例に説明する。MacやLinuxでも同様なのでそれぞれの環境に読み換えてください。T-Monitor,config,T-Kernel３つのファイルを作成する。

コンパイル実行するには、下記環境変数が設定されている事が必要。

|環境変数 |説明                       |備考          |
|:--------|:--------------------------|:-------------|
|BD       |T-Kernelソースのベースパス |              |
|GNU_BD   |ARMコンパイラのベースパス  |              |

#### 環境変数の設定
`$HOME/.bashrc`に下記設定を追記しておくと便利。本プログラムは `C:¥work¥` にgitで取得したものとする。

```
export BD=/C/work/rpi_t-kernel/tkernel_source
export GNU_BD='/C/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q2'
```

#### ■T-Monitorのコンパイル
下記フォルダにて `make` を実行する事でコンパイルする。

|ビルドフォルダ                            |機種                |備考        |
|:-----------------------------------------|:-------------------|:-----------|
|$BD/monitor/tmmain/build/rpi_bcm283x      |Raspberry Pi Zero/1 |            |
|$BD/monitor/tmmain/build/rpi_bcm283x.rpi2 |Raspberry Pi 2      |            |
|$BD/monitor/tmmain/build/rpi_bcm283x.rpi3 |Raspberry Pi 3      |            |

* `.debug`ついたフォルダでコンパイルするとデバック情報を付加する
* `make clean`で生成されたファイルを削除

生成されるT-Monitorオブジェクトファイル

|ファイル名    |説明                       |備考          |
|:-------------|:--------------------------|:-------------|
|tmonitor      |elfファイル                |              |
|tmonitor.bin  |バイナリファイル           |←これを利用  |
|tmonitor.mot  |Sフォーマットファイル      |              |

#### ■config情報のコンパイル
下記フォルダにて `make` を実行する事でconfigファイルを作成する。

|ビルドフォルダ                            |機種                |備考       |
|:-----------------------------------------|:-------------------|:----------|
|$BD/config/build_t2ex/rpi_bcm283x         |Raspberry Pi Zero/1 |           |
|$BD/config/build_t2ex/rpi_bcm283x.rpi2    |Raspberry Pi 2      |           |
|$BD/config/build_t2ex/rpi_bcm283x.rpi3    |Raspberry Pi 3      |           |

* `make clean`で生成されたファイルを削除

生成されるconfigオブジェクトファイル

|ファイル名           |説明                       |備考          |
|:--------------------|:--------------------------|:-------------|
|rominfo-rom          |elfファイル                |              |
|rominfo_t2ex-rom.bin |バイナリファイル           |←これを利用  |
|rominfo_t2ex-rom.mot |Sフォーマットファイル      |              |

#### ■T-Kernelのコンパイル
下記フォルダにて `make` を実行する事でコンパイルする。

|ビルドフォルダ                                 |機種                |備考     |
|:----------------------------------------------|:-------------------|:--------|
|$BD/kernel/sysmain/build_t2ex/rpi_bcm283x      |Raspberry Pi Zero/1 |         |
|$BD/kernel/sysmain/build_t2ex/rpi_bcm283x.rpi2 |Raspberry Pi 2      |         |
|$BD/kernel/sysmain/build_t2ex/rpi_bcm283x.rpi3 |Raspberry Pi 3      |         |

* `.debug`ついたフォルダでコンパイルするとデバック情報を付加する
* `make clean`で生成されたファイルを削除

生成されるT-Kernelオブジェクトファイル

|ファイル名            |説明                       |備考          |
|:---------------------|:--------------------------|:-------------|
|kernel_t2ex-rom.rom   |elfファイル                |              |
|kernel_t2ex-rom.bin   |バイナリファイル           |←これを利用  |
|kernel_t2ex-rom.mot   |Sフォーマットファイル      |              |

#### コンパイルの実行例
```
$ cd $BD/monitor/tmmain/build/rpi_bcm283x.rpi2
$ make
$ cd $BD/config/build_t2ex/rpi_bcm283x.rpi2
$ make
$ $BD/kernel/sysmain/build_t2ex/rpi_bcm283x.rpi2
$ make
```

シリアルコンソール
------------------
Raspberry Piとパソコンとは、シリアル通信で接続する。Raspberry Piのシリアル通信関係のコネクタピンは下記。尚、Raspberry Pi側は3.3V信号なので、シリアル-USB変換ケーブルなどが必要。詳しくは参考リンク[4.]を参照。

|Raspberry Pi ピン名称|ピン番号|シリアル信号|
|:--------------------|:------:|:-----------|
|GND                  |6       |GND         |
|GPIO 14 (TXD)        |8       |RX          |
|GPIO 15 (RXD)        |10      |TX          |

※通信仕様は下記

ボーレート :115200、データ長 :8bit、パリティー :なし、ストップビット :1、フロー制御 :なし

起動SDカード準備
----------------
T-Kernel/T-Monitorプログラムの起動は、[u-boot](https://ja.wikipedia.org/wiki/Das_U-Boot)を利用。（T-Monitor単体でも起動はできるのですが、2016/9/18現在 SDカードアクセスが未サポートの為）

ファイル容量は4GByteもあれば十分なので、FATフォーマットされたマイクロSDカードを準備し、下記の合計７のファイルを書き込む。

#### u-boot関連ファイルの書き込み
GitHub（<https://github.com/jr4qpv/rpi_u-boot_jtag_bins>）から、各Raspberry Piの機種用の下記4つのファイルを入手し、SDカードのルートに書き込む。詳しくは、参考リンク[4.]を参照。

1. `bootcode.bin`
2. `start.elf`
3. `u-boot.bin`
4. `config.txt`

#### T-Kernel関連ファイルの書き込み
T-Kernel関連のファイルを、前記コンパイル手順で作成した下記3つのファイルを、SDカードに書き込む。

1. `tmonitor.bin`
2. `rominfo_t2ex-rom.bin`
3. `kernel_t2ex-rom.bin`

T-Kernelの起動
--------------
前記で作成した起動用SDカードを Raspberry Piにセットして電源起動すると、シリアル通信でu-bootの起動メッセージが表示される。 カウントダウンするので、その間にキー入力するとu-bootコマンドが入力できる。

続いて、下記コマンドを実行してT-Kernelを起動。

```
u-boot> fatload mmc 0 8000 tmonitor.bin
u-boot> fatload mmc 0 4000 rominfo_t2ex-rom.bin
u-boot> fatload mmc 0 30000 kernel_t2ex-rom.bin
u-boot> go 8000
```

起動メッセージと `T2EX>>` のプロンプトが表示されたら、無事T-Kernelが起動し、簡易コマンドインタプリタからコマンド入力できる。（下記、コマンド実行例）

```
T2EX>> ref tsk
TSK STATE (MAX:150)
TID PRI:BPR SLT WUP SUS STS(*:NODISWAI)  ST+UT(x10) RID EXINF
  1 138:138   0   0   0 RUN                 0+0       1
  2 140:140   0   0   0 WAI-DLY             0+0       1
T2EX>>
```

u-bootの調整
------------
毎回手動でコマンド入力するのは大変なので、下記コマンドでT-Kernelを自動起動するようにする。詳しくはu-bootのhelpやドキュメントを参照ください。

u-bootのコマンド入力待ちから、下記コマンドを入力

```
u-boot> setenv tk_bootcmd 'fatload mmc 0 8000 tmonitor.bin; fatload mmc 0 4000 rominfo_t2ex-rom.bin; fatload mmc 0 30000 kernel_t2ex-rom.bin; go 8000'
u-boot> setenv bootcmd 'run tk_bootcmd'
u-boot> setenv bootdelay 2
u-boot> saveenv
```

デバック中など自動でT-Kernelまで起動せずに、u-bootのコマンド待ちにしたい場合は、下記コマンドを実行。

```
u-boot> setenv bootdelay -1
u-boot> saveenv
```

電源投入でu-bootのコマンド待ちとなり、`boot`コマンド入力で、bootcmd変数登録のコマンドが実行されるのでT-Kernelが起動。

メモリマップ
------------
実装仕様書`doc/T-Kernel2.0/doc/impl-tef_em1d.txt`からの差分

```
(2) ROM 詳細（Raspberry PiではD-RAMでu-bootでロード）

         0x00004000 +-----------------------+
                    |ROM 情報(config)       |
         0x00004080 +-----------------------+
                    |(予約)                 |
         0x00008000 +-----------------------+
                    |例外分岐処理           |
                    |T-Monitor              |
         0x00030000 +-----------------------+
                    |T-Kernel               |
                    +-----------------------+
                    |(ユーザーエリア)       |
                    +-----------------------+
                    |(u-bootは起動時に      |
                    | このあたりに移動)     |
         0x08000000 +-----------------------+
```

```
(3) RAM 詳細

         0x10000000 +-----------------------+
                    |第１レベル             |
                    |ページテーブル         |
         0x10004000 +-----------------------+
                    |例外ベクターテーブル   |
         0x10004400 +-----------------------+
                    |システム共有情報       |
         0x10004440 +-----------------------+
                    |T-Monitor              |
                    |データ／スタック       |
         0x10006000 +-----------------------+ ← RAM_TOP
                    |OS 用                  |
         0x20000000 +-----------------------+ ← RAM_END
```

例外ベクターテーブル
--------------------
```
2.4 例外ベクターテーブル
                                               ベクター番号
         0x10004000 +-----------------------+
                    |デフォルトハンドラ     |       0
                    |未定義命令             |       1
                    |プリフェッチアボート   |       2
                    |データアボート         |       3
         0x10004010 +-----------------------+
                    |スーパーバイザ SVC 4   |       4
                    |コール          :      |       |
                    |               SVC 28  |       28
         0x10004074 +-----------------------+
                    |デバッグアボート   命令|       29
                    |                 データ|       30
         0x1000407c +-----------------------+
                    |高速割込       FIQ     |       31
         0x10004080 +-----------------------+
                    |割込           IRQ 0   |       32
                    |                :      |       |
                    |               IRQ 95  |       127
         0x10004200 +-----------------------+
                    |予備           IRQ 96  |       128
                    |                :      |       |
                    |               IRQ  127|       255
         0x10004400 +-----------------------+

    ・デフォルトハンドラは、ハンドラが登録されていない例外・割込が発生した場
      合に呼び出されるハンドラ。NULL を未登録とする。
    ・ベクター番号は、tk_def_int() の割込定義番号(dintno)として使用する。
    ・ベクター番号は、SVC 命令の番号(イミディエート値)として使用する。
    ・IRQ 0〜95 は、割込コントローラ(INT)の INT 0〜95 に対応する
```

シリアルポート割り当て
----------------------
T-Kernel/T-Monitorではシリアルポートは、Raspberry Piでは下記を利用

|モデル        | TYPE_RPI |UART#|備考             |
|:-------------|:--------:|:---:|:----------------|
|Pi Zero       | 1        | 0   |PL011            |
|Pi 1 Model B+ | 1        | 0   |PL011            |
|Pi 2 Model B  | 2        | 0   |PL011            |
|Pi 3 Model B  | 3        | 1   |mini UART        |

タイマー
--------
システムタイマーとしてTimer(ARM side)を使用。割り込み周期はconfigで変更できるが、デフォルトでは1msecに設定。

LED割り当て
-----------
T-MonitorのcpuLED()関数で、LED操作ができる。LED表示の為に下記のGIIOポートを割り当てている。

#### Raspberry Pi Zero/1/2

|bit番号|GPIO番号|備考                             |
|:-----:|:------:|:--------------------------------|
|  0    | 47     |ACT（緑)                         |
|  1    | 35     |PWR（赤）,Raspberry Pi Zeroは無し|

#### Raspberry Pi 3
Raspberry Pi 3ではACT/PWRはARM CPUから操作できない。

|bit番号|GPIO番号|備考                             |
|:-----:|:------:|:--------------------------------|
|  0    | 17     |コネクタの11ピン                 |

JTAGピン
--------
JTAGデバッグ可能なように、T-Monitorの初期化時に、Raspberry PiのJTAGピンを有効にしている。Raspberry PiコネクタのJTAGピンの割り当ては下記。

|ピン番号|GPIO番号|GPIOモード|機能（JTAG）|
|:-------|:-------|:---------|:-----------|
|15      |GPIO22  |ALT4      |TRST        |
|7       |GPIO4   |ALT5      |TDI         |
|13      |GPIO27  |ALT5      |TMS         |
|22      |GPIO25  |ALT5      |TCK         |
|18      |GPIO24  |ALT5      |TDO         |

* Raspberry Pi Zero/2ではJTAGデバッガ接続できるは確認したが、Raspberry Pi 3では未確認。

ユーザアプリケーション
----------------------
T-Kernelが起動すると、usermain()タスクから、`tkernel_source/kernel/sysmain/src/appl_main.c`のappl_main()関数が呼び出される。ここに、ユーザ定義のアプリケーションプログラムを記述していく。
作者のサンプルでは、`sample_task.c`のsample_tsk()をタスクとして登録し、LEDを点滅させている。

仕様書
------
T-Kernel関連の仕様書は[Tronフォーラム](http://www.tron.org/ja/)を参照ください。

1. [T-Kernel 2.0 仕様書](http://www.tron.org/ja/wp-content/themes/dp-magjam/pdf/specifications/TEF020-S001-02.01.00_ja.pdf)
2. [T-Kenrel 2.0 Extension 仕様書 (T2EX)](http://www.tron.org/ja/wp-content/themes/dp-magjam/pdf/specifications/ja/TEF020-S009-02.00.00_ja.pdf)
3. [T-Monitor仕様書](http://www.tron.org/ja/wp-content/themes/dp-magjam/pdf/specifications/ja/TEF020-S002-01.00.01_ja.pdf)

ライセンス
----------
T-License2.1に従う。添付ドキュメント`TEF000-218-150401.pdf`を参照ください。

ディストリビューションucode
---------------------------
T-Kernel再配布規約に従い、本ソフトウェアのディストリビューション番号は「`00070055`」

免責
----
本プログラムの使用にあたっては、使用者自身の責任で行ってください。作者は何の保証もしないし、本プログラムを利用した上で生じたいかなる障害や損害についても、作者は責任を負いません。

参考リンク
---------
1. [【msys2】ARMのコンパイル環境を構築する](https://www.yokoweb.net/2016/08/31/msys2-arm-gcc/)
2. [【macOS】ARMのコンパイル環境を構築する](https://www.yokoweb.net/2016/09/03/macos-arm-gcc/)
3. [【Ubuntu 16.04 LTS Server】ARMのコンパイル環境を構築する](https://www.yokoweb.net/2016/08/11/ubuntu-arm-gcc/)
4. [【Raspberry Pi】u-bootを動かす](https://www.yokoweb.net/2016/08/13/raspberrypi-uboot/)
5. [【Ubuntu 16.04 LTS Server】u-bootをコンパイルする](https://www.yokoweb.net/2016/08/11/ubuntu-uboot-gcc/)
6. [【Raspberry Pi】参考になるベアメタルなドキュメント](https://www.yokoweb.net/2016/09/04/raspberrypi-document/)
7. [【Raspberry Pi】u-bootでJTAGピンを有効にする](https://www.yokoweb.net/2016/08/23/raspberrypi-jtag/)

作者関連サイト
-------------
* [GitHub (jr4qpv)](https://github.com/jr4qpv/)
* [The modern stone age.](https://www.yokoweb.net/)
* [JR4QPV Yoko's Library](http://jr4qpv.my.coocan.jp/)

補足
----
本ソフトウェアに関しては、作者のブログ「[The modern stone age.](https://www.yokoweb.net/)」でも情報公開していきますので参考にください。

来歴
----
* 2016/09/18 r0.10公開
* 2017/04/11 r0.11：作者サイトのURL変更，本ファイル拡張子.mdに変更
