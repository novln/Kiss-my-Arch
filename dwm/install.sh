#!/bin/bash

cp config.h src/
cp dwm.desktop src/
cp dwm-5.9.tar.gz src/

if [ $1 == "nocheck" ]; then
	makepkg -efi --skipinteg
else
	makepkg -efi
fi

rm src/config.h
rm src/dwm.desktop
rm src/dwm-5.9.tar.gz
