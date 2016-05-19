## Recipe for building the qt5 module QtWebsockets.
##
## Qt Websockets

LICENSE = "LGPL-2.1"

inherit qt5module

DEPENDS += "qtdeclarative"
DEPENDS += "qtsvg qtxmlpatterns libstdc++ glib"
RDEPENDS_${PN} += "libdl libqt5sql libqt5network libqt5core libpthread libstdc++ libm libgcc libc"
PROVIDES_${PN} += "libqt5websockets"
require qt5-${PV}.inc
require qt5.inc
