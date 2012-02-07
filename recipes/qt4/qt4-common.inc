# Supported archs in src/corelib/arch/*

SRC_URI =+ "http://get.qt.nokia.com/qt/source/qt-everywhere-opensource-src-${PV}.tar.gz"
S = "${SRCDIR}/qt-everywhere-opensource-src-${PV}"


CONFIGURE_PREFUNCS = "do_configure_mkspecs"
do_configure[prefuncs] += "${CONFIGURE_PREFUNCS}"

do_configure_mkspecs() {
    if [ ! -e mkspecs/${TARGET_OS}-g++ ]; then
        ln -sf linux-g++ mkspecs/${TARGET_OS}-g++
    fi

    if [ -f ${SRCDIR}/g++.conf ] && [ -f ${SRCDIR}/linux.conf ]; then
        install_conf "${SRCDIR}/g++.conf" \
            "mkspecs/common/g++.conf"
        cp -vf ${SRCDIR}/linux.conf mkspecs/common/
    fi
}

do_configure() {

    if [ ! -e bin/qmake ]; then
        ln -sf ${BUILD_SYSROOT}/${stage_bindir}/qmake bin/qmake
    fi

    (echo o; echo yes) | ./configure ${QT_CONFIG_FLAGS} || die "Configuring qt failed. cmdl was ${QT_CONFIG_FLAGS}"

}


#Map Arch to QT Arch
QT_ARCHS = "alpha arm armv6 avr32 bfin generic i386 ia64 mips parisc powerpc \
s390 sh sh4a sparc x86_64 \
"
QT_ARCHS[nohash] = "1"

QT_ARCH = "${@map_qt_arch(d, 'HOST_ARCH')}"
QT_ARCH[import] = "map_qt_arch"

def map_qt_arch(d, arch):
    import bb, re
    arch = d.get(arch)
    valid_archs = d.get("QT_ARCHS")

    (arch,march) = re.split('-', arch)[0:2]

    if re.match("i.86$", arch):
        return "i386"
    elif re.match("arm", arch):
        if re.match("cortex|1136", march):
            return "armv6"
        else:
            return "arm"
    elif arch == "x86_64":
        return arch
    elif arch == "mipsel":
        return "mips"

    if arch in valid_archs:                 return arch
    else:
        bb.error("cannot map '%s' to a qt architecture" % arch)
