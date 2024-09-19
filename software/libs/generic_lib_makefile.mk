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
RVEXT      = -march=rv32i -mabi=ilp32
CFLAGS     = -Wall -O3 -pedantic $(RVEXT) -DPRINTF_DISABLE_SUPPORT_FLOAT -DPRINTF_DISABLE_SUPPORT_EXPONENTIAL -DPRINTF_DISABLE_SUPPORT_LONG_LONG #-fno-exceptions -fno-asynchronous-unwind-tables -fno-ident 

########################################################
# Toolchain path 
########################################################
RISCVDIR   = /opt/riscv
CC         =$(RISCVDIR)/bin/riscv32-unknown-elf-gcc
AR         =$(RISCVDIR)/bin/riscv32-unknown-elf-ar

########################################################
# Folders
########################################################
INCDIR     = include
SRCDIR     = source
OBJDIR     = rv32i/obj
ARDIR      = rv32i/ar

SOURCES   := $(wildcard $(SRCDIR)/*.c)
OBJECTS   := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ARCHIVES  := $(OBJECTS:$(OBJDIR)/%.o=$(ARDIR)/%.a)

########################################################
# Text Format
########################################################
_BOLD                   =\033[1m
_END                    =\033[0m
_RED                    =\033[31m
_BLUE                   =\033[34m
_CYAN                   =\033[36m
_YELLOW                 =\033[33m
_GREEN                  =\033[32m
_WHITE                  =\033[37m
_GREY                   =\033[90m
_BLACK                  =\033[30m

INIT_LI    =0

########################################################
# Make rules
########################################################
ALL_LIBS   = $(ARDIR)/libasterisc.a $(ARDIR)/libspi_slave.a

.PHONY: all
all: $(ALL_LIBS)

# Libraries:
$(ARDIR)/libasterisc.a: $(SRCDIR)/asterisc.c $(ARDIR)/libdebug.a $(INCDIR)/asterisc.h
	$(call compile_lib,asterisc,)

$(ARDIR)/libdebug.a: $(SRCDIR)/debug.c $(INCDIR)/debug.h
	$(call compile_lib,debug,)

$(ARDIR)/libspi_slave.a: $(SRCDIR)/spi_slave.c $(INCDIR)/spi_slave.h
	$(call compile_lib,spi_slave,$(OBJDIR)/asterisc.o)

# Compile lib$(1).a
define compile_lib
	@if [ $(INIT_LI) = 0 ]; then\
		printf "\n > building lib $(_BOLD)$(1)$(_END)\n";\
	fi
	@$(eval INIT_LI = 1)
	@mkdir -p $(ARDIR)
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $(SRCDIR)/$(1).c -o $(OBJDIR)/$(1).o -I$(SRCDIR)
	@$(AR) rc -o $(ARDIR)/lib$(1).a $(OBJDIR)/$(1).o $(2)
endef
#	@/bin/echo -e " $(_GREEN)done$(_END)"

