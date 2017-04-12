# .bashrc

# Emacs setting
if [ -f ~/.emacs.d/etc/macos/emacs_env.sh ] ; then
  source ~/.emacs.d/etc/macos/emacs_env.sh
fi

##export TC_KIND=TOOL
if [ -f ~/envfiles/macOS/my_bashrc.sh ] ; then
  source ~/envfiles/macOS/my_bashrc.sh
fi
