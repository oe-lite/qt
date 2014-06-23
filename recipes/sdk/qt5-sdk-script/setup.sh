#!/bin/bash
MACHINE_ARCH=${CROSS_COMPILE::-1}

export AR=${CROSS_COMPILE}ar
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}g++
export STRIP=${CROSS_COMPILE}strip

export TOOL=$(dirname $(which ${CROSS_COMPILE}gcc))"/../"
export SYS=${TOOL}${MACHINE_ARCH}/sysroot/
export PKG_CONFIG_LIBDIR=${SYS}usr/lib/pkgconfig
export PKG_CONFIG_PATH=${SYS}usr/lib/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=${SYS}

export QMAKESPEC="${SYS}usr/share/qt5/mkspecs/linux-oe-g++"
export OE_QMAKE_UIC="${TOOL}bin/uic"
export OE_QMAKE_MOC="${TOOL}bin/moc"
export OE_QMAKE_RCC="${TOOL}bin/rcc"

export OE_QMAKE_LINK="${CXX}"
export OE_QMAKE_CC="${CC}"
export OE_QMAKE_CXX="${CXX}"
export OE_QMAKE_AR="${AR}"
export OE_QMAKE_STRIP="${STRIP}"

echo "[Paths]" > qt.conf
echo "Prefix = /usr" >> qt.conf
echo "Headers = ${SYS}usr/include/qt5" >> qt.conf
echo "Libraries = ${SYS}usr/lib" >> qt.conf
echo "ArchData = ${SYS}usr/lib" >> qt.conf
echo "Data = ${SYS}usr/share" >> qt.conf
echo "Plugins = ${SYS}usr/lib/qt5/plugins" >> qt.conf
echo "HostBinaries = ${TOOL}bin" >> qt.conf
echo "HostData = ${SYS}usr/share/qt5/" >> qt.conf
echo "HostSpec = ${SYS}usr/share/qt5/mkspecs/linux-oe-g++" >> qt.conf
echo "TargetSpec = ${SYS}usr/share/qt5/mkspecs/linux-oe-g++" >> qt.conf
export QT_CONF_PATH="${TOOL}qt.conf"
