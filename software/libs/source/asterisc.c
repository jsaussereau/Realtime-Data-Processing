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

#include "../include/asterisc.h" 


// IOF
volatile unsigned long *iof_sel                  = (unsigned long *) (IOF_ADDR    + (IOF_SEL_OFFSET              << 2));

volatile iof_config_t *iof_config_bits           = (iof_config_t *)  (PWM0_ADDR   + (PWM_CONFIG_OFFSET           << 2));


// uart              
volatile unsigned long *uart0_data_tx            = (unsigned long *) (UART0_ADDR  + (UART_DATA_TX_OFFSET         << 2 ));
volatile unsigned long *uart0_data_rx            = (unsigned long *) (UART0_ADDR  + (UART_DATA_RX_OFFSET         << 2 ));
volatile unsigned long *uart0_config             = (unsigned long *) (UART0_ADDR  + (UART_CONFIG_OFFSET          << 2 ));
volatile unsigned long *uart0_status             = (unsigned long *) (UART0_ADDR  + (UART_STATUS_OFFSET          << 2 ));
  
volatile uart_data_t *uart0_data_tx_bits         = (uart_data_t *)   (UART0_ADDR  + (UART_DATA_TX_OFFSET         << 2 ));
volatile uart_data_t *uart0_data_rx_bits         = (uart_data_t *)   (UART0_ADDR  + (UART_DATA_RX_OFFSET         << 2 ));
volatile uart_config_t *uart0_config_bits        = (uart_config_t *) (UART0_ADDR  + (UART_CONFIG_OFFSET          << 2 ));
volatile uart_status_t *uart0_status_bits        = (uart_status_t *) (UART0_ADDR  + (UART_STATUS_OFFSET          << 2 ));


// spi
volatile unsigned long *spi_config               = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_CONFIG_OFFSET            << 2 ));
volatile unsigned long *spi_cmp_pattern          = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_CMP_PATTERN_OFFSET       << 2 ));
volatile unsigned long *spi_cmp_pattern_msk      = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_CMP_PATTERN_MSK_OFFSET   << 2 ));
volatile unsigned long *spi_start_pattern        = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_START_PATTERN_OFFSET     << 2 ));
volatile unsigned long *spi_start_pattern_msk    = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_START_PATTERN_MSK_OFFSET << 2 ));
volatile unsigned long *spi_status_tx            = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_STATUS_TX_OFFSET         << 2 ));
volatile unsigned long *spi_status_rx            = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_STATUS_RX_OFFSET         << 2 ));
volatile unsigned long *spi_data_tx              = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_DATA_TX_OFFSET           << 2 ));
volatile unsigned long *spi_data_tx_x4           = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_DATA_TX_x4_OFFSET        << 2 ));
volatile unsigned long *spi_data_rx              = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_DATA_RX_OFFSET           << 2 ));
volatile unsigned long *spi_data_rx_x4           = (unsigned long *) (SPI_SLAVE0_ADDR + (SPI_DATA_RX_x4_OFFSET        << 2 ));

// bytes from spi_status 
volatile unsigned char *spi_status_rx_fifo_full     = (unsigned char *) (SPI_SLAVE0_ADDR + (SPI_STATUS_RX_OFFSET << 2 ) + _SPI_STATUS_MOSI_FIFO_FULL_POSN);
volatile unsigned char *spi_status_rx_fifo_empty    = (unsigned char *) (SPI_SLAVE0_ADDR + (SPI_STATUS_RX_OFFSET << 2 ) + _SPI_STATUS_MOSI_FIFO_EMPTY_POSN);
volatile unsigned char *spi_status_rx_fifo_empty_x4 = (unsigned char *) (SPI_SLAVE0_ADDR + (SPI_STATUS_RX_OFFSET << 2 ) + _SPI_STATUS_MOSI_FIFO_EMPTY_x4_POSN);
volatile unsigned char *spi_status_tx_fifo_full     = (unsigned char *) (SPI_SLAVE0_ADDR + (SPI_STATUS_TX_OFFSET << 2 ) + _SPI_STATUS_MISO_FIFO_FULL_POSN);
//volatile unsigned char *spi_status_tx_fifo_full_x4  = (unsigned char *) (SPI_SLAVE0_ADDR + (SPI_STATUS_TX_OFFSET << 2 ) + _SPI_STATUS_MISO_FIFO_FULL_x4_POSN);
volatile unsigned char *spi_status_tx_fifo_empty    = (unsigned char *) (SPI_SLAVE0_ADDR + (SPI_STATUS_TX_OFFSET << 2 ) + _SPI_STATUS_MISO_FIFO_EMPTY_POSN);

volatile spi_config_t *spi_config_bits                       = (spi_config_t *)            (SPI_SLAVE0_ADDR + (SPI_CONFIG_OFFSET            << 2 ));
volatile spi_cmp_pattern_t *spi_cmp_pattern_bits             = (spi_cmp_pattern_t *)       (SPI_SLAVE0_ADDR + (SPI_CMP_PATTERN_OFFSET       << 2 ));
volatile spi_cmp_pattern_msk_t *spi_cmp_pattern_msk_bits     = (spi_cmp_pattern_msk_t *)   (SPI_SLAVE0_ADDR + (SPI_CMP_PATTERN_MSK_OFFSET   << 2 ));
volatile spi_start_pattern_t *spi_start_pattern_bits         = (spi_start_pattern_t *)     (SPI_SLAVE0_ADDR + (SPI_START_PATTERN_OFFSET     << 2 ));
volatile spi_start_pattern_msk_t *spi_start_pattern_msk_bits = (spi_start_pattern_msk_t *) (SPI_SLAVE0_ADDR + (SPI_START_PATTERN_MSK_OFFSET << 2 ));
volatile spi_status_t *spi_status_tx_bits                    = (spi_status_t *)            (SPI_SLAVE0_ADDR + (SPI_STATUS_TX_OFFSET         << 2 ));
volatile spi_status_t *spi_status_rx_bits                    = (spi_status_t *)            (SPI_SLAVE0_ADDR + (SPI_STATUS_RX_OFFSET         << 2 ));
volatile spi_data_t *spi_data_tx_bits                        = (spi_data_t *)              (SPI_SLAVE0_ADDR + (SPI_DATA_TX_OFFSET           << 2 ));
volatile spi_data_t *spi_data_rx_bits                        = (spi_data_t *)              (SPI_SLAVE0_ADDR + (SPI_DATA_RX_OFFSET           << 2 ));

// pwm  
volatile unsigned long *pwm0_config              = (unsigned long *) (PWM0_ADDR   + (PWM_CONFIG_OFFSET           << 2 ));
volatile unsigned long *pwm0_counter             = (unsigned long *) (PWM0_ADDR   + (PWM_COUNTER_OFFSET          << 2 ));
volatile unsigned long *pwm0_scaled_counter      = (unsigned long *) (PWM0_ADDR   + (PWM_SCALED_COUNTER_OFFSET   << 2 ));
volatile unsigned long *pwm0_compare0            = (unsigned long *) (PWM0_ADDR   + (PWM_COMPARE0_OFFSET         << 2 ));
volatile unsigned long *pwm0_compare1            = (unsigned long *) (PWM0_ADDR   + (PWM_COMPARE1_OFFSET         << 2 ));
volatile unsigned long *pwm0_compare2            = (unsigned long *) (PWM0_ADDR   + (PWM_COMPARE2_OFFSET         << 2 ));
volatile unsigned long *pwm0_compare3            = (unsigned long *) (PWM0_ADDR   + (PWM_COMPARE3_OFFSET         << 2 ));

volatile pwm_config_t *pwm0_config_bits          = (pwm_config_t *)  (PWM0_ADDR   + (PWM_CONFIG_OFFSET           << 2 ));


// timer 
volatile unsigned long *timer0_config            = (unsigned long *) (TIMER0_ADDR + (TIMER_CONFIG_OFFSET         << 2 ));
volatile unsigned long *timer0_counter_msb       = (unsigned long *) (TIMER0_ADDR + (TIMER_COUNTER_MSB_OFFSET    << 2 ));
volatile unsigned long *timer0_counter_lsb       = (unsigned long *) (TIMER0_ADDR + (TIMER_COUNTER_LSB_OFFSET    << 2 ));
volatile unsigned long *timer0_scaled_counter    = (unsigned long *) (TIMER0_ADDR + (TIMER_SCALED_COUNTER_OFFSET << 2 ));
volatile unsigned long *timer0_compare           = (unsigned long *) (TIMER0_ADDR + (TIMER_COMPARE_OFFSET        << 2 ));

volatile timer_config_t *timer0_config_bits      = (timer_config_t *)(TIMER0_ADDR + (TIMER_CONFIG_OFFSET         << 2 ));


// gpio 
volatile unsigned long *gpio_write_value         = (unsigned long *) (GPIO_ADDR   + (GPIO_WRITE_VALUE_OFFSET     << 2 ));
volatile unsigned long *gpio_read_value          = (unsigned long *) (GPIO_ADDR   + (GPIO_READ_VALUE_OFFSET      << 2 ));
volatile unsigned long *gpio_output_en           = (unsigned long *) (GPIO_ADDR   + (GPIO_OUTPUT_EN_OFFSET       << 2 ));
volatile unsigned long *gpio_pullup_en           = (unsigned long *) (GPIO_ADDR   + (GPIO_PULLUP_EN_OFFSET       << 2 ));
volatile unsigned long *gpio_pulldown_en         = (unsigned long *) (GPIO_ADDR   + (GPIO_PULLDOWN_EN_OFFSET     << 2 ));


//debug 
volatile unsigned long *debug0                   = (unsigned long *) (DEBUG_ADDR  + (DEBUG_REG0_OFFSET           << 2 ));
volatile unsigned long *debug1                   = (unsigned long *) (DEBUG_ADDR  + (DEBUG_REG1_OFFSET           << 2 ));
volatile unsigned long *debug2                   = (unsigned long *) (DEBUG_ADDR  + (DEBUG_REG2_OFFSET           << 2 ));
volatile unsigned long *debug3                   = (unsigned long *) (DEBUG_ADDR  + (DEBUG_REG3_OFFSET           << 2 ));
volatile unsigned long *debug4                   = (unsigned long *) (DEBUG_ADDR  + (DEBUG_REG4_OFFSET           << 2 ));
volatile unsigned long *debug5                   = (unsigned long *) (DEBUG_ADDR  + (DEBUG_REG5_OFFSET           << 2 ));
volatile unsigned long *debug6                   = (unsigned long *) (DEBUG_ADDR  + (DEBUG_REG6_OFFSET           << 2 ));
volatile unsigned long *debug7                   = (unsigned long *) (DEBUG_ADDR  + (DEBUG_REG7_OFFSET           << 2 ));

volatile debug_reg32_t *debug0_bits              = (debug_reg32_t *) (DEBUG_ADDR  + (DEBUG_REG0_OFFSET           << 2 ));
volatile debug_reg32_t *debug1_bits              = (debug_reg32_t *) (DEBUG_ADDR  + (DEBUG_REG1_OFFSET           << 2 ));
volatile debug_reg32_t *debug2_bits              = (debug_reg32_t *) (DEBUG_ADDR  + (DEBUG_REG2_OFFSET           << 2 ));
volatile debug_reg32_t *debug3_bits              = (debug_reg32_t *) (DEBUG_ADDR  + (DEBUG_REG3_OFFSET           << 2 ));
volatile debug_reg32_t *debug4_bits              = (debug_reg32_t *) (DEBUG_ADDR  + (DEBUG_REG4_OFFSET           << 2 ));
volatile debug_reg32_t *debug5_bits              = (debug_reg32_t *) (DEBUG_ADDR  + (DEBUG_REG5_OFFSET           << 2 ));
volatile debug_reg32_t *debug6_bits              = (debug_reg32_t *) (DEBUG_ADDR  + (DEBUG_REG6_OFFSET           << 2 ));
volatile debug_reg32_t *debug7_bits              = (debug_reg32_t *) (DEBUG_ADDR  + (DEBUG_REG7_OFFSET           << 2 ));

