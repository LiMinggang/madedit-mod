SUBDIRS = resources

AM_CPPFLAGS = $(WX_CXXFLAGS) \
           -I$(top_srcdir)/src/plugins/astyle/astyle \
           -I$(top_srcdir)/src/include \
           -I$(top_srcdir)/src/sdk/wxscintilla/include

pluginlibdir = $(pkglibdir)/plugins

pluginlib_LTLIBRARIES = libastyle.la

libastyle_la_LDFLAGS = -module -version-info 0:1:0 -shared -no-undefined -avoid-version

libastyle_la_LIBADD = ../../sdk/libcodeblocks.la $(WX_LIBS)

libastyle_la_SOURCES = asstreamiterator.cpp \
                       astyleconfigdlg.cpp \
                       astyleplugin.cpp \
                       dlgformattersettings.cpp \
                       formattersettings.cpp \
                       astyle/ASBeautifier.cpp \
                       astyle/ASEnhancer.cpp \
                       astyle/ASFormatter.cpp \
                       astyle/ASResource.cpp

noinst_HEADERS = asstreamiterator.h \
                 astyleconfigdlg.h \
                 astyleplugin.h \
                 astylepredefinedstyles.h \
                 dlgformattersettings.h \
                 formattersettings.h \
                 astyle/astyle.h

EXTRA_DIST = astyle/ASLocalizer.cpp \
             astyle/ASLocalizer.h \
             astyle/astyle.html \
             astyle/astyle_main.cpp \
             astyle/astyle_main.h \
             astyle/index.html \
             astyle/install.html \
             astyle/license.html \
             astyle/links.html \
             astyle/news.html \
             astyle/notes.html \
             astyle/scripts.html \
             astyle/styles.css \
             astyle/subversion.html \
             astyle/vsinstall.html
