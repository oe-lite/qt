LICENSE = "LGPLv2.1 GPLv3"

FILESPATHPKG =. "qt-${PV}:"

SRC_URI += "\
    file://0004-no-qmake.patch \
    file://hack-out-pg2-4.7.0.patch \
    file://0006-freetype-host-includes.patch \
    file://fix-translations.patch \
    file://add_nostrip_for_debug_packages.diff \
    file://target_qmake.patch \
    file://qmake_pri_fixes.patch \
    file://g++.conf \
    file://linux.conf \
    file://qte.sh \
"

SRC_URI:>USE_qt_imx_codecs += "file://0001-Add-support-for-i.MX-codecs-to-phonon.patch"

RECIPE_FLAGS += "qt_imx_codecs" 
RDEPENDS_${PN}:>USE_qt_imx_codecs = " gst-fsl-plugin"

SRC_URI:>USE_qt_xinput2 = " \
file://meego-mtev-add-xinput2-support.patch \
file://qt-everywhere-opensource-src-4.7.2-meego-add_scaling_based_on_screen_size.patch \
file://qt-everywhere-opensource-src-4.7.2-meego-adapt_XInput2.0_code_to_support_multiple_devices.patch "

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

FILES_${PN}-tools                += "${bindir}/qml"
