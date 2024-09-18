/**********************************************************************\
*                               AsteRISC                               *
************************************************************************
*
* Copyright (C) 2022 Jonathan Saussereau
*
* This file is part of AsteRISC.
* AsteRISC is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* AsteRISC is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with AsteRISC. If not, see <https://www.gnu.org/licenses/>.
*
*/ 


OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

SEARCH_DIR("=/opt/riscv/riscv32-unknown-elf/lib");
SEARCH_DIR("=/opt/riscv/riscv32-unknown-elf/lib32/ilp32d");
SEARCH_DIR("=/opt/riscv/riscv32-unknown-linux-gnu/lib32");
SEARCH_DIR("=/opt/riscv/lib");
SEARCH_DIR("=/opt/riscv/lib/gcc/riscv32-unknown-elf/11.1.0/");
SEARCH_DIR("=/opt/riscv/riscv32-unknown-linux-gnu/lib");
SEARCH_DIR("=/usr/local/lib32/ilp32d");
SEARCH_DIR("=/usr/local/lib32");
SEARCH_DIR("=/usr/local/lib"); 
SEARCH_DIR("=/lib32/ilp32d");
SEARCH_DIR("=/lib");
SEARCH_DIR("=/lib32");
SEARCH_DIR("=/usr/lib32/ilp32d");
SEARCH_DIR("=/usr/local/lib");
SEARCH_DIR("=/usr/lib");
SEARCH_DIR("=/usr/lib32");

SECTIONS
{
  .text           :
  {
    *(.text.unlikely .text.*_unlikely .text.unlikely.*)
    *(.text.exit .text.exit.*)
    *(.text.startup .text.startup.*)
    *(.text.hot .text.hot.*)
    *(.text .stub .text.* .gnu.linkonce.t.*)

    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
  }
  .data           :
  {
    __DATA_BEGIN__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata .srodata.*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
    /* Put .rodata in .text */
    *(.rodata .rodata.* .gnu.linkonce.r.*) 
    *(.rodata1)
  }
  /*/DISCARD/ : { *(.comment) *(.riscv.attributes) }*/
}
