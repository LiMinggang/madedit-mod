# Created by: Minggang Li <minggang.li@gmail.com>
# $FreeBSD$

PORTNAME=	madedit
PORTVERSION=	0.3.2
CATEGORIES=	editors
MASTER_SITES=	SF/${PORTNAME}/${PORTVERSION}
DISTNAME=	madedit-${PORTVERSION}

MAINTAINER=	Minggang Li <minggang.li@gmail.com>
COMMENT=	madedit: Cross-platform Text/Hex Editor, Improved Version of MadEdit

LICENSE=	GPLv3

USES=		gettext pkgconfig desktop-file-utils
WANT_UNICODE=	yes

GNU_CONFIGURE=	yes
CONFIGURE_ARGS+=--with-wx-config=${WX_CONFIG}
CONFIGURE_ENV+=	PKG_CONFIG="${PKG_CONFIG}" glib_gtk2_CFLAGS="${glib_gtk2_CFLAGS}" glib_gtk2_LIBS="${glib_gtk2_LIBS}"
CXXFLAGS+=	${glib_gtk2_CFLAGS}

PKG_CONFIG?=	${LOCALBASE}/bin/pkgconf
glib_gtk2_CFLAGS=`${PKG_CONFIG} --cflags gtk+-2.0`
glib_gtk2_LIBS=	`${PKG_CONFIG} --libs gtk+-2.0`

BROKEN_sparc64=	yes

.include <bsd.port.mk>