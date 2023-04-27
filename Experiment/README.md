# rootConvertPixieNet

## Overview
The `rootConvertPixieNet` tool is a component of the Pixie-Net Nuclear Spectroscopy Toolkit designed to convert raw XIA Pixie-Net data files into a ROOT TTree format. Developed as part of a senior class project led by students and supported by faculty advisors, this conversion facilitates further analysis and visualization of nuclear spectroscopy data.

## Usage
To use the `rootConvertPixieNet` tool, follow these steps:

1. Clone the Pixie-Net Nuclear Spectroscopy Toolkit repository to your local machine.
2. Place your raw .dat files in the same directory as the `rootConvertPixieNet.cpp` and `rootConvertPixieNet.h` files.
3. Compile the `rootConvertPixieNet.cpp` file using the following command:

$ make clean && make

4. Execute the compiled program with your input .dat file as an argument:

$ ./rootConvertPixieNet PixieNetOutput.dat

The program will process the input file, convert the data into a ROOT TTree format, and save the output in a .root file with the same name as the input file. In this case, that would be PixieNetOutput.root

## Output
Upon successful execution, the `rootConvertPixieNet` tool will generate a .root file containing a TTree with the following branches:

- `eventHitCount`: Number of hits in the event.
- `adcEnergy`: Array of ADC energy values for each hit in the event.
- `hitTime`: Array of hit times for each hit in the event.
- `channel`: Array of channel numbers for each hit in the event.

These branches can be used for further analysis and visualization of the nuclear spectroscopy data using the ROOT framework.

## Analysis
Now that data has been stored as a TTree, histograms of branches may be drawn to analyze data collected with the Pixie-Net. This may be done from the command line during testing, but we recommend writing a macro to automate this process. plot.cpp is provided as an example macro.  From the ROOT command line, the following commands may be used to generate an adc energy spectrum.  Please note that this is an example.  More analysis should be performed to extract as much information about our sample as possible.

$ TFile *_file0 = TFile::Open("PixieNetOutput.root")

$ TTree *tree = (TTree *)_file0->Get("data")

$ tree->Draw("adcEnergy[0]>>h1(Number of Bins, Start Bin, End Bin)", "", "")

In the example, Number of Bins, Start Bin, and End Bin will all be numerical values.  Please note that in the second command, we are defining the pointer `tree` because my version of ROOT (and yours if you are using the MSU Advanced Lab's Manjaro machine) protects the name `data.`
