LICENSE = "LGPLv2.1 GPLv3"

FILESPATHPKG =. "qt-${PV}:"

SRC_URI += "\
    file://0004-no-qmake.patch \
    file://hack-out-pg2-4.7.0.patch \
    file://0006-freetype-host-includes.patch \
    file://Fix-compilation-with-thumb2.patch \
    file://g++.conf \
    file://linux.conf \
    file://qte.sh \
"

do_configure[prefuncs] += "do_configure_smallfix"
do_configure_smallfix() {
    sed -i \
    -e /QMAKE_MOC\ /d \
    -e /QMAKE_UIC\ /d \
    -e /QMAKE_UIC3\ /d \
    -e /QMAKE_RCC\ /d \
    ${S}/configure
}

QT_GLFLAGS ?= ""
QT_CONFIG_FLAGS += "-no-rpath -reduce-relocations -silent ${QT_GLFLAGS}"

do_install[postfuncs] += "do_install_smallfix"
do_install_smallfix() {
	install -d ${D}${bindir}
	for i in rcc uic moc ; do
		install -m 0755 ${S}/bin/$i ${D}${bindir}/
	done
}

FILES_${PN}-tools                += "${bindir}/qml"
