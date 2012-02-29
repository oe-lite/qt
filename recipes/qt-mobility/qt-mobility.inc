DESCRIPTION = "Qt Mobility preview"
HOMEPAGE = "http://qt.nokia.com"
LICENSE = "GPL QPL"
#checkup on the licence
inherit qt4e make c++ pkgconfig

DEPENDS += "\
    tslib libz freetype \
    libgstbase \
    qt4-embedded \
    qt4-embedded-dev \
    libxml2 \
    libglib libgthread \
    libm libpthread libdl \
    librt libgcc \
    native:glib native:qt4-tools"

RDEPENDS_${PN} += "tslib freetype \
    gst-plugins-base \
    qt4-embedded libxml2 \
    glib glib-libglib glib-libgthread \
    libm libpthread libdl \
    librt libgcc libc libstdc++"

QT-MOBILITY_MODULES="multimedia"

export OE_QMAKE_INCDIR_QT = "${STAGE_DIR}/machine/${includedir}/${QT_DIR_NAME}"
export OE_QMAKE_LIBDIR_QT = "${STAGE_DIR}/machine/${libdir}"

QT_CONFIG += "declarative"
do_configure() {
        ./configure -modules ${QT-MOBILITY_MODULES} \
        -prefix "" \
        -plugindir ${libdir}/plugins \
        -bindir ${bindir} \
        -libdir ${libdir} \
        -headerdir ${includedir}/${QT_DIR_NAME}
}
export OE_QMAKE_MOC = "${BUILD_SYSROOT}${stage_bindir}/moc"
CFLAGS += "-DQT_NO_XVIDEO"
do_install() {
        oe_runmake install INSTALL_ROOT=${D}
	mkdir -p ${D}${libdir}/${QT_DIR_NAME}
	mv ${D}${prefix}/imports ${D}${libdir}/${QT_DIR_NAME}
}

FILES_${PN} += "${libdir}/plugins"
FILES_${PN} += "${libdir}/${QT_DIR_NAME}/"
FILES_${PN} += "${libdir}/libQtMultimediaKit.prl"
FILES_${PN} += "${datadir}/${QT_DIR_NAME}"
