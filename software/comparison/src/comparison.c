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

#include <stdint.h>

#include <asterisc.h>
#include <debug.h>

#include "crc6.h"

#define START_ADDR 0
#define STOP_ADDR 64

#define ADDR_RANGE (STOP_ADDR-START_ADDR)

#define PRECISION 8

uint8_t i = 0;

uint8_t bytes[STOP_ADDR];



/*****************************
      Measure variables       
*****************************/

volatile uint32_t start_cycles_ref;
volatile uint32_t stop_cycles_ref;
volatile uint32_t start_cycles_a;
volatile uint32_t stop_cycles_a;

volatile uint32_t start_addr = START_ADDR;
volatile uint32_t stop_addr = STOP_ADDR;

#define MEASURE_OVERHEAD(start_cycles, start_instr, offset_cycles, offset_instr) \
  volatile uint32_t start_cycles = mcycle(); \
  volatile uint32_t start_instr = minstret(); \
  volatile uint32_t stop_instr = minstret(); \
  volatile uint32_t stop_cycles = mcycle(); \
  uint32_t offset_cycles = stop_cycles - start_cycles; \
  uint32_t offset_instr = stop_instr - start_instr;

#define MEASURE_BLOCK(start_cycles, start_instr, offset_cycles, offset_instr, cycles, instr, code_block) \
  start_cycles = mcycle(); \
  start_instr = minstret(); \
  code_block; \
  stop_instr = minstret(); \
  stop_cycles = mcycle(); \
  cycles = stop_cycles - start_cycles - offset_cycles; \
  instr = stop_instr - start_instr - offset_instr;

/*****************************
       Read CSR values       
*****************************/

static inline uint32_t minstret() {
	uint32_t insns;
  __asm__ volatile (
		"csrr    %0, minstret" 
		: "=r" (insns) 
	);
	return insns;
}

static inline uint32_t mcycle() {
	long cycles;
  	__asm__ volatile (
		"csrr    %0, mcycle" 
		: "=r" (cycles)
	);
	return cycles;
}


/*****************************
        SPI functions        
*****************************/

uint8_t extremely_bad_spi_get_byte() {
  while(spi_status_rx_bits->fifo_empty);
  return *spi_data_rx;
}

uint8_t very_bad_spi_get_byte() {
  while(*spi_status_rx_fifo_empty);
  return *spi_data_rx;
}

static inline uint8_t bad_spi_get_byte() {
  while(*spi_status_rx_fifo_empty);
  return *spi_data_rx;
}

static inline void spi_get_byte(uint8_t *byte) {
  while(*spi_status_rx_fifo_empty);
  asm volatile("lw %0, 0(%1)" : "=r"(*byte) : "r"(spi_data_rx));
}

static inline void spi_get_byte_x4(uint32_t *bytes) {
  while(*spi_status_rx_fifo_empty_x4);
  asm volatile("lw %0, 0(%1)" : "=r"(*bytes) : "r"(spi_data_rx_x4));
}


/*****************************
        CRC functions        
*****************************/

static inline uint8_t bad_crc6(uint8_t init_val, uint8_t in_byte) {
  uint8_t out;
  asm volatile("crc6 %0, %1, %2" : "=r"(out) : "r"(init_val), "r"(in_byte));
  return out;
}

static inline void crc6(uint32_t *dest, uint32_t init_val, uint8_t in_byte) {
  asm volatile("crc6 %0, %1, %2" : "=r"(*dest) : "r"(init_val), "r"(in_byte));
}

static inline void crc6_x4(uint32_t *dest, uint32_t init_val, uint32_t in_bytes) {
  asm volatile("crc6 %0, %1, %2" : "=r"(*dest) : "r"(init_val), "r"(in_bytes));
}


/*****************************
             Main     
*****************************/

int _entry() {

  uint8_t crc_init;
  crc_init = get_init_value();
  uint8_t crc_data;
  crc_data = get_test_value();


  /*****************************
         Mesure overhead     
  *****************************/

  MEASURE_OVERHEAD(start_cycles, start_instr, offset_cycles, offset_instr);

  volatile uint32_t cycles_loop;
  volatile uint32_t instr_loop;
  volatile uint32_t start = 0;
  volatile uint32_t stop = 1;
  MEASURE_BLOCK(start_cycles, start_instr, offset_cycles, offset_instr, cycles_loop, instr_loop, {
    for (uint16_t i = start; i < stop; i++) {
    }
  });


  /*****************************
        CRC implementations
  *****************************/

  uint32_t crc_calc;
  uint32_t crc_tab;
  uint32_t crc_instr;
  uint32_t crc_instr_inline;

  volatile uint32_t cycles_crc_calc;
  volatile uint32_t instr_crc_calc;
  MEASURE_BLOCK(start_cycles, start_instr, offset_cycles, offset_instr, cycles_crc_calc, instr_crc_calc, {
    crc_calc = crc6_calc(crc_init, crc_data);
  });

  volatile uint32_t cycles_crc_tab;
  volatile uint32_t instr_crc_tab;
  MEASURE_BLOCK(start_cycles, start_instr, offset_cycles, offset_instr, cycles_crc_tab, instr_crc_tab, {
    crc_tab = crc6_tab(crc_init, crc_data);
  });

  volatile uint32_t cycles_crc_instr;
  volatile uint32_t instr_crc_instr;
  MEASURE_BLOCK(start_cycles, start_instr, offset_cycles, offset_instr, cycles_crc_instr, instr_crc_instr, {
    crc_instr = crc6_instr(crc_init, crc_data);
  });

  volatile uint32_t cycles_crc_instr_inline;
  volatile uint32_t instr_crc_instr_inline;
  MEASURE_BLOCK(start_cycles, start_instr, offset_cycles, offset_instr, cycles_crc_instr_inline, instr_crc_instr_inline, {
    crc6(&crc_instr_inline, crc_init, crc_data);
  });

  uint8_t crc_8bits = get_init_value();
  uint32_t crc_32bits = get_init_value();


  /*****************************
   Data process implementations
  *****************************/

  // Ref: Intuitive reference implementation
  volatile uint32_t cycles_process_ref;
  volatile uint32_t instr_process_ref;
  volatile uint32_t start_instr_ref;
  MEASURE_BLOCK(start_cycles, start_instr_ref, offset_cycles, offset_instr, cycles_process_ref, instr_process_ref, {
    for (uint16_t i = start_addr; i < stop_addr; i++) {
      uint8_t byte = extremely_bad_spi_get_byte();
      bytes[i] = byte;
      crc_8bits = crc6_instr(crc_8bits, byte);
    }
  });

  // A: Improved memory mapping -> removing masking instructions
  volatile uint32_t cycles_process_a;
  volatile uint32_t instr_process_a;
  volatile uint32_t start_instr_a;
  MEASURE_BLOCK(start_cycles, start_instr_a, offset_cycles, offset_instr, cycles_process_a, instr_process_a, {
    for (uint16_t i = start_addr; i < stop_addr; i++) {
      uint8_t byte = very_bad_spi_get_byte();
      bytes[i] = byte;
      crc_8bits = crc6_instr(crc_8bits, byte);
    }
  });

  // B1: Inline function calls -> removing jump penalties
  volatile uint32_t cycles_process_b1;
  volatile uint32_t instr_process_b1;
  volatile uint32_t start_instr_b1;
  MEASURE_BLOCK(start_cycles, start_instr_b1, offset_cycles, offset_instr, cycles_process_b1, instr_process_b1, {
    for (uint16_t i = start_addr; i < stop_addr; i++) {
      uint8_t byte = bad_spi_get_byte();
      bytes[i] = byte;
      crc_8bits = bad_crc6(crc_8bits, crc_data);
    }
  });

  // B2: Removing redundant instructions
  volatile uint32_t cycles_process_b2;
  volatile uint32_t instr_process_b2;
  volatile uint32_t start_instr_b2;
  MEASURE_BLOCK(start_cycles, start_instr_b2, offset_cycles, offset_instr, cycles_process_b2, instr_process_b2, {
    for (uint16_t i = start_addr; i < stop_addr; i++) {
      uint8_t byte;
      spi_get_byte(&byte);
      bytes[i] = byte;
      crc6(&crc_32bits, crc_32bits, crc_data);
    }
  });

  // C0: Intuitive loop unrolling x4
  volatile uint32_t cycles_process_loop_unrolling_x4;
  volatile uint32_t instr_process_loop_unrolling_x4;
  volatile uint32_t start_instr_loop_unrolling_x4;
  MEASURE_BLOCK(start_cycles, start_instr_loop_unrolling_x4, offset_cycles, offset_instr, cycles_process_loop_unrolling_x4, instr_process_loop_unrolling_x4, {
    uint16_t i = start_addr;
    while (i <= stop_addr - 4) {
      uint8_t byte0;
      spi_get_byte(&byte0);
      bytes[i] = byte0;
      crc6(&crc_32bits, crc_32bits, byte0);

      uint8_t byte1;
      spi_get_byte(&byte1);
      bytes[i+1] = byte1;
      crc6(&crc_32bits, crc_32bits, byte1);

      uint8_t byte2;
      spi_get_byte(&byte2);
      bytes[i+2] = byte2;
      crc6(&crc_32bits, crc_32bits, byte2);
      
      uint8_t byte3;
      spi_get_byte(&byte3);
      bytes[i+3] = byte3;
      crc6(&crc_32bits, crc_32bits, byte3);

      i+= 4;
    }
  });

  // C1: Better loop unrolling x4 -> limiting data dependency
  volatile uint32_t cycles_process_loop_unrolling_2_x4;
  volatile uint32_t instr_process_loop_unrolling_2_x4;
  volatile uint32_t start_instr_loop_unrolling_2_x4;
  MEASURE_BLOCK(start_cycles, start_instr_loop_unrolling_2_x4, offset_cycles, offset_instr, cycles_process_loop_unrolling_2_x4, instr_process_loop_unrolling_2_x4, {
    uint16_t i = start_addr;
    while (i <= stop_addr - 4) {
      uint8_t byte0;
      uint8_t byte1;
      uint8_t byte2;
      uint8_t byte3;

      spi_get_byte(&byte0);
      spi_get_byte(&byte1);
      spi_get_byte(&byte2);
      spi_get_byte(&byte3);

      bytes[i  ] = byte0;
      bytes[i+1] = byte1;
      bytes[i+2] = byte2;
      bytes[i+3] = byte3;

      crc6(&crc_32bits, crc_32bits, byte0);
      crc6(&crc_32bits, crc_32bits, byte1);
      crc6(&crc_32bits, crc_32bits, byte2);
      crc6(&crc_32bits, crc_32bits, byte3);

      i+= 4;
    }
  });

  // C2: Better loop unrolling x8 -> limiting data dependency
  volatile uint32_t cycles_process_loop_unrolling_x8;
  volatile uint32_t instr_process_loop_unrolling_x8;
  volatile uint32_t start_instr_loop_unrolling_x8;
  MEASURE_BLOCK(start_cycles, start_instr_loop_unrolling_x8, offset_cycles, offset_instr, cycles_process_loop_unrolling_x8, instr_process_loop_unrolling_x8, {
    uint16_t i = start_addr;
    while (i <= stop_addr - 8) {
      uint8_t byte0;
      uint8_t byte1;
      uint8_t byte2;
      uint8_t byte3;
      uint8_t byte4;
      uint8_t byte5;
      uint8_t byte6;
      uint8_t byte7;

      spi_get_byte(&byte0);
      spi_get_byte(&byte1);
      spi_get_byte(&byte2);
      spi_get_byte(&byte3);
      spi_get_byte(&byte4);
      spi_get_byte(&byte5);
      spi_get_byte(&byte6);
      spi_get_byte(&byte7);

      bytes[i  ] = byte0;
      bytes[i+1] = byte1;
      bytes[i+2] = byte2;
      bytes[i+3] = byte3;
      bytes[i+4] = byte4;
      bytes[i+5] = byte5;
      bytes[i+6] = byte6;
      bytes[i+7] = byte7;

      crc6(&crc_32bits, crc_32bits, byte0);
      crc6(&crc_32bits, crc_32bits, byte1);
      crc6(&crc_32bits, crc_32bits, byte2);
      crc6(&crc_32bits, crc_32bits, byte3);
      crc6(&crc_32bits, crc_32bits, byte4);
      crc6(&crc_32bits, crc_32bits, byte5);
      crc6(&crc_32bits, crc_32bits, byte6);
      crc6(&crc_32bits, crc_32bits, byte7);

      i+= 8;
    }
  });

  // D1: SIMD x4 spi_get_byte and memory store
  volatile uint32_t cycles_process_simd_light_x4;
  volatile uint32_t instr_process_simd_light_x4;
  volatile uint32_t start_instr_simd_light_x4;
  MEASURE_BLOCK(start_cycles, start_instr_simd_light_x4, offset_cycles, offset_instr, cycles_process_simd_light_x4, instr_process_simd_light_x4, {
    uint16_t i = start_addr;
    while (i <= stop_addr - 4) {
      uint32_t bytes_x4;
      spi_get_byte_x4(&bytes_x4);

      uint32_t *dest = (uint32_t *) &(bytes[i]);
      *dest = bytes_x4;

      crc6(&crc_32bits, crc_32bits, bytes_x4);
      crc6(&crc_32bits, crc_32bits, bytes_x4 >> 8);
      crc6(&crc_32bits, crc_32bits, bytes_x4 >> 16);
      crc6(&crc_32bits, crc_32bits, bytes_x4 >> 24);

      i += 4;
    }
  });

  // D2: SIMD x8 spi_get_byte and memory store
  volatile uint32_t cycles_process_simd_light_x8;
  volatile uint32_t instr_process_simd_light_x8;
  volatile uint32_t start_instr_simd_light_x8;
  MEASURE_BLOCK(start_cycles, start_instr_simd_light_x8, offset_cycles, offset_instr, cycles_process_simd_light_x8, instr_process_simd_light_x8, {
    uint16_t i = start_addr;
    while (i <= stop_addr - 8) {
      uint32_t bytes_x4_0;
      spi_get_byte_x4(&bytes_x4_0);
      uint32_t bytes_x4_1;
      spi_get_byte_x4(&bytes_x4_1);

      uint32_t *dest_0 = (uint32_t *) &(bytes[i]);
      *dest_0 = bytes_x4_0;
      uint32_t *dest_1 = (uint32_t *) &(bytes[i+4]);
      *dest_1 = bytes_x4_1;

      crc6(&crc_32bits, crc_32bits, bytes_x4_0);
      crc6(&crc_32bits, crc_32bits, bytes_x4_0 >> 8);
      crc6(&crc_32bits, crc_32bits, bytes_x4_0 >> 16);
      crc6(&crc_32bits, crc_32bits, bytes_x4_0 >> 24);
      crc6(&crc_32bits, crc_32bits, bytes_x4_1);
      crc6(&crc_32bits, crc_32bits, bytes_x4_1 >> 8);
      crc6(&crc_32bits, crc_32bits, bytes_x4_1 >> 16);
      crc6(&crc_32bits, crc_32bits, bytes_x4_1 >> 24);

      i += 8;
    }
  });

  // F1: SIMD x4 spi_get_byte, memory store and CRC
  volatile uint32_t cycles_process_simd_full_x4;
  volatile uint32_t instr_process_simd_full_x4;
  volatile uint32_t start_instr_simd_full_x4;
  MEASURE_BLOCK(start_cycles, start_instr_simd_full_x4, offset_cycles, offset_instr, cycles_process_simd_full_x4, instr_process_simd_full_x4, {
    uint16_t i = start_addr;
    while (i <= stop_addr - 4) {
      uint32_t bytes_x4;
      spi_get_byte_x4(&bytes_x4);

      uint32_t *dest = (uint32_t *) &(bytes[i]);

      *dest = bytes_x4;

      crc6_x4(&crc_32bits, crc_32bits, bytes_x4);
    
      i += 4;
    }
  });

  // F2: SIMD x8 spi_get_byte, memory store and CRC
  volatile uint32_t cycles_process_simd_full_x8;
  volatile uint32_t instr_process_simd_full_x8;
  volatile uint32_t start_instr_simd_full_x8;
  MEASURE_BLOCK(start_cycles, start_instr_simd_full_x8, offset_cycles, offset_instr, cycles_process_simd_full_x8, instr_process_simd_full_x8, {
    uint16_t i = start_addr;
    while (i <= stop_addr - 8) {
      uint32_t bytes_x4_0;
      spi_get_byte_x4(&bytes_x4_0);
      uint32_t bytes_x4_1;
      spi_get_byte_x4(&bytes_x4_1);

      uint32_t *dest_0 = (uint32_t *) &(bytes[i]);
      *dest_0 = bytes_x4_0;
      uint32_t *dest_1 = (uint32_t *) &(bytes[i+4]);
      *dest_1 = bytes_x4_1;

      crc6_x4(&crc_32bits, crc_32bits, bytes_x4_0);
      crc6_x4(&crc_32bits, crc_32bits, bytes_x4_1);

      i += 8;
    }
  });

  // F3: SIMD x16 spi_get_byte, memory store and CRC
  volatile uint32_t cycles_process_simd_full_x16;
  volatile uint32_t instr_process_simd_full_x16;
  volatile uint32_t start_instr_simd_full_x16;
  MEASURE_BLOCK(start_cycles, start_instr_simd_full_x16, offset_cycles, offset_instr, cycles_process_simd_full_x16, instr_process_simd_full_x16, {
    uint16_t i = start_addr;
    while (i <= stop_addr - 16) {
      uint32_t bytes_x4_0;
      spi_get_byte_x4(&bytes_x4_0);
      uint32_t bytes_x4_1;
      spi_get_byte_x4(&bytes_x4_1);
      uint32_t bytes_x4_2;
      spi_get_byte_x4(&bytes_x4_2);
      uint32_t bytes_x4_3;
      spi_get_byte_x4(&bytes_x4_3);

      uint32_t *dest_0 = (uint32_t *) &(bytes[i]);
      *dest_0 = bytes_x4_0;
      uint32_t *dest_1 = (uint32_t *) &(bytes[i+4]);
      *dest_1 = bytes_x4_1;
      uint32_t *dest_2 = (uint32_t *) &(bytes[i+8]);
      *dest_2 = bytes_x4_0;
      uint32_t *dest_3 = (uint32_t *) &(bytes[i+12]);
      *dest_3 = bytes_x4_1;

      crc6_x4(&crc_32bits, crc_32bits, bytes_x4_0);
      crc6_x4(&crc_32bits, crc_32bits, bytes_x4_1);
      crc6_x4(&crc_32bits, crc_32bits, bytes_x4_2);
      crc6_x4(&crc_32bits, crc_32bits, bytes_x4_3);

      i += 16;
    }
  });

  /*****************************
          Print results     
  *****************************/

  debug_printf("*********************\n");
  debug_printf("     Calibration     \n");
  debug_printf("*********************\n");  
  debug_printf("start_instr = %d\n", start_instr);
  debug_printf("total cycles = %d\n", offset_cycles);
  debug_printf("total instr = %d\n", offset_instr);

  debug_printf("\n");
  debug_printf("*********************\n");
  debug_printf("         CRC         \n");
  debug_printf("*********************\n");

  debug_printf("\nintuitive:\n");
  debug_printf("crc = %d\n", crc_calc);
  debug_printf("total cycles = %d\n", cycles_crc_calc);
  debug_printf("total instr = %d\n", instr_crc_calc);

  debug_printf("\ntabulated:\n");
  debug_printf("crc = %d\n", crc_tab);
  debug_printf("total cycles = %d\n", cycles_crc_tab);
  debug_printf("total instr = %d\n", instr_crc_tab);

  debug_printf("\ncustom instruction:\n");
  debug_printf("crc = %d\n", crc_instr);
  debug_printf("total cycles = %d\n", cycles_crc_instr);
  debug_printf("total instr = %d\n", instr_crc_instr);

  debug_printf("\ncustom instruction (inline):\n");
  debug_printf("crc = %d\n", crc_instr_inline);
  debug_printf("total cycles = %d\n", cycles_crc_instr_inline);
  debug_printf("total instr = %d\n", instr_crc_instr_inline);

  debug_printf("\n");
  debug_printf("*********************\n");
  debug_printf("   Data Processing   \n");
  debug_printf("*********************\n");

  debug_printf("\nLoop:\n");
  debug_printf("total cycles (16 bytes) = %d\n", cycles_loop);
  debug_printf("total instr (16 bytes) = %d\n", instr_loop);

  debug_printf("\nRef [instr %d]\n", start_instr_ref);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_ref);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_ref);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_ref - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_ref - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nA [instr %d]\n", start_instr_a);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_a);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_a);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_a - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_a - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nB1 [instr %d]\n", start_instr_b1);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_b1);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_b1);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_b1 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_b1 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nB2 [instr %d]\n", start_instr_b2);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_b2);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_b2);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_b2 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_b2 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nC1: Loop Unrolling 1 (x4) [instr %d]\n", start_instr_loop_unrolling_x4);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_loop_unrolling_x4);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_loop_unrolling_x4);
  debug_printf("total cycles (1 byte) = "); 
  debug_printf_fixed((cycles_process_loop_unrolling_x4 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = "); 
  debug_printf_fixed((instr_process_loop_unrolling_x4 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nC1: Loop Unrolling 2 (x4) [instr %d]\n", start_instr_loop_unrolling_2_x4);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_loop_unrolling_2_x4);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_loop_unrolling_2_x4);
  debug_printf("total cycles (1 byte) = "); 
  debug_printf_fixed((cycles_process_loop_unrolling_2_x4 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = "); 
  debug_printf_fixed((instr_process_loop_unrolling_2_x4 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nC2: Loop Unrolling (x8) [instr %d]\n", start_instr_loop_unrolling_x8);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_loop_unrolling_x8);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_loop_unrolling_x8);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_loop_unrolling_x8 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_loop_unrolling_x8 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nD1: SIMD light (x4) [instr %d]\n", start_instr_simd_light_x4);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_simd_light_x4);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_simd_light_x4);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_simd_light_x4 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_simd_light_x4 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nD2: SIMD light (x8) [instr %d]\n", start_instr_simd_light_x8);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_simd_light_x8);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_simd_light_x8);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_simd_light_x8 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_simd_light_x8 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nF1: SIMD full (x4) [instr %d]\n", start_instr_simd_full_x4);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_simd_full_x4);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_simd_full_x4);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_simd_full_x4 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_simd_full_x4 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nF2: SIMD full (x8) [instr %d]\n", start_instr_simd_full_x8);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_simd_full_x8);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_simd_full_x8);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_simd_full_x8 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_simd_full_x8 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\nF3: SIMD full (x16) [instr %d]\n", start_instr_simd_full_x16);
  debug_printf("total cycles (16 bytes) = %d\n", cycles_process_simd_full_x16);
  debug_printf("total instr (16 bytes) = %d\n", instr_process_simd_full_x16);
  debug_printf("total cycles (1 byte) = ");
  debug_printf_fixed((cycles_process_simd_full_x16 - cycles_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");
  debug_printf("total instr (1 byte) = ");
  debug_printf_fixed((instr_process_simd_full_x16 - instr_loop) * (1 << PRECISION) / (STOP_ADDR - START_ADDR), PRECISION);
  debug_printf("\n");

  debug_printf("\n");
  return 0;

}
