# my_bashrc.sh

if [[ ! -v TC_KIND ]]; then
  #export TC_KIND=TOOL
  #export TC_KIND=ARMCT41
  #export TC_KIND=SH_GCC_v332
  #export TC_KIND=SH_GCC_v402
  #export TC_KIND=SH_GCC_KPIT
  export TC_KIND=ARM_GCC_EMB
fi

export C_DRV=/C
export PRJPATH=${C_DRV}/work/t-kernel
if [ -f ${PRJPATH}/_misc/etc/toolenv.sh ] ; then
  source ${PRJPATH}/_misc/etc/toolenv.sh
  alias gterm='${PRJPATH}/_misc/etc/msys2/gterm -l /dev/ttyS0 -b -x'
fi

alias wz='"${C_DRV}/Program Files/WZ EDITOR 9"/wzeditor'
alias markcat='start "${C_DRV}/Tools/MarkCat-win32-x64"/MarkCat.exe'

if [ ! ${TC_KIND} = "TOOL" ]; then
  cd $BD
fi

function wincmd() {
    CMD=$1
    shift
    $CMD $* 2>&1 | iconv -f cp932 -t utf-8
}
alias ipconfig='wincmd ipconfig'
alias netstat='wincmd netstat'
alias ping='wincmd ping'
alias nslookup='wincmd nslookup'
