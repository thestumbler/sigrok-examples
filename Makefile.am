##
## This file is part of the sigrok-cli project.
##
## Copyright (C) 2013 Bert Vermeulen <bert@biot.com>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

ACLOCAL_AMFLAGS = -I m4
GNUMAKEFLAGS = --no-print-directory

AM_CFLAGS = $(SC_EXTRA_CFLAGS) $(SC_WFLAGS) $(SIGROK_EXAMPLES_CFLAGS)

bin_PROGRAMS = enumports wrpoem rdpoem

enumports_SOURCES = \
	enumports.c

wrpoem_SOURCES = \
	wrpoem.c

rdpoem_SOURCES = \
	rdpoem.c

enumports_LDADD = $(SIGROK_EXAMPLES_LIBS)

wrpoem_LDADD = $(SIGROK_EXAMPLES_LIBS)

rdpoem_LDADD = $(SIGROK_EXAMPLES_LIBS)

## dist_man_MANS = doc/examples.1

## desktopdir = @datadir@/applications
## desktop_DATA = contrib/org.sigrok.examples.desktop

## hicolor_icondir = @datadir@/icons/hicolor/scalable/apps
## hicolor_icon_DATA = contrib/sigrok-cli.svg

## EXTRA_DIST = \
## 	contrib/sigrok-logo-notext.ico \
## 	contrib/org.sigrok.sigrok-cli.desktop \
## 	contrib/sigrok-cli.svg

MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog
ChangeLog:
	git --git-dir '$(top_srcdir)/.git' log >$@ || touch $@

dist-hook: ChangeLog

