# .bashrc

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

#export TC_KIND=TOOL
export TC_KIND=MAC_ARM_GCC_EMB
export PRJPATH=${HOME}/work/t-kernel
source ${PRJPATH}/_misc/etc/toolenv.sh

#--- for u-boot ---
export CROSS_COMPILE=arm-none-eabi-

alias gterm='${BD}/tool/Darwin-i386/etc/gterm -l /dev/tty.usbserial -b -x'
##cd $BD/monitor/tmmain/build
##cd $BD/kernel/sysmain/build_t2ex/app_rzt1.debug
cd $BD
