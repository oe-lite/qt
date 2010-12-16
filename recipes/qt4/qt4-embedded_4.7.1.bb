SUMMARY = "Qt is a versatile cross-platform application framework -- this is the embedded version."
SECTION = "libs"
LICENSE = "GPL QPL"
PRIORITY = "optional"
HOMEPAGE = "http://www.trolltech.com"
DEPENDS += "tslib-dev directfb-dev"

DEFAULT_PREFERENCE = "-1"

INC_PR = "r0"

S = "${WORKDIR}/qt-embedded-linux-opensource-src-${PV}"

QT_BASE_NAME ?= "qt4-embedded"
QT_BASE_LIB  ?= "libqt-embedded"
QT_DIR_NAME = "qtopia"
QT_LIBINFIX = "E"

QT_CONFIG_FLAGS += " \
    -qtlibinfix ${QT_LIBINFIX} \
    -plugin-gfx-transformed -plugin-gfx-qvfb -plugin-gfx-vnc -plugin-gfx-directfb \
    -plugin-mouse-tslib -qt-mouse-pc -qt-mouse-qvfb -qt-mouse-linuxinput \
    -qt-kbd-tty \
    -DQT_KEYPAD_NAVIGATION \
    "
do_install_append() {
	install -d ${D}${sysconfdir}/profile.d/
	install -m 0755 ${WORKDIR}/qte.sh ${D}${sysconfdir}/profile.d/
}

FILES_${PN} += " ${sysconfdir}/profile.d/qte.sh"

require qt4.inc

inherit qt4e

PR = "${INC_PR}.0"

QT_CONFIG_FLAGS_append_armv6 = " -no-neon "

require qt-${PV}.inc

QT_CONFIG_FLAGS += " \
 -exceptions \
"

