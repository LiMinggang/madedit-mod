#!/bin/sh

#======================================================================
# 1. copy files
#----------------------------------------------------------------------

export LC_ALL=C

SRCBASEDIR="../.."

ver=`cat "$SRCBASEDIR/VERSION"`
echo "$ver" | grep '-' > /dev/null
[ $? -eq 0 ] || ver="$ver"-1

verbase=`echo "$ver" | sed 's/-.*$//'`
release=`echo "$ver" | sed 's/^.*-//'`

sedcmd_ver="s/@VER@/$verbase/;s/@REL@/$release/"

SRCPARENTDIR="$SRCBASEDIR/.."
SRCVERDIR="$SRCPARENTDIR/madedit-mod-$verbase"

mkdir -p "$SRCVERDIR"
chmod +x "$SRCBASEDIR/configure"
/bin/cp -Rf "$SRCBASEDIR/"* "$SRCVERDIR"

#======================================================================
# 2. generate .spec files
#----------------------------------------------------------------------

cd "$SRCVERDIR/packaging/rpm"

NEW_SRCBASEDIR="../.."
NEW_SRCPARENTDIR="$NEW_SRCBASEDIR/.."

#gawk -f changelog_rpm.awk "$NEW_SRCBASEDIR/ChangeLog" > changelog

cat madedit.spec._ | sed "$sedcmd_ver" > madedit.spec

sed -f static_spec.sed madedit.spec > madedit-static.spec

#======================================================================
# 3. create .tar.gz
#----------------------------------------------------------------------

cd "$NEW_SRCPARENTDIR"

SRCPACKDIR="madedit-mod-$verbase"
SRCPACK="$SRCPACKDIR.tar.gz"

tar -czvf "$SRCPACK" "$SRCPACKDIR"

#======================================================================
# 4. build rpm
#----------------------------------------------------------------------

mv "$SRCPACK" ~/rpmbuild/SOURCES/

RPMBUILDOPT=""
uname -r | grep 'fc[0-9]\+\.i686' > /dev/null && RPMBUILDOPT="--target=i686"
rpmbuild $RPMBUILDOPT -bb "$SRCPACKDIR/packaging/rpm/madedit.spec"
