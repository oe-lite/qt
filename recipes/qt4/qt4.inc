## Common file for building qt 4.x for both embedded and x11
##
## Commonalities for qt embedded and x11 is in here.
##
## @useflag qt_embedded Use qt embedded version. Default is set to 1.
## @useflag qt_opengl Use qt opengl. The useflag is default set to -no-opengl,
##          which disables opengl. Other options are _only_ glesv1, glesv2, or
##          desktop.
## @useflag qt_opengl_egl_flags If @useflag qt_opengl is set, use the flags
##          specified in this useflag. Default set to -lEGL.
## @useflag qt_opengl_flags If @useflag qt_opengl is set, use the flags
##          specified in this useflag. Default set to -LGLU -lGL.
## @useflag qt_opengl_qt_flags If @useflag qt_opengl is set, use the flags
##          specified in this useflag. Default set to -lGL.
## @useflag qt_opengl_es1_flags If @useflag qt_opengl is set, use the flags
##          specified in this useflag. Default set to -lGLES_CM.
## @useflag qt_opengl_es2_flags If @useflag qt_opengl is set, use the flags
##          specified in this useflag. Default set to -lGLESv2.
## @useflag qt_opengl_openvg_flags If @useflag qt_opengl is set, use the flags
##          specified in this useflag. Default set to -lOpenVG.
## @useflag qt_dbus Enabled qt dbus. Disabled by default.
## @useflag qt_demos Enables qt examples and demos. Default disabled. Requires
##          @useflag qt_tools to be set to 1.
## @useflag qt_svg Enabled qt svg functionality. Disabled by default.
## @useflag qt_webkit Enable qt webkit. Default disabled.
## @useflag qt_tools Enable qt tools. Default disabled.
## @useflag qt_cursor Enable qt cursor. Default enabled. Requires @useflag
##          qt_tools to be set to 1.
## @useflag qt_gstreamer Enable qt gstreamer. Default disabled.
## @useflag qt_xinput2 Enable xinput2. Default disabled.
## @useflag qt_imx_codecs Enable imx codecs. Default disabled.

SUMMARY = "Qt is a versatile cross-platform application framework"
LICENSE = "LGPLv2.1 | GPLv3"
HOMEPAGE = "http://qt-project.org/"

inherit qmake_base
inherit c++ make pkgconfig
require qt4-common.inc
RECIPE_TYPES = "machine sdk"

PROVIDES_${PN} = "qt"
PROVIDES_${PN}-dev = "qt-dev"

export OE_QMAKE_INCDIR_QT = "${HOST_SYSROOT}${includedir}/${OE_QMAKE_LIBS_QT}"
export OE_QMAKE_LIBDIR_QT = "${HOST_SYSROOT}${libdir}"
export OE_QMAKE_LIBS_QT = "qt"
export OE_QMAKE_EXTRA_MODULES = "network"

RECIPE_FLAGS += "qt_embedded"

OE_QMAKE_LIBS_X11 = "-lXext -lX11 -lm -lXrender -lXfixes"
OE_QMAKE_LIBS_X11:USE_qt_embedded = ""
export OE_QMAKE_LIBS_X11

##################################
### DEPENDS
##################################

QT_DEPENDS = "\
    libx11 libxft \
    libxext libxrender libxrandr libxcursor \
    freetype"
QT_DEPENDS:USE_qt_embedded = "libdirectfb"
DEPENDS += "${QT_DEPENDS}"

DEPENDS += "\
    native:qmake_${PV} native:glib \
    libfreetype libjpeg libpng12 libz libssl \
    libgio libglib libgthread \
    libm libpthread libdl librt libgcc \
    libxml2"

##################################
### RDEPENDS
##################################

QT_LIB_RDEPENDS = "\
    libx11 libxrandr libxfixes \
    libxrender libxext"
QT_LIB_RDEPENDS:USE_qt_embedded = "libdirectfb libts tslib-calibrate"

RDEPENDS_${PN} += "${QT_LIB_RDEPENDS}"

##################################
### CONFIGURE
##################################

QT_CONFIG_FLAGS += "\
    -v -release -reduce-relocations -shared  \
    -qt-libjpeg -qt-libpng -system-zlib -stl -glib \
    -force-pkg-config \
    -depths all \
    -prefix '${prefix}/' \
    -crossarch ${QT_ARCH} \
    -xplatform ${TARGET_OS}-g++ -platform ${TARGET_OS}-g++ \
    -fast \
    -I${STAGE_DIR}/sysroot/usr/include/freetype2"

QT_CONFIG_FLAGS += "\
    -no-sql-ibase -no-sql-mysql -no-sql-odbc \
    -no-sql-psql -no-sql-sqlite -no-sql-sqlite2 \
    -no-pch \
    -no-nas-sound \
    -no-sm \
    -no-nis \
    -no-cups \
    -no-accessibility \
    -no-javascript-jit \
    -no-qt3support \
    -no-audio-backend \
    -nomake docs -nomake translations"

QTE_CONFIG = " \
    -no-xinerama -no-xkb \
    -no-embedded \
    -xrandr \
    -x11"

QTE_CONFIG:USE_qt_embedded = "\
    -qt-gfx-transformed -plugin-gfx-qvfb -plugin-gfx-vnc -plugin-gfx-directfb \
    -qt-mouse-tslib -qt-mouse-pc -qt-mouse-qvfb -qt-mouse-linuxinput \
    -qt-kbd-tty \
    -qt-kbd-linuxinput \
    -DQT_KEYPAD_NAVIGATION \
    -embedded"

QT_CONFIG_FLAGS += "${QTE_CONFIG} \
    -exceptions \
    ${QT_PATH}"

##################################
### Enable-able features
##################################

RECIPE_FLAGS += "qt_opengl"
DEFAULT_USE_qt_opengl = ""
addhook parse_qt_opengl_config to post_recipe_parse first after set_useflags before auto_package_utils auto_package_libs
def parse_qt_opengl_config(d):
    qt_opengl = d.get("USE_qt_opengl")
    if not qt_opengl or qt_opengl == "-no-opengl":
        d.set("QT_OPENGL_CONFIG_OPTION","-no-opengl")
        d.set("USE_qt_opengl","")
    elif qt_opengl == "glesv1":
        d.set("QT_OPENGL_DEPENDS","libglesv1 libegl")
        d.set("QT_OPENGL_CONFIG_OPTION","-opengl es -egl")
    elif qt_opengl == "glesv2":
        d.set("QT_OPENGL_DEPENDS","libglesv2 libegl")
        d.set("QT_OPENGL_CONFIG_OPTION","-opengl es2 -egl")
    elif qt_opengl == "desktop":
        d.set("QT_OPENGL_DEPENDS","libgl")
        d.set("QT_OPENGL_CONFIG_OPTION","-opengl desktop")
    else :
        print "ERROR! Wrong qt opengl option"
        return False
    return True

#Overwritten by above function
QT_OPENGL_DEPENDS = ""
QT_OPENGL_CONFIG_OPTION = ""

RECIPE_FLAGS += "opengl \
    qt_opengl_egl_flags qt_opengl_flags \
    qt_opengl_qt_flags qt_opengl_es1_flags \
    qt_opengl_es2_flags qt_opengl_openvg_flags"
DEPENDS += " ${QT_OPENGL_DEPENDS}"
QT_CONFIG_FLAGS += "${QT_OPENGL_CONFIG_OPTION}"
DEFAULT_USE_qt_opengl_egl_flags = "-lEGL"
DEFAULT_USE_qt_opengl_flags = "-lGLU -lGL"
DEFAULT_USE_qt_opengl_qt_flags = "-lGL"
DEFAULT_USE_qt_opengl_es1_flags = "-lGLES_CM"
DEFAULT_USE_qt_opengl_es2_flags = "-lGLESv2"
DEFAULT_USE_qt_opengl_openvg_flags = "-lOpenVG"
QT_OPENGL_FIXUP_FUNCS = ""
QT_OPENGL_FIXUP_FUNCS:USE_qt_opengl = "do_configure_opengl_libs"
do_configure[prefuncs] =+ "${QT_OPENGL_FIXUP_FUNCS}"
do_configure_opengl_libs () {
    sed -i \
    -e "s:PLACEHOLDER_QMAKE_LIBS_OPENGL_EGL$:${USE_qt_opengl_egl_flags}:g" \
    -e "s:PLACEHOLDER_QMAKE_LIBS_OPENGL_OPENGL$:${USE_qt_opengl_flags}:g" \
    -e "s:PLACEHOLDER_QMAKE_LIBS_OPENGL_QT$:${USE_qt_opengl_qt_flags}:g" \
    -e "s:PLACEHOLDER_QMAKE_LIBS_OPENGL_ES1$:${USE_qt_opengl_es1_flags}:g" \
    -e "s:PLACEHOLDER_QMAKE_LIBS_OPENGL_ES2$:${USE_qt_opengl_es2_flags}:g" \
    -e "s:PLACEHOLDER_QMAKE_LIBS_OPENGL_OPENVG$:${USE_qt_opengl_openvg_flags}:g" \
    ${SRCDIR}/linux.conf
}

RECIPE_FLAGS += "qt_dbus"
QT_DBUS_CONFIG = ""
QT_DBUS_CONFIG:USE_qt_dbus = " -qdbus"
QT_CONFIG_FLAGS += "${QT_DBUS_CONFIG}"
DEPENDS:>USE_qt_dbus += "dbus-libdbus dbus"

RECIPE_FLAGS += "qt_demos"
DEMOS = "-nomake examples -nomake demos"
DEMOS:USE_qt_demos = "\
    -make examples -make demos \
    -examplesdir ${bindir}/${OE_QMAKE_LIBS_QT}/examples \
    -demosdir ${bindir}/${OE_QMAKE_LIBS_QT}/demos"
QT_CONFIG_FLAGS += "${DEMOS}"
RDEPENDS_${PN}-demos += "${PN}"

RECIPE_FLAGS += "qt_svg"
SVG_CONFIG = "-no-svg"
SVG_CONFIG:USE_qt_svg = "-svg"
QT_CONFIG_FLAGS += "${SVG_CONFIG}"

RECIPE_FLAGS += "qt_webkit"
WEBKIT_CONFIG = "-no-webkit"
WEBKIT_CONFIG:USE_qt_webkit = ""
QT_CONFIG_FLAGS += "${WEBKIT_CONFIG}"

RECIPE_FLAGS += "qt_alsa"
DEPENDS:>USE_qt_alsa += " libasound"
RDEPENDS_${PN}:>USE_qt_alsa += " libasound"

RECIPE_FLAGS += "qt_tools"
TOOLS_CONFIG = "-nomake tools"
TOOLS_CONFIG:USE_qt_tools = "-make tools"
QT_CONFIG_FLAGS += "${TOOLS_CONFIG}"

RECIPE_FLAGS += "qt_cursor"
CURSOR_CONFIG = "-no-feature-CURSOR"
CURSOR_CONFIG:USE_qt_cursor = ""
DEFAULT_USE_qt_cursor = "1"
QT_CONFIG_FLAGS += "${TOOLS_CONFIG}"

RECIPE_FLAGS += "qt_gstreamer"
GSTREAMER_CONFIG = "-no-gstreamer"
GSTREAMER_CONFIG:USE_qt_gstreamer = ""
QT_CONFIG_FLAGS += "${GSTREAMER_CONFIG}"
DEPENDS:>USE_qt_gstreamer += " gstreamer libgstbase"
RDEPENDS_${PN}:>USE_qt_gstreamer += " gstreamer libgstbase"

##################################
### Install
##################################

do_install() {
    oe_runmake install INSTALL_ROOT=${D}

    rm ${D}/${bindir}/qmake
    install -d ${D}${libdir}/${OE_QMAKE_LIBS_QT}
    mv ${D}/usr/imports ${D}${libdir}/${OE_QMAKE_LIBS_QT}/

    ln -sf linux-g++ ${D}${datadir}/${OE_QMAKE_LIBS_QT}/mkspecs/${BUILD_CXX}
    cp -f ${SRCDIR}/linux.conf ${D}${datadir}/${OE_QMAKE_LIBS_QT}/mkspecs/common/
    cp -f ${SRCDIR}/g++.conf ${D}${datadir}/${OE_QMAKE_LIBS_QT}/mkspecs/common/g++-unix.conf
}

QTE_INSTALL = ""
QTE_INSTALL:USE_qt_embedded = "do_install_fixsysconfdir"
EXTRA_INSTALL ?= ""

do_install[postfuncs] += "${QTE_INSTALL} ${EXTRA_INSTALL}"
do_install_fixsysconfdir() {
    install -d ${D}${sysconfdir}/profile.d/
    install -m 0755 ${SRCDIR}/qte.sh ${D}${sysconfdir}/profile.d/
}

##################################
### Packaging
##################################

inherit auto-package-utils

AUTO_PACKAGE_UTILS += "\
    assistant designer lconvert linguist lrelease \
    lupdate makeqpf moc pixeltool qcollectiongenerator qdbus \
    qdbuscpp2xml qdbusviewer qdbusxml2cpp qdoc3 qhelpconverter \
    qhelpgenerator qmlviewer qt3to4 qttracereplay rcc uic \
    xmlpatterns xmlpatternsvalidator"

inherit auto-package-libs
AUTO_PACKAGE_LIBS += "\
    QtNetwork QtCLucene QtDeclarative \
    QtScript QtScriptTools phonon \
    QtSql QtGui QtDesignerComponents \
    QtXml QtDesigner QtXmlPatterns \
    QtCore QtDBus QtMultimedia \
    QtTest QtHelp"
AUTO_PACKAGE_LIBS:>USE_opengl += " QtOpenGL QtMeeGoGraphicsSystemHelper"

AUTO_PACKAGE_LIBS_RDEPENDS += "libdl libm libgcc libc libstdc++ libpthread"
AUTO_PACKAGE_LIBS_DEV_DEPENDS += "libdl libm libgcc libc libstdc++ libpthread"

PACKAGES =+ "${PN}-mkspecs ${PN}-prl"
PACKAGES:<USE_qt_demos = "${PN}-demos-dbg ${PN}-demos "

PROVIDES_${PN}-fonts += "qt-fonts"
PROVIDES_${PN}-demos = "qt-demos"
PROVIDES_${PN}-demos-dbg = "qt-demos-dbg"

RDEPENDS_${PN}-libqtclucene += "libqtcore"
RDEPENDS_${PN}-libqtdeclarative += "libqtscript libqtsql libqtxmlpatterns libqtgui libqtnetwork libqtcore librt"
DEPENDS_${PN}-libqtdeclarative += "libqtscript libqtsql libqtxmlpatterns libqtgui libqtnetwork libqtcore librt"
RDEPENDS_${PN}-libqtscript += "libqtcore "
RDEPENDS_${PN}-libqtscripttools += "libqtscript libqtgui libqtnetwork libqtcore "
RDEPENDS_${PN}-libphonon += "libqtdbus libqtxml libqtgui libqtnetwork libqtcore"
RDEPENDS_${PN}-libqtsql += "libqtcore"
RDEPENDS_${PN}-libqtgui += "libfreetype libqtnetwork libqtcore libgthread libglib libz"
DEPENDS_${PN}-libqtgui += "libfreetype libqtnetwork libqtcore libgthread libglib libz"
RDEPENDS_${PN}-libqtdesignercomponents += "libqtdesigner"
RDEPENDS_${PN}-libqtxml += "libqtcore"
RDEPENDS_${PN}-libqtdesigner += "libqtscript libqtxml libqtgui libqtnetwork libqtcore"
RDEPENDS_${PN}-libqtxmlpatterns += "libqtnetwork libqtcore"
RDEPENDS_${PN}-libqtcore += "libglib libgthread libz"
DEPENDS_${PN}-libqtcore += "libglib libgthread libz"
RDEPENDS_${PN}-libqtdbus += "libqtxml libqtcore "
RDEPENDS_${PN}-libqtmultimedia += "libqtgui libqtnetwork libqtcore "
RDEPENDS_${PN}-libqttest += "libqtcore"
RDEPENDS_${PN}-libqthelp += "libqtsql libqtxml libqtgui libqtnetwork libqtcore libqtclucene"
RDEPENDS_${PN}-libqtopengl += "libqtgui libqtcore libfreetype libxrandr libxfixes libegl libglesv2 libxrender libxext libx11"
DEPENDS_${PN}-libqtopengl += "libqtgui libqtcore libfreetype libxrandr libxfixes libegl libglesv2 libxrender libxext libx11"
RDEPENDS_${PN}-dev += "${LIBS_AUTO_PACKAGES}"
DEPENDS_${PN}-dev += "${LIBS_AUTO_PACKAGES}"
RDEPENDS_${PN} += "${LIBS_AUTO_RPACKAGES} libxml2 libgmodule libgobject"
DEPENDS_${PN}-dev += "${PN}-mkspecs"
RDEPENDS_${PN}-dev += "${PN}-mkspecs"
RDEPENDS_${PN}-libqtopengl += "libqtgui libqtcore libegl libglesv1 libglesv2"
RDEPENDS_${PN}-libqtgui:>USE_opengl += " libegl libglesv1 libglesv2"
RDEPENDS_${PN}-libqtmeegographicssystemhelper += " \
    libqtopengl libqtgui libqtcore libglesv2"

FILES_${PN}-libphonon += "${libdir}/${OE_QMAKE_LIBS_QT}/plugins/phonon_backend"
FILES_${PN}-libphonon-dbg += "${libdir}/${OE_QMAKE_LIBS_QT}/plugins/phonon_backend/.debug"
FILES_${PN}-libqtgui += "${libdir}/${OE_QMAKE_LIBS_QT}/plugins/imageformats"
FILES_${PN}-libqtgui-dbg += "${libdir}/${OE_QMAKE_LIBS_QT}/plugins/imageformats/.debug"
FILES_${PN}:>USE_qt_embedded += " ${sysconfdir}/profile.d/qte.sh"
FILES_${PN} += "${libdir}/fonts/*" 
FILES_${PN} += "${libdir}/${OE_QMAKE_LIBS_QT}"
FILES_${PN}-mkspecs = "${datadir}/${OE_QMAKE_LIBS_QT}/"
FILES_${PN}-prl = "${libdir}/*.prl"
FILES_${PN}-demos = "${bindir}/${OE_QMAKE_LIBS_QT}/examples \
${bindir}/${OE_QMAKE_LIBS_QT}/demos"
FILES_${PN}-demos-dbg = " \
    ${bindir}/${OE_QMAKE_LIBS_QT}/examples/*/*/*/*/.debug \
    ${bindir}/${OE_QMAKE_LIBS_QT}/demos/*/*/*/*/.debug \
    ${bindir}/${OE_QMAKE_LIBS_QT}/examples/*/*/*/.debug \
    ${bindir}/${OE_QMAKE_LIBS_QT}/demos/*/*/*/.debug \
    ${bindir}/${OE_QMAKE_LIBS_QT}/examples/*/*/.debug \
    ${bindir}/${OE_QMAKE_LIBS_QT}/demos/*/*/.debug \
    ${bindir}/${OE_QMAKE_LIBS_QT}/examples/*/.debug \
    ${bindir}/${OE_QMAKE_LIBS_QT}/demos/*/.debug"
