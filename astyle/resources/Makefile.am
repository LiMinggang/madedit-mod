EXTRA_DIST = configuration.xrc manifest.xml

pkgdata_DATA = astyle.zip
CLEANFILES = $(pkgdata_DATA)

astyle.zip: $(EXTRA_DIST)
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/astyle.zip manifest.xml *.xrc > /dev/null
