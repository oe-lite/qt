# Copyright (C) 2012 O.S. Systems Software LTDA.
# Copyright (C) 2013 Martin Jansa <martin.jansa@gmail.com>

LICENSE = "GFDL-1.3 & LGPL-2.1 | GPL-3.0"

QT_VERSION ?= "${PV}"

SRC_URI += " \
    http://download.qt.io/archive/qt/5.3/5.3.1/submodules/${QT_MODULE}-opensource-src-${QT_VERSION}.tar.xz"
S = "${SRCDIR}/${QT_MODULE}-opensource-src-${QT_VERSION}"
