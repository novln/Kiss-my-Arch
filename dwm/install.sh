#!/bin/sh

makepkg -efi
cp .Xmodmap $HOME/.Xmodmap
echo -e 'xmodmap ~/.Xmodmap\nDEFAULT_SESSION=dwm\nexec ck-launch-session $DEFAULT_SESSION\n' >> $HOME/.xinitrc
$EDITOR $HOME/.xinitrc
