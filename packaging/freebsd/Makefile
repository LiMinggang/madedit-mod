# Created by: Minggang Li <minggang.li@gmail.com>
# $FreeBSD$

PORTNAME=	madedit-mod
PORTVERSION=	0.4.17
CATEGORIES=	editors
MASTER_SITES=	https://sourceforge.net/projects/${PORTNAME}/files/${PORTVERSION}/
DISTNAME=	MadEdit-Mod\ ${PORTVERSION}

MAINTAINER=	Minggang Li <minggang.li@gmail.com>
COMMENT=	madedit-mod: Cross-platform Text/Hex Editor, Improved Version of MadEdit

LICENSE=	GPLv3

USES=		gettext pkgconfig desktop-file-utils
WANT_UNICODE=	yes

GNU_CONFIGURE=	yes
CONFIGURE_ARGS+=--with-boost=${HOME}/boost_1_70_0
CONFIGURE_ENV+=	PKG_CONFIG="${PKG_CONFIG}" glib_gtk3_CFLAGS="${glib_gtk2_CFLAGS}" glib_gtk3_LIBS="${glib_gtk2_LIBS}"
CXXFLAGS+=	${glib_gtk3_CFLAGS}
CPPFLAGS+=	-I/usr/local/include
CFLAGS+=	-I/usr/local/include
CXXFLAGS+=	-I/usr/local/include
LDFLAGS+=	-L/usr/local/lib

PKG_CONFIG?=	${LOCALBASE}/bin/pkgconf
glib_gtk2_CFLAGS=`${PKG_CONFIG} --cflags gtk+-3.0`
glib_gtk2_LIBS=	`${PKG_CONFIG} --libs gtk+-3.0`

BROKEN_sparc64=	yes

.include <bsd.port.mk>