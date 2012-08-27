LICENSE = "LGPLv2.1 | GPLv3"
inherit c++

SRC_URI += " \
           file://qt-config.patch \
           file://only-qmake.patch \
"

require qt4-common.inc

RECIPE_TYPES = "native sdk machine"
QT_CONFIG_FLAGS = " \
    -prefix '${prefix}/' -verbose -release \
    -xplatform ${TARGET_OS}-g++ -platform ${TARGET_OS}-g++ \
    -arch ${QT_ARCH} -fast \
"

PACKAGES = "${PN}-dbg ${PN}"
do_configure() {
    :
}

do_compile() {
    (echo o; echo yes) | NO_CONF="yes" ./configure ${QT_CONFIG_FLAGS}
}

do_install() {
    	install -d ${D}${bindir}/
	install -m 0755 bin/qmake ${D}${bindir}/
}
