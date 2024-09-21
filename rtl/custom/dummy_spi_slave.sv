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

`ifndef __DUMMY_SPI_SLAVE__
`define __DUMMY_SPI_SLAVE__


`include "../AsteRISC-rtl/rtl/soc/packages/pck_memory_map.sv"
`include "../AsteRISC-rtl/rtl/soc/packages/pck_registers.sv"


module dummy_spi_slave
  import pck_memory_map::*;
#(
  parameter p_fifo_in_depth  = 256,       //! bytes
  parameter p_fifo_out_depth = 128        //! bytes
)(
  input  wire         i_clk,              //! global clock
  input  wire         i_rst,              //! global reset
  input  wire  [31:2] i_addr,             //! read/write address
  input  wire  [ 3:0] i_be,               //! write byte enable
  input  wire         i_wr_en,            //! write enable
  input  wire  [31:0] i_wr_data,          //! write data
  input  wire         i_rd_en,            //! read enable
  output logic [31:0] o_rd_data,          //! read data
  output logic        o_busy,             //! busy
  output logic        o_ack,              //! transfer acknowledge
  
  input  wire         i_sck,              //! SPI clock
  input  wire         i_csb,              //! SPI chip select
  input  wire         i_sdi,              //! SPI in (MOSI)
  output wire         o_sdo,              //! SPI out (MISO)
  output wire         o_sdo_en            //! SPI out enable
);

  spi_slave_registers regs ();

  logic [63:0] input_reg;
  logic [ 2:0] bit_counter;
  logic [ 8:0] byte_counter;
  logic [ 7:0] msg_size;
  logic        msg_size_stored;
  logic        rst_bit_counter;
  logic        aligned;
  logic        synchronized;
  logic        rd_en_old;

  logic        fifo_in_wr_en;
  logic [ 7:0] fifo_in_wr_data;
  logic        fifo_in_rd_en;
  logic [ 7:0] fifo_in_rd_data;
  logic        fifo_in_empty;
  logic        fifo_in_rd_en_x4;
  logic [31:0] fifo_in_rd_data_x4;
  logic        fifo_in_empty_x4;
  logic        fifo_in_full;

  logic        fifo_out_wr_en;
  logic [ 7:0] fifo_out_wr_data;
  logic        fifo_out_rd_en;
  logic [ 7:0] fifo_out_rd_data;
  logic        fifo_out_empty;
  logic        fifo_out_full;
    
  logic [ 2:0] p2s_bit_counter;
  logic        sdo;

  assign regs.spi_status_rx.fifo_full     = 1'b0;
  assign regs.spi_status_rx.fifo_full_x4  = 1'b0;
  assign regs.spi_status_rx.fifo_empty    = 1'b0;
  assign regs.spi_status_rx.fifo_empty_x4 = 1'b0;

  assign regs.spi_status_tx.fifo_full     = 1'b0;
  assign regs.spi_status_tx.fifo_full_x4  = 1'b0;
  assign regs.spi_status_tx.fifo_empty    = 1'b0;
  assign regs.spi_status_tx.fifo_empty_x4 = 1'b0;

  /******************
     Registers R/W
  ******************/

  logic        wr_ack;
  logic        rd_ack;
  logic [31:0] rd_data;

  assign o_busy = 1'b0; //! module cannot be busy
  assign o_ack  = (i_wr_en & wr_ack) | (i_rd_en & rd_ack);
  
  always_ff @(posedge i_clk) begin: registers_write
    if (i_rst) begin
      regs.spi_config.send_countdown      <= 6'd24;
      regs.spi_config.compare_offset      <= 4'd4;
      regs.spi_config.send_on_compare     <= 1'b1;
      regs.spi_config.write_start_pattern <= 1'b1;
      regs.spi_config.msg_size_byte       <= 4'd4;
      regs.spi_config.msg_size_init_value <= 4'd4;
      regs.spi_config.use_msg_size        <= 1'b1;
      regs.spi_config.no_current_msg      <= 1'b1;
      regs.spi_config.wait_for_msg_start  <= 1'b1;
      regs.spi_start_pattern              <= 32'h00ffa5a5;
      regs.spi_start_pattern_msk          <= 32'h00ffffff;
      regs.spi_data_tx                    <= 32'h00000000;
      fifo_out_wr_en <= 1'b0;
    end else begin 
      wr_ack <= 1'b1;
      fifo_out_wr_en <= 1'b0;
      if (i_wr_en) begin
        case (i_addr)
          SPI_CONFIG_OFFSET: begin
            if (i_be[3]) regs.spi_config            [31:24] <= i_wr_data[31:24];
            if (i_be[2]) regs.spi_config            [23:16] <= i_wr_data[23:16];
            if (i_be[1]) regs.spi_config            [15: 8] <= i_wr_data[15: 8];
            if (i_be[0]) regs.spi_config            [ 7: 0] <= i_wr_data[ 7: 0];
          end
          SPI_CMP_PATTERN_OFFSET: begin
            if (i_be[3]) regs.spi_cmp_pattern       [31:24] <= i_wr_data[31:24];
            if (i_be[2]) regs.spi_cmp_pattern       [23:16] <= i_wr_data[23:16];
            if (i_be[1]) regs.spi_cmp_pattern       [15: 8] <= i_wr_data[15: 8];
            if (i_be[0]) regs.spi_cmp_pattern       [ 7: 0] <= i_wr_data[ 7: 0];
          end
          SPI_CMP_PATTERN_MSK_OFFSET: begin
            if (i_be[3]) regs.spi_cmp_pattern_msk   [31:24] <= i_wr_data[31:24];
            if (i_be[2]) regs.spi_cmp_pattern_msk   [23:16] <= i_wr_data[23:16];
            if (i_be[1]) regs.spi_cmp_pattern_msk   [15: 8] <= i_wr_data[15: 8];
            if (i_be[0]) regs.spi_cmp_pattern_msk   [ 7: 0] <= i_wr_data[ 7: 0];
          end 
          SPI_START_PATTERN_OFFSET: begin
            if (i_be[3]) regs.spi_start_pattern     [31:24] <= i_wr_data[31:24];
            if (i_be[2]) regs.spi_start_pattern     [23:16] <= i_wr_data[23:16];
            if (i_be[1]) regs.spi_start_pattern     [15: 8] <= i_wr_data[15: 8];
            if (i_be[0]) regs.spi_start_pattern     [ 7: 0] <= i_wr_data[ 7: 0];
          end
          SPI_START_PATTERN_MSK_OFFSET: begin
            if (i_be[3]) regs.spi_start_pattern_msk [31:24] <= i_wr_data[31:24];
            if (i_be[2]) regs.spi_start_pattern_msk [23:16] <= i_wr_data[23:16];
            if (i_be[1]) regs.spi_start_pattern_msk [15: 8] <= i_wr_data[15: 8];
            if (i_be[0]) regs.spi_start_pattern_msk [ 7: 0] <= i_wr_data[ 7: 0];
          end
          SPI_DATA_TX_OFFSET: begin
            if (i_be[0]) begin
              regs.spi_data_tx                      [ 7: 0] <= i_wr_data[ 7: 0];
            end
          end
          SPI_DATA_TX_x4_OFFSET: begin
            if (i_be[3]) regs.spi_data_tx_x4        [31:24] <= i_wr_data[31:24];
            if (i_be[2]) regs.spi_data_tx_x4        [23:16] <= i_wr_data[23:16];
            if (i_be[1]) regs.spi_data_tx_x4        [15: 8] <= i_wr_data[15: 8];
            if (i_be[0]) regs.spi_data_tx_x4        [ 7: 0] <= i_wr_data[ 7: 0];
          end
          default: wr_ack <= 1'b0;
        endcase
      end
    end
  end

  always_ff @(posedge i_clk) begin: registers_read
    if (i_rst) begin
      rd_ack <= 1'b0;
      rd_data <= 0;
    end else begin
      rd_ack <= 1'b1;
      rd_data <= rd_data;
      if (i_rd_en) begin
        rd_data <= 0;
        case (i_addr)
          SPI_CONFIG_OFFSET            : rd_data <= regs.spi_config;
          SPI_CMP_PATTERN_OFFSET       : rd_data <= regs.spi_cmp_pattern;
          SPI_CMP_PATTERN_MSK_OFFSET   : rd_data <= regs.spi_cmp_pattern_msk;
          SPI_START_PATTERN_OFFSET     : rd_data <= regs.spi_start_pattern;
          SPI_START_PATTERN_MSK_OFFSET : rd_data <= regs.spi_start_pattern_msk;          
          SPI_STATUS_RX_OFFSET         : rd_data <= { 7'b0, regs.spi_status_rx.fifo_empty_x4, 7'b0, regs.spi_status_rx.fifo_empty,  7'b0, regs.spi_status_rx.fifo_full_x4, 7'b0, regs.spi_status_rx.fifo_full }; // read-only
          SPI_STATUS_TX_OFFSET         : rd_data <= { 7'b0, regs.spi_status_tx.fifo_empty_x4, 7'b0, regs.spi_status_tx.fifo_empty,  7'b0, regs.spi_status_tx.fifo_full_x4, 7'b0, regs.spi_status_tx.fifo_full }; // read-only
          SPI_DATA_TX_OFFSET           : rd_data <= regs.spi_data_tx;
          SPI_DATA_RX_OFFSET           : rd_data <= { 24'b0, fifo_in_rd_data };
          SPI_DATA_RX_x4_OFFSET        : rd_data <= fifo_in_rd_data_x4;
          default: rd_ack <= 1'b0;
        endcase
      end
    end
  end

  always_ff @(posedge i_clk) begin
    rd_en_old <= i_rd_en;
  end 
  
  assign o_rd_data = rd_data;

endmodule

`endif
