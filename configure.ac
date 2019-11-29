#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([madedit-mod], [0.4.18], [minggang.li@gmail.com])
AC_CONFIG_SRCDIR([src/MadEditApp.cpp])
AC_CONFIG_HEADER([config.h])
AC_PREFIX_DEFAULT([/usr])

AM_INIT_AUTOMAKE([subdir-objects])

AC_USE_SYSTEM_EXTENSIONS
ALL_LINGUAS="it_IT ja_JP zh_CN zh_TW ru_RU es el de_DE pl_PL"
AM_GNU_GETTEXT([external])
AC_SUBST(ALL_LINGUAS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_LANG(C++)

CXXFLAGS="-O2 -Wno-write-strings -fno-strict-aliasing -fPIC "

# Checks for MacOS
IS_MACOS=
case "${host}" in
    *-*-darwin* | powerpc-apple-macos* )
        IS_MACOS=1
    ;;
esac

# Checks for glib2, gtk2 libraries
#AM_PATH_GLIB_2_0(2.6.0)
#AM_PATH_GTK_2_0(2.6.0)

if test "x$IS_MACOS" == "x"; then
    want_GTK=
    AC_ARG_WITH(gtk,
    [  --with-gtk=version    use specified GTK+,
                otherwise try to find GTK+2 then try GTK+3],
    [want_GTK="$withval"],
    [want_GTK=0])

    case "$want_GTK" in
        3)     ;;
        2)     ;;
        0)     ;;
        *)      AC_MSG_ERROR([
Unsupported GTK+ version $withval specified. Only GTK+2 and GTK+3
are supported. 0 is auto.
                        ])
                ;;
    esac

    have_GTK=
    PKG_CHECK_MODULES(glib_gtk2, [
            glib-2.0 >= 2.6.0
            gtk+-2.0 >= 2.6.0
    ], have_GTK=2, have_GTK=0)

    if test "$have_GTK" -ne 0; then
        if test "$want_GTK" -eq 0; then
            CXXFLAGS="$CXXFLAGS $glib_gtk2_CFLAGS"
            LIBS="$LIBS $glib_gtk2_LIBS"
        else
            if test "$want_GTK" -eq "$have_GTK"; then
                CXXFLAGS="$CXXFLAGS $glib_gtk2_CFLAGS"
                LIBS="$LIBS $glib_gtk2_LIBS"
            else
                PKG_CHECK_MODULES(glib_gtk3, [
                    glib-2.0 >= 2.6.0
                    gtk+-3.0
                ])
                CXXFLAGS="$CXXFLAGS $glib_gtk3_CFLAGS"
                LIBS="$LIBS $glib_gtk3_LIBS"
            fi
        fi
    else
        if test "$want_GTK" -eq 2; then
            AC_MSG_ERROR([
Package gtk+-2.0 was not found in the pkg-config search path.
Perhaps you should add the directory containing `gtk+-2.0.pc' to the PKG_CONFIG_PATH environment variable
No package 'gtk+-2.0' found
            ])
        fi
        PKG_CHECK_MODULES(glib_gtk3, [
            glib-2.0 >= 2.6.0
            gtk+-3.0
        ])
        CXXFLAGS="$CXXFLAGS $glib_gtk3_CFLAGS"
        LIBS="$LIBS $glib_gtk3_LIBS"
    fi
fi

#if test "x$IS_MACOS" == "x"; then
#    PKG_CHECK_MODULES(python, [
#            python >= 2.7
#    ])
#    CXXFLAGS="$CXXFLAGS $python_CFLAGS"
#    LIBS="$LIBS $python_LIBS"
#fi

# option for additional Xpressive lib
AC_ARG_WITH(xpressive, 
    [  --with-xpressive=PATH   use additional Boost.Xpressive library path,
                          otherwise use default path "$(top_srcdir)/xpressive"],
    [CXXFLAGS="$CXXFLAGS -I$withval"],
    [CXXFLAGS="$CXXFLAGS -I\$(top_srcdir)/xpressive"])

# option for additional Boost lib
AC_ARG_WITH(boost, 
    [  --with-boost=PATH       use additional Boost library path,
                          otherwise use system include path],
    [CXXFLAGS="$CXXFLAGS -I$withval"])
#    [LIBS="$LIBS $withval/stage/lib/libboost_python.a"])

# Checks for wxWidgets library
WX_LIBS_set="0"
if test "x$WX_LIBS" != "x"; then
    WX_LIBS_set="1"
fi

AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG(2.8.0, wxWin=1, , [std,net,aui,html], [--static])
if test "$wxWin" != 1; then
   AC_MSG_ERROR([
           wxWidgets must be installed on your system
           but wx-config script couldn't be found.

           Please check that wx-config is in path, the directory
           where wxWidgets libraries are installed (returned by
           'wx-config --libs' command) is in LD_LIBRARY_PATH or
           equivalent variable and wxWidgets version is 2.8.0 or above.
   ])
fi
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
#CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
if test "$WX_LIBS_set" != 1; then
    LIBS="$LIBS $WX_LIBS"
fi

UNICODE_FLAG=$(test ! $($WX_CONFIG_WITH_ARGS - -unicode >/dev/null 2>&1))

AC_MSG_CHECKING([if wxWidgets was built with UNICODE enabled])
if test "$UNICODE_FLAG" != "0"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([wxWidgets must be built with UNICODE enabled.])
fi

# Make sure we have X11, and also have it added to LIBS
#AC_CHECK_LIB([X11], [main],[ac_X11="yes"],[ac_X11="no"])
#if test "x$ac_X11" = "xno"; then
#   AC_MSG_ERROR([Could not find XOpenDisplay in X11.])
#else
#   LIBS="$LIBS -lX11"
#fi
LIBS="$LIBS -lX11"

# Make sure we have hunspell, and also have it added to LIBS
# Checks for header files.
#AC_CHECK_HEADERS([hunspell/hunspell.hxx])
#LIBS="$LIBS -lhunspell"
#if test "x$IS_MACOS" == "x"; then
#    PKG_CHECK_MODULES(hunspell, [
#            hunspell >= 1.3
#    ])
#    CXXFLAGS="$CXXFLAGS $hunspell_CFLAGS"
#    LIBS="$LIBS $hunspell_LIBS"
#fi

# Checks for header files.
AC_CHECK_HEADERS([boost/config.hpp])

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
#AC_FUNC_MALLOC
#AC_HEADER_STDC
AC_CHECK_FUNCS([dup2])

AC_CONFIG_FILES([Makefile syntax/Makefile scripts/Makefile po/Makefile.in])

AC_OUTPUT
