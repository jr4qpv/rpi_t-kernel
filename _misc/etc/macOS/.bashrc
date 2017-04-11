# .bashrc

# some more ls aliases
alias ll='ls -lF'
alias la='ls -A'
alias l='ls -CF'

alias e='emacsclient -n'
##alias e='emacsclient -a "" -t'
alias ec='emacsclient'
alias ee='open -a /Applications/Emacs.app $1'
alias em='emacs'
alias ekill='emacsclient -e "(kill-emacs)"'

#export TC_KIND=TOOL
export TC_KIND=MAC_ARM_GCC_EMB
export PRJPATH=${HOME}/work/t-kernel
source ${PRJPATH}/_misc/etc/toolenv.sh

alias gterm='${BD}/tool/Darwin-i386/etc/gterm -l /dev/tty.usbserial -b -x'
##cd $BD/monitor/tmmain/build
##cd $BD/kernel/sysmain/build_t2ex/app_rzt1.debug

if [ ! ${TC_KIND} = "TOOL" ]; then
  cd $BD
fi
