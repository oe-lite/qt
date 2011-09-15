DESCRIPTION = "Qt Mobility preview"
HOMEPAGE = "http://qt.nokia.com"
LICENSE = "GPL QPL"
#checkup on the licence
inherit qt4e make c++ pkgconfig

DEPENDS += "\
    tslib libz freetype \
    gstreamer-dev gstreamer-libgstreamer-0.10 \
    gstreamer-libgstbase-0.10 gstreamer-libgstbase-0.10 \
    gst-plugin-audio-0.10 gst-plugin-interfaces-0.10 \
    gst-plugin-video-0.10 gst-plugin-app-0.10 \
    gst-plugins-base-dev \
    qt4-embedded-dev \
    libxml2-dev \
    glib glib-libglib glib-libgthread \
    libm libpthread libdl \
    librt libgcc libc libstdc++ \
    native:glib-dev native:qt4-tools"

RDEPENDS_${PN} += "tslib freetype \
    gstreamer gstreamer-libgstreamer-0.10 \
    gstreamer-libgstbase-0.10 gst-plugin-audio-0.10 \
    gst-plugin-interfaces-0.10 gst-plugin-alsa \
    gst-plugin-video-0.10 gst-plugin-app-0.10 \
    gst-plugins-base \
    qt4-embedded libxml2-dev \
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
        -qmake-exec qmake2 \
        -plugindir ${libdir}/plugins \
        -bindir ${bindir} \
        -libdir ${libdir} \
        -headerdir ${includedir}/${QT_DIR_NAME}
}

CFLAGS += "-DQT_NO_XVIDEO"
do_install() {
        oe_runmake install INSTALL_ROOT=${D}
	    mkdir ${D}${libdir}/qt4
    	mv ${D}/usr/local/Trolltech/QtEmbedded-4.7.2/imports/ ${D}${libdir}/qt4/imports
    	mv ${D}/usr/local/Trolltech/QtEmbedded-4.7.2/mkspecs ${D}${libdir}/qt4/mkspecs
}

FILES_${PN} += "${libdir}/plugins"
FILES_${PN} += "${libdir}/qt4/"
FILES_${PN} += "${libdir}/libQtMultimediaKit.prl"
