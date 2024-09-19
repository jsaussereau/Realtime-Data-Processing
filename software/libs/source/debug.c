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

#include <stdlib.h>
#include "../include/debug.h"

void debug_printf_c(char  c) {
  *((volatile char*)DEBUG_PRINT_ADDR) = c;
}

void debug_printf_s(char *p) {
  while (*p)
    *((volatile char*)DEBUG_PRINT_ADDR) = *(p++);
}

void debug_printf_x(unsigned long val, unsigned char char_count){
  unsigned char count = 0;
  char result[9]; // for 32 bit input: 8 hex characters + '\0'
  char hex;

  // get every hex characters from lsb to msb
  while (val > 0) {
    hex = val & 0x0000000f;
    if (hex < 10) { // from '0' to '9'
      hex += '0';
    } else { // from 'a' to 'f'
      hex += 'a' - 0xa;
    }
    result[count] = hex;
    count ++;
    val >>= 4; 
  }

  // print zeroes 
  if (char_count > 0) {
    for (unsigned char i = 0 ; i < char_count - count ; i++) {
      debug_printf_c('0');
    }
  }

  // prints hex characters in reverse order: from msb to lsb
  while (count > 0) {
    debug_printf_c(result[count-1]);
    count--; 
  }
}

void debug_printf_d(int val) {
  char buffer[32];
  char *p = buffer;
  if (val < 0) {
    debug_printf_c('-');
    val = -val;
  }
  while (val || p == buffer) {
    *(p++) = '0' + val % 10;
    val = val / 10;
  }
  while (p != buffer)
    debug_printf_c(*(--p));
}

void debug_printf(const char *format, ...) {
  int i;
  va_list ap;

  va_start(ap, format);

  for (i = 0; format[i]; i++)
    if (format[i] == '%') {
      while (format[++i]) {
      if (format[i] == 'c') {
        debug_printf_c(va_arg(ap,int));
        break;
      }
      if (format[i] == 's') {
        debug_printf_s(va_arg(ap,char*));
        break;
      }
      if (format[i] == 'd') {
        debug_printf_d(va_arg(ap,int));
        break;
      }
      if (format[i] == 'x') {
        debug_printf_x(va_arg(ap,int), 8);
        break;
      }
      }
    } else {
      debug_printf_c(format[i]);
    }
  va_end(ap);
}

uint8_t get_init_value() {
  return 37;
}

uint8_t get_test_value() {
  return 42;
}
