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
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.x.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.x document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the TRON Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.tron.org/tk/?lang=en
 *    http://trace.tron.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.x, TRON Forum ensures that 
 * the portion of the software that is copyrighted by Ken Sakamura or 
 * the TRON Forum does not infringe the copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: TRON Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * The source codes in bsd_source.tar.gz in this software package are 
 * derived from NetBSD or OpenBSD and not covered under T-License 2.x.
 * They need to be changed or redistributed according to the 
 * representation of each source header.
 */
/*
 * Acknowledgement
 *
 * TRON Forum would like to acknowledge the contribution of the 
 * TRON Forum T-Kernel Working Group members for completion of 
 * the T2EX package.
 * The working group provided comments, held in-depth discussion 
 * and offered much assistance to detailed testing for the source code 
 * of the T2EX.
 *
 * T-Kernel Working Group members: 
 *   eSOL Co., Ltd. 
 *   FUJITSU COMPUTER TECHNOLOGIES LIMITED
 *   FUJITSU SEMICONDUCTOR LIMITED
 *   Hitachi ULSI Systems Co., Ltd.
 *   Personal Media Corporation
 *   Renesas Electronics Corporation
 *   YRP Ubiquitous Networking Laboratory
 */

==============================================================================
   T-Kernel 2.0 Extension Development Environment for T-Engine Reference Board
==============================================================================

------------------------------------------------------------------------------
 Table of Contents
------------------------------------------------------------------------------
  1. Foreword
  2. Package Contents
  3. Workflow
    3.1 Installing Development Environment
    3.2 Extracting Source Code Package
    3.3 How To Build System
    3.4 System Configuration Information
    3.5 Building Application

------------------------------------------------------------------------------
1. Foreword
------------------------------------------------------------------------------

This document describes the procedure to build T-Kernel 2.0 Extension for
T-Engine reference board (tef_em1d) and applications running on it using the
Eclipse or Linux command line development environment.

T-Kernel 2.0 Extension (T2EX) is an extension added onto T-Kernel 2.0, and the
underlying T-Kernel 2.0 system is used as it is.

This package contains only the part added as T2EX to the T-Kernel 2.0 software
package. T-Kernel 2.0 source code, development environment, and emulator 
included in T-Kernel 2.0 software package (T-Kernel 2.02.00 Software Package) 
can be used as they are.

The T-Kernel 2.0 software package is registered with the T-Kernel traceability
service in TRON Forum, where the latest version can be obtained.
Obtain the latest versions (T-Kernel 2.02.00 and above) from there and use them.
Make sure to use the latest version, since the T-Kernel 2.0 software package 
does not operate properly on the older versions than T-Kernel 2.02.00.

------------------------------------------------------------------------------
2. Package Contents
------------------------------------------------------------------------------

This package contains:

(1) Guides

    ReadMe.txt                 Release guide (this document)
    t2ex-dist-ucode.png        Distribution ucode
    TEF000-218-150401.pdf      T-License 2.1

    srcpkg/doc/ja/
        impl-t2ex.txt          T2EX implementation specifications

(2) Source code

    srcpkg/
        t2ex_source.tar.gz     Source code package
                               (T2EX and device driver)
        bsd_source.tar.gz      Source code package
                               (the source code derived from NetBSD or OpenBSD)

------------------------------------------------------------------------------
3. Workflow
------------------------------------------------------------------------------

This section describes the tasks to build the T2EX system.

Application on T2EX system can be developed using Eclipse, Cygwin or Linux in 
the same manner as on T-Kernel 2.0 system. For details, refer to T-Kernel 2.0 
software package manual.

--------------------------------------------------
3.1 Installing Development Environment
--------------------------------------------------

Install the T-Kernel 2.0 development environment included in the T-Kernel 2.0
software package in the host PC for development.

It can be installed in any directory. By default, it is installed in the
following directory.

    /usr/local/tef_em1d

--------------------------------------------------
3.2 Extracting Source Code Package
--------------------------------------------------

First, extract (untar) source files from the T-Kernel source code package 
(tkernel_source.tar.gz) in T-Kernel 2.0 software package under the directory 
(/usr/local/tef_em1d) where development environment is installed. 
This creates tkernel_source directory.

Next, extract files from T2EX source code package (t2ex_source.tar.gz, 
bsd_source.tar.gz) in this package under the same directory. 
This creates t2ex_source directory.

And then, execute the following commands to copy the content of t2ex_source 
directory to tkernel_source directory.

    $ cd /usr/local/tef_em1d/t2ex_source
    $ cp -r * ../tkernel_source

    * The content of t2ex_source directory is no longer needed after copying 
      and can be deleted.

The source files of T2EX will be added to tkernel_source directory by copying.
However, the following file will overwrite the original source files of 
T-Kernel as an exception. (Other files will be added.)

    tkernel_source/etc/sysdepend/tef_em1d/makerules.sysdepend

    * The above overwritten (and thus changed) file can be used as-is 
      for T-Kernel, too.

The content of T2EX source code package (t2ex_source.tar.gz, t2ex_source.tar.gz) 
is as follows. The content will be added to tkernel_source directory.

    include/                Definition files
        t2ex/**                 T2EX function definition files
        sys/
            atomic.h                Atomic operation definition file
            sysdepend/
                atomic_common.h
                tef_em1d/
                    atomic_depend.h
        device/
            netdrv.h                Network driver definition file

    kernel/                 Kernel
        extension/              Extension
            memory/
                t2ex/**             Memory manager for T2EX

        sysdepend/t2ex/**       System-dependent portion

        sysinit/                System initialize
            build_t2ex/**           For T2EX build
            src_t2ex/**             Sources for T2EX

        sysmain/                System main
            build_t2ex/**           For T2EX build
            src/
                usermain_t2ex.c     User main for T2EX
                appl_main.c         Application main for T2EX
                commoand.c          Command processer for T2EX
                ref_command.c       Reference command processing program for T2EX
                network_sample/**   Network Sample sources for T2EX

        sysmgr/                 System manager
            build_t2ex/**           For T2EX build
            src_t2ex/**             Sources for T2EX

        tkernel/                T-Kernel
            build_t2ex/**           For T2EX build
            src_t2ex/**             Sources for T2EX

    lib/                    Libraries
        build_t2ex/**           For T2EX build
        libtk/                  T-Kernel libraries
            build_t2ex/**           For T2EX build
            src_t2ex/**             Sources for T2EX
        libc/**                 Standard C compatible libraries
        libusermod/**           Libraries for T2EX program module

    t2ex/                   T2EX function sources
        build/**                For T2EX build
        fs/**                   T2EX file management function
        network/**              T2EX network communication function
        datetime/**             T2EX calendar function
        load/**                 T2EX program load function

    module/**               For T2EX program module sample/build

    config/                 Configuration (RomInfo, SYSCONF, DEVCONF)
        src_t2ex/**             Sources for T2EX
        build_t2ex/**           For T2EX build

    driver/                 Device drivers
        tef_em1d/
            build_t2ex/**       For T2EX build
            netdrv/**           Network drivers for reference board

    etc/                    Others (script, etc.)
        mkt2exsvc               for creating T2EX SVC I/F library
        sysdepend/
            cpu/
                em1d/
                    makeift2ex.pl       for creating T2EX SVC I/F library
            tef_em1d/
                makerules.sysdepend     Common Make rules [*overwrite*]

--------------------------------------------------
3.3 How To Build System
--------------------------------------------------

Execute the following commands to build the whole T2EX system including T-Kernel.

    $ cd $BD/kernel/sysmain/build_t2ex/tef_em1d
    $ make emu

    * $BD is the value of environment variable BD, i.e., tkernel_source 
      subdirectory (/usr/local/tef_em1d/tkernel_source) of the directory 
      where T-Kernel source code package is installed.

    * In case of Eclipse environment, set up the build environment with 
      "kernel/sysmain/build_t2ex/tef_em1d" as its build destination 
      directory and execute "make emu".

    * If T-Kernel was built under $BD, make sure to execute "make clean" 
      before building T2EX system.

    * The original T-Kernel not including the T2EX addition can be built by 
      executing "make clean" and "make emu" under "kernel/sysmain/build/tef_em1d".

The following files, the build results, will be created in 
"$BD/kernel/sysmain/build_t2ex/tef_em1d" directory where "make emu" 
is executed.
"_t2ex" will be added to the file names of T2EX system binaries.
These files will also be copied to "$BD/bin/tef_em1d" directory.

  (a) For RAM execution (for debug)

    kernel_t2ex-ram.sys     Executable object of RAM version T2EX
    kernel_t2ex-ram.map     Link map of RAM version T2EX
    kernel_t2ex-ram.mot     Load image of RAM version T2EX (S-Format)
    kernel_t2ex-ram.bin     Load image of RAM version T2EX (Binary)

    rominfo_t2ex-ram.mot    Load image of RAM version RomInfo (S-Format)

    rom_t2ex-dbg.bin        Load image of RAM version for Emulator (Binary)
                            ( T-Monitor + RomInfo )

  (b) For ROM execution (for writing into Flash ROM)

    kernel_t2ex-rom.rom     Executable object of ROM version T2EX
    kernel_t2ex-rom.map     Link map of ROM version T2EX
    kernel_t2ex-rom.mot     Load image of ROM version T2EX (S-Format)

    rominfo_t2ex-rom.mot    Load image of ROM version RomInfo (S-Format)

    rom_t2ex.bin            Load image of ROM version for Emulator (Binary)
                            ( T-Monitor + RomInfo + T-Kernel )

  (c) For RAM/ROM execution (for writing into Flash ROM)

    tmonitor.mot            Load image of T-Monitor (S-Format)
    tmonitor.map            Link map of T-Monitor

    * The used T-Monitor is the same one built for T-Kernel.

Using the above build files, you can run and debug T2EX application by using 
emulator (QEMU) or target hardware in the development environment of Eclipse, 
Cygwin, or Linux. For details, refer to T-Kernel 2.0 software package manual.


Executable image including only a limited set of T2EX functions can be 
created by deleting the following definitions in Makefile in 
"$BD/kernel/sysmain/build_t2ex/tef_em1d" directory.

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

    * When the file management function or network communication function 
      are used, the calendar function is required and cannot be deleted.

    * If the file management function is deleted, the standard I/O functions 
      of standard C library cannot be used.

The device drivers to be embedded in the executable image can be selected 
by changing the following definitions.

    # device driver definitions
    DRV_CONSOLE = yes
    DRV_CLOCK   = yes
    DRV_SYSDISK = yes
    DRV_SCREEN  = yes
    DRV_KBPD    = yes
    DRV_LOWKBPD = yes
    DRV_NET     = yes

    * The device drivers required for each T2EX function are as follows.

         DRV_CLOCK     T2EX calendar function
         DRV_CONSOLE   T2EX file management function (Standard input/output)
         DRV_SYSDISK   T2EX file management function
         DRV_NET       T2EX network communication function

--------------------------------------------------
3.4 System Configuration Information
--------------------------------------------------

Settings for T2EX have been added to the system configuration information.
To modify their values, modify SYSCONF in the following directories.

    $BD/config/src_t2ex/sysdepend/tef_em1d/SYSCONF

    * For the setting for T2EX of the system configuration information, 
      see "T2EX Specifications" and "T2EX Implementation Specifications".

--------------------------------------------------
3.5 Building Application
--------------------------------------------------

A T2EX user application is linked with the T2EX system to be an object at
runtime.

To write an application program, generally, a program in the $BD/kernel/sysmain/src 
directory can be modified or added. The following sample programs are 
included in the $BD/kernel/sysmain/src directory of this package for 
reference.

    usermain_t2ex.c
        This program is run as the initial task of an application.
        It performs the initialization of device drivers and T2EX functions,
        and calls appl_main().

    appl_main.c
        This synchronizes the calendar time with the RTC hardware and handles
        command inputs from the debugging console.

    command.c
        This handles basic commands about date/time or file.
        It also provides the function to display the status of T-Kernel
        resources.
        Simple help summary for commands will be shown by "?".

    ref_command.c
        This processes of ref command which displays the status of T-Kernel 
        resources.
        It is included in command.c.

    network_sample/*
        Sample programs to test T2EX network function.
        It can be executed by "net" command. For execution, target needs to 
        be connected to Internet-ready network environment with a DHCP server.

For details and limitations on each T2EX function implemented in this package,
see "T2EX Implementation Specifications".

END.
