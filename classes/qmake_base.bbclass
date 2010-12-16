#
# QMake variables for Qt4
#
DEPENDS_prepend = "qt4-tools-native "

export QMAKESPEC = "${STAGE_DIR}/sysroot/usr/share/qt4/mkspecs/${CXX}"
export OE_QMAKE_UIC = "${STAGE_DIR}/native/bin/uic4"
export OE_QMAKE_UIC3 = "${STAGE_DIR}/native/bin/uic34"
export OE_QMAKE_MOC = "${STAGE_DIR}/native/bin/moc4"
export OE_QMAKE_RCC = "${STAGE_DIR}/native/bin/rcc4"
export OE_QMAKE_QDBUSCPP2XML = "${STAGE_DIR}/native/bin/qdbuscpp2xml4"
export OE_QMAKE_QDBUSXML2CPP = "${STAGE_DIR}/native/bin/qdbusxml2cpp4"
export OE_QMAKE_QMAKE = "${STAGE_DIR}/native/bin/qmake2"
export OE_QMAKE_LINK = "${CXX}"
export OE_QMAKE_CXXFLAGS = "${CXXFLAGS}"
export OE_QMAKE_INCDIR_QT = "${STAGE_DIR}/sysroot/usr/include/qt4"
export OE_QMAKE_LIBDIR_QT = "${STAGE_DIR}/sysroot/usr/lib"
export OE_QMAKE_LIBS_QT = "qt"
export OE_QMAKE_LIBS_X11 = "-lXext -lX11 -lm"
export OE_QMAKE_LRELEASE = "${STAGE_DIR}/native/bin/lrelease4"
export OE_QMAKE_LUPDATE = "${STAGE_DIR}/native/bin/lupdate4"


OE_QMAKE_PLATFORM = "${CXX}"
QMAKESPEC = "${QMAKE_MKSPEC_PATH}/${OE_QMAKE_PLATFORM}"

# We override this completely to eliminate the -e normally passed in

export OE_QMAKE_CC="${CC}"
export OE_QMAKE_CFLAGS="${CFLAGS}"
export OE_QMAKE_CXX="${CXX}"
export OE_QMAKE_LDFLAGS="${LDFLAGS}"
export OE_QMAKE_AR="${AR}"
export OE_QMAKE_STRIP="echo"
export OE_QMAKE_RPATH="-Wl,-rpath-link,"

# do not export STRIP to the environment
STRIP[unexport] = "1"

