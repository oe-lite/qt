DEFAULT_PREFERENCE = "-1"

require qt4-tools-native.inc
LICENSE = "LGPLv2.1 GPLv3"

# Find the g++.conf/linux.conf in the right directory.
FILESPATHPKG =. "qt-${PV}:"

EXTRA_OECONF += " -fast -silent -no-rpath"

TOBUILD := "src/tools/bootstrap ${TOBUILD}"
