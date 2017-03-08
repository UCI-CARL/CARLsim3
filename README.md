<div align="center">
	<img src="http://socsci.uci.edu/~jkrichma/CARL-Logo-small.jpg" width="300"/>
</div>

# CARLsim 3

[![Build Status](https://travis-ci.org/UCI-CARL/CARLsim3.svg?branch=master)](https://travis-ci.org/UCI-CARL/CARLsim3)
[![Coverage Status](https://coveralls.io/repos/github/UCI-CARL/CARLsim3/badge.svg?branch=master)](https://coveralls.io/github/UCI-CARL/CARLsim3?branch=master)
[![Docs](https://img.shields.io/badge/docs-v3.1.2-blue.svg)](http://uci-carl.github.io/CARLsim3)
[![Google group](https://img.shields.io/badge/Google-Discussion%20group-blue.svg)](https://groups.google.com/forum/#!forum/carlsim-snn-simulator)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

CARLsim is an efficient, easy-to-use, GPU-accelerated library for simulating large-scale spiking neural network (SNN) models with a high degree of biological detail. CARLsim allows execution of networks of Izhikevich spiking neurons with realistic synaptic dynamics on both generic x86 CPUs and standard off-the-shelf GPUs. The simulator provides a PyNN-like programming interface in C/C++, which allows for details and parameters to be specified at the synapse, neuron, and network level.

The present release, CARLsim 3, builds on the efficiency and scalability of earlier releases (Nageswaran et al., 2009; Richert et al., 2011). The functionality of the simulator has been greatly expanded by the addition of a number of features that enable and simplify the creation, tuning, and simulation of complex networks with spatial structure. 
New features include:
- improved platform compatibility (Linux, Mac OS X, and Windows)
- real-time and offline data analysis tools
- a more complete STDP implementation which includes dopaminergic neuromodulation
- an automated parameter tuning interface that utilizes evolutionary algorithms to construct functional SNNs
- a test suite for functional code verification
- an exhaustive User Guide and Tutorials

If you use CARLsim 3 in a scholarly publication, please cite as follows:
> Beyeler, M., Carlson, K.D., Chou, T.-S., Dutt, N., Krichmar, J.L. (2015).
> CARLsim 3: A user-friendly and highly optimized library for the creation of neurobiologically
> detailed spiking neural networks.
> Proceedings of the International Joint Conference on Neural Networks, [doi:10.1109/IJCNN.2015.7280424](http://dx.doi.org/10.1109/IJCNN.2015.7280424)

Or use the following bibtex:
```
@inproceedings{CARLsim3,
	author = {M. Beyeler and K. D. Carlson and T.-S. Chou and N. Dutt and J. L. Krichmar}, 
 	booktitle = {2015 International Joint Conference on Neural Networks (IJCNN)}, 
	title = {{CARL}sim 3: {A} user-friendly and highly optimized library for the creation of neurobiologically detailed spiking neural networks}, 
	year = {2015}, 
	pages = {1-8}, 
	doi = {10.1109/IJCNN.2015.7280424},
	url = {http://dx.doi.org/10.1109/IJCNN.2015.7280424},
	ISSN = {2161-4393}, 
	month = {July}
}
```



## Installation

Detailed instructions for installing the latest stable release of CARLsim on Mac OS X / Linux / Windows
can be found in our [User Guide](http://uci-carl.github.io/CARLsim3/ch1_getting_started.html).

In brief (OS X/Linux):

1. Fork CARLsim 3 by clicking on the [`Fork`](https://github.com/UCI-CARL/CARLsim3#fork-destination-box) box
   in the top-right corner.

2. Clone the repo, where `YourUsername` is your actual GitHub user name:
   ```
   $ git clone --recursive https://github.com/YourUsername/CARLsim3
   $ cd CARLsim3
   ```
   Note the `--recursive` option: It will make sure Google Test gets installed.

3. Choose between stable release and latest development version:
   - For the stable release, use the `stable` branch:
     ```
     $ git checkout stable
     ```
   - For the latest development branch, you are already on the right branch (`master`).

4. Consider your options: You can choose the installation directory and whether you want GPU support.
   - Installation directory: By default, the CARLsim library lives in `/usr/local/lib`, and CARLsim
     include files live in `/usr/local/include/carlsim`.
     You can overwrite these by exporting an evironment variable called `CARLSIM3_INSTALL_DIR`:
     ```
     $ export CARLSIM3_INSTALL_DIR=/path/to/your/preferred/dir
     ```
     
   - GPU support: By default, CARLsim comes with CUDA support. Obviously, this requires CUDA to be installed
     first. If you want to run CARLsim without GPU support, you need to export an environment variable
     called `CARLSIM3_NO_CUDA` and set it to `1`:
     ```
     $ export CARLSIM3_NO_CUDA=1
     ```

6. Make and install:
   ```
   $ make -j4
   $ sudo -E make install
   ```
   Note the `-E` flag, which will cause `sudo` to remember any environment variables you set above
   (such as `CARLSIM3_INSTALL_DIR` and `CARLSIM3_NO_CUDA`).

7. In order to make sure the installation was successful, you can run the regression suite:

   ```
   $ make test
   $ ./carlsim/test/carlsim_tests
   ```
   
8. Start your own project! The "Hello World" project is a goot starting point for this.
   Make sure it runs:

   ```
   $ cd projects/hello_world
   $ make
   $ ./hello_world
   ```

On Windows 7/10: Simply download the code and open/run the "Hello World" project file
`projects\hello_world\hello_world.vcxproj`.


## Prerequisites

CARLsim 3.1 comes with the following requirements:
- (Windows) Microsoft Visual Studio 2012 or higher.
- (optional) CUDA Toolkit 5.0 or higher. For platform-specific CUDA installation instructions, please navigate to 
  the [NVIDIA CUDA Zone](https://developer.nvidia.com/cuda-zone).
  This is only required if you want to run CARLsim in `GPU_MODE`. Make sure to install the 
  CUDA samples, too, as CARLsim relies on the file helper_cuda.h.
- (optional) A GPU with compute capability 2.0 or higher. To find the compute capability of your device please 
  refer to the [CUDA article on Wikipedia](http://en.wikipedia.org/wiki/CUDA).
  This is only required if you want to run CARLsim in `GPU_MODE`.
- (optional) MATLAB R2014a or higher. This is only required if you want to use the Offline Analysis Toolbox (OAT).

As of CARLsim 3.1 it is no longer necessary to have the CUDA framework installed. However, CARLsim development 
will continue to focus on the GPU implementation.

The current release has been tested on the following platforms:
- Windows 7, 10
- Ubuntu 12.04, 12.10, 13.04, 13.10, 14.04, 16.04
- Arch Linux
- CentOS 6
- OpenSUSE 13.1
- Mac OS X
