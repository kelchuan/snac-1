##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
##
## Copyright (C), 2003, Victorian Partnership for Advanced Computing (VPAC) Ltd, 110 Victoria Street, Melbourne, 3053, Australia.
##
## Authors:
##	Stevan M. Quenette, Senior Software Engineer, VPAC. (steve@vpac.org)
##	Patrick D. Sunter, Software Engineer, VPAC. (pds@vpac.org)
##	Luke J. Hodkinson, Computational Engineer, VPAC. (lhodkins@vpac.org)
##	Siew-Ching Tan, Software Engineer, VPAC. (siew@vpac.org)
##	Alan H. Lo, Computational Engineer, VPAC. (alan@vpac.org)
##	Raquibul Hassan, Computational Engineer, VPAC. (raq@vpac.org)
##
##  This library is free software; you can redistribute it and/or
##  modify it under the terms of the GNU Lesser General Public
##  License as published by the Free Software Foundation; either
##  version 2.1 of the License, or (at your option) any later version.
##
##  This library is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
##  Lesser General Public License for more details.
##
##  You should have received a copy of the GNU Lesser General Public
##  License along with this library; if not, write to the Free Software
##  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##
## $Id: makefile 3138 2005-07-29 04:21:55Z AlanLo $
##
##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# obtain defaults for required variables according to system and project location, and then run the build.
ifndef PROJ_ROOT
	PROJ_ROOT=../../..
endif
include ${PROJ_ROOT}/Makefile.system

include Makefile.def

subdirs=${def_sub}

tests = ${def_tst}

checks = ${def_checks}

SRCS = ${def_srcs}
TEST_SUPPORT_SRCS = ${def_test_support_srcs}

PROJ_LIBS = ${def_libs}
EXTERNAL_INCLUDES = -I${INC_DIR}/StGermain -DLIB_DIR=\"${LIB_DIR}\"

EXTERNAL_LIBS = -L${LIB_DIR} -lStGermainBaseExtensibility -lStGermainBaseAutomation -lStGermainBaseContainer -lStGermainBaseIO -lStGermainBaseFoundation ${RPATH_LFLAGS}

packages = MPI XML MATH DL

ifdef HAVE_SOAP
	EXTERNAL_LIBS += -lStGermainRegresstor
	packages += CSOAP NANOHTTP
endif
	
include ${PROJ_ROOT}/Makefile.vmake
