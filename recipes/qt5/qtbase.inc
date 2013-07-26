inherit c++ make pkgconfig

RECIPE_FLAGS += "opengl qtx11 qtdirectfb qttslib \
	qtwidgets qtxml qtwebkit qtphonon qtdbus qtexamples qtqmldebug"

require qt5.inc

RECIPE_TYPES = "machine"

SRC_URI += " \
    file://0001-Add-linux-oe-g-platform.patch \
    file://0002-qlibraryinfo-allow-to-set-qt.conf-from-the-outside-u.patch \
    file://0005-qmake-is-already-built-in-qtbase-native.patch \
    file://0006-Allow-building-a-separate-qmake-for-the-target.patch \
    file://0007-configureapp-Prefix-default-LIBDIRS-and-INCDIRS-with.patch \
    file://fix_pkgconfig_and_prl_files_path.patch \
    file://dont_export_system_variables.patch \
    file://0013-Disable-mkv8snapshot.patch \
    file://0014-enables-tslib-device-to-be-read-from-env-variable.patch \
    file://allow_building_examples.patch \
    file://0018-QOpenGLPaintDevice-sub-area-support.patch \
    file://0019-Fix-FBO-restoring-in-QOpenGLTextureGlyphCache.patch \
"

DEPENDS += "native:qmake5 freetype jpeg libpng zlib openssl fontconfig \
	glib libpthread libm libdl librt libgcc libgio libglib libgthread libstdc++ \
	libgstbase gstreamer-dev"
RDEPENDS_${PN} += "freetype libjpeg libpng libz openssl glib fontconfig-utils \
		libpthread libm libdl librt libgcc libgio libglib libgthread libstdc++ \
	libgstbase gstreamer"

DEPENDS:>USE_qtdirectfb = " directfb-dev libdirectfb"
RDEPENDS_${PN}:>USE_qtdirectfb = " libdirectfb libdirect directfb-internal"

X11_EXTRA_DEPENDS = "libx11 libxft libxcb libxext libxrender libxrandr \
	libxcursor freetype libxdamage libdl libgstbase libxi libdrm \
	libxcb xcb-proto libpcre xinput libxi libstdc++ "
DEPENDS:>USE_qtx11 = " ${X11_EXTRA_DEPENDS}"
DEPENDS_${PN}:>USE_qtx11 = " ${X11_EXTRA_DEPENDS}"
RDEPENDS_${PN}:>USE_qtx11 = " ${X11_EXTRA_DEPENDS}"

QT_XCB = " -no-xcb"
QT_XCB:USE_qtx11 = " -qt-xcb"

QT_GLFLAGS = " -no-opengl"
QT_GLFLAGS:USE_opengl = " -opengl es2 -eglfs"

DEPENDS:>USE_qttslib = " tslib"
RDEPENDS:>USE_qttslib = " tslib tslib-calibrate tslib-tests"
QT_TSLIB = ""
QT_TSLIB:USE_qttslib = " -tslib"

DEFAULT_USE_qtwidgets = "1"
QT_WIDGETS = "-no-widgets"
QT_WIDGETS:USE_qtwidgets = " -widgets"

DEFAULT_USE_qtxml = "1"
QT_XML = "-no-xmlpatterns"
QT_XML:USE_qtxml = " "

QT_PHONON = ""
QT_PHONON:USE_qtphonon = " -phonon"

QT_DBUS = "-no-dbus"
QT_DBUS:USE_qtdbus = " -dbus"

DEFAULT_USE_qtexamples = "1"
QT_EXAMPLES = "-nomake examples"
QT_EXAMPLES:USE_qtexamples = " -make examples"

QT_QMLDEBUG = " -no-qml-debug"
QT_QMLDEBUG:USE_qtqmldebug = " -qml-debug"

QT_CONFIG_FLAGS += " \
    -release -reduce-relocations \
    -shared -silent -glib -fontconfig -pkg-config \
    \
    -no-pch -no-rpath -no-nis -no-cups \
    -no-sql-ibase -no-sql-mysql -no-sql-psql -no-sql-odbc -plugin-sql-sqlite \
    -no-accessibility -no-sm -hostbindir ./hest \
    -system-libjpeg -system-libpng -system-zlib \
    -linuxfb \
    ${QT_WIDGETS} \
    ${QT_XML} \
    ${QT_PHONON} \
    ${QT_DBUS} \
    ${QT_XCB} \
    ${QT_GLFLAGS} \
    ${QT_EXAMPLES} \
    ${QT_TSLIB} \
    ${QT_QMLDEBUG} \
"

do_generateqtconfigfile[postfuncs] += "do_generateqtconfigfile_fixup"
do_generateqtconfigfile_fixup() {
    cat >> ${QT_CONF_PATH} <<EOF

[EffectivePaths]
Prefix=..
EOF
}

# Qt uses atomic instructions not supported in thumb mode
ARM_INSTRUCTION_SET = "arm"
# qtbase is exception, we need to use mkspecs from ${S}
QMAKE_MKSPEC_PATH = "${S}/mkspecs/"

# another exception is that we need to run bin/qmake, because EffectivePaths are relative to qmake location
OE_QMAKE_QMAKE_ORIG = "${STAGE_DIR}/native/bin/${QT_DIR_NAME}/qmake"
OE_QMAKE_QMAKE = "bin/qmake"

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

#${OE_QMAKE_PATH_PREFIX} \
do_configure() {
    # we need symlink in path relative to source, because
    # EffectivePaths:Prefix is relative to qmake location
    if [ ! -e ${S}/bin/qmake ]; then
	if [ ! -d ${S}/bin ]; then
		mkdir ${S}/bin
	fi
        ln -sf ${OE_QMAKE_QMAKE_ORIG} ${S}/bin/qmake
        ln -sf ${STAGE_DIR}/native/bin/${QT_DIR_NAME}/moc ${S}/bin/moc
        ln -sf ${STAGE_DIR}/native/bin/${QT_DIR_NAME}/rcc ${S}/bin/rcc
        ln -sf ${STAGE_DIR}/native/bin/${QT_DIR_NAME}/uic ${S}/bin/uic
    fi

    ${S}/configure -v \
        -dont-process \
        -opensource -confirm-license \
        -no-gcc-sysroot \
        -prefix '...' \
        -bindir ${OE_QMAKE_PATH_BINS} \
        -libdir ${OE_QMAKE_PATH_LIBS} \
        -datadir ${OE_QMAKE_PATH_DATA} \
        -sysconfdir ${OE_QMAKE_PATH_SETTINGS} \
        -docdir ${OE_QMAKE_PATH_DOCS} \
        -headerdir ${OE_QMAKE_PATH_HEADERS} \
        -archdatadir ${OE_QMAKE_PATH_ARCHDATA} \
        -libexecdir ${OE_QMAKE_PATH_LIBEXECS} \
        -plugindir ${OE_QMAKE_PATH_PLUGINS} \
        -importdir ${OE_QMAKE_PATH_IMPORTS} \
        -qmldir ${OE_QMAKE_PATH_QML} \
        -translationdir ${OE_QMAKE_PATH_TRANSLATIONS} \
        -testsdir ${OE_QMAKE_PATH_TESTS} \
        -examplesdir ${OE_QMAKE_PATH_EXAMPLES} \
        -platform linux-oe-g++ \
        -xplatform linux-oe-g++ \
        ${QT_CONFIG_FLAGS}

	qmake5_base_do_configure
	echo "TARGET = \"realmoc\"" >> ${S}/src/tools/moc/moc.pro
	echo "TARGET = \"realrcc\"" >> ${S}/src/tools/rcc/rcc.pro
	echo "TARGET = \"realuic\"" >> ${S}/src/tools/uic/uic.pro
}

do_install[postfuncs] += "do_install_qtfonts"
do_install_qtfonts() {
    # install fonts manually if they are missing
    if [ ! -d ${D}/${datadir}/fonts ]; then
        cp -aR ${S}/lib/fonts ${D}/${datadir}/
    fi
}


FILES_${PN} += "${datadir}/fonts/"
