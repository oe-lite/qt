DESCRIPTION = "Qt Mobility preview"
HOMEPAGE = "http://qt.nokia.com"
LICENSE = "GPL QPL"
#checkup on the licence
inherit qmake_base make c++ pkgconfig

RECIPE_FLAGS += "qt_embedded"
DEPENDS:>USE_qt_embedded = "tslib"
RDEPENDS_${PN}:>USE_QT_embedded = "tslib"

DEPENDS += "\
    libz freetype \
    libgstbase \
    qt \
    qt-dev \
    libxml2 \
    libglib libgthread \
    libm libpthread libdl \
    librt libgcc \
    native:glib native:qt4-tools"

RDEPENDS_${PN} += "freetype \
    gst-plugins-base \
    qt libxml2 \
    glib glib-libglib glib-libgthread \
    libm libpthread libdl \
    librt libgcc libc libstdc++"

QT-MOBILITY_MODULES="multimedia"

export OE_QMAKE_INCDIR_QT = "${STAGE_DIR}/machine/${includedir}/${OE_QMAKE_LIBS_QT}"
export OE_QMAKE_LIBDIR_QT = "${STAGE_DIR}/machine/${libdir}"

QT_CONFIG += "declarative"
do_configure() {
        ./configure -modules ${QT-MOBILITY_MODULES} \
        -prefix "" \
        -plugindir ${libdir}/plugins \
        -bindir ${bindir} \
        -libdir ${libdir} \
        -headerdir ${includedir}/${OE_QMAKE_LIBS_QT}
}
export OE_QMAKE_MOC = "${BUILD_SYSROOT}${stage_bindir}/moc"
CFLAGS += "-DQT_NO_XVIDEO"
do_install() {
        oe_runmake install INSTALL_ROOT=${D}
	mkdir -p ${D}${libdir}/${OE_QMAKE_LIBS_QT}
	mv ${D}${prefix}/imports ${D}${libdir}/${OE_QMAKE_LIBS_QT}
}

FILES_${PN} += "${libdir}/plugins"
FILES_${PN} += "${libdir}/${OE_QMAKE_LIBS_QT}/"
FILES_${PN} += "${libdir}/libQtMultimediaKit.prl"
FILES_${PN} += "${datadir}/${OE_QMAKE_LIBS_QT}"
