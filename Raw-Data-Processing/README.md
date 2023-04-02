# rootConvertPixieNet

## Overview
This repository contains a user-friendly toolset designed to analyze data produced in Mississippi State University's (MSU) Department of Physics and Astronomy Intermediate/Advanced Laboratory Nuclear Spectroscopy experiment. Analysis tools include rootConvertPixieNet used to convert raw XIA Pixie-Net data files into a ROOT TTree format facilitating further analysis and visualization of nuclear spectroscopy data, ___ used to perform three dimensional Gaussian fits necessary for coincidence measurements, as well as prewritten BASH scripts to automate Pixie-Net operation. This work is part of a student class project supported by faculty advisors.

## Author(s)
- **Students**: M.S. Wright, B.R. Clark, D.C. Heson
- **Faculty Advisors**: B.P. Crider, J.A. Winger

## Prerequisites
To use this toolset, you must have the following software installed on your system:
- [ROOT](https://root.cern.ch/) (C++ data analysis framework)

## Usage
1. Clone this repository to your local machine.
2. Place your raw .dat files in the same directory as the `rootConvertPixieNet.cpp` and `rootConvertPixieNet.h` files.
3. Compile the `rootConvertPixieNet.cpp` file using the following command:

$ make clean && make

4. Execute the compiled program with your input .dat file as an argument:

The program will process the input file, convert the data into a ROOT TTree format, and save the output in a .root file with the same name as the input file.

## License
This project is open-source and available for research and educational purposes. Please provide appropriate citation and credit to the authors if you use or modify this code for your research or projects.

## Resources
As this project is primarily intended for undergraduate physics majors, a number of links to helpful resources are provided below.
1. Updated on a yearly basis, Columbia University's NEVIS Laboratory ROOT tutorial provides a structured introduction to ROOT.  Tutorials are provided in 2 paths, Python and C++.  For operation of tools in this repository, C++ is recommended.
[NEVIS ROOT Tutorials](https://www.nevis.columbia.edu/~seligman/root-class/)

2. The Physics Matters YouTube channel provides easy to follow ROOT video tutorials.
[CERN ROOT Tutorials Playlist](https://www.youtube.com/playlist?list=PLLybgCU6QCGWLdDO4ZDaB0kLrO3maeYAe)
