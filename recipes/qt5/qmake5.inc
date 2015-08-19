DESCRIPTION = "Native version of Qt/[X11|Mac|Embedded]"
LICENSE = "GFDL-1.3 & LGPL-2.1 & GPL-3.0"
DEPENDS = "zlib"

EXTRA_DEPENDS_SDK = "libdl librt"
EXTRA_DEPENDS_SDK:HOST_LIBC_mingw = ""
DEPENDS:>sdk = " libpthread libm ${EXTRA_DEPENDS_SDK} native:qmake5"
RDEPENDS_${PN}:>sdk = " libpthread libm ${EXTRA_DEPENDS_SDK}"
HOMEPAGE = "http://qt-project.org"

QT_MODULE = "qtbase"

inherit c++ make pkgconfig

require qmake5-${PV}.inc

# Ensure that changes to configure task or rdepends on that triggers new
# do_unpack. Solves compile error on rebuild caused by do_configure change.
do_unpack[emit] += "do_stage"

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
    -platform ${BUILD_PLATFORM} \
"
BUILD_PLATFORM = "linux-oe-g++"
BUILD_PLATFORM:HOST_LIBC_mingw = "win32-g++"
EXTRA_OECONF:>HOST_LIBC_mingw += " \
    -xplatform win32-g++ \
    -device-option CROSS_COMPILE=${HOST_PREFIX} \
    -qpa windows"

RECIPE_FLAGS += "qtdbus"
EXTRA_OECONF:>USE_qtdbus = "-dbus"
DEPENDS:>USE_qtdbus = " libdbus"

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

QTTOOLS_SUFFIX = ""
QTTOOLS_SUFFIX:HOST_LIBC_mingw = ".exe"

QTTOOLS_CONFIGURE_PREFUNCS = ""
QTTOOLS_CONFIGURE_PREFUNCS:sdk = "do_configure_qtools_fixup"
do_configure[prefuncs] += "${QTTOOLS_CONFIGURE_PREFUNCS}"

#Qt expects a native qmake in ${SRC}/bin/
do_configure_qtools_fixup() {
    #Use native tools to build instead of using the mingw tools
    ln -sf ${BUILD_SYSROOT}/${stage_bindir}/qmake bin/qmake
    sed -i -e 's/\/bin\/qmake/\/bin\/realqmake/' ${S}/qmake/Makefile.*
    ln -sf ${STAGE_DIR}/native/bin/moc ${S}/bin/moc${QTTOOLS_SUFFIX}
    ln -sf ${STAGE_DIR}/native/bin/rcc ${S}/bin/rcc${QTTOOLS_SUFFIX}
    ln -sf ${STAGE_DIR}/native/bin/uic ${S}/bin/uic${QTTOOLS_SUFFIX}
}

QTTOOLS_CONFIGURE_POSTFUNCS = ""
QTTOOLS_CONFIGURE_POSTFUNCS:sdk = "do_configure_qtools_fixup_post"
do_configure[postfuncs] += "${QTTOOLS_CONFIGURE_POSTFUNCS}"
do_configure_qtools_fixup_post() {
    #Set the build target to something else, so it does not overwrite
    #the native tools
    echo "TARGET = \"realmoc\"" >> ${S}/src/tools/moc/moc.pro
    echo "TARGET = \"realrcc\"" >> ${S}/src/tools/rcc/rcc.pro
    echo "TARGET = \"realuic\"" >> ${S}/src/tools/uic/uic.pro
}

USE_MINGW_SPEC = ""
USE_MINGW_SPEC:HOST_LIBC_mingw = "-spec ${S}/mkspecs/win32-g++"

do_configure[prefuncs] += "do_configure_preqt"
do_configure_preqt() {
    [ -e Makefile ] && oe_runmake -k clean || :
    (echo o; echo yes) | ${S}/configure ${EXTRA_OECONF} || die "Configuring qt failed. EXTRA_OECONF was ${EXTRA_OECONF}"
    bin/qmake ${USE_MINGW_SPEC} ${OE_QMAKE_DEBUG_OUTPUT} ${S} -o Makefile || die "Configuring qt with qmake failed. EXTRA_OECONF was ${EXTRA_OECONF}"
}

do_install() {
    oe_runmake install INSTALL_ROOT=${D}

}

INSTALL_QT_UTILS = ""
INSTALL_QT_UTILS:sdk = "do_install_qt_utils_fixup"
do_install[postfuncs] += "${INSTALL_QT_UTILS}"
do_install_qt_utils_fixup () {
    #Set the correct name of the build utils in sdk
    rm ${D}${bindir}/qmake
    install ${S}/bin/realqmake${QTTOOLS_SUFFIX} ${D}${bindir}/qmake${QTTOOLS_SUFFIX}
    mv ${D}${bindir}/realmoc${QTTOOLS_SUFFIX} ${D}${bindir}/moc${QTTOOLS_SUFFIX}
    mv ${D}${bindir}/realuic${QTTOOLS_SUFFIX} ${D}${bindir}/uic${QTTOOLS_SUFFIX}
    mv ${D}${bindir}/realrcc${QTTOOLS_SUFFIX} ${D}${bindir}/rcc${QTTOOLS_SUFFIX}
}

FILES_${PN} += " ${libdir}"

PACKAGES =+ "${PN}-libs"
DEPENDS_${PN} += "${PN}-libs_${PV}"
RDEPENDS_${PN} += "${PN}-libs_${PV}"
# Make libs hold shared objects only - to be used in the SDK
FILES_${PN}-libs = "${libdir}/*.so*"

PACKAGES =+ "${PN}-cmake"
FILES_${PN}-cmake = "${libdir}/cmake"

PACKAGES =+ "${PN}-plugins"
DEPENDS_${PN} += "${PN}-plugins_${PV}"
RDEPENDS_${PN} += "${PN}-plugins_${PV}"
FILES_${PN}-plugins = "${libdir}/qt5/plugins"

PACKAGES =+ "${PN}-mkspecs"
DEPENDS_${PN} += "${PN}-mkspecs_${PV}"
RDEPENDS_${PN} += "${PN}-mkspecs_${PV}"
FILES_${PN}-mkspecs = "${libdir}/qt5/mkspecs"

PACKAGES =+ " ${PN}-utilities "
FILES_${PN}-utilities = "${bindir}"
DEPENDS_${PN} += "${PN}-utilities_${PV}"
EXTRA_RDEPENDS_UTILITIES = "librt libpthread"
EXTRA_RDEPENDS_UTILITIES:HOST_LIBC_mingw = ""
RDEPENDS_${PN}-utilities = "${EXTRA_RDEPENDS_UTILITIES} ${PN}-libs libgcc libpthread libz libstdc++ libm"
