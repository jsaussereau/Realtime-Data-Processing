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

#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdarg.h>
#include <stdint.h>

#define DEBUG_PRINT_ADDR 0x0A000000

void debug_printf_c(char c);

void debug_printf_s(char *p);

void debug_printf_d(int val);

void debug_printf_x(unsigned long val, unsigned char char_count);

void debug_printf(const char *format, ...);

uint8_t get_init_value();

uint8_t get_test_value();

#endif
