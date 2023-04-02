# rootConvertPixieNet

## Overview
The `rootConvertPixieNet` tool is a user-friendly component of the Pixie-Net Nuclear Spectroscopy Toolkit designed to convert raw XIA Pixie-Net data files into a ROOT TTree format. This conversion facilitates further analysis and visualization of nuclear spectroscopy data. The tool is part of a senior class project led by students and supported by faculty advisors.

## Usage
To use the `rootConvertPixieNet` tool, follow these steps:

1. Clone the Pixie-Net Nuclear Spectroscopy Toolkit repository to your local machine.
2. Place your raw .dat files in the same directory as the `rootConvertPixieNet.cpp` and `rootConvertPixieNet.h` files.
3. Compile the `rootConvertPixieNet.cpp` file using the following command:

$ make clean && make

4. Execute the compiled program with your input .dat file as an argument:

$ ./rootConvertPixieNet input.dat

The program will process the input file, convert the data into a ROOT TTree format, and save the output in a .root file with the same name as the input file.

## Output
Upon successful execution, the `rootConvertPixieNet` tool will generate a .root file containing a TTree with the following branches:

- `eventHitCount`: Number of hits in the event.
- `adcEnergy`: Array of ADC energy values for each hit in the event.
- `hitTime`: Array of hit times for each hit in the event.
- `channel`: Array of channel numbers for each hit in the event.

These branches can be used for further analysis and visualization of the nuclear spectroscopy data using the ROOT framework.

