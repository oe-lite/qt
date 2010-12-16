DESCRIPTION = "Qt is a versatile cross-platform application framework -- this is the X11 version."
SECTION = "x11/libs"
PRIORITY = "optional"
HOMEPAGE = "http://www.trolltech.com"
LICENSE = "GPL QPL"
DEPENDS += "libX11-dev fontconfig-dev"
#libxft libxext libxrender libxrandr libxcursor"/*!\cbstodo*/
DEFAULT_PREFERENCE = "-1"

INC_PR = "r0"

S = "${WORKDIR}/qt-x11-opensource-src-${PV}"

QT_GLFLAGS ?= "-no-opengl"
QT_CONFIG_FLAGS += "-no-xinerama -no-xkb ${QT_GLFLAGS}"
QT_BASE_NAME ?= "qt4"
QT_BASE_LIB  ?= "libqt"
QT_DIR_NAME = "qt4"
QT_LIBINFIX = ""

require qt4.inc

inherit qt4x11


PR = "${INC_PR}.0"

QT_CONFIG_FLAGS_append_armv6 = " -no-neon "

require qt-${PV}.inc

QT_CONFIG_FLAGS += " \
 -no-embedded \
 -xrandr \
 -x11"

