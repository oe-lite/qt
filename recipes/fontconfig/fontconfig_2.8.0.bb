DESCRIPTION = "A library for configuring and customizing font access."
SECTION = "libs"
LICENSE = "BSD"
DEPENDS = "expat${RE}-dev freetype${RE}-dev zlib${RE}-dev"

# Work around past breakage in debian.bbclass
RPROVIDES_fontconfig-utils = "libfontconfig-utils"
RCONFLICTS_fontconfig-utils = "libfontconfig-utils"

SRC_URI = "http://fontconfig.org/release/fontconfig-${PV}.tar.gz"

S = "${WORKDIR}/fontconfig-${PV}"

inherit autotools pkgconfig

EXTRA_OECONF = " --disable-docs --with-arch=${HOST_ARCH} --with-cache-dir=/var/lib/fontconfig"
EXTRA_OEMAKE = "FC_LANG=fc-lang FC_GLYPHNAME=fc-glyphname"

export HASDOCBOOK="no"
BUILD_CFLAGS += " -I${STAGING_INCDIR}/freetype2"
PARALLEL_MAKE = ""

do_configure_append () {
        sed -i 's|LDFLAGS =.*|LDFLAGS =|' fc-case/Makefile
        sed -i 's|LDFLAGS =.*|LDFLAGS =|' fc-glyphname/Makefile
        sed -i 's|LDFLAGS =.*|LDFLAGS =|' fc-lang/Makefile
        sed -i 's|LDFLAGS =.*|LDFLAGS =|' fc-arch/Makefile
        sed -i 's|CFLAGS =.*|CFLAGS =${BUILD_CFLAGS}|' fc-case/Makefile
        sed -i 's|CFLAGS =.*|CFLAGS =${BUILD_CFLAGS}|' fc-glyphname/Makefile
        sed -i 's|CFLAGS =.*|CFLAGS =${BUILD_CFLAGS}|' fc-lang/Makefile
        sed -i 's|CFLAGS =.*|CFLAGS =${BUILD_CFLAGS}|' fc-arch/Makefile
        sed -i 's|CPPFLAGS =.*|CPPFLAGS =${BUILD_CPPFLAGS}|' fc-case/Makefile
        sed -i 's|CPPFLAGS =.*|CPPFLAGS =${BUILD_CPPFLAGS}|' fc-glyphname/Makefile
        sed -i 's|CPPFLAGS =.*|CPPFLAGS =${BUILD_CPPFLAGS}|' fc-lang/Makefile
        sed -i 's|CPPFLAGS =.*|CPPFLAGS =${BUILD_CPPFLAGS}|' fc-arch/Makefile
        sed -i 's|CXXFLAGS =.*|CFLAGS =${BUILD_CXXFLAGS}|' fc-case/Makefile
        sed -i 's|CXXFLAGS =.*|CFLAGS =${BUILD_CXXFLAGS}|' fc-glyphname/Makefile
        sed -i 's|CXXFLAGS =.*|CFLAGS =${BUILD_CXXFLAGS}|' fc-lang/Makefile
        sed -i 's|CXXFLAGS =.*|CFLAGS =${BUILD_CXXFLAGS}|' fc-arch/Makefile
}

PACKAGES =+ "fontconfig-utils-dbg fontconfig-utils "

FILES_fontconfig-utils-dbg = "${bindir}/*.dbg"
FILES_fontconfig-utils = "${bindir}/*"

DEBIAN_NOAUTONAME_fontconfig-utils = "1"
RREPLACES_fontconfig-utils = "libfontconfig-utils"

