rpi_t-kernel
=============
T-Kernel for Raspberry Pi.

### 2019/9/29追記
下記GitHubのプロジェクトに、ここで使っているRTOS(T-Kernel)部分のみを分離し独立させた「[yt-kernel](https://github.com/jr4qpv/yt-kernel)」を公開しました。今後の更新は、その「yt-kernel」で行っていきますので、そちらをご利用ください。

<https://github.com/jr4qpv/yt-kernel.git>

* T-Kernel部分のみを分離することで、保守性を向上し見通しよくするのが目的。
* ここで動かしている簡易コマンドシェルも、サンプルアプリとして別プロジェクトで公開していく予定。

概要
----
Tronフォーラム（<http://www.tron.org/ja/>）で公開のオープンソース T-Kernel2.0 & Extensyon(T2EX)を、[Raspberry Pi](https://ja.wikipedia.org/wiki/Raspberry_Pi) にポーティングしたリアルタイムＯＳ。

コンパイル手順、起動方法などは、[`doc/rpi/porting_ja.md`](https://github.com/jr4qpv/rpi_t-kernel/blob/master/doc/rpi/porting_ja.md)を参照ください。

#### 【Raspberry Pi 対応機種】

|モデル        | TYPE_RPI |備考                    |
|:-------------|:--------:|:-----------------------|
|Pi Zero       |1         |                        |
|Pi 1 Model B+ |1         |動作未確認              |
|Pi 2 Model B  |2         |シングルコア動作        |
|Pi 3 Model B  |3         |32bit,シングルコア動作  |

ビルド環境
----------
作者が確認した環境。GCCのバージョンは多少異なっても大丈夫と思う。又、インストール手順は参考リンク[1.][2.][3.]を参照。
#### 【Windows】
* [msys2](https://msys2.github.io/)
* [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded)  5-2016-q2-update

#### 【macOS】
* macOS Sierra v10.12.4
* [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded)  5-2016-q2-update

#### 【Linux】
* [Ubuntu 16.04 LTS Server](https://www.ubuntulinux.jp/home)
* [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded)  5-2016-q2-update

シリアルコンソール
------------------
Raspberry Piとパソコンとは、シリアル通信で接続する。尚、Raspberry Pi側は3.3V信号なので、シリアル-USB変換ケーブルなどが必要。詳しくは参考リンク[4.]を参照。

ブート
-----
T-Kernel/T-Monitorプログラムの起動は、[u-boot](https://ja.wikipedia.org/wiki/Das_U-Boot)を利用します。（T-Monitor単体でも起動はできるのですが、2016/9/18現在 SDカードアクセスが未サポートの為）

尚、Raspberry Pi用のu-bootバイナリを[GitHub](https://github.com/jr4qpv/rpi_u-boot_jtag_bins)に公開していますので、手軽に試したい方はそちらも利用できます。詳しくは、参考リンク[4.]を参照。

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
5. [【Ubuntu 16.04 LTS Server】Raspberry Pi用のu-bootをコンパイルする](https://www.yokoweb.net/2016/08/11/ubuntu-uboot-gcc/)
6. [【Raspberry Pi】参考になるベアメタルなドキュメント](https://www.yokoweb.net/2016/09/04/raspberrypi-document/)

作者関連サイト
-------------
* [GitHub (jr4qpv)](https://github.com/jr4qpv/)
* [The modern stone age.](https://www.yokoweb.net/)
* [JR4QPV Yoko's Library](http://jr4qpv.my.coocan.jp/)

補足
----
まずは、T-Kernelの基本的な機能を動くようにしました。まだ未対応の部分、便利にしていきたい所は沢山あるのですが、徐々に整備できたらと思っています。なのでレビジョンも0.10からのスタートとしました。

LinuxやWindosでハードウェアを直接制御するプログラムを書くのはドライバ経由でアクセスしないといけなかったりで結構大変です。特権モードで動かせるT-Kernelタスクを使って、小型で安価な Raspberry Piの拡張コネクタに自作機器などを接続したりして、リアルタイムＯＳプログラミンを手軽に楽しめるのではないかと思います。

実用事例として、作者がGameBoyやMS-DOS用に公開しているモールス練習ソフト(CWTA/CWT)などを移植して公開できたらとも考えています。

本ソフトウェアに関しては、作者のブログ「[The modern stone age.](https://www.yokoweb.net/)」でも情報公開していきますので参考にください。

来歴
----
* 2016/09/18 r0.10公開
* 2017/04/11 r0.11：フォルダ構成見直し、他
* 2019/09/29 yt-kernel 公開案内の追記を記載。
