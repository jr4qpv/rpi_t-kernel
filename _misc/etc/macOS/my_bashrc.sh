# my_bashrc.sh

# some more ls aliases
alias ll='ls -lF'
alias la='ls -A'
alias l='ls -CF'

if [ -z "$TC_KIND" ]; then
  #export TC_KIND=TOOL
  export TC_KIND=MAC_ARM_GCC_EMB
fi
export PRJPATH=${HOME}/work/t-kernel
if [ -f ${PRJPATH}/_misc/etc/toolenv.sh ] ; then
  source ${PRJPATH}/_misc/etc/toolenv.sh
  alias gterm='${PRJPATH}/_misc/etc/macOS/gterm -l /dev/tty.usbserial -b -x'
fi
  
if [ ! ${TC_KIND} = "TOOL" ]; then
  cd $BD
fi
