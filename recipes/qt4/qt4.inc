SUMMARY = "Qt is a versatile cross-platform application framework"
LICENSE = "LGPLv2.1 | GPLv3"
HOMEPAGE = "http://qt-project.org/"

inherit qmake_base
inherit c++ make pkgconfig
require qt4-common.inc
RECIPE_TYPES = "machine sdk"

export OE_QMAKE_INCDIR_QT = "${HOST_SYSROOT}${includedir}/${OE_QMAKE_LIBS_QT}"
export OE_QMAKE_LIBDIR_QT = "${HOST_SYSROOT}${libdir}"
export OE_QMAKE_LIBS_QT = "qt"
export OE_QMAKE_EXTRA_MODULES = "network"

RECIPE_FLAGS += "qt_embedded"

OE_QMAKE_LIBS_X11 = "-lXext -lX11 -lm -lXrender -lXfixes"
OE_QMAKE_LIBS_X11:USE_qt_embedded = ""
export OE_QMAKE_LIBS_X11

QT_DEPENDS = "libx11 libxft \
libxext libxrender libxrandr libxcursor \
freetype \
"
QT_DEPENDS:USE_qt_embedded = "libdirectfb \
"
DEPENDS += "${QT_DEPENDS}"

QT_LIB_RDEPENDS = "libx11 libxrandr libxfixes \
libxrender libxext \
"
QT_LIB_RDEPENDS:USE_qt_embedded = "libdirectfb libts"

QT_SRC_URI = ""
QT_SRC_URI:USE_qt_embedded = ""
SRC_URI += "${QT_SRC_URI}"

AUTO_PACKAGE_LIBS:>USE_opengl += " QtMeeGoGraphicsSystemHelper"
RDEPENDS_${PN}-libqtmeegographicssystemhelper += " \
libqtopengl libqtgui libqtcore libglesv2 \
"

RECIPE_FLAGS += "qt_dbus"
QT_DBUS_CONFIG = ""
QT_DBUS_CONFIG:USE_qt_dbus = " -qdbus"
QT_CONFIG_FLAGS += "${QT_DBUS_CONFIG}"
QT_DBUS_DEPENDS = ""
QT_DBUS_DEPENDS:USE_qt_dbus = "dbus-libdbus dbus"
DEPENDS += "${QT_DBUS_DEPENDS}"

RECIPE_FLAGS += "opengl"
OPENGL = "-no-opengl"
OPENGL:USE_opengl = " -opengl es2 -openvg"
QT_CONFIG_FLAGS += "${OPENGL}"

DEPENDS:>USE_opengl = " libegl libglesv1 libglesv2 libopenvg"
AUTO_PACKAGE_LIBS:>USE_opengl += " QtOpenGL"
RDEPENDS_${PN}-libqtopengl += "libqtgui libqtcore libegl libglesv1 libglesv2"
RDEPENDS_${PN}-libqtgui:>USE_opengl += " libegl libglesv1 libglesv2"

RECIPE_FLAGS += "qt_fontconfig"
require conf/fonts.conf
FONTCONFIG = "-no-fontconfig"
FONTCONFIG:USE_qt_fontconfig = "-fontconfig"
QT_CONFIG_FLAGS += "${FONTCONFIG}"

DEPENDS:>USE_qt_fontconfig = " libfontconfig"
DEPENDS_${PN}-libqtgui:>USE_qt_fontconfig = " libfontconfig"
RDEPENDS_${PN}-libqtgui:>USE_qt_fontconfig += " libfontconfig"
PACKAGES:>USE_qt_fontconfig = " ${PN}-fonts"
FILES_${PN}-fonts = "${fontdir}"
EXTRA_INSTALL:>USE_qt_fontconfig = " do_install_ttffonts"
do_install_ttffonts() {
    install -d -m 0755 ${D}/${fontdir}/ttf
    install -m 0644 lib/fonts/*.ttf ${D}/${fontdir}/ttf/
}

RECIPE_FLAGS += "qt_demos"
DEMOS = "-nomake examples -nomake demos"
DEMOS:USE_qt_demos = "-make tools -make examples -make demos"
QT_CONFIG_FLAGS += "${DEMOS}"
RDEPENDS_${PN}-demos += "${PN}"
PACKAGES:<USE_qt_demos = "${PN}-demos-dbg ${PN}-demos "
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
${bindir}/${OE_QMAKE_LIBS_QT}/demos/*/.debug \
"

RECIPE_FLAGS += "qt_svg"
SVG_CONFIG = "-no-svg"
SVG_CONFIG:USE_qt_svg = "-svg"
QT_CONFIG_FLAGS += "${SVG_CONFIG}"

DEPENDS += "\
    native:qt4-tools_${PV} native:glib \
    libfreetype libjpeg libpng12 libz libssl \
    libgstbase \
    libasound \
    libgio libglib libgthread \
    libm libpthread libdl librt libgcc \
"


#The base:
QT_CONFIG_FLAGS += "-verbose -verbose -v -release -reduce-relocations -shared  \
                    -qt-libjpeg -qt-libpng -system-zlib -stl -glib \
                    -force-pkg-config \
		    -depths all \
                    -prefix '${prefix}/' \
                    -embedded ${QT_ARCH} -arch ${QT_ARCH} \
                    -xplatform ${TARGET_OS}-g++ -platform ${TARGET_OS}-g++ \
                    -fast \
                    -I${STAGE_DIR}/sysroot/usr/include/freetype2 \
"

#Features DISABLED!
QT_CONFIG_FLAGS += "-no-sql-ibase -no-sql-mysql -no-sql-odbc \
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
                    -no-webkit \
                    -nomake docs -nomake translations \
"

#CFLAGS += "-fPIC"

QTE_CONFIG = " \
-no-xinerama -no-xkb \
-no-embedded \
-xrandr \
-x11 \
"
QTE_CONFIG:USE_qt_embedded = "\
-qt-gfx-transformed -plugin-gfx-qvfb -plugin-gfx-vnc -plugin-gfx-directfb \
-qt-mouse-tslib -qt-mouse-pc -qt-mouse-qvfb -qt-mouse-linuxinput \
-qt-kbd-tty \
-qt-kbd-linuxinput \
-DQT_KEYPAD_NAVIGATION \
"

QT_CONFIG_FLAGS += "${QTE_CONFIG} \
-exceptions \
${QT_PATH} \
"

do_install() {
    oe_runmake install INSTALL_ROOT=${D}

    rm ${D}/${bindir}/qmake
    install -d ${D}${libdir}/${OE_QMAKE_LIBS_QT}
    mv ${D}/usr/imports ${D}${libdir}/${OE_QMAKE_LIBS_QT}/

    ln -sf linux-g++ ${D}${datadir}/${OE_QMAKE_LIBS_QT}/mkspecs/${BUILD_CXX}
    cp -f ${SRCDIR}/linux.conf ${D}${datadir}/${OE_QMAKE_LIBS_QT}/mkspecs/common/
    cp -f ${SRCDIR}/g++.conf ${D}${datadir}/${OE_QMAKE_LIBS_QT}/mkspecs/common/g++-unix.conf

}

inherit auto-package-utils

AUTO_PACKAGE_UTILS += "assistant designer lconvert linguist lrelease \
	lupdate makeqpf moc pixeltool qcollectiongenerator qdbus \
	qdbuscpp2xml qdbusviewer qdbusxml2cpp qdoc3 qhelpconverter \
	qhelpgenerator qmlviewer qt3to4 qttracereplay rcc uic \
	xmlpatterns xmlpatternsvalidator"

inherit auto-package-libs
AUTO_PACKAGE_LIBS += "QtNetwork \
QtCLucene \
QtDeclarative \
QtScript \
QtScriptTools \
phonon \
QtSql \
QtGui \
QtDesignerComponents \
QtXml \
QtDesigner \
QtXmlPatterns \
QtCore \
QtDBus \
QtMultimedia \
QtTest \
QtHelp \
"
AUTO_PACKAGE_LIBS_RDEPENDS += "libdl libm libgcc libc libstdc++ libpthread"
AUTO_PACKAGE_LIBS_DEV_DEPENDS += "libdl libm libgcc libc libstdc++ libpthread"
FILES_${PN}-libphonon += "${libdir}/${OE_QMAKE_LIBS_QT}/plugins/phonon_backend"
FILES_${PN}-libphonon-dbg += "${libdir}/${OE_QMAKE_LIBS_QT}/plugins/phonon_backend/.debug"
FILES_${PN}-libqtgui += "${libdir}/${OE_QMAKE_LIBS_QT}/plugins/imageformats"
FILES_${PN}-libqtgui-dbg += "${libdir}/${OE_QMAKE_LIBS_QT}/plugins/imageformats/.debug"

RDEPENDS_${PN}-libqtclucene += "libqtcore"
RDEPENDS_${PN}-libqtdeclarative += "libqtscript libqtsql libqtxmlpatterns libqtgui libqtnetwork libqtcore librt"
DEPENDS_${PN}-libqtdeclarative += "libqtscript libqtsql libqtxmlpatterns libqtgui libqtnetwork libqtcore librt"
RDEPENDS_${PN}-libqtscript += "libqtcore "
RDEPENDS_${PN}-libqtscripttools += "libqtscript libqtgui libqtnetwork libqtcore "
RDEPENDS_${PN}-libphonon += "libqtdbus libqtxml libqtgui libqtnetwork libqtcore"
RDEPENDS_${PN}-libqtsql += "libqtcore"
RDEPENDS_${PN}-libqtgui += "libfreetype libqtnetwork libqtcore libgthread libglib libz ${QT_LIB_RDEPENDS}"
DEPENDS_${PN}-libqtgui += "libfreetype libqtnetwork libqtcore libgthread libglib libz ${QT_LIB_RDEPENDS}"
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
RDEPENDS_${PN} += "${LIBS_AUTO_RPACKAGES}"
FILES_${PN} += "${libdir}/${OE_QMAKE_LIBS_QT}"
PACKAGES =+ "${PN}-mkspecs"
FILES_${PN}-mkspecs = "${datadir}/${OE_QMAKE_LIBS_QT}/"
DEPENDS_${PN}-dev += "${PN}-mkspecs"
RDEPENDS_${PN}-dev += "${PN}-mkspecs"
PACKAGES =+ "${PN}-prl"
FILES_${PN}-prl = "${libdir}/*.prl"

#if fontconfig is used. ttf files are in qt-fonts
FILES_${PN} += "${libdir}/fonts/*" 

QTE_INSTALL = ""
QTE_INSTALL:USE_qt_embedded = ""

do_install[postfuncs] += "${QTE_INSTALL} ${EXTRA_INSTALL}"
do_install_fixsysconfdir() {
	install -d ${D}${sysconfdir}/profile.d/
	install -m 0755 ${SRCDIR}/qte.sh ${D}${sysconfdir}/profile.d/
}

FILES_${PN}:>USE_qt_embedded += " ${sysconfdir}/profile.d/qte.sh"
