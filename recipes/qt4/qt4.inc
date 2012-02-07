# Qt4.inc specially written to the the current requirements: 
# At the moment, as much as possible is removed while 
# qt quick still functions on target

RECIPE_TYPES = "machine sdk"

inherit c++ make pkgconfig
require qt4-common.inc

DEPENDS += "\
    native:qt4-tools native:glib \
    libfreetype libjpeg libpng12 libz libdbus dbus libssl \
    libgstbase \
    libasound \
    libgio libglib libgthread \
    libm libpthread libdl librt libgcc libstdc++"

RDEPENDS_${PN} += "\
    libm libpthread libdl librt libgcc libc libstdc++ \
    libasound \
    libz libgthread libglib libfreetype"

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
                    -nomake examples -nomake demos \
                    -nomake docs -nomake translations \
                    -prefix '${prefix}/' \
                    -embedded ${QT_ARCH} -arch ${QT_ARCH} \
                    -xplatform ${TARGET_OS}-g++ -platform ${TARGET_OS}-g++ \
                    -fast \
                    -I${STAGE_DIR}/sysroot/usr/include/freetype2 \
"

CFLAGS += "-fPIC"
    

do_install() {
    oe_runmake install INSTALL_ROOT=${D}

    rm ${D}/${bindir}/qmake
    install -d ${D}/${libdir}/fonts
    touch ${D}/${libdir}/fonts/fontdir
	mkdir ${D}${libdir}/qt4
	mv ${D}/usr/imports/ ${D}${libdir}/qt4/imports
}

inherit auto-package-libs
AUTO_PACKAGE_LIBS = "QtNetwork \
QtCLucene \
QtDeclarative \
QtScript \
QtScriptTools \
phonon \
QtSql \
QtGui \
QtDesignerComponents \
QtXml \
QtDesigner \
QtXmlPatterns \
QtCore \
QtDBus \
QtMultimedia \
QtTest \
QtHelp \
"
AUTO_PACKAGE_LIBS_RDEPENDS = "libdl libm libgcc libc libstdc++ libpthread"

FILES_${PN}-libphonon += "${libdir}/${QT_DIR_NAME}/plugins/phonon_backend"
FILES_${PN}-libphonon-dev += "${libdir}/${QT_DIR_NAME}/plugins/phonon_backend/.debug"
FILES_${PN}-libqtgui += "${libdir}/${QT_DIR_NAME}/plugins/imageformats"
FILES_${PN}-libqtgui-dev += "${libdir}/${QT_DIR_NAME}/plugins/imageformats/.debug"

RDEPENDS_${PN}-libqtclucene += "libqtcore"
RDEPENDS_${PN}-libqtdeclarative += "libqtscript libqtsql libqtxmlpatterns libqtgui libqtnetwork libqtcore librt"
RDEPENDS_${PN}-libqtscript += "libqtcore "
RDEPENDS_${PN}-libqtscripttools += "libqtscript libqtgui libqtnetwork libqtcore "
RDEPENDS_${PN}-libphonon += "libqtdbus libqtxml libqtgui libqtnetwork libqtcore"
RDEPENDS_${PN}-libqtsql += "libqtcore"
RDEPENDS_${PN}-libqtgui += "libqtnetwork libqtcore libts libgthread libglib libz"
RDEPENDS_${PN}-libqtdesignercomponents += "libqtdesigner"
RDEPENDS_${PN}-libqtxml += "libqtcore"
RDEPENDS_${PN}-libqtdesigner += "libqtscript libqtxml libqtgui libqtnetwork libqtcore"
RDEPENDS_${PN}-libqtxmlpatterns += "libqtnetwork libqtcore"
RDEPENDS_${PN}-libqtcore += "libglib libgthread libz"
RDEPENDS_${PN}-libqtdbus += "libqtxml libqtcore "
RDEPENDS_${PN}-libqtmultimedia += "libqtgui libqtnetwork libqtcore "
RDEPENDS_${PN}-libqttest += "libqtcore"
RDEPENDS_${PN}-libqthelp += "libqtsql libqtxml libqtgui libqtnetwork libqtcore libqtclucene"

RDEPENDS_${PN}-dev += "${LIBS_AUTO_PACKAGES}"
DEPENDS_${PN}-dev += "${LIBS_AUTO_PACKAGES}"
FILES_${PN} += "${libdir}/fonts/*"
FILES_${PN} += "${libdir}/qtopia"
FILES_${PN} += "${libdir}/qt4"
PACKAGES += "${PN}-mkspecs"
FILES_${PN}-mkspecs = "${datadir}/qtopia/"
PACKAGES += "${PN}-prl"
FILES_${PN}-prl = "${libdir}/*.prl"
