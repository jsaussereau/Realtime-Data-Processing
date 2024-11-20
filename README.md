# Real-Time Data Processing for Embedded Communicating Systems: a Hardware/Software Co-Design Approach

Embedded systems typically require the transmission of significant amounts of data to small-scale CPUs for applications such as radar signal processing, image processing, and embedded AI. 
Ensuring data integrity during transmission is typically managed using Cyclic Redundancy Check (CRC) algorithms. 
However, achieving real-time CRC calculation and data storage poses challenges, often necessitating large FIFO memories and multiple clock domains. 
These additional resources involve a greater hardware complexity.
This repository presents an approach aimed at synchronizing the CPU frequency with data transmission. 
This enables having a single clock domain and a reduction of power consumption. 
Using hardware/software co-design, it is possible to achieve real-time data storage and CRC calculation without data loss and with a low power consumption. 

## Installation

- Install [Verilator](https://verilator.org/guide/latest/install.html)
- Install [Odatix](https://odatix.readthedocs.io/en/latest/installation/install_from_pypi.html)

## How to use 

Simply run the make command

```bash
make
```
