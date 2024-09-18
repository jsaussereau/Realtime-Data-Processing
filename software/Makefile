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

.PHONY : all
all: libs comparison

.PHONY : init
all: libs

.PHONY: libs
libs:
	@+$(MAKE) -C $@ --no-print-directory

.PHONY: comparison
comparison: generic_makefile.mk
	@+$(MAKE) -C $@ --no-print-directory

.PHONY: complete_msg
complete_msg: generic_makefile.mk
	@/bin/echo -e "\n$(_GREEN)build complete!$(_END)"

                                         

