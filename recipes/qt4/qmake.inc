LICENSE = "LGPLv2.1 | GPLv3"
inherit c++ make

RECIPE_TYPES = "native sdk"

SRC_URI += " \
    file://0001-qlibraryinfo-allow-to-set-qt.conf-from-the-outside-u.patch \
    file://0002-qkbdtty_qws-fix-build-with-old-kernel-headers.patch \
    file://0003-webkit2-set-OUTPUT_DIR-value-if-empty.patch \
    file://0021-configure-make-qt4-native-work-with-long-building-pa.patch"

DEPENDS:>sdk = "libz libgcc libm libpthread libdl librt"
RDEPENDS:>sdk = "libz libgcc libm libpthread libdl librt"

require qt4-common.inc

QT_CONFIG_FLAGS = "\
    -prefix '${prefix}' \
    -bindir ${bindir} \
    -libdir ${libdir} \
    -datadir ${datadir}/qt4 \
    -sysconfdir ${sysconfdir}/qt4 \
    -docdir ${docdir}/qt4 \
    -headerdir ${includedir}/qt4 \
    -plugindir ${libdir}/qt4/plugins \
    -importdir ${libdir}/qt4/imports \
    -translationdir ${datadir}/qt4/translations \
    -examplesdir ${bindir}/qt4/examples \
    -demosdir ${bindir}/qt4/demos \
    -L ${STAGE_DIR}/native/usr/lib \
    -I ${STAGE_DIR}/native/usr/include \
    -qt-libjpeg -system-zlib \
    -no-libjpeg -no-libpng -no-libmng -no-libtiff \
    -no-accessibility \
    -no-cups \
    -no-nas-sound \
    -no-nis -no-openssl \
    -verbose -release \
    -embedded -no-freetype -no-glib -no-iconv \
    -exceptions -xmlpatterns \
    -qt3support \
    -no-fast -silent -no-rpath"

do_configure() {
    :
}

TOBUILD = "\
  src/tools/bootstrap \
  src/tools/moc \
  src/tools/uic \
  src/tools/rcc \
"

do_compile() {
    (echo o; echo yes) | NO_CONF="yes" ./configure ${QT_CONFIG_FLAGS}
    for i in ${TOBUILD}; do
        cd ${S}/$i && oe_runmake CC="${CC}" CXX="${CXX}"
    done
}

do_install() {
    install -d ${D}${bindir}/
    install -m 0755 bin/moc ${D}${bindir}/
    install -m 0755 bin/uic ${D}${bindir}/
    install -m 0755 bin/rcc ${D}${bindir}/
    install -m 0755 bin/qmake ${D}${bindir}/
}

PACKAGES = "${PN}-dbg ${PN}"
FILES_${PN} += "${bindir}/.qmake"
