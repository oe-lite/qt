SUMMARY = "Qt is a versatile cross-platform application framework -- this is the embedded version."
LICENSE = "GPL QPL"
HOMEPAGE = "http://www.trolltech.com"
DEPENDS += "tslib-dev directfb-dev"
RDEPENDS_${PN} += "tslib-calibrate directfb"

require qt4.inc 
require qt-${PV}.inc

inherit qt4e

QT_BASE_NAME ?= "qt4-embedded"
QT_BASE_LIB  ?= "libqt-embedded"
QT_DIR_NAME = "qtopia"
QT_LIBINFIX = "E"

QT_CONFIG_FLAGS += " \
    -qtlibinfix ${QT_LIBINFIX} \
    -plugin-gfx-transformed -plugin-gfx-qvfb -plugin-gfx-vnc -plugin-gfx-directfb \
    -qt-mouse-tslib -qt-mouse-pc -qt-mouse-qvfb -qt-mouse-linuxinput \
    -qt-kbd-tty \
    -DQT_KEYPAD_NAVIGATION \
    "

QT_CONFIG_FLAGS_append_armv6 = " -no-neon "

QT_CONFIG_FLAGS += " \
 -exceptions \
"

do_install_append() {
	install -d ${D}${sysconfdir}/profile.d/
	install -m 0755 ${SRCDIR}/qte.sh ${D}${sysconfdir}/profile.d/
}

FILES_${PN} += " ${sysconfdir}/profile.d/qte.sh"
