#!/bin/sh

(
  cd /usr/ports/editors/
  [ -e madedit-mod_bak ] && sudo rm -rf madedit-mod_bak
  [ -e madedit-mod ] && sudo mv -f madedit-mod madedit-mod_bak
  sudo mkdir madedit-mod
)

sudo cp -f Makefile pkg-descr /usr/ports/editors/madedit-mod/

sudo sh -c "sed -e 's#share/doc/madedit-mod#%%DOCSDIR%%#' \
                -e 's#share/madedit-mod#%%DATADIR%%#' \
                -e 's/@dirrm /@dirrmtry /' pkg-plist_old > \
                /usr/ports/editors/madedit-mod/pkg-plist"

cd /usr/ports/editors/madedit-mod
sudo make makesum
sudo make package