DESCRIPTION = "Qt Widget Extension for Technical Applications"

inherit qt5module
inherit qmake5 c++
inherit auto-package-libs
AUTO_PACKAGE_LIBS = "qwt"

# LGPLv2.1 + some exceptions
LICENSE = "QWTv1.0"

require conf/fetch/sourceforge.conf
SRC_URI = "${SOURCEFORGE_MIRROR}/qwt/qwt-${PV}.tar.bz2;name=qwt \
"

DEPENDS += "libstdc++ qtbase qtsvg-dev"

do_configure[prefuncs] =+ "do_configure_prepend"
do_configure_prepend() {
    sed -e 's/# QWT_CONFIG     += QwtExamples/QWT_CONFIG     += QwtExamples/g' -i qwtconfig.pri
    sed -e 's/^\(QWT_CONFIG\s\+[+]=\s\+QwtDesigner\)/#\1/'  -i qwtconfig.pri
    sed -i -e 's:/usr/local/qwt-$$QWT_VERSION:${D}${prefix}:g' ${S}/*.pri
}

do_install_qmake5_base() {
    oe_runmake -e install
    install -d ${D}${datadir}/doc/${PN}
    mv ${D}${prefix}/doc/* ${D}${datadir}/doc/${PN}/
    rmdir ${D}${prefix}/doc
    cd ${S}/examples
    install -d ${D}/${bindir}
    if [ -d bin/ ]; then
        cd bin/
        for i in * ; do
            cp -pPR $i ${D}/${bindir}/$i
        done
    fi
    if [ -d ${D}${prefix}/plugins ]; then
        install -d ${D}${libdir}
        mv ${D}${prefix}/plugins ${D}${libdir}
    fi
}

PACKAGES =+ "${PN}-examples ${PN}-features ${PN}-plugins "
FILES_${PN}-examples = "${bindir}/*"
FILES_${PN}-features = "${prefix}/features"
FILES_${PN}-plugins = "${OE_QMAKE_PATH_PLUGINS}/designer/*.so"
FILES_${PN}-dbg += "${OE_QMAKE_PATH_PLUGINS}/plugins/designer/.debug"
FILES_${PN}-libqwt-dev += "${includedir}"
DEPENDS_${PN}-libqwt-dev += "${PN}-features"

COM_DEP = "libc libdl libegl libgal libgcc libglesv2 libm libpthread \
	libqt5concurrent libqt5core libqt5gui libqt5opengl libqt5printsupport \
	libqt5svg libqt5widgets libstdc++ \
"
DEPENDS_${PN}-libqwt += "${COM_DEP}"
RDEPENDS_${PN}-libqwt += "${COM_DEP}"
