#!/bin/sh

export LC_ALL=C

TOP_DIR="../.."
DEBIAN_DIR="$TOP_DIR/debian"
DEBSOURCE_DIR="$DEBIAN_DIR/source"
mkdir -p "$DEBSOURCE_DIR"
cat ./changelog > "$DEBIAN_DIR/changelog"
cp control copyright menu rules "$DEBIAN_DIR"
echo '1.0' > "$DEBSOURCE_DIR/format"
echo 8 > "$DEBIAN_DIR/compat"
cd "$TOP_DIR"
sh autogen.sh
touch aclocal.m4 configure Makefile.in config.h.in
chmod +x configure
dpkg-buildpackage