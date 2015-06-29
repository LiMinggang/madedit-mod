#!/bin/sh

(
  cd /usr/ports/editors/
  [ -e madedit_bak ] && sudo rm -rf madedit_bak
  [ -e madedit ] && sudo mv -f madedit madedit_bak
  sudo mkdir madedit
)

sudo cp -f Makefile pkg-descr /usr/ports/editors/madedit/

sudo sh -c "sed -e 's#share/doc/madedit#%%DOCSDIR%%#' \
                -e 's#share/madedit#%%DATADIR%%#' \
                -e 's/@dirrm /@dirrmtry /' pkg-plist_old > \
                /usr/ports/editors/madedit/pkg-plist"

cd /usr/ports/editors/madedit
sudo make makesum
sudo make package