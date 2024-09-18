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

.text
.align 2
.globl _start
.type  _start, @function
.org   0

_start:
  addi x1, zero, 0
  addi x2, zero, 0
  addi x3, zero, 0
  addi x4, zero, 0
  addi x5, zero, 0
  addi x6, zero, 0
  addi x7, zero, 0
  addi x8, zero, 0
  addi x9, zero, 0
  addi x10, zero, 0
  addi x11, zero, 0
  addi x12, zero, 0
  addi x13, zero, 0
  addi x14, zero, 0
  addi x15, zero, 0
  addi x16, zero, 0
  addi x17, zero, 0
  addi x18, zero, 0
  addi x19, zero, 0
  addi x20, zero, 0
  addi x21, zero, 0
  addi x22, zero, 0
  addi x23, zero, 0
  addi x24, zero, 0
  addi x25, zero, 0
  addi x26, zero, 0
  addi x27, zero, 0
  addi x28, zero, 0
  addi x29, zero, 0
  addi x30, zero, 0
  addi x31, zero, 0
  lui  a1, 0x10001
  addi sp, a1, -4
  jal  ra, _entry
	ebreak
