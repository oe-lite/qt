DESCRIPTION = "GLib is a general-purpose utility library, \
which provides many useful data types, macros, \
type conversions, string utilities, file utilities, a main \
loop abstraction, and so on. It works on many \
UNIX-like platforms, Windows, OS/2 and BeOS."
LICENSE = "LGPLv2+"
SECTION = "libs"
PRIORITY = "optional"
DEPENDS = "glib-2.0-native-utils \
           gtk-doc-dev \
           ${TARGET_ARCH}/sysroot-libpthread  \
           zlib-dev"
DEPENDS_recipe-native = "gettext-native-dev gtk-doc-native-dev \
                            pkg-config-native-dev"


PR = "r2"

SRC_URI = "\
  http://ftp.gnome.org/pub/GNOME/sources/glib/2.24/glib-${PV}.tar.bz2 \
  file://glibconfig-sysdefs.h \
  file://configure-libtool.patch;patch=1 \
  file://g_once_init_enter.patch;patch=1 \
  file://gatomic-proper-pointer-get-cast.patch;patch=1 \
  file://gio.patch;patch=1 \
  file://60_wait-longer-for-threads-to-die.patch;patch=1 \
  file://glib-mkenums-interpreter.patch;patch=1 \
#  file://disable-ipv6.patch \
"

#noipv6 = "${@base_contains('DISTRO_FEATURES', 'ipv6', '', '-DDISABLE_IPV6', d)}"
EXTRA_OEMAKE_append = "'CFLAGS=${CFLAGS}'"
#${noipv6}'"


SRC_URI_append_arm = " file://atomic-thumb.patch"
SRC_URI_append_armv6 = " file://gatomic_armv6.patch"
SRC_URI_append_armv7a = " file://gatomic_armv6.patch"

inherit autotools gettext

S = "${WORKDIR}/glib-${PV}"

EXTRA_OECONF = "--disable-debug"

# Add and entry for your favourite arch if your (g)libc has a sane printf
EXTRA_OECONF_append_glibc_arm = "  --enable-included-printf=no "

do_configure_prepend () {
	install -m 0644 ${WORKDIR}/glibconfig-sysdefs.h .
}

do_install_append() {
	sed -i -e s:${STAGE_DIR}/native/bin:${bindir}:g ${D}${bindir}/glib-mkenums || true
}

EXTRA_OECONF_recipe-native = ""

do_configure_prepend_recipe-native() {
    if [ -e ${S}/${TARGET_SYS}-libtool ] ; then
                echo "${TARGET_SYS}-libtool already present"
    else
        cp ${STAGE_DIR}/native/bin/${TARGET_SYS}-libtool ${S}
    fi

}

BBCLASSEXTEND = "native"

PACKAGES =+ "gobject-2.0${RE} gmodule-2.0${RE} gthread-2.0${RE} gio-2.0${RE} glib-2.0${RE}-utils "
LEAD_SONAME = "libglib-2.0.*"
FILES_glib-2.0-utils = "${bindir}/*"
FILES_${PN} = "${libdir}/lib*so.* ${libdir}/gio/modules/*.so"
FILES_${PN}-dev += "${libdir}/glib-2.0 ${datadir}/glib-2.0 ${libdir}/gio/modules/*.la"
FILES_${PN}-dbg += "${libdir}/gio/modules/.debug"
FILES_gmodule-2.0 = "${libdir}/libgmodule-2.0.so.*"
FILES_gobject-2.0 = "${libdir}/libgobject-2.0.so.*"
FILES_gio-2.0 = "${libdir}/libgio-2.0.so.*"
FILES_gthread-2.0 = "${libdir}/libgthread-2.0.so.*"

# Let various glib components end up in glib package
# for compatibility (with binary packages from Maemo).
FILES_gthread_chinook-compat = ""
FILES_gmodule_chinook-compat = ""
FILES_gobject_chinook-compat = ""
FILES_gio_chinook-compat = ""
