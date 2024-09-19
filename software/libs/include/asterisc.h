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

#ifndef _ASTERISC_LIB_H_
#define _ASTERISC_LIB_H_


/*************************
      Flash Mapping   
*************************/

extern int __bootrom_start__;
extern int __bootrom_size__;
extern int __approm_start__;
extern int __approm_size__;


/*************************
   Register Definitions   
*************************/

// IOF
#define IOF_ADDR                                   0x03000000                      
#define IOF_ADDR_MASK                              0x00000003                      
#define IOF_SEL_OFFSET                             0x0         

extern volatile unsigned long *iof_sel;


// uart              
#define UART0_ADDR                                 0x09000000
#define UART0_ADDR_MASK                            0x00000fff                      
#define UART_DATA_TX_OFFSET                        0x00
#define UART_DATA_RX_OFFSET                        0x04
#define UART_CONFIG_OFFSET                         0x08
#define UART_STATUS_OFFSET                         0x0C

extern volatile unsigned long *uart0_data_tx;
extern volatile unsigned long *uart0_data_rx;
extern volatile unsigned long *uart0_config;
extern volatile unsigned long *uart0_status;

// spi
#define SPI_SLAVE0_ADDR                            0x0b000000                      
#define SPI_SLAVE0_ADDR_MASK                       0xfffff000                      
#define SPI_CONFIG_OFFSET                          0x00                            
#define SPI_CMP_PATTERN_OFFSET                     0x04                            
#define SPI_CMP_PATTERN_MSK_OFFSET                 0x08                            
#define SPI_START_PATTERN_OFFSET                   0x0C                            
#define SPI_START_PATTERN_MSK_OFFSET               0x10                            
#define SPI_STATUS_TX_OFFSET                       0x14                            
#define SPI_STATUS_RX_OFFSET                       0x18                            
#define SPI_DATA_TX_OFFSET                         0x1c                            
#define SPI_DATA_TX_x4_OFFSET                      0x20                            
#define SPI_DATA_RX_OFFSET                         0x24    
#define SPI_DATA_RX_x4_OFFSET                      0x28    

extern volatile unsigned long *spi_config;            
extern volatile unsigned long *spi_cmp_pattern;       
extern volatile unsigned long *spi_cmp_pattern_msk;   
extern volatile unsigned long *spi_start_pattern;     
extern volatile unsigned long *spi_start_pattern_msk; 
extern volatile unsigned long *spi_status_tx;
extern volatile unsigned long *spi_status_rx;
extern volatile unsigned long *spi_data_tx;
extern volatile unsigned long *spi_data_tx_x4;
extern volatile unsigned long *spi_data_rx;
extern volatile unsigned long *spi_data_rx_x4;

// bytes from spi_status 
extern volatile unsigned char *spi_status_rx_fifo_full;
extern volatile unsigned char *spi_status_rx_fifo_empty;
extern volatile unsigned char *spi_status_rx_fifo_empty_x4;
extern volatile unsigned char *spi_status_tx_fifo_full;
//extern volatile unsigned char *spi_status_tx_fifo_full_x4;
extern volatile unsigned char *spi_status_tx_fifo_empty;


// pwm
#define PWM0_ADDR                                  0x05000000
#define PWM0_MASK                                  0x00000fff
#define PWM_CONFIG_OFFSET                          0x00
#define PWM_COUNTER_OFFSET                         0x08
#define PWM_SCALED_COUNTER_OFFSET                  0x10
#define PWM_COMPARE0_OFFSET                        0x20
#define PWM_COMPARE1_OFFSET                        0x24
#define PWM_COMPARE2_OFFSET                        0x28
#define PWM_COMPARE3_OFFSET                        0x2C

extern volatile unsigned long *pwm0_config        ;
extern volatile unsigned long *pwm0_counter       ;
extern volatile unsigned long *pwm0_scaled_counter;
extern volatile unsigned long *pwm0_compare0      ;
extern volatile unsigned long *pwm0_compare1      ;
extern volatile unsigned long *pwm0_compare2      ;
extern volatile unsigned long *pwm0_compare3      ;


// timer 
#define TIMER0_ADDR                                0x08000000                      
#define TIMER0_ADDR_MASK                           0x00000fff                      
#define TIMER_CONFIG_OFFSET                        0x00                            
#define TIMER_COUNTER_MSB_OFFSET                   0x08                            
#define TIMER_COUNTER_LSB_OFFSET                   0x10                            
#define TIMER_SCALED_COUNTER_OFFSET                0x12                            
#define TIMER_COMPARE_OFFSET                       0x20      

extern volatile unsigned long *timer0_config        ;
extern volatile unsigned long *timer0_counter_msb   ;
extern volatile unsigned long *timer0_counter_lsb   ;
extern volatile unsigned long *timer0_scaled_counter;
extern volatile unsigned long *timer0_compare       ;


// gpio 
#define GPIO_ADDR                                  0x04000000                      
#define GPIO_ADDR_MASK                             0x00000fff                      
#define GPIO_WRITE_VALUE_OFFSET                    0x00                            
#define GPIO_READ_VALUE_OFFSET                     0x04                            
#define GPIO_OUTPUT_EN_OFFSET                      0x08                            
#define GPIO_PULLUP_EN_OFFSET                      0x0C                            
#define GPIO_PULLDOWN_EN_OFFSET                    0x10   

extern volatile unsigned long *gpio_write_value;
extern volatile unsigned long *gpio_read_value ;
extern volatile unsigned long *gpio_output_en  ;
extern volatile unsigned long *gpio_pullup_en  ;
extern volatile unsigned long *gpio_pulldown_en;

//debug
#define DEBUG_ADDR                                 0x0A000000                      
#define DEBUG_ADDR_MASK                            0x0000ffff                      
#define DEBUG_REG0_OFFSET                          0x00                            
#define DEBUG_REG1_OFFSET                          0x04                            
#define DEBUG_REG2_OFFSET                          0x08                            
#define DEBUG_REG3_OFFSET                          0x0c                            
#define DEBUG_REG4_OFFSET                          0x10                            
#define DEBUG_REG5_OFFSET                          0x14                            
#define DEBUG_REG6_OFFSET                          0x18                            
#define DEBUG_REG7_OFFSET                          0x1c                            
#define DEBUG_REG8_OFFSET                          0x20                            

extern volatile unsigned long *debug0;
extern volatile unsigned long *debug1;
extern volatile unsigned long *debug2;
extern volatile unsigned long *debug3;
extern volatile unsigned long *debug4;
extern volatile unsigned long *debug5;
extern volatile unsigned long *debug6;
extern volatile unsigned long *debug7;


/*************************
   Bitfields Definitions   
*************************/

//iof_registers:

typedef union {
  struct {
    unsigned long sel                            : 24;
    unsigned long reserved                       : 8;
  };
} iof_config_t;
extern volatile iof_config_t *iof_config_bits;

#define _IOF_CONFIG_SEL_POSN                       0x0
#define _IOF_CONFIG_SEL_SIZE                       0x10
#define _IOF_CONFIG_SEL_MASK                       0x0000ffff
#define _IOF_CONFIG_RESERVED_POSN                  0x10
#define _IOF_CONFIG_RESERVED_SIZE                  0x10
#define _IOF_CONFIG_RESERVED_MASK                  0xffff0000


//gpio_registers:

typedef union {
	struct {
		unsigned long value                        : 24;
	 	unsigned long reserved                     : 8;
	};
} gpio_t;

#define _GPIO_VALUE_POSN                           0x0
#define _GPIO_VALUE_SIZE                           0x18
#define _GPIO_VALUE_MASK                           0x00ffffff
#define _GPIO_RESERVED_POSN                        0x18
#define _GPIO_RESERVED_SIZE                        0x8
#define _GPIO_RESERVED_MASK                        0xff000000


typedef union {
  struct {
    unsigned long scale                          : 4;
    unsigned long reserved0                      : 4;
    unsigned long sticky                         : 1;
    unsigned long zero_cmp                       : 1;
    unsigned long deglitch                       : 1;
    unsigned long reserved1                      : 1;
    unsigned long en_always                      : 1;
    unsigned long en_oneshot                     : 1;
    unsigned long reserved2                      : 2;
    unsigned long compare0_center                : 1;
    unsigned long compare1_center                : 1;
    unsigned long compare2_center                : 1;
    unsigned long compare3_center                : 1;
    unsigned long compare0_complement            : 1;
    unsigned long compare1_complement            : 1;
    unsigned long compare2_complement            : 1;
    unsigned long compare3_complement            : 1;
    unsigned long compare0_gang                  : 1;
    unsigned long compare1_gang                  : 1;
    unsigned long compare2_gang                  : 1;
    unsigned long compare3_gang                  : 1;
    unsigned long compare0_ip                    : 1;
    unsigned long compare1_ip                    : 1;
    unsigned long compare2_ip                    : 1;
    unsigned long compare3_ip                    : 1;
  };
} pwm_config_t;

extern volatile pwm_config_t *pwm0_config_bits;


// bitfield macros
#define _PWM_CONFIG_SCALE_POSN                     0x0
#define _PWM_CONFIG_SCALE_SIZE                     0x4
#define _PWM_CONFIG_SCALE_MASK                     0x0000000f
#define _PWM_CONFIG_RESERVED0_POSN                 0x4
#define _PWM_CONFIG_RESERVED0_SIZE                 0x4
#define _PWM_CONFIG_RESERVED0_MASK                 0x000000f0
#define _PWM_CONFIG_STICKY_POSN                    0x8
#define _PWM_CONFIG_STICKY_SIZE                    0x1
#define _PWM_CONFIG_STICKY_MASK                    0x00000100
#define _PWM_CONFIG_ZERO_CMP_POSN                  0x9
#define _PWM_CONFIG_ZERO_CMP_SIZE                  0x1
#define _PWM_CONFIG_ZERO_CMP_MASK                  0x00000200
#define _PWM_CONFIG_DEGLITCH_POSN                  0xa
#define _PWM_CONFIG_DEGLITCH_SIZE                  0x1
#define _PWM_CONFIG_DEGLITCH_MASK                  0x00000400
#define _PWM_CONFIG_RESERVED1_POSN                 0xb
#define _PWM_CONFIG_RESERVED1_SIZE                 0x1
#define _PWM_CONFIG_RESERVED1_MASK                 0x00000800
#define _PWM_CONFIG_EN_ALWAYS_POSN                 0xc
#define _PWM_CONFIG_EN_ALWAYS_SIZE                 0x1
#define _PWM_CONFIG_EN_ALWAYS_MASK                 0x00001000
#define _PWM_CONFIG_EN_ONESHOT_POSN                0xd
#define _PWM_CONFIG_EN_ONESHOT_SIZE                0x1
#define _PWM_CONFIG_EN_ONESHOT_MASK                0x00002000
#define _PWM_CONFIG_RESERVED2_POSN                 0xe
#define _PWM_CONFIG_RESERVED2_SIZE                 0x2
#define _PWM_CONFIG_RESERVED2_MASK                 0x0000c000
#define _PWM_CONFIG_COMPARE0_CENTER_POSN           0x10
#define _PWM_CONFIG_COMPARE0_CENTER_SIZE           0x1
#define _PWM_CONFIG_COMPARE0_CENTER_MASK           0x00010000
#define _PWM_CONFIG_COMPARE1_CENTER_POSN           0x11
#define _PWM_CONFIG_COMPARE1_CENTER_SIZE           0x1
#define _PWM_CONFIG_COMPARE1_CENTER_MASK           0x00020000
#define _PWM_CONFIG_COMPARE2_CENTER_POSN           0x12
#define _PWM_CONFIG_COMPARE2_CENTER_SIZE           0x1
#define _PWM_CONFIG_COMPARE2_CENTER_MASK           0x00040000
#define _PWM_CONFIG_COMPARE3_CENTER_POSN           0x13
#define _PWM_CONFIG_COMPARE3_CENTER_SIZE           0x1
#define _PWM_CONFIG_COMPARE3_CENTER_MASK           0x00080000
#define _PWM_CONFIG_COMPARE0_COMPLEMENT_POSN       0x14
#define _PWM_CONFIG_COMPARE0_COMPLEMENT_SIZE       0x1
#define _PWM_CONFIG_COMPARE0_COMPLEMENT_MASK       0x00100000
#define _PWM_CONFIG_COMPARE1_COMPLEMENT_POSN       0x15
#define _PWM_CONFIG_COMPARE1_COMPLEMENT_SIZE       0x1
#define _PWM_CONFIG_COMPARE1_COMPLEMENT_MASK       0x00200000
#define _PWM_CONFIG_COMPARE2_COMPLEMENT_POSN       0x16
#define _PWM_CONFIG_COMPARE2_COMPLEMENT_SIZE       0x1
#define _PWM_CONFIG_COMPARE2_COMPLEMENT_MASK       0x00400000
#define _PWM_CONFIG_COMPARE3_COMPLEMENT_POSN       0x17
#define _PWM_CONFIG_COMPARE3_COMPLEMENT_SIZE       0x1
#define _PWM_CONFIG_COMPARE3_COMPLEMENT_MASK       0x00800000
#define _PWM_CONFIG_COMPARE0_GANG_POSN             0x18
#define _PWM_CONFIG_COMPARE0_GANG_SIZE             0x1
#define _PWM_CONFIG_COMPARE0_GANG_MASK             0x01000000
#define _PWM_CONFIG_COMPARE1_GANG_POSN             0x19
#define _PWM_CONFIG_COMPARE1_GANG_SIZE             0x1
#define _PWM_CONFIG_COMPARE1_GANG_MASK             0x02000000
#define _PWM_CONFIG_COMPARE2_GANG_POSN             0x1a
#define _PWM_CONFIG_COMPARE2_GANG_SIZE             0x1
#define _PWM_CONFIG_COMPARE2_GANG_MASK             0x04000000
#define _PWM_CONFIG_COMPARE3_GANG_POSN             0x1b
#define _PWM_CONFIG_COMPARE3_GANG_SIZE             0x1
#define _PWM_CONFIG_COMPARE3_GANG_MASK             0x08000000
#define _PWM_CONFIG_COMPARE0_IP_POSN               0x1c
#define _PWM_CONFIG_COMPARE0_IP_SIZE               0x1
#define _PWM_CONFIG_COMPARE0_IP_MASK               0x10000000
#define _PWM_CONFIG_COMPARE1_IP_POSN               0x1d
#define _PWM_CONFIG_COMPARE1_IP_SIZE               0x1
#define _PWM_CONFIG_COMPARE1_IP_MASK               0x20000000
#define _PWM_CONFIG_COMPARE2_IP_POSN               0x1e
#define _PWM_CONFIG_COMPARE2_IP_SIZE               0x1
#define _PWM_CONFIG_COMPARE2_IP_MASK               0x40000000
#define _PWM_CONFIG_COMPARE3_IP_POSN               0x1f
#define _PWM_CONFIG_COMPARE3_IP_SIZE               0x1
#define _PWM_CONFIG_COMPARE3_IP_MASK               0x80000000


//timer_registers:

typedef union {
  struct {
    unsigned long postscale                      : 4;
    unsigned long reserved0                      : 4;
    unsigned long prescale                       : 8;
    unsigned long zero_cmp                       : 1;
    unsigned long count_up                       : 1;
    unsigned long en_oneshot                     : 1;
    unsigned long en_always                      : 1;
    unsigned long clock_source                   : 1;
    unsigned long sticky                         : 1;
    unsigned long compare_ie                     : 1;
    unsigned long overflow_ie                    : 1;
    unsigned long compare_if                     : 1;
    unsigned long overflow_if                    : 1;
    unsigned long compare_ip                     : 1;
    unsigned long overflow_ip                    : 1;
    unsigned long reserved1                      : 6;
  };
} timer_config_t;
extern volatile timer_config_t *timer0_config_bits;

#define _TIMER_CONFIG_POSTSCALE_POSN               0x0
#define _TIMER_CONFIG_POSTSCALE_SIZE               0x4
#define _TIMER_CONFIG_POSTSCALE_MASK               0x00000000f
#define _TIMER_CONFIG_RESERVED0_POSN               0x4
#define _TIMER_CONFIG_RESERVED0_SIZE               0x4
#define _TIMER_CONFIG_RESERVED0_MASK               0x0000000f0
#define _TIMER_CONFIG_PRESCALE_POSN                0x8
#define _TIMER_CONFIG_PRESCALE_SIZE                0x8
#define _TIMER_CONFIG_PRESCALE_MASK                0x00000ff00
#define _TIMER_CONFIG_ZERO_CMP_POSN                0x10
#define _TIMER_CONFIG_ZERO_CMP_SIZE                0x1
#define _TIMER_CONFIG_ZERO_CMP_MASK                0x000010000
#define _TIMER_CONFIG_COUNT_UP_POSN                0x11
#define _TIMER_CONFIG_COUNT_UP_SIZE                0x1
#define _TIMER_CONFIG_COUNT_UP_MASK                0x000020000
#define _TIMER_CONFIG_EN_ONESHOT_POSN              0x12
#define _TIMER_CONFIG_EN_ONESHOT_SIZE              0x1
#define _TIMER_CONFIG_EN_ONESHOT_MASK              0x000040000
#define _TIMER_CONFIG_EN_ALWAYS_POSN               0x13
#define _TIMER_CONFIG_EN_ALWAYS_SIZE               0x1
#define _TIMER_CONFIG_EN_ALWAYS_MASK               0x000080000
#define _TIMER_CONFIG_CLOCK_SOURCE_POSN            0x14
#define _TIMER_CONFIG_CLOCK_SOURCE_SIZE            0x1
#define _TIMER_CONFIG_CLOCK_SOURCE_MASK            0x000100000
#define _TIMER_CONFIG_STICKY_POSN                  0x15
#define _TIMER_CONFIG_STICKY_SIZE                  0x1
#define _TIMER_CONFIG_STICKY_MASK                  0x000200000
#define _TIMER_CONFIG_COMPARE_IE_POSN              0x16
#define _TIMER_CONFIG_COMPARE_IE_SIZE              0x1
#define _TIMER_CONFIG_COMPARE_IE_MASK              0x000400000
#define _TIMER_CONFIG_OVERFLOW_IE_POSN             0x17
#define _TIMER_CONFIG_OVERFLOW_IE_SIZE             0x1
#define _TIMER_CONFIG_OVERFLOW_IE_MASK             0x000800000
#define _TIMER_CONFIG_COMPARE_IF_POSN              0x18
#define _TIMER_CONFIG_COMPARE_IF_SIZE              0x1
#define _TIMER_CONFIG_COMPARE_IF_MASK              0x001000000
#define _TIMER_CONFIG_OVERFLOW_IF_POSN             0x19
#define _TIMER_CONFIG_OVERFLOW_IF_SIZE             0x1
#define _TIMER_CONFIG_OVERFLOW_IF_MASK             0x002000000
#define _TIMER_CONFIG_COMPARE_IP_POSN              0x1a
#define _TIMER_CONFIG_COMPARE_IP_SIZE              0x1
#define _TIMER_CONFIG_COMPARE_IP_MASK              0x004000000
#define _TIMER_CONFIG_OVERFLOW_IP_POSN             0x1b
#define _TIMER_CONFIG_OVERFLOW_IP_SIZE             0x1
#define _TIMER_CONFIG_OVERFLOW_IP_MASK             0x008000000
#define _TIMER_CONFIG_RESERVED1_POSN               0x1c
#define _TIMER_CONFIG_RESERVED1_SIZE               0x6
#define _TIMER_CONFIG_RESERVED1_MASK               0x3f0000000


typedef union {
  struct {
    unsigned long long value                     : 47;
    unsigned long reserved                       : 18;
  };
} timer_counter_t;

#define _TIMER_COUNTER_VALUE_POSN                  0x0
#define _TIMER_COUNTER_VALUE_SIZE                  0x2f
#define _TIMER_COUNTER_VALUE_MASK                  0x000007fffffffffff
#define _TIMER_COUNTER_RESERVED_POSN               0x2f
#define _TIMER_COUNTER_RESERVED_SIZE               0x12
#define _TIMER_COUNTER_RESERVED_MASK               0x1ffff800000000000


typedef union {
  struct {
    unsigned long value                          : 32;
  };
} timer_scounter_t;

#define _TIMER_SCOUNTER_VALUE_POSN                 0x0
#define _TIMER_SCOUNTER_VALUE_SIZE                 0x20
#define _TIMER_SCOUNTER_VALUE_MASK                 0xffffffff


//uart_registers:

typedef union {
  struct {
    unsigned long value                          : 8;
    unsigned long reserved                       : 24;
  };
} uart_data_t;
extern volatile uart_data_t *uart0_data_tx_bits;
extern volatile uart_data_t *uart0_data_rx_bits;

#define _UART_DATA_VALUE_POSN                      0x0
#define _UART_DATA_VALUE_SIZE                      0x8
#define _UART_DATA_VALUE_MASK                      0x000000ff
#define _UART_DATA_RESERVED_POSN                   0x8
#define _UART_DATA_RESERVED_SIZE                   0x18
#define _UART_DATA_RESERVED_MASK                   0xffffff00


typedef union {
  struct {
    unsigned long baudrate                       : 16;
    unsigned long tx_en                          : 1;
    unsigned long rx_en                          : 1;
    unsigned long parity                         : 1;
    unsigned long nb_stop_bits                   : 1;
    unsigned long reserved                       : 12;
  };
} uart_config_t;
extern volatile uart_config_t *uart0_config_bits;

//TODO: update below
#define _UART_CONFIG_BAUDRATE_POSN                 0x0
#define _UART_CONFIG_BAUDRATE_SIZE                 0x10
#define _UART_CONFIG_BAUDRATE_MASK                 0x0000ffff
#define _UART_CONFIG_RESERVED_POSN                 0x10
#define _UART_CONFIG_RESERVED_SIZE                 0x10
#define _UART_CONFIG_RESERVED_MASK                 0xffff0000


typedef union {
  struct {
    unsigned long tx_full                        : 1;
    unsigned long rx_empty                       : 1;
    unsigned long reserved                       : 30;
  };
} uart_status_t;
extern volatile uart_status_t *uart0_status_bits;

#define _UART_STATUS_TX_FULL_POSN                  0x0
#define _UART_STATUS_TX_FULL_SIZE                  0x1
#define _UART_STATUS_TX_FULL_MASK                  0x00000001
#define _UART_STATUS_RX_EMPTY_POSN                 0x1
#define _UART_STATUS_RX_EMPTY_SIZE                 0x1
#define _UART_STATUS_RX_EMPTY_MASK                 0x00000002
#define _UART_STATUS_RESERVED_POSN                 0x2
#define _UART_STATUS_RESERVED_SIZE                 0x1e
#define _UART_STATUS_RESERVED_MASK                 0xfffffffc


//spi_slave_registers:

typedef union {
  struct {
    unsigned long wait_for_msg_start             : 1;
    unsigned long no_current_msg                 : 1;
    unsigned long send_on_compare                : 1;
    unsigned long compare_offset                 : 4;
    unsigned long send_countdown                 : 6;
    unsigned long reserved1                      : 18;
    unsigned long interrupt_enable               : 1;
    unsigned long system_enable                  : 1;
  };
} spi_config_t;

#define _SPI_CONFIG_WAIT_FOR_MSG_START_POSN        0x0
#define _SPI_CONFIG_WAIT_FOR_MSG_START_SIZE        0x1
#define _SPI_CONFIG_WAIT_FOR_MSG_START_MASK        0x000000001
#define _SPI_CONFIG_NO_CURRENT_MSG_POSN            0x1
#define _SPI_CONFIG_NO_CURRENT_MSG_SIZE            0x1
#define _SPI_CONFIG_NO_CURRENT_MSG_MASK            0x000000002
#define _SPI_CONFIG_SEND_ON_COMPARE_POSN           0x2
#define _SPI_CONFIG_SEND_ON_COMPARE_SIZE           0x1
#define _SPI_CONFIG_SEND_ON_COMPARE_MASK           0x000000004
#define _SPI_CONFIG_COMPARE_OFFSET_POSN            0x3
#define _SPI_CONFIG_COMPARE_OFFSET_SIZE            0x4
#define _SPI_CONFIG_COMPARE_OFFSET_MASK            0x000000078
#define _SPI_CONFIG_SEND_COUNTDOWN_POSN            0x7
#define _SPI_CONFIG_SEND_COUNTDOWN_SIZE            0x6
#define _SPI_CONFIG_SEND_COUNTDOWN_MASK            0x000001f80
#define _SPI_CONFIG_RESERVED1_POSN                 0xd
#define _SPI_CONFIG_RESERVED1_SIZE                 0x12
#define _SPI_CONFIG_RESERVED1_MASK                 0x07fffe000
#define _SPI_CONFIG_INTERRUPT_ENABLE_POSN          0x1f
#define _SPI_CONFIG_INTERRUPT_ENABLE_SIZE          0x1
#define _SPI_CONFIG_INTERRUPT_ENABLE_MASK          0x080000000
#define _SPI_CONFIG_SYSTEM_ENABLE_POSN             0x20
#define _SPI_CONFIG_SYSTEM_ENABLE_SIZE             0x1
#define _SPI_CONFIG_SYSTEM_ENABLE_MASK             0x100000000


typedef union {
  struct {
    unsigned long value                          : 32;
  };
} spi_cmp_pattern_t;

#define _SPI_CMP_PATTERN_VALUE_POSN                0x0
#define _SPI_CMP_PATTERN_VALUE_SIZE                0x20
#define _SPI_CMP_PATTERN_VALUE_MASK                0xffffffff


typedef union {
  struct {
    unsigned long value                          : 32;
  };
} spi_cmp_pattern_msk_t;

#define _SPI_CMP_PATTERN_MSK_VALUE_POSN            0x0
#define _SPI_CMP_PATTERN_MSK_VALUE_SIZE            0x20
#define _SPI_CMP_PATTERN_MSK_VALUE_MASK            0xffffffff


typedef union {
  struct {
    unsigned long value                          : 32;
  };
} spi_start_pattern_t;

#define _SPI_START_PATTERN_VALUE_POSN              0x0
#define _SPI_START_PATTERN_VALUE_SIZE              0x20
#define _SPI_START_PATTERN_VALUE_MASK              0xffffffff


typedef union {
  struct {
    unsigned long value                          : 32;
  };
} spi_start_pattern_msk_t;

#define _SPI_START_PATTERN_MSK_VALUE_POSN          0x0
#define _SPI_START_PATTERN_MSK_VALUE_SIZE          0x20
#define _SPI_START_PATTERN_MSK_VALUE_MASK          0xffffffff


typedef union {
  struct {
    unsigned long fifo_full                 : 1;
    unsigned long reserved0                 : 7;
    unsigned long fifo_full_x4              : 1;
    unsigned long reserved1                 : 7;
    unsigned long fifo_empty                : 1;
    unsigned long reserved2                 : 7;
    unsigned long fifo_empty_x4             : 1;
    unsigned long reserved3                 : 7;
  };
} spi_status_t;

#define _SPI_STATUS_MOSI_FIFO_FULL_POSN            0x0
#define _SPI_STATUS_MOSI_FIFO_FULL_x4_POSN         0x1
#define _SPI_STATUS_MOSI_FIFO_EMPTY_POSN           0x2
#define _SPI_STATUS_MOSI_FIFO_EMPTY_x4_POSN        0x3

#define _SPI_STATUS_MISO_FIFO_FULL_POSN            0x0
#define _SPI_STATUS_MISO_FIFO_FULL_x4_POSN         0x1
#define _SPI_STATUS_MISO_FIFO_EMPTY_POSN           0x2
#define _SPI_STATUS_MISO_FIFO_EMPTY_x4_POSN        0x3


typedef union {
  struct {
    unsigned long value                          : 8;
    unsigned long reserved                       : 24;
  };
} spi_data_t;

#define _SPI_DATA_VALUE_POSN                       0x0
#define _SPI_DATA_VALUE_SIZE                       0x8
#define _SPI_DATA_VALUE_MASK                       0x000000ff
#define _SPI_DATA_RESERVED_POSN                    0x8
#define _SPI_DATA_RESERVED_SIZE                    0x18
#define _SPI_DATA_RESERVED_MASK                    0xffffff00

extern volatile spi_config_t *spi_config_bits                      ;
extern volatile spi_cmp_pattern_t *spi_cmp_pattern_bits            ;
extern volatile spi_cmp_pattern_msk_t *spi_cmp_pattern_msk_bits    ;
extern volatile spi_start_pattern_t *spi_start_pattern_bits        ;
extern volatile spi_start_pattern_msk_t *spi_start_pattern_msk_bits;
extern volatile spi_status_t *spi_status_tx_bits                   ;
extern volatile spi_status_t *spi_status_rx_bits                   ;
extern volatile spi_data_t *spi_data_tx_bits                       ;
extern volatile spi_data_t *spi_data_rx_bits                       ;

//debug_registers:

typedef union {
  struct {
    unsigned long byte0                          : 8;
    unsigned long byte1                          : 8;
    unsigned long byte2                          : 8;
    unsigned long byte3                          : 8;
  };
} debug_reg32_t;

#define _DEBUG_REG32_BYTE0_POSN                    0x0
#define _DEBUG_REG32_BYTE0_SIZE                    0x8
#define _DEBUG_REG32_BYTE0_MASK                    0x000000ff
#define _DEBUG_REG32_BYTE1_POSN                    0x8
#define _DEBUG_REG32_BYTE1_SIZE                    0x8
#define _DEBUG_REG32_BYTE1_MASK                    0x0000ff00
#define _DEBUG_REG32_BYTE2_POSN                    0x10
#define _DEBUG_REG32_BYTE2_SIZE                    0x8
#define _DEBUG_REG32_BYTE2_MASK                    0x00ff0000
#define _DEBUG_REG32_BYTE3_POSN                    0x18
#define _DEBUG_REG32_BYTE3_SIZE                    0x8
#define _DEBUG_REG32_BYTE3_MASK                    0xff000000

extern volatile debug_reg32_t *debug0_bits;
extern volatile debug_reg32_t *debug1_bits;
extern volatile debug_reg32_t *debug2_bits;
extern volatile debug_reg32_t *debug3_bits;
extern volatile debug_reg32_t *debug4_bits;
extern volatile debug_reg32_t *debug5_bits;
extern volatile debug_reg32_t *debug6_bits;
extern volatile debug_reg32_t *debug7_bits;


#endif // _ASTERISC_LIB_H_