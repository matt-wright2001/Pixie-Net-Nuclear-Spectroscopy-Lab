# rootConvertPixieNet

## Overview
The rootConvertPixieNet project is a user-friendly tool designed to convert raw XIA Pixie-Net data files into a ROOT TTree format. This conversion facilitates further analysis and visualization of nuclear spectroscopy data. The project is part of a senior class project led by students and supported by faculty advisors.

## Authors
- **Students**: M.S. Wright, B.R. Clark, D.C. Heson
- **Faculty Advisors**: B.P. Crider, J.A. Winger

## Prerequisites
To use this tool, you must have the following software installed on your system:
- [ROOT](https://root.cern.ch/) (C++ data analysis framework)

## Usage
1. Clone this repository to your local machine.
3. Compile the `rootConvertPixieNet.cpp` file using the following command:

$ make clean && make


4. Execute the compiled program with your input .dat file as an argument:


The program will process the input file, convert the data into a ROOT TTree format, and save the output in a .root file with the same name as the input file.

## License
This project is open-source and available for research and educational purposes. Please provide appropriate citation and credit to the authors if you use or modify this code for your research or projects.

