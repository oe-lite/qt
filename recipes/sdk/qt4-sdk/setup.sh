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

QT_DIR_NAME="qt"
QT_LIBINFIX=""

export QMAKESPEC="${SYS}usr/share/qt/mkspecs/linux-g++"
export OE_QMAKE_INCDIR_QT="${SYS}usr/include/qt"
export OE_QMAKE_LIBDIR_QT="${SYS}usr/lib"
export OE_QMAKE_LIBS_QT="qt"
export OE_QMAKE_UIC="${TOOL}bin/uic"
export OE_QMAKE_MOC="${TOOL}bin/moc"
export OE_QMAKE_RCC="${TOOL}bin/rcc"

export OE_QMAKE_LINK="${CXX}"
export OE_QMAKE_CC="${CC}"
export OE_QMAKE_CXX="${CXX}"
export OE_QMAKE_AR="${AR}"
export OE_QMAKE_STRIP="${STRIP}"
export OE_QMAKE_QT_CONFIG=${SYS}usr/share/qt/mkspecs/qconfig.pri
