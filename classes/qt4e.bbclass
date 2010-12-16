DEPENDS_prepend = "${@["qt4-embedded ", ""][(bb.data.getVar('PN', d, 1)[:12] == 'qt4-embedded')]}"
inherit qmake_base

QT_DIR_NAME = "qtopia"
QT_LIBINFIX = "E"
# override variables set by qmake-base to compile Qt/Embedded apps
#
export QMAKESPEC = "${STAGE_DIR}/sysroot/usr/share/${QT_DIR_NAME}/mkspecs/${CXX}"
export OE_QMAKE_INCDIR_QT = "${STAGE_DIR}/sysroot/usr/include/${QT_DIR_NAME}"
export OE_QMAKE_LIBDIR_QT = "${STAGE_DIR}/sysroot/usr/lib"
export OE_QMAKE_LIBS_QT = "qt"
export OE_QMAKE_LIBS_X11 = ""
export OE_QMAKE_EXTRA_MODULES = "network"
EXTRA_QMAKEVARS_PRE += " QT_LIBINFIX=${QT_LIBINFIX} "

# Qt4 uses atomic instructions not supported in thumb mode
ARM_INSTRUCTION_SET = "arm"
