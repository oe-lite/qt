DESCRIPTION = "Qt Mobility preview"
HOMEPAGE = "http://qt.nokia.com"
LICENSE = "GPL QPL"


DEPENDS += "\
    tslib-dev libz freetype-dev \
    gstreamer-dev gstreamer-libgstreamer-0.10-dev \
    qt4-embedded qt4-embedded-dev \
    glib-dev glib-libglib-dev glib-libgthread-dev glib-native-dev \
    ${HOST_ARCH}/sysroot-libm ${HOST_ARCH}/sysroot-libpthread ${HOST_ARCH}/sysroot-libdl \
    ${HOST_ARCH}/sysroot-librt ${HOST_ARCH}/sysroot-libgcc ${HOST_ARCH}/sysroot-libc ${HOST_ARCH}/sysroot-libstdc++"

#gst-plugins-base-dev \
SRC_URI = "ftp://ftp.trolltech.com/qt/solutions/qt-mobility-opensource-src-${PV}.tar.gz \
           file://qtmobility_pro.patch \
           file://signal_fixup.patch \
           file://qmake2.patch \
          "
S = "${SRCDIR}/qt-mobility-opensource-src-1.1.0-tp"

inherit qt4e

do_configure_prepend() {
        ./configure -modules multimedia
}
CFLAGS += "-DQT_NO_XVIDEO"
do_install() {
        oe_runmake install INSTALL_ROOT=${D}
}
