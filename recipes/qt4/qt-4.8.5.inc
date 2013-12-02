LICENSE = "LGPLv2.1 GPLv3"

FILESPATHPKG =. "qt-${PV}:"

SRC_URI += "\
    file://0003-webkit2-set-OUTPUT_DIR-value-if-empty.patch \
    file://0004-configure-qmake-is-already-built-in-qt4-tools-native.patch \
    file://0008-qmake.pro-Allow-building-a-separate-qmake-for-the-ta.patch \
    file://0009-qmake-fix-source-file-references-in-qmake.pri.patch \
    file://0010-configure-Hack-to-not-use-the-pg_config-of-the-host-.patch \
    file://0011-freetype-host-includes.patch \
    file://0012-Add-2bpp-support.patch \
    file://0013-configure-add-crossarch-option.patch \
    file://0014-translations-fix-phony-translation-linking-error.patch \
    file://0015-configure-add-nostrip-for-debug-packages.patch \
    file://0016-configure-eval-QMAKE_CXX.patch \
    file://0017-configure-ensure-we-identify-the-compiler-as-g-in-co.patch \
    file://0018-configure-make-pulseaudio-a-configurable-option.patch \
    file://0019-Fixes-for-gcc-4.7.0-particularly-on-qemux86.patch \
    file://0020-webkit-disable-the-fuse-ld-gold-flag.patch \
    file://0022-Fix-drawing-of-0-width-polylines-from-outside-the-de.patch \
    file://0023-QHttpMultiPart-fix-data-corruption-in-readData-metho.patch \
    file://0027-tools.pro-disable-qmeegographicssystemhelper.patch \
    file://g++.conf \
    file://linux.conf "

SRC_URI:>USE_qt_embedded = "file://qte.sh"
SRC_URI:>USE_qt_imx_codecs += "file://0001-Add-support-for-i.MX-codecs-to-phonon.patch"

RECIPE_FLAGS += "qt_imx_codecs" 
RDEPENDS_${PN}:>USE_qt_imx_codecs = " gst-fsl-plugin"

SRC_URI:>USE_qt_xinput2 = " \
    file://meego-mtev-add-xinput2-support.patch \
    file://qt-everywhere-opensource-src-4.7.2-meego-add_scaling_based_on_screen_size.patch \
    file://qt-everywhere-opensource-src-4.7.2-meego-adapt_XInput2.0_code_to_support_multiple_devices.patch"

RECIPE_FLAGS += "qt_xinput2"
DEPENDS:>USE_qt_xinput2 = " libxi inputproto"
RDEPENDS_${PN}-libqtgui:>USE_qt_xinput2 = " libxi"
DEPENDS_${PN}-libqtgui:>USE_qt_xinput2 = " libxi"
QT_CONFIG_FLAGS:>USE_qt_xinput2 += " -xinput2 -xinput -xrender"

do_configure[prefuncs] += "do_configure_smallfix"
do_configure_smallfix() {
    sed -i \
    -e /QMAKE_MOC\ /d \
    -e /QMAKE_UIC\ /d \
    -e /QMAKE_UIC3\ /d \
    -e /QMAKE_RCC\ /d \
    ${S}/configure
}

QT_GLFLAGS ?= ""
QT_CONFIG_FLAGS += "-no-rpath -reduce-relocations -silent ${QT_GLFLAGS}"

do_install[postfuncs] += "do_install_smallfix"
do_install_smallfix() {
    install -d ${D}${bindir}
    for i in rcc uic moc ; do
        install -m 0755 ${S}/bin/$i ${D}${bindir}/
    done
}

FILES_${PN}-tools += "${bindir}/qml"
