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

#include <verilated.h>
#include <verilated_vcd_c.h>
#include "include_all.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <cstdio> 
#include <getopt.h>

#define PERIOD 10 // nanoseconds

#define _BOLD "\x1b[1m"
#define _END "\x1b[0m"
#define _BLACK "\x1b[30m"
#define _RED "\x1b[31m"
#define _GREEN "\x1b[32m"
#define _YELLOW "\x1b[33m"
#define _BLUE "\x1b[34m"
#define _MAGENTA "\x1b[35m"
#define _CYAN "\x1b[36m"
#define _WHITE "\x1b[37m"

#define DEBUG_ADDR 0x0a000000

#define CPU top->top->soc_top_level->pipe__DOT__cpu

void help() {
    std::cout << "\n" << _BOLD << "Description:\n" << _END
                 "  This program simulates AsteRISC using Verilator.\n"
                 "\n" << _BOLD "Options:\n" << _END
                 "  -i, --imem_file \tPath to the instruction memory initialization file.\n"
                 "  -d, --dmem_file \tPath to the data memory initialization file.\n"
                 "  -I, --imem_depth\tPower of two representing the depth of the instruction memory.\n"
                 "  -D, --dmem_depth\tPower of two representing the depth of the data memory.\n"
                 "  -v, --vcd_file  \tPath to the output VCD file for waveform dumping.\n"
                 "  -l, --log_file  \tPath of the CPU activity log file.\n"
                 "  -p, --print     \tPrint characters written at debug address.\n"
                 "  -f, --fprint    \tPath of the file to save characters written at debug address.\n"
                 "  -h, --help      \tPrint this help message and exit.\n"
                 "\n" << _BOLD "Example:\n" << _END
                 "  ./Vtop --imem_file=imem.hex --dmem_file=dmem.hex --vcd_file=output.vcd --imem_depth=10 --dmem_depth=12\n\n";
}

int count_lines(const std::string& filename) {
    std::ifstream file(filename);
    return std::count(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>(),
                      '\n');
}

int main(int argc, char** argv) {

    int opt;

    std::string imem_file_path;
    std::string dmem_file_path;
    std::string debug_print_file_path = "./log/print.log";
    std::string vcd_file_path = "./waveform.vcd";
    std::string cpu_log_file_path = "./log/cpu_activity.log";

    int p_imem_depth_pw2;
    int p_dmem_depth_pw2;

    bool debug_print = false;
    bool debug_print_save = false;

    static struct option long_options[] = {
        {"imem_file", required_argument, 0, 'i'},
        {"dmem_file", required_argument, 0, 'd'},
        {"imem_depth", required_argument, 0, 'I'},
        {"dmem_depth", required_argument, 0, 'D'},
        {"vcd_file", required_argument, 0, 'v'},
        {"print", no_argument, 0, 'p'},
        {"print_save", required_argument, 0, 's'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "i:d:I:D:v:s:ph", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i':
                imem_file_path = optarg;
                break;
            case 'd':
                dmem_file_path = optarg;
                break;
            case 'I':
                p_imem_depth_pw2 = std::stoi(optarg);
                break;
            case 'D':
                p_dmem_depth_pw2 = std::stoi(optarg);
                break;
            case 'v':
                vcd_file_path = optarg;
                break;
            case 'l':
                cpu_log_file_path = optarg;
                break;
            case 'p':
                debug_print = true;
                break;
            case 's':
                debug_print_save = true;
                debug_print_file_path = optarg;
                break;
            case 'h':
                help();
                return 0;
            default:
                std::cout << _BOLD << _RED << "error: invalid option '" << (char)opt << "'" << _END << std::endl;
                return 1;
        }
    }

    // Check options
    if (imem_file_path.empty() || dmem_file_path.empty()) {
        std::cout << _BOLD << _RED << "error: missing required arguments: ";
        if (imem_file_path.empty()) {
            std::cout << "--imem_file";
        } else {
            std::cout << "--dmem_file";
        }
        std::cout << _END << std::endl;
        help();
        return 1;
    }

    // Print options
    std::cout << std::endl << _BOLD << "Options: " << _END << std::endl;
    std::cout << "  imem_file = " << imem_file_path << std::endl;
    std::cout << "  dmem_file = " << dmem_file_path << std::endl;
    //std::cout << "  imem_depth = 2^" << p_imem_depth_pw2 << std::endl;
    //std::cout << "  dmem_depth = 2^" << p_dmem_depth_pw2 << std::endl;
    std::cout << "  vcd_file_path = " << vcd_file_path << std::endl;
    std::cout << "  cpu_log_file_path = " << cpu_log_file_path << std::endl;
    std::cout << "  debug_print = " << debug_print << std::endl;
    std::cout << "  print_save = " << debug_print_save <<  std::endl;
    if (debug_print_save) {
        std::cout << "  print_save_file = " << debug_print_file_path << std::endl;
    }
    std::cout << std::endl;

    // Initialise Verilator
    Verilated::commandArgs(argc, argv);

    // Get a verilated instance
    Vtop* top = new Vtop;

    // Init clock and reset
    top->clock = 0;
    top->reset = 1;

    // Init imem an dmem
    std::ifstream imem_file(imem_file_path, std::ios::in);
    if (!imem_file.is_open()) {
        std::cout << _BOLD << _RED << "error: could not open imem init file \"" << imem_file_path << "\" " << _END << std::endl;
        return 1;
    }

    std::ifstream dmem_file(dmem_file_path, std::ios::in);
    if (!dmem_file.is_open()) {
        std::cout << _BOLD << _RED << "error: could not open dmem init file \"" << imem_file_path << "\" " << _END << std::endl;
        return 1;
    }

    int imem_lines = count_lines(imem_file_path);
    int dmem_lines = count_lines(dmem_file_path);

    int imem_depth = (1 << top->top->p_imem_depth_pw2);
    int dmem_depth = (1 << top->top->p_dmem_depth_pw2);

    std::cout << _BOLD << "Sanity check: " << _END << std::endl;
    std::cout << "  imem init file size = " << ((imem_lines > imem_depth) ? _RED : _END) << imem_lines << _END << "/" << imem_depth << std::endl;
    std::cout << "  dmem init file size = " << ((dmem_lines > dmem_depth) ? _RED : _END) << dmem_lines << _END << "/" << dmem_depth << std::endl;
    std::cout << std::endl;

    if (imem_lines > imem_depth) {
        std::cout << _BOLD << _RED << "error:" << _END << " " << _RED << "imem init file is too large for the current instruction memory depth" << _END << std::endl;
        return 1;
    }

    if (dmem_lines > dmem_depth) {
        std::cout << _BOLD << _RED << "error:" << _END << " " << _RED << "dmem init file is too large for the current data memory depth" << _END << std::endl;
        return 1;
    }

    // Load imem and dmem
    for (uint32_t i = 0 ; i < imem_depth ; ++i) {
        std::string line;
        if (std::getline(imem_file, line)) {
            std::istringstream iss(line);
            uint32_t value;
            if (iss >> std::hex >> value) {
                top->top->soc_top_level->imem->imem->set_mem_content(i, value);
                //uint32_t mem_content = top->top->soc_top_level->imem->imem->get_mem_content(i);
                //std::cout << std::hex << std::setw(8) << std::setfill('0') << value << std::endl;
            } else {
                std::cout << _BOLD << _RED << "error: failed parsing " << _END << std::endl;
            }
        } else {
            break;
        }
    }

    for (uint32_t i = 0 ; i < dmem_depth ; ++i) {
        std::string line;
        if (std::getline(dmem_file, line)) {
            std::istringstream iss(line);
            uint32_t value;
            if (iss >> std::hex >> value) {
                top->top->soc_top_level->dmem->dmem->set_mem_content(i, value);
            } else {
                std::cout << _BOLD << _RED << "error: failed parsing " << _END << std::endl;
            }
        } else {
            break;
        }
    }

    // Close mem files
    /*imem_file.close();
    dmem_file.close();*/

    // Initialisez le fichier VCD
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open(vcd_file_path.c_str());

    FILE *output_file_stream;
    output_file_stream = fopen(cpu_log_file_path.c_str(), "w");

    // Check if the file stream is valid
    if (output_file_stream == NULL) {
        // Handle error if file cannot be opened
        printf("error: Unable to open file \"%s\" for writing.\n", cpu_log_file_path);
        exit(1);
    }

    FILE *print_save_file_stream;

    if (debug_print_save) {

        print_save_file_stream = fopen(debug_print_file_path.c_str(), "w");

        // Check if the file stream is valid
        if (print_save_file_stream == NULL) {
            // Handle error if file cannot be opened
            printf("error: Unable to open file \"%s\" for writing.\n", debug_print_file_path);
            exit(1);
        }
    }

    std::cout << _BOLD << "Software Print Log: " << _END << std::endl << "  ";
    uint64_t last_instret = 2;

    for (int time = 0; time < 1000000; ++time) {
        top->clock = !top->clock;

        top->eval();
        tfp->dump(time);
        Verilated::timeInc(PERIOD / 2);

        // reset 
        if (time == 6) {
            top->reset = 0;
        }

        bool p_bypass_log = false;

        if (time%2 == 0 && !top->reset) {

            // print execution log
            if (CPU->get_instret() != 0 && CPU->get_instret() != last_instret) {
                last_instret = CPU->get_instret();
                fprintf(output_file_stream, "%0d) pc 0x%08x:\n ", CPU->get_instret(), CPU->get_pc());
                fprintf(output_file_stream, " instr: 0x%08x > %s\n", CPU->get_instr_code(), CPU->get_instr_name().c_str());
                if (CPU->get_br_taken()) {
                    fprintf(output_file_stream, "  -> branch taken\n");
                }
                fprintf(output_file_stream, "  imm       = 0x%08x\n", CPU->get_imm());
                fprintf(output_file_stream, "  rs1 (x%02d) = 0x%08x", CPU->get_rs1_addr(), CPU->get_rs1_data());
                if (p_bypass_log && CPU->get_bp_rs1_ex()) {
                    fprintf(output_file_stream, " (bp ex)\n");
                } else if (p_bypass_log && CPU->get_bp_rs1_ma()) {
                    if (CPU->get_stall_rs1()) {
                        fprintf(output_file_stream, " (stall + ");
                    } else {
                        fprintf(output_file_stream, " (");
                    }
                    fprintf(output_file_stream, "bp ma)\n");
                } else if (p_bypass_log && CPU->get_bp_rs1_wb()) {
                    fprintf(output_file_stream, " (bp wb)\n");
                } else {
                    fprintf(output_file_stream, "\n");
                }
                if (CPU->get_rs2_used()) {
                    fprintf(output_file_stream, "  rs2 (x%02d) = 0x%08x", CPU->get_rs2_addr(), CPU->get_rs2_data());
                    if (p_bypass_log && CPU->get_bp_rs2_ex()) {
                        fprintf(output_file_stream, " (bp ex)\n");
                    } else if (p_bypass_log && CPU->get_bp_rs2_ma()) {
                        if (CPU->get_stall_rs2()) {
                            fprintf(output_file_stream, " (stall + ");
                        } else {
                            fprintf(output_file_stream, " (");
                        }
                        fprintf(output_file_stream, "bp ma)\n");
                    } else if (p_bypass_log && CPU->get_bp_rs2_wb()) {
                        fprintf(output_file_stream, " (bp wb)\n");
                    } else {
                        fprintf(output_file_stream, "\n");
                    }
                }
                if (CPU->get_wb_en()) {
                    fprintf(output_file_stream, "  rd  (x%02d) = 0x%08x\n", CPU->get_wb_addr(), CPU->get_wb_data());  
                }
            }

            if (debug_print) {
                uint32_t dbus_addr = CPU->mem_stage->get_dbus_addr();
                uint32_t dbus_wr_data = CPU->mem_stage->get_dbus_wr_data();
                bool dbus_wr_en = CPU->mem_stage->get_dbus_wr_en();
                if (dbus_addr == DEBUG_ADDR) {
                    //std::cout << "  dbus wr: @ 0x" << std::hex << std::setw(8) << std::setfill('0') << dbus_addr << " = 0x"<< std::hex << std::setw(8) << std::setfill('0') << dbus_wr_data << std::endl;
                    if (dbus_wr_en) {
                        if (dbus_wr_data == '\n') {
                            printf("\n  ");
                        } else {
                            printf("%c", dbus_wr_data);
                        }
                        fprintf(print_save_file_stream, "%c", dbus_wr_data);
                    }
                }
            }

            uint32_t imem_addr = top->top->soc_top_level->imem->imem->get_mem_addr();
            uint32_t imem_rd_data = top->top->soc_top_level->imem->imem->get_mem_rd_data();
            //std::cout << "  imem rd: @" << std::hex << std::setw(8) << std::setfill('0') << imem_addr << " = 0x"<< std::hex << std::setw(8) << std::setfill('0') << imem_rd_data << std::endl;
            if (CPU->get_instr_code() == 0x00100073) { //ebreak
                std::cout << "  ebreak" << std::endl;
                break;
            }
        }
    }

    // Close file streams
    fclose(output_file_stream);
    fclose(print_save_file_stream);

    // Close VCD file
    tfp->close();

    // Clean
    delete top;
    delete tfp;

    return 0;
}
