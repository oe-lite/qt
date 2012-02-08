DESCRIPTION = "Native and sdk  tools for Qt/[X11|Mac|Embedded] version 4.x"
HOMEPAGE = "http://qt.nokia.com"
LICENSE = "LGPLv2.1 | GPLv3"

RECIPE_TYPES = "native sdk"

DEPENDS += "libz libgcc libm libpthread libdl librt libdbus" 
DEPENDS:>sdk = " native:qt4-tools"

inherit c++ make qmake_base
require qt4-common.inc

SRC_URI += " \
           file://g++.conf \
           file://linux.conf \
           file://0004-no-qmake.patch \
"

SRC_URI:>sdk = " \
           file://configure-lflags.patch \
           file://compile.test-lflags.patch \
           file://configure-paths.patch \
           file://qt-dbus-const.patch \
"

QT_CONFIG_FLAGS = " -prefix '${prefix}/' \
                -L${TARGET_SYSROOT}/${libdir} \
                -I${TARGET_SYSROOT}/${includedir} \
                -qt-libjpeg -system-zlib \
                -no-libjpeg -no-libpng -no-libmng -no-libtiff \
                -no-gif -no-svg \
                -no-accessibility \
                -no-cups \
                -no-exceptions  \
                -no-nas-sound \
                -no-nis -no-openssl \
                -v -release \
                -embedded -no-freetype -no-glib -no-iconv \
                -fast \
                -force-pkg-config \
                -no-qt3support \
"

QT_CONFIG_FLAGS += " \
             -xplatform ${TARGET_OS}-g++ -static -platform ${TARGET_OS}-g++ \
             -arch ${QT_ARCH} -no-rpath \
"





TOBUILD = "\
  src/tools/bootstrap \
  src/tools/moc \
  src/corelib \
  src/xml \
  src/sql \
  src/dbus \
  src/tools/uic \
  src/tools/rcc \
  src/network \
  src/gui \
  tools/linguist/lrelease \
  tools/linguist/lupdate \
  tools/qdbus \
"

do_compile() {
    for i in ${TOBUILD}; do
        cd ${S}/$i && oe_runmake
    done
}

PACKAGES += "${PN}-fonts"
FILES_${PN}-fonts = "${libdir}/fonts"

PACKAGES += "${PN}-prl"
FILES_${PN}-prl = "${libdir}/*.prl"


do_install() {
    for i in ${TOBUILD}; do
        cd ${S}/$i && oe_runmake install INSTALL_ROOT="${D}"
    done
    cd ${S}

    rm ${S}/bin/qmake
    install -d ${D}${datadir}/qt4/
    cp -PfR mkspecs ${D}${datadir}/qt4/
    ln -sf linux-g++ ${D}${datadir}/qt4/mkspecs/${BUILD_CXX}
    cp -f ${SRCDIR}/g++.conf ${SRCDIR}/linux.conf ${D}${datadir}/qt4/mkspecs/common/

}

FILES_${PN} += "${datadir}"
RDEPENDS_${PN} += "libm libgcc libstdc++ libz libdbus libdl librt libpthread libglib"
