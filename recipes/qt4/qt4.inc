# Qt4.inc specially written to the the current requirements: 
# At the moment, as much as possible is removed while 
# qt quick still functions on target

RECIPE_TYPES = "machine sdk"

inherit c++ make pkgconfig

DEPENDS += "\
    native:qt4-tools native:glib \
    freetype jpeg libpng12 libz dbus-dev dbus-libdbus-dev openssl gstreamer \
 gst-plugins-base \
    glib-dev glib-libglib glib-libgthread \
    libm libpthread libdl librt libgcc libc libstdc++"

RDEPENDS_${PN} += "\
    libm libpthread libdl librt libgcc libc libstdc++ \
    libz glib-libgthread glib-libglib freetype"

#The base:
QT_CONFIG_FLAGS += "-release -reduce-relocations -shared -qt-gif \
                    -qt-libjpeg -qt-libpng -system-zlib -qdbus -stl -glib"

#Features DISABLED!
QT_CONFIG_FLAGS += "-no-sql-ibase -no-sql-mysql -no-sql-odbc -no-sql-psql -no-sql-sqlite -no-sql-sqlite2 \
                    -no-pch \
                    -no-nas-sound \
                    -no-sm \
                    -no-nis \
                    -no-cups \
                    -no-accessibility \
                    -no-fontconfig \
                    -no-svg \
                    -no-javascript-jit \
                    -no-qt3support \
                    -no-audio-backend \
                    -no-largefile \
                    -no-opengl \
                    -no-webkit \
                    -nomake examples -nomake demos -nomake docs -nomake translations"

CFLAGS += "-fPIC"
EXTRA_ENV = 'QMAKE="${STAGE_DIR}/native/bin/qmake2 -after \
             INCPATH+=${STAGE_DIR}/machine/usr/include/freetype2" \
             QMAKESPEC="${QMAKESPEC}" LINK="${CXX} -ldl" \
             AR="${TARGET_PREFIX}ar cqs" \
             MOC="${STAGE_DIR}/native/bin/moc4" UIC="${STAGE_DIR}/native/bin/uic4" MAKE="make"'

export QT_CONF_PATH="${SRCDIR}/qt.conf"

addtask mangle after do_patch before do_configure
do_mangle[dirs] = "${SRCDIR}"
do_mangle () {
    sed -i -e 's#\$\((OE_QMAKE_CXX)\)#${OE_QMAKE_CXX}#' \
        -i -e 's#\$\((OE_QMAKE_CC)\)#${OE_QMAKE_CC}#' \
        -i -e 's#\$\((OE_QMAKE_LINK)\)#${OE_QMAKE_LINK}#' \
        -i -e 's#\$\((OE_QMAKE_QT_CONFIG)\)#\$\{OE_QMAKE_QT_CONFIG\}#' \
        -i -e 's#\$\((OE_QMAKE_LDFLAGS)\)#${OE_QMAKE_LDFLAGS}#' \
        -i -e 's#\$\((OE_QMAKE_CFLAGS)\)#${OE_QMAKE_CFLAGS}#' \
        -i -e 's#\$\((OE_QMAKE_CXXFLAGS)\)#${OE_QMAKE_CXXFLAGS}#' \
        g++.conf
}

do_configure() {
    if [ ! -e bin/qmake ]; then
        ln -sf ${STAGE_DIR}/native/bin/qmake2 bin/qmake
    else
        die "Rebuilding is NOT supported"
    fi
    
    if [ ! -e mkspecs/${TARGET_OS}-g++ ]; then
        ln -sf linux-g++ mkspecs/${TARGET_OS}-g++
    fi

    cp -f ${SRCDIR}/g++.conf ${SRCDIR}/linux.conf mkspecs/common/

    echo "[Paths]"                                 > $QT_CONF_PATH
    echo "Prefix=${prefix}/"                      >> $QT_CONF_PATH
    echo "Documentation=${docdir}/${QT_DIR_NAME}" >> $QT_CONF_PATH
    echo "Headers=${includedir}/${QT_DIR_NAME}"   >> $QT_CONF_PATH
    echo "Libraries=${libdir}"                    >> $QT_CONF_PATH
    echo "Binaries=${bindir}"                     >> $QT_CONF_PATH
    echo "Plugins=${libdir}/${QT_DIR_NAME}/plugins" >> $QT_CONF_PATH
    echo "Data=${datadir}/${QT_DIR_NAME}"         >> $QT_CONF_PATH
    echo "Translations=${datadir}/${QT_DIR_NAME}/translations" >> $QT_CONF_PATH
    echo "Settings=${sysconfdir}/${QT_DIR_NAME}"  >> $QT_CONF_PATH
    echo "Examples=${bindir}/${QT_DIR_NAME}/examples" >> $QT_CONF_PATH
    echo "Demos=${bindir}/${QT_DIR_NAME}/demos"   >> $QT_CONF_PATH

    export PKG_CONFIG="pkg-config"
    (echo o; echo yes) | ./configure -v \
            -prefix ${prefix}/ \
            -bindir ${bindir} \
            -libdir ${libdir} \
            -datadir ${datadir}/${QT_DIR_NAME} \
            -sysconfdir ${sysconfdir}/${QT_DIR_NAME} \
            -docdir ${docdir}/${QT_DIR_NAME} \
            -headerdir ${includedir}/${QT_DIR_NAME} \
            -plugindir ${libdir}/${QT_DIR_NAME}/plugins \
            -embedded ${HOST_CPU} -arch ${HOST_CPU} \
            -xplatform ${TARGET_OS}-g++ -platform ${TARGET_OS}-g++ \
            ${QT_CONFIG_FLAGS} -fast \
            -I${STAGE_DIR}/sysroot/usr/include/freetype2 
}

do_install() {
    oe_runmake install INSTALL_ROOT=${D}
	
    install -d ${D}/${libdir}/fonts
    touch ${D}/${libdir}/fonts/fontdir
	mkdir ${D}${libdir}/qt4
	mv ${D}/usr/imports/ ${D}${libdir}/qt4/imports
}

FILES_${PN} += "${libdir}/fonts/*"
FILES_${PN} += "${libdir}/qtopia"
FILES_${PN} += "${libdir}/qt4"
PACKAGES += "${PN}-mkspecs"
FILES_${PN}-mkspecs = "${datadir}/qtopia/"
PACKAGES += "${PN}-prl"
FILES_${PN}-prl = "${libdir}/*.prl"

