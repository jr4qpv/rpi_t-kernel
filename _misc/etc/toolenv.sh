#!/bin/sh

#--- T-Kernel ---
export BD=${PRJPATH}/tkernel_source
export GNUs=/usr

if test ${TC_KIND} = SH_GCC_TF
then
  echo "--- SH7760 GCC TF ---"
  export GNU_BD=/usr/local/tool/sh-elf
  export _GNU_CONFIG=sh-unknown-tkernel
  export GNUsh=${GNU_BD}/${_GNU_CONFIG}
  export GCC_EXEC_PREFIX=$GNU_BD/lib/gcc-lib/
elif test ${TC_KIND} = SH_GCC_KPIT
then
  echo "--- SH7760 GCC KPIT(Renesas GNU Tool) ---"
  export GNU_BD=${C_DRV}/Renesas/e2_studio/GNUSHv13.01-ELF/sh-elf
  export _GNU_CONFIG=sh-elf
  export GNUsh=${GNU_BD}/${_GNU_CONFIG}
  export GCC_EXEC_PREFIX=$GNU_BD/lib/gcc/
elif test ${TC_KIND} = SH_GCC_v332
then
  echo "--- SH7760 GCC v3.3.2 ---"
  export GNU_BD=${C_DRV}/eSOL/eBinderSH/cl/gcc/sh/3.3.2
  export _GNU_CONFIG=sh-elf
  export GNUsh=${GNU_BD}/${_GNU_CONFIG}
  export GCC_EXEC_PREFIX=$GNU_BD/lib/gcc-lib/
elif test ${TC_KIND} = SH_GCC_v402
then
  echo "--- SH7760 GCC v4.0.2 ---"
  export GNU_BD=${C_DRV}/eSOL/eBinderSH/cl/gcc/sh/4.0.2
  export _GNU_CONFIG=sh-elf
  export GNUsh=${GNU_BD}/${_GNU_CONFIG}
  export GCC_EXEC_PREFIX=$GNU_BD/lib/gcc/
elif test ${TC_KIND} = ARM_GCC_TF
then
  echo "--- ARM GCC TF ---"
  export GNU_BD=/usr/local/tool/arm_2-elf
  export _GNU_CONFIG=arm_2-unknown-tkernel-
  export GNUARM_2=${GNU_BD}/arm_2-unknown-tkernel
elif test ${TC_KIND} = ARM_GCC_KPIT
then
  echo "--- ARM GCC KPIT ---"
  # export GNU_BD='/C/Program Files (x86)/KPIT/GNUARM-NONEv14.02-EABI/arm-none-eabi/arm-none-eabi'
  export GNU_BD='/C/Program Files (x86)/KPIT/GNUARM-NONEv16.01-EABI/arm-none-eabi/arm-none-eabi'
  export _GNU_CONFIG=arm-none-eabi-
elif test ${TC_KIND} = ARM_GCC_EMB
then
  echo "--- GNU ARM Embedded ---"
  # export GNU_BD='/C/Program Files (x86)/GNU Tools ARM Embedded/4.9 2015q3'
  export GNU_BD='/C/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q2'
  export _GNU_CONFIG=arm-none-eabi-
elif test ${TC_KIND} = ARMCT50
then
  echo "--- ARMCT 5.0x ---"
  export ARM_BD=/C/eSOL/eBinder/cl/ARM/ARMCT_5.0x
  export ARMCC5INC=${ARM_BD}/include
  export ARMCC5LIB=${ARM_BD}/lib
  export PATH=${ARM_BD}/bin:$PATH
elif test ${TC_KIND} = ARMCT41
then
  echo "--- ARMCT 4.1 ---"
  export ARM_BD=/C/eSOL/eBinder/cl/ARM/RVCT/Programs/4.1/462/win_32-pentium
  export ARMCC41INC=/C/eSOL/eBinder/cl/ARM/RVCT/Data/4.1/462/include/windows
  export ARMCC41LIB=/C/eSOL/eBinder/cl/ARM\RVCT/Data/4.1/462/lib
  export PATH=${ARM_BD}:$PATH
elif test ${TC_KIND} = MAC_ARM_GCC_EMB
then
  echo "--- macOS GNU ARM Embedded ---"
  export GNU_BD=/opt/gnu_arm/gcc-arm-none-eabi-5_4-2016q2
  export _GNU_CONFIG=arm-none-eabi-
  export PATH=$PATH:${GNU_BD}/bin
else
  echo "--- TOOL ---"
fi

alias gdbarm='${GNU_BD}/bin/${_GNU_CONFIG}gdb'
# cd $BD
