# rootConvertPixieNet

## Overview
rootConvertPixieNet is used to convert raw data files produced by the XIA Pixie-Net into a ROOT TTree for further analysis and visualization of nuclear spectroscopic data. This program takes .dat files as input and outputs .root files. 


## Usage
1. Clone this repository to your local machine.
2. Place your raw .dat files in the same directory as the `rootConvertPixieNet.cpp` and `rootConvertPixieNet.h` files.
3. Compile the `rootConvertPixieNet.cpp` file using the following command:

$ make clean && make

4. Execute the compiled program with your input .dat file as an argument:

$ ./rootConvertPixieNet input.dat

The program will process the input file, convert the data into a ROOT TTree format, and save the output in a .root file with the same name as the input file.
