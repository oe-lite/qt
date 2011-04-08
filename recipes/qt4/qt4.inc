# Qt4.inc specially written to the the current requirements: 
# At the moment, as much as possible is removed while 
# qt quick still functions on target

DEPENDS += "\
    qt4-tools-native-dev \
    freetype-dev \
    jpeg-dev libpng \
    libz \
    dbus-dev \
    openssl-dev \
    gstreamer \
    glib-dev glib-libglib-dev glib-libgthread-dev glib-native-dev \
    ${HOST_ARCH}/sysroot-libm ${HOST_ARCH}/sysroot-libpthread ${HOST_ARCH}/sysroot-libdl \
    ${HOST_ARCH}/sysroot-librt ${HOST_ARCH}/sysroot-libgcc ${HOST_ARCH}/sysroot-libc ${HOST_ARCH}/sysroot-libstdc++"

RDEPENDS_${PN} += "\
    ${HOST_ARCH}/sysroot-libm ${HOST_ARCH}/sysroot-libpthread ${HOST_ARCH}/sysroot-libdl \
    ${HOST_ARCH}/sysroot-librt ${HOST_ARCH}/sysroot-libgcc ${HOST_ARCH}/sysroot-libc ${HOST_ARCH}/sysroot-libstdc++ \
    libz \
    glib-libgthread glib-libglib \
    freetype"

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
                    -no-opengl"

#No need to make these.
QT_CONFIG_FLAGS += " -nomake examples -nomake demos -nomake docs -nomake translations"

EXTRA_OEMAKE = "-e"

CFLAGS += "-fPIC"
EXTRA_ENV = 'QMAKE="${STAGE_DIR}/native/bin/qmake2 -after \
             INCPATH+=${STAGE_DIR}/sysroot/usr/include/freetype2" \
             QMAKESPEC="${QMAKESPEC}" LINK="${CXX} -ldl" \
             AR="${TARGET_PREFIX}ar cqs" \
             MOC="${STAGE_DIR}/native/bin/moc4" UIC="${STAGE_DIR}/native/bin/uic4" MAKE="make -e"'

export QT_CONF_PATH="${SRCDIR}/qt.conf"

addtask mangle after do_patch before do_configure
do_mangle[dirs] = "${SRCDIR}"
do_mangle () {
    sed -i -e 's#\$\((OE_QMAKE_CXX)\)#${OE_QMAKE_CXX}#' \
        -i -e 's#\$\((OE_QMAKE_CC)\)#${OE_QMAKE_CC}#' \
        -i -e 's#\$\((OE_QMAKE_LINK)\)#${OE_QMAKE_LINK}#' \
        -i -e 's#\$\((OE_QMAKE_QT_CONFIG)\)#${OE_QMAKE_QT_CONFIG}#' \
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
    pwd
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

    ${EXTRA_QMAKE_MUNGE}|| true
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
	
    # These are host binaries, we should only use them in staging
    rm -rf ${D}/${bindir}/qmake

    # fix pkgconfig, libtool and prl files
    sed -i -e s#-L${S}/lib##g \
           -e s#-L${STAGE_DIR}/sysroot/usr/lib##g \
           -e 's#STAGE_DIR}/sysroot/usr/lib#libdir}'#g \ 
           -e s#-L${libdir}##g \
           -e s#'$(OE_QMAKE_LIBS_X11)'#"${OE_QMAKE_LIBS_X11}"#g \
           -e s#" -Wl,-rpath-link,${S}/lib"##g \
           -e s#" -Wl,-rpath-link,${libdir}"##g \
           -e 's#I/usr/include#Iincludedir}#g' \ 
           -e 's#Iin#I${in#g' \
           ${D}${libdir}/*.la ${D}${libdir}/*.prl ${D}${libdir}/pkgconfig/*.pc

    sed -i -e s#" -Wl,-rpath-link,${S}/lib"##g \
            ${D}${datadir}/${QT_DIR_NAME}/mkspecs/common/linux.conf

    # fix pkgconfig files
    sed -i -e s#"moc_location=.*$"#"moc_location=${bindir}/moc4"# \
           -e s#"uic_location=.*$"#"uic_location=${bindir}/uic4"# \
           ${D}${libdir}/pkgconfig/*.pc

	# QT abuses $includedir to point to its headers, which breaks pkgconfig sysroot, so manually fix it up here:
	for pc in ${D}${libdir}/pkgconfig/*.pc ; do
		sed -i -e "s:prefix}include/${QT_BASE_NAME}/$(basename $pc .pc):prefix}/include:" \
		       -e "s,Cflags: ,Cflags: -IP{includedir}/${QT_BASE_NAME}/$(basename $pc .pc) ," \
		       -e 's:IP{:I${:g' $pc
	done

    install -d ${D}/${libdir}/fonts
    touch ${D}/${libdir}/fonts/fontdir
}

FILES_${PN} += "${libdir}/fonts/*"
FILES_${PN} += "${libdir}/qtopia"
