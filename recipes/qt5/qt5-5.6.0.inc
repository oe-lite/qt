# Copyright (C) 2012 O.S. Systems Software LTDA.
# Copyright (C) 2013 Martin Jansa <martin.jansa@gmail.com>

LICENSE = "GFDL-1.3 & LGPL-2.1 | GPL-3.0"

SRC_URI += " \
    http://download.qt.io/official_releases/qt/5.6/${PV}/submodules/${QT_MODULE}-opensource-src-${PV}.tar.xz"
S = "${SRCDIR}/${QT_MODULE}-opensource-src-${PV}"
