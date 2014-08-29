## Common file for building qtbase
##
## All qt5 modules build, changes when qtbase changes, so this file contains
## the relevant useflags for qt5.
##
## @useflag qtx11 Enable the x11 platform. Disabled by default.
## @useflag qtdirectfb Enabled the directfb platform. Disabled by default.
## @useflag qttslib Enable tslib as touch input. Disabled by default.
## @useflag qtgstreamer Enable gstreamer. Disabled by default.
## @useflag qtwidgets Enable qtwidgets. Enabled by default.
## @useflag qtwebkit Enable webkit. Disabled by default.
## @useflag qtphonon Enable phonon. Disabled by default.
## @useflag qtdbus Enable dbus. Disabled by default.
## @useflag qtqmldebug Enable qtqmldebug. Disabled by default.
## @useflag qtopengl Use qt opengl. The useflag is default set to -no-opengl,
##          which disables opengl. Other options are _only_ glesv1, glesv2, or
##          desktop.
## @useflag qtopengl_egl_flags If @useflag qtopengl is set, use the flags
##          specified in this useflag. Default set to -lEGL.
## @useflag qtopengl_flags If @useflag qtopengl is set, use the flags
##          specified in this useflag. Default set to -LGLU -lGL.
## @useflag qtopengl_qt_flags If @useflag qtopengl is set, use the flags
##          specified in this useflag. Default set to -lGL.
## @useflag qtopengl_es1_flags If @useflag qtopengl is set, use the flags
##          specified in this useflag. Default set to -lGLES_CM.
## @useflag qtopengl_es2_flags If @useflag qtopengl is set, use the flags
##          specified in this useflag. Default set to -lGLESv2.
## @useflag qtopengl_openvg_flags If @useflag qtopengl is set, use the flags
##          specified in this useflag. Default set to -lOpenVG.
## @useflag qtopengl_hooks_file Set to a file to copy into the mkspecs dir.
##          Is not set by default.
## @useflag qtmtdev Set if you want mtdev support. Is not set by default.

LICENSE = "GFDL-1.3 & LGPL-2.1 & GPL-3.0"

inherit c++ make pkgconfig

RECIPE_FLAGS += "qtx11 qtdirectfb qttslib qtgstreamer \
	qtwidgets qtwebkit qtphonon qtdbus qtqmldebug \
	qtopengl \
	qtopengl_egl_flags qtopengl_flags \
	qtopengl_qt_flags qtopengl_es1_flags \
	qtopengl_es2_flags qtopengl_openvg_flags \
	qtopengl_hooks_file \
	qtmtdev \
	"

require qt5.inc

RECIPE_TYPES = "machine"

DEPENDS:>USE_qtmtdev += " mtdev mtdev-dev"
RDEPENDS_${PN}:>USE_qtmtdev += " mtdev"

##################################
### DEPENDS
##################################

DEPENDS += "native:qmake5_${PV} freetype jpeg libpng zlib openssl fontconfig \
	glib libpthread libm libdl librt libgcc libgio libglib libgthread \
	libstdc++"

##################################
### RDEPENDS
##################################

RDEPENDS_${PN} += "freetype libjpeg libpng libz openssl glib fontconfig-utils \
		libpthread libm libdl librt libgcc libgio libglib libgthread \
		libstdc++"

RDEPENDS_${PN}-dev += "libxml2-dev"

##################################
### CONFIGURE
##################################

QT_CONFIG_FLAGS += " \
    -release \
    -shared -silent -glib -fontconfig -pkg-config \
    \
    -no-pch -no-rpath -no-nis -no-cups \
    -no-sql-ibase -no-sql-mysql -no-sql-psql -no-sql-odbc -plugin-sql-sqlite \
    -no-accessibility -no-sm -hostbindir \
    -system-libjpeg -system-libpng -system-zlib \
    -linuxfb \
    ${QT_WIDGETS} \
    ${QT_PHONON} \
    ${QT_DBUS} \
    ${QT_XCB} \
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

# another exception is that we need to run bin/qmake, because EffectivePaths
# are relative to qmake location
OE_BUILD_BIN_DIR = "${BUILD_SYSROOT}${stage_bindir}"
OE_QMAKE_QMAKE_ORIG = "${OE_BUILD_BIN_DIR}/qmake"
OE_QMAKE_QMAKE = "bin/qmake"

# qtbase is exception, configure script is using our get(X)QEvalMakeConf
# and setBootstrapEvalVariable functions to read it from shell
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
        ln -sf ${STAGE_DIR}/native/bin/moc ${S}/bin/moc
        ln -sf ${STAGE_DIR}/native/bin/rcc ${S}/bin/rcc
        ln -sf ${STAGE_DIR}/native/bin/uic ${S}/bin/uic
        if [ -n "$USE_qtdbus" ]; then
                ln -sf ${STAGE_DIR}/native/bin/qdbuscpp2xml ${S}/bin/qdbuscpp2xml
                ln -sf ${STAGE_DIR}/native/bin/qdbusxml2cpp ${S}/bin/qdbusxml2cpp
        fi
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

	do_configure_qmake5_base
	echo "TARGET = \"realmoc\"" >> ${S}/src/tools/moc/moc.pro
	echo "TARGET = \"realrcc\"" >> ${S}/src/tools/rcc/rcc.pro
	echo "TARGET = \"realuic\"" >> ${S}/src/tools/uic/uic.pro
        if [ -n "$USE_qtdbus" ]; then
                echo "TARGET = \"realqdbuscpp2xml\"" >> ${S}/src/tools/qdbuscpp2xml/qdbuscpp2xml.pro
                echo "TARGET = \"realqdbusxml2cpp\"" >> ${S}/src/tools/qdbusxml2cpp/qdbusxml2cpp.pro
        fi
}

##################################
### Enable-able features
##################################

DEFAULT_USE_qtopengl = ""
addhook parse_qt_opengl_config to post_recipe_parse first after set_useflags before auto_package_utils auto_package_libs
def parse_qt_opengl_config(d):
    qtopengl = d.get("USE_qtopengl")
    if not qtopengl or qtopengl == "-no-opengl":
        d.set("QT_OPENGL_CONFIG_OPTION","-no-opengl")
        d.set("USE_qtopengl","")
    elif qtopengl == "glesv1":
        d.set("QT_OPENGL_DEPENDS","libglesv1 libegl")
        d.set("QT_OPENGL_CONFIG_OPTION","-opengl es -egl")
    elif qtopengl == "glesv2":
        d.set("QT_OPENGL_DEPENDS","libglesv2 libegl")
        d.set("QT_OPENGL_CONFIG_OPTION","-opengl es2 -egl")
    elif qtopengl == "desktop":
        d.set("QT_OPENGL_DEPENDS","libgl")
        d.set("QT_OPENGL_CONFIG_OPTION","-opengl desktop")
    else :
        print "ERROR! Wrong qt opengl option"
        return False
    return True

#Overwritten by above function
QT_OPENGL_DEPENDS = ""
QT_OPENGL_CONFIG_OPTION = ""

DEPENDS += " ${QT_OPENGL_DEPENDS}"
DEPENDS_${PN} += " ${QT_OPENGL_DEPENDS}"
RDEPENDS_${PN} += " ${QT_OPENGL_DEPENDS}"
QT_CONFIG_FLAGS += "${QT_OPENGL_CONFIG_OPTION}"
DEFAULT_USE_qtopengl_egl_flags = "-lEGL"
DEFAULT_USE_qtopengl_flags = "-lGLU -lGL"
DEFAULT_USE_qtopengl_qt_flags = "-lGL"
DEFAULT_USE_qtopengl_es1_flags = "-lGLES_CM -lEGL"
DEFAULT_USE_qtopengl_es2_flags = "-lGLESv2 -lEGL"
DEFAULT_USE_qtopengl_openvg_flags = "-lOpenVG"
DEFAULT_USE_qtopengl_hooks_file = "none"
QT_OPENGL_FIXUP_FUNCS = ""
QT_OPENGL_FIXUP_FUNCS:USE_qtopengl = "do_configure_opengl_libs"
do_compile[prefuncs] =+ "${QT_OPENGL_FIXUP_FUNCS}"
do_compile_opengl_libs () {
    sed -i 's!load(qt_config)!!' ${S}/mkspecs/linux-oe-g++/qmake.conf
    cat >> ${S}/mkspecs/linux-oe-g++/qmake.conf <<EOF
EGLFS_PLATFORM_HOOKS_SOURCES = \$\$PWD/qeglfshooks.cpp
IMX_CFLAGS             = -DLINUX=1
QMAKE_LIBS_EGL         += ${USE_qtopengl_egl_flags}
QMAKE_LIBS_OPENGL_ES1  += ${USE_qtopengl_es1_flags}
QMAKE_LIBS_OPENGL_ES2  += ${USE_qtopengl_es2_flags}
QMAKE_LIBS_OPENVG      += ${USE_qtopengl_openvg_flags}
QMAKE_CFLAGS_RELEASE   += \$\$IMX_CFLAGS
QMAKE_CXXFLAGS_RELEASE += \$\$IMX_CFLAGS
QMAKE_CFLAGS_DEBUG   += \$\$IMX_CFLAGS
QMAKE_CXXFLAGS_DEBUG += \$\$IMX_CFLAGS
QMAKE_CFLAGS_EGL += \$\$IMX_CFLAGS
load(qt_config)

EOF

    if [ "${USE_qtopengl_hooks_file}" != "none" ]; then
        cp ${USE_qtopengl_hooks_file} ${S}/mkspecs/linux-oe-g++/qeglfshooks.cpp
    fi
}

DEPENDS:>USE_qtdirectfb = " directfb-dev libdirectfb"
RDEPENDS_${PN}:>USE_qtdirectfb = " libdirectfb libdirect directfb-internal"

X11_EXTRA_DEPENDS = "libx11 libxft libxcb libxext libxrender libxrandr \
	libxcursor freetype libxdamage libdl libxi \
	libxcb xcb-proto libpcre xinput libxi libstdc++ "
DEPENDS:>USE_qtx11 = " ${X11_EXTRA_DEPENDS}"
DEPENDS_${PN}:>USE_qtx11 = " ${X11_EXTRA_DEPENDS}"
RDEPENDS_${PN}:>USE_qtx11 = " ${X11_EXTRA_DEPENDS}"

QT_XCB = " -no-xcb"
QT_XCB:USE_qtx11 = " -qt-xcb"

DEPENDS:>USE_qttslib = " tslib"
RDEPENDS:>USE_qttslib = " tslib tslib-calibrate tslib-tests"
QT_TSLIB = ""
QT_TSLIB:USE_qttslib = " -tslib"

DEFAULT_USE_qtwidgets = "1"
QT_WIDGETS = "-no-widgets"
QT_WIDGETS:USE_qtwidgets = " -widgets"

QT_PHONON = ""
QT_PHONON:USE_qtphonon = " -phonon"

QT_DBUS = "-no-dbus"
QT_DBUS:USE_qtdbus = " -dbus"
DEPENDS:>USE_qtdbus = " libdbus"
RDEPENDS_${PN}:>USE_qtdbus = " libdbus"

QT_EXAMPLES = "-nomake examples"
QT_EXAMPLES:USE_qtexamples = " -make examples"

QT_QMLDEBUG = " -no-qml-debug"
QT_QMLDEBUG:USE_qtqmldebug = " -qml-debug"

##################################
### Install
##################################

do_install[postfuncs] += "do_install_qtfonts"
do_install_qtfonts() {
    # install fonts manually if they are missing
    if [ ! -d ${D}/${datadir}/fonts ]; then
        cp -aR ${S}/lib/fonts ${D}/${datadir}/
    fi
}

do_install[postfuncs] += "do_install_rm_qmake"
do_install_rm_qmake() {
	rm ${D}${bindir}/qmake
}
##################################
### Packaging
##################################

FILES_${PN} += "${datadir}/fonts/"
