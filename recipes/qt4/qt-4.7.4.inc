LICENSE = "LGPLv2.1 GPLv3"

FILESPATHPKG =. "qt-${PV}:"

SRC_URI = "\
    ftp://ftp.trolltech.com/qt/source/qt-everywhere-opensource-src-${PV}.tar.gz \
    file://0004-no-qmake.patch \
    file://hack-out-pg2-4.7.0.patch \
    file://0006-freetype-host-includes.patch \
    file://Fix-compilation-with-thumb2.patch \
    file://g++.conf \
    file://linux.conf \
    file://qte.sh"
S = "${SRCDIR}/qt-everywhere-opensource-src-${PV}"

do_configure[prefuncs] += "do_configure_smallfix"
do_configure_smallfix() {
    for pro in $(find ${S} -name "*.pro") ; do
        sed -i 's:$$QT_BUILD_TREE/bin/lrelease:${OE_QMAKE_LRELEASE}:g' $pro
    done
    sed -i s:SEDME:${S}: ${SRCDIR}/linux.conf
    sed -i \
    -e /QMAKE_MOC\ /d \
    -e /QMAKE_UIC\ /d \
    -e /QMAKE_UIC3\ /d \
    -e /QMAKE_RCC\ /d \
    ${S}/configure
}

QT_GLFLAGS ?= ""
QT_CONFIG_FLAGS += "-no-rpath -reduce-relocations -silent ${QT_GLFLAGS}"

do_compile() {
    # Fixup missing wsegl header in some SGX SDKs
    if ! [ -e ${STAGE_DIR}/${HOST_TYPE}${includedir}/wsegl.h ] ; then
        cp src/3rdparty/powervr/wsegl.h src/plugins/gfxdrivers/powervr/QWSWSEGL/
    fi
    install -m 0755 ${STAGE_DIR}/native/bin/rcc4 ${S}/bin/rcc
    install -m 0755 ${STAGE_DIR}/native/bin/moc4 ${S}/bin/moc
    install -m 0755 ${STAGE_DIR}/native/bin/uic4 ${S}/bin/uic
    install -m 0755 ${STAGE_DIR}/native/bin/lrelease4 ${S}/bin/lrelease
    oe_runmake ${EXTRA_ENV}
}

do_install[postfuncs] += "do_install_smallfix"
do_install_smallfix() {
	install -d ${D}${bindir}
	for i in rcc uic moc ; do
		install -m 0755 ${S}/bin/$i ${D}${bindir}/
	done
}

FILES_${QT_BASE_NAME}-tools                += "${bindir}/qml"

