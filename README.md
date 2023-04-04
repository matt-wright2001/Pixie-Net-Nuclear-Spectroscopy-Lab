# Pixie-Net Nuclear Spectroscopy 

## Overview
This repository contains a comprehensive toolset designed for analyzing data produced in Mississippi State University's (MSU) Department of Physics and Astronomy Intermediate/Advanced Laboratory Nuclear Spectroscopy experiment. The analysis tools include:

- `rootConvertPixieNet`: A program to convert raw XIA Pixie-Net data files into a ROOT TTree format, which facilitates further analysis and visualization of nuclear spectroscopy data.
- `GaussianFitCoincidence`: A program to perform three-dimensional Gaussian fits necessary for coincidence measurements.
- `Co60_Calibration`: A program used to correlate photon energies measured by the detector in adc units to MeV by perfroming a linear fit on Cobalt-60 peaks.  With the cobalt energy peaks known from the available literature, the adc output is calibrated to usable energy units.
- BASH scripts to automate Pixie-Net operation.

This work is part of a student class project supported by faculty advisors.

## Authors
- **Students**: M.S. Wright, B.R. Clark, D.C. Heson
- **Faculty Advisors**: B.P. Crider, J.A. Winger

## Prerequisites
To use these tools, you must have the following software installed on your system:
- [ROOT](https://root.cern.ch/) (C++ data analysis framework)

## License
This project is proudly open-source and available for research and educational purposes. Please provide appropriate citation and credit to the authors if you use or modify this code for your research or projects.

## Resources
As this project is primarily intended for undergraduate physics majors, a number of links to helpful resources are provided below:

1. [Official CERN ROOT Data Analysis Framework Manual](https://root.cern/manual/)
2. Updated on a yearly basis, [Columbia University's NEVIS Laboratory ROOT Tutorial](https://www.nevis.columbia.edu/~seligman/root-class/) provides a structured introduction to ROOT. Tutorials are available in both Python and C++ paths. For operating the tools in this repository, the C++ path is recommended.
3. The Physics Matters [CERN ROOT Tutorials Playlist](https://www.youtube.com/playlist?list=PLLybgCU6QCGWLdDO4ZDaB0kLrO3maeYAe) on YouTube provides easy-to-follow ROOT video tutorials.

