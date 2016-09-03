/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012-2015 by Ken Sakamura.
 *    This software is distributed under the latest version of T-License 2.x.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *    Modified by T-Engine Forum at 2015/02/27.
 *    Modified by TRON Forum(http://www.tron.org/) at 2015/06/04.
 *
 *----------------------------------------------------------------------
 */
/*
 * 本ソフトウェアパッケージは、添付のT-License 2.xの利用条件に従って利用、
 * 改変、再配布ができます。ソースコードの再配布を行う場合は、T-License 2.x
 * 契約文書を添付する必要があります。
 * 改変を行った場合でも改変内容を公開する義務や、トロンフォーラムに開示す
 * る義務はありません。
 * 改変したソースコードを配布することもできます。この場合は、T-Kernelトレー
 * サビリティサービスに登録をお願いします。これにより改変の変遷を知ること
 * ができ、ある修正がどの版に継承しているか、いないかを確認することができ
 * ます。
 *
 *   http://trace.tron.org/tk/?lang=ja
 *   http://trace.tron.org/tk/?lang=en
 *
 * T-License 2.xの規定の通り、トロンフォーラムあるいは坂村健が著作権を持
 * つソフトウェアに対して第三者の著作権を侵害していないことを保証しますが、
 * それ以外の保証は行いません。
 * また、本ソフトウェアパッケージを利用して直接的あるいは間接的に損害を生
 * じたとしてもトロンフォーラムおよび坂村健は一切免責されるものとします。
 *
 * 本ソフトウェアパッケージのうち、bsd_source.tar.gz に含まれるソースコー
 * ドは NetBSD もしくは、OpenBSD 由来のソースコードであり、T-License 2.x 
 * は適用されません。それぞれのソースヘッダの記述にしたがって改変、再配布
 * を行う必要があります。
 */
/*
 * 謝辞
 *
 * トロンフォーラムは、T2EXパッケージを完成させることに対し貢献をいただ
 * いたトロンフォーラムT-Kernelワーキンググループのメンバーに謝意を表し
 * ます。
 * ワーキンググループメンバーは、T2EXソースコードに対して有意義なコメント
 * や議論をいただくとともに、詳細なテストに多大な協力をいただきました。
 *
 * T-Kernelワーキンググループメンバー
 *   イーソル株式会社
 *   パーソナルメディア株式会社
 *   株式会社日立超LSIシステムズ
 *   株式会社富士通コンピュータテクノロジーズ
 *   富士通セミコンダクター株式会社
 *   ルネサス エレクトロニクス株式会社
 *   YRPユビキタス・ネットワーキング研究所
 */

==============================================================================
   T-Engineリファレンスボード用 T-Kernel 2.0 Extension 開発環境
==============================================================================

------------------------------------------------------------------------------
 目次
------------------------------------------------------------------------------
  1. はじめに
  2. パッケージ内容
  3. 作業の流れ
    3.1 開発環境のインストール
    3.2 ソースコードパッケージの展開
    3.3 システム構築方法
    3.4 システム構成情報
    3.5 アプリケーションの構築

------------------------------------------------------------------------------
1. はじめに
------------------------------------------------------------------------------

本書は、EclipseあるいはLinuxコマンドラインの開発環境を使用して、T-Engineリファ
レンスボード(tef_em1d)用のT-Kernel 2.0 Extension、およびそれらの上で動くアプリ
ケーションを構築する際の手順を説明する文書です。

T-Kernel 2.0 Extension(T2EX)は、T-Kernel 2.0上に追加される拡張機能であり、ベー
スとなるT-Kernel2.0システムはそのまま使用します。

本パッケージは、T2EXとしてT-Kernel 2.0ソフトウェアパッケージに追加される部分の
みを含んでいます。T-Kernel2.0のソースコード、開発環境およびエミュレータは、
T-Kernel 2.0ソフトウェアパッケージ(T-Kernel 2.02.00 Software Package)に含まれ
ている内容をそのまま使用します。

T-Kernel 2.0のソフトウェアパッケージは、トロンフォーラムのT-Kernelトレーサビ
リティサービスに登録されており、最新版(T-Kernel 2.02.00 以降)をそこから入手し
て使用してください。T-Kernel 2.02.00 より前のバージョンでは、正しく動作しませ
んので、必ず最新版を使用してください。

------------------------------------------------------------------------------
2. パッケージ内容
------------------------------------------------------------------------------

本パッケージには以下の内容が含まれています。

(1) 説明書

    ReadMe.txt                  リリース説明書(本書)
    t2ex-dist-ucode.png         ディストリビューションucode
    TEF000-218-150401.pdf       T-License 2.1

    srcpkg/doc/ja/
        impl-t2ex.txt           T2EX実装仕様書

(2) ソースコード

    srcpkg/
        t2ex_source.tar.gz      ソースコードパッケージ
                                (T2EX, デバイスドライバ)
        bsd_source.tar.gz       ソースコードパッケージ
                                (NetBSDまたはOpenBSD由来のソースコード)

------------------------------------------------------------------------------
3. 作業の流れ
------------------------------------------------------------------------------

T2EX システムの構築(ビルド)作業に関して説明します。

開発方法に関しては、T-Kernel2.0 システムと同様に、Eclipse、Cygwin および Linux
利用の 3 種類の開発方法を選択することができます。詳しくは、T-Kernel 2.0 ソフト
ウェアパッケージ説明書を参照してください。

--------------------------------------------------
3.1 開発環境のインストール
--------------------------------------------------

開発用ホストPCに、T-Kernel 2.0 ソフトウェアパッケージに含まれているT-Kernel2.0
開発環境をインストールします。

インストールするディレクトリは標準では以下となりますが、別のディレクトリにイン
ストールしても構いません。

    /usr/local/tef_em1d

--------------------------------------------------
3.2 ソースコードパッケージの展開
--------------------------------------------------

最初に、開発環境をインストールしたディレクトリ(/usr/local/tef_em1d)上で、
T-Kernel 2.0ソフトウェアパッケージに含まれているT-Kernel ソースコードパッケージ
(tkernel_source.tar.gz)を展開して、tkernel_source ディレクトリを作成します。

次に、同じディレクトリ上で、本パッケージに含まれているT2EX ソースコード
パッケージ(t2ex_source.tar.gz, bsd_source.tar.gz)を展開して、t2ex_source
ディレクトリを作成します。

続いて、以下のコマンドを実行して、t2ex_source ディレクトリの内容をすべて、
tkernel_source ディレクトリにコピーします。

    $ cd /usr/local/tef_em1d/t2ex_source
    $ cp -r * ../tkernel_source

    ※ コピー後は、t2ex_source ディレクトリの内容は不要になりますので、すべて
       削除して構いません。

コピーにより、tkernel_source ディレクトリ内に、T2EX 用のソースファイルが追加さ
れます。ただし、例外として、以下のファイルだけは、T-Kernel の元のソースを上書
きします。(他のファイルはすべて追加です。)

    tkernel_source/etc/sysdepend/tef_em1d/makerules.sysdepend

    ※ 上書きされ変更された上記ファイルは、T-Kernel 用としても、そのまま利用で
       きます。


T2EX ソースコードパッケージ(t2ex_source.tar.gz, t2ex_source.tar.gz)の構成は
以下の通りで、この内容が tkernel_source ディレクトリ内に追加されます。

    include/                定義ファイル
        t2ex/**                 T2EX機能定義ファイル
        sys/
            atomic.h                アトミック操作定義ファイル
            sysdepend/
                atomic_common.h
                tef_em1d/
                    atomic_depend.h
        device/
            netdrv.h                ネットワークドライバ定義ファイル

    kernel/                 カーネル
        extension/              拡張部
            memory/
                t2ex/**             T2EX用メモリマネージャ

        sysdepend/t2ex/**       T2EX用システム依存部

        sysinit/                システム初期化
            build_t2ex/**           T2EXビルド用
            src_t2ex/**             T2EX用ソース

        sysmain/                システムメイン
            build_t2ex/**           T2EXビルド用
            src/
                usermain_t2ex.c     T2EX用ユーザメインプログラム
                appl_main.c         T2EX用アプリメインプログラム
                commoand.c          T2EX用コマンド処理プログラム
                ref_command.c       T2EX用参照コマンド処理プログラム
                network_sample/**   T2EX用ネットワークサンプルプログラム

        sysmgr/                 システムマネージャ
            build_t2ex/**           T2EXビルド用
            src_t2ex/**             T2EX用ソース

        tkernel/                T-Kernel
            build_t2ex/**           T2EXビルド用
            src_t2ex/**             T2EX用ソース

    lib/                    ライブラリ
        build_t2ex/**           T2EXビルド用
        libtk/                  T-Kernelライブラリ
            build_t2ex/**           T2EXビルド用
            src_t2ex/**             T2EX用ソース
        libc/**                 標準C互換ライブラリ
        libusermod/**           T2EXプログラムモジュール用ライブラリ

    t2ex/                   T2EX機能ソース
        build/**                T2EXビルド用
        fs/**                   T2EXファイル管理機能
        network/**              T2EXネットワーク通信機能
        datetime/**             T2EXカレンダー機能
        load/**                 T2EXプログラムロード機能

    module/**               T2EXプログラムモジュールサンプル/ビルド用

    config/                 設定情報(RomInfo, SYSCONF, DEVCONF)
        src_t2ex/**             T2EX用ソース
        build_t2ex/**           T2EXビルド用

    driver/                 デバイスドライバ
        tef_em1d/
            build_t2ex/**       T2EXビルド用
            netdrv/**           リファレンスボード用ネットワークドライバ

    etc/                    その他(スクリプト等)
        mkt2exsvc               T2EX SVC I/F ライブラリ作成用
        sysdepend/
            cpu/
                em1d/
                    makeift2ex.pl       T2EX SVC I/F ライブラリ作成用
            tef_em1d/
                makerules.sysdepend     共通 Make ルール [上書き]

--------------------------------------------------
3.3 システム構築方法
--------------------------------------------------

以下のコマンドを実行して、T-Kernel 部分を含めた、T2EXシステム全体を構築します。

    $ cd $BD/kernel/sysmain/build_t2ex/tef_em1d
    $ make emu

    ※ $BD は、環境変数 BD の値、すなわち TｰKernel ソースコードパッケージを
       インストールしたディレクトリの tkernel_source サブディレクトリ
       (/usr/local/tef_em1d/tkernel_source) となります。

    ※ Eclipse 環境の場合は、"kernel/sysmain/build_t2ex/tef_em1d" をビルド先
       ディレクトリとして、ビルド環境を設定したのち、"make emu" を実行します。

    ※ 以前に $BD の下で、T-Kernel をビルドした場合は、必ず、"make clean" を実
       行してから、T2EX システムをビルドしてください。

    ※ "kernel/sysmain/build/tef_em1d" 上で "make clean", "make emu" を実行す
       ると、T2EX 部分を含まないオリジナルのT-Kernel システムを構築できます。

"make emu" を実行した "$BD/kernel/sysmain/build_t2ex/tef_em1d" ディレクトリに、
ビルド結果である以下のファイルが生成されます。T2EX システムのファイル名には、
"_t2ex" が付加されます。
これらのファイルは、"$BD/bin/tef_em1d" ディレクトリにもコピーされます。

  (a) RAM 実行用 (デバッグ用)

    kernel_t2ex-ram.sys     RAM 版 T2EX の実行オブジェクト
    kernel_t2ex-ram.map     RAM 版 T2EX のリンクマップ
    kernel_t2ex-ram.mot     RAM 版 T2EX のロードイメージ (S-Format)
    kernel_t2ex-ram.bin     RAM 版 T2EX のロードイメージ (Binary)

    rominfo_t2ex-ram.mot    RAM 版 RomInfo のロードイメージ (S-Format)

    rom_t2ex-dbg.bin        Emulator 用 RAM 版 ロードイメージ (Binary)
                            ( T-Monitor + RomInfo )

  (b) ROM 実行用 (Flash ROM 書込み用)

    kernel_t2ex-rom.rom     ROM 版 T2EX の実行オブジェクト
    kernel_t2ex-rom.map     ROM 版 T2EX のリンクマップ
    kernel_t2ex-rom.mot     ROM 版 T2EX のロードイメージ (S-Format)

    rominfo_t2ex-rom.mot    ROM 版 RomInfo のロードイメージ (S-Format)

    rom_t2ex.bin            Emulator 用 ROM ロードイメージ (Binary)
                            ( T-Monitor + RomInfo + T-Kernel )

  (c) RAM/ROM 実行用 (Flash ROM 書込み用)

    tmonitor.mot            T-Monitor のロードイメージ (S-Format)
    tmonitor.map            T-Monitor のリンクマップ

    ※ T-Monitor は、T-Kernel 用に構築したものと同じです。

上記のビルド結果のファイルを使用して、Eclipse、Cygwin、または Linux 開発環境下
で、エミュレータ(QEMU)や実機を使用した実行、およびデバッグ実行を行うことができ
ます。詳しくは、T-Kernel 2.0 ソフトウェアパッケージ説明書を参照してください。


"$BD/kernel/sysmain/build_t2ex/tef_em1d" ディレクトリにある Makefile 内の以下
の定義を削除することにより、T2EX の機能の一部のみを含む実行イメージを作成でき
ます。

    # use T2EX memory management
    T2EX_MM = _t2ex

    # use task fault function of T2EX memory management
    T2EX_MM_USE_TASKEXCEPTION = 1

    # use default memory fault handlers
    T2EX_MM_USE_DEFAULT_FAULT_HANDLER = 1

    # use T2EX file management
    T2EX_FS = yes

    # use T2EX network communication
    T2EX_NET = yes

    # use T2EX calendar
    T2EX_DT = yes

    # use T2EX program load
    T2EX_PM = yes

    # use T2EX network sample programs
    T2EX_NET_SAMPLE = yes

    ※ ファイル管理機能、およびネットワーク通信機能を使用する場合、カレンダ
       機能は必要ですので、削除できません。

    ※ ファイル管理機能を削除した場合は、標準C互換ライブラリの標準入出力は使用
       できません。

また、以下の定義を変更することにより、実行イメージに組み込むデバイスドライバを
選択できます。

    # device driver definitions
    DRV_CONSOLE = yes
    DRV_CLOCK   = yes
    DRV_SYSDISK = yes
    DRV_SCREEN  = yes
    DRV_KBPD    = yes
    DRV_LOWKBPD = yes
    DRV_NET     = yes

    ※ T2EX の各機能で必要とするデバイスドライバは以下の通りです。

         DRV_CLOCK     カレンダ機能
         DRV_CONSOLE   ファイル管理機能(標準入出力)
         DRV_SYSDISK   ファイル管理機能
         DRV_NET       ネットワーク通信機能

--------------------------------------------------
3.4 システム構成情報
--------------------------------------------------

システム構成情報には、T2EX用の設定が追加されています。
設定値を変更する場合は、以下の SYSCONF ファイルを修正してください。

    $BD/config/src_t2ex/sysdepend/tef_em1d/SYSCONF

    ※ システム構成情報の T2EX用の設定に関しては「T2EX仕様書」および「T2EX実装
       仕様書」を参照してください。

--------------------------------------------------
3.5 アプリケーションの構築
--------------------------------------------------

T2EXのユーザアプリケーションは、T2EX システムとリンクされ、実行時には1つのオブ
ジェクトになります。

通常は、$BD/kernel/sysmain/src ディレクトリ内のプログラムに書き替えや追加を行
う形で、アプリケーションプログラムを記述します。サンプルとして、以下のファイル
が含まれていますので、参考にしてください。

    usermain_t2ex.c
        アプリケーションの初期タスクで実行されるプログラムです。
        デバイスドライバ、およびT2EX機能の初期化処理を実行して、appl_main() を
        呼び出します。

    appl_main.c
        カレンダ時刻のRTCハードウェアとの同期処理、およびデバッグコンソールから
        の入力によるコマンド処理を行います。

    command.c
        日付時刻やファイルに関する基本的なコマンドの処理を行います。
        また、T-Kernelのリソースの状態を表示する機能も入っています。
        "?" によりコマンドの簡単なヘルプを表示します。

    ref_command.c
        T-Kernelのリソースの状態を表示する ref コマンドの処理部分です。
        command.c にインクルードされます。

    network_sample/*
        T2EX ネットワーク機能をテストするためのサンプルプログラムです。
        "net" コマンドにより実行できます。実行の際は、インターネットに接続可能
        な DHCP サーバのあるネットワーク環境に接続する必要があります。

本パッケージで実装されている T2EX の各機能の詳細説明や制限事項などにつきまして
は、「T2EX実装仕様書」を参照してください。


以上
