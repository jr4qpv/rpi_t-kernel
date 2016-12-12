# .bashrc

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

alias e='emacs'
alias ee='open -a /Applications/Emacs.app $1'

#export TC_KIND=TOOL
export TC_KIND=MAC_ARM_GCC_EMB
export PRJPATH=${HOME}/work/t-kernel
source ${PRJPATH}/_misc/etc/toolenv.sh

alias gterm='${BD}/tool/mingw32-x86_64/etc/gterm -l /dev/ttyS0 -b -x'
##cd $BD/monitor/tmmain/build
##cd $BD/kernel/sysmain/build_t2ex/app_rzt1.debug
cd $BD
