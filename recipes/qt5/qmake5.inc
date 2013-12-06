DESCRIPTION = "Native version of Qt/[X11|Mac|Embedded]"
DEPENDS = "zlib dbus"

DEPENDS:>sdk = " libdl libpthread librt"
RDEPENDS:>sdk = "libdl libphtread librt"
HOMEPAGE = "http://qt-project.org"

QT_MODULE = "qtbase"

inherit c++ make

require qmake5-5.1.1.inc

SRC_URI += " \
    file://0001-Add-linux-oe-g-platform.patch \
    file://0002-qlibraryinfo-allow-to-set-qt.conf-from-the-outside-u.patch \
    file://0007-configureapp-Prefix-default-LIBDIRS-and-INCDIRS-with.patch \
    file://fix_pkgconfig_and_prl_files_path.patch \
    file://dont_export_system_variables.patch \
    file://0013-Disable-mkv8snapshot.patch \
    file://0014-enables-tslib-device-to-be-read-from-env-variable.patch \
    file://allow_building_examples.patch \
    file://always_build_uic.patch \
"

QT_CONF_PATH = "${B}/qt.conf"

EXTRA_OECONF = " \
    -prefix '${prefix}/' \
    -dont-process \
    -no-gcc-sysroot \
    -system-zlib \
    -no-libjpeg \
    -no-libpng \
    -no-gif \
    -no-accessibility \
    -no-cups \
    -no-nis \
    -no-gui \
    -no-qml-debug \
    -no-sql-mysql \
    -no-sql-sqlite \
    -no-opengl \
    -no-openssl \
    -no-xcb \
    -verbose \
    -release \
    -bindir ${OE_QMAKE_PATH_BINS} \
    -libdir ${OE_QMAKE_PATH_LIBS} \
    -headerdir ${OE_QMAKE_PATH_HEADERS} \
    -archdatadir ${OE_QMAKE_PATH_ARCHDATA} \
    -datadir ${OE_QMAKE_PATH_DATA} \
    -docdir ${OE_QMAKE_PATH_DOCS} \
    -sysconfdir ${OE_QMAKE_PATH_SETTINGS} \
    -no-glib \
    -no-iconv \
    -silent \
    -nomake examples \
    -nomake tests \
    -no-rpath \
    -platform linux-oe-g++ \
"

# qtbase is exception, configure script is using our get(X)QEvalMakeConf and setBootstrapEvalVariable functions to read it from shell
export OE_QMAKE_COMPILER
export OE_QMAKE_CC
export OE_QMAKE_CFLAGS
export OE_QMAKE_CXX
export OE_QMAKE_CXXFLAGS
export OE_QMAKE_LINK
export OE_QMAKE_LDFLAGS
export OE_QMAKE_AR
export OE_QMAKE_STRIP

do_configure[prefuncs] += "do_configure_preqt"
do_configure_preqt() {
    (echo o; echo yes) | ${S}/configure ${EXTRA_OECONF} || die "Configuring qt failed. EXTRA_OECONF was ${EXTRA_OECONF}"
    bin/qmake ${OE_QMAKE_DEBUG_OUTPUT} ${S} -o Makefile || die "Configuring qt with qmake failed. EXTRA_OECONF was ${EXTRA_OECONF}"
}

do_install() {

    oe_runmake install INSTALL_ROOT=${D}

}

FILES_${PN} += " ${libdir}"
