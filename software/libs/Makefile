#**********************************************************************#
#                               AsteRISC                               #
#**********************************************************************#
#
# Copyright (C) 2022 Jonathan Saussereau
#
# This file is part of AsteRISC.
# AsteRISC is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# AsteRISC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with AsteRISC. If not, see <https://www.gnu.org/licenses/>.
#

########################################################
# Build settings
########################################################
CFLAGSI    = -Wall -O3 -pedantic -march=rv32i -mabi=ilp32 #-fno-exceptions -fno-asynchronous-unwind-tables -fno-ident 
CFLAGSE    = -Wall -O3 -pedantic -march=rv32e -mabi=ilp32e #-fno-exceptions -fno-asynchronous-unwind-tables -fno-ident 

########################################################
# Folders
########################################################
OBJDIR_I   = rv32i/obj
ARDIR_I    = rv32i/ar
OBJDIR_E   = rv32e/obj
ARDIR_E    = rv32e/ar

.PHONY : all
all: 
	@+$(MAKE) -s -f ./generic_lib_makefile.mk CFLAGS="$(CFLAGSI)" OBJDIR=$(OBJDIR_I) ARDIR=$(ARDIR_I) --no-print-directory
	@+$(MAKE) -s -f ./generic_lib_makefile.mk CFLAGS="$(CFLAGSE)" OBJDIR=$(OBJDIR_E) ARDIR=$(ARDIR_E) --no-print-directory
