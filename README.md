# CARLsim 3

[![GitHub tag](https://img.shields.io/github/tag/strongloop/express.svg)]()
[![Docs](https://img.shields.io/badge/docs-v3.1.2-green.svg)]
[![Google group](https://img.shields.io/badge/Google-Discussion%20group-lightgrey.svg)](https://groups.google.com/d/forum/opencv-python-blueprints)]
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)]

CARLsim is an efficient, easy-to-use, GPU-accelerated library for simulating large-scale spiking neural network (SNN) models with a high degree of biological detail. CARLsim allows execution of networks of Izhikevich spiking neurons with realistic synaptic dynamics on both generic x86 CPUs and standard off-the-shelf GPUs. The simulator provides a PyNN-like programming interface in C/C++, which allows for details and parameters to be specified at the synapse, neuron, and network level.

The present release, CARLsim 3, builds on the efficiency and scalability of earlier releases (Nageswaran et al., 2009; Richert et al., 2011). The functionality of the simulator has been greatly expanded by the addition of a number of features that enable and simplify the creation, tuning, and simulation of complex networks with spatial structure. 
New features include:
- improved platform compatibility (Linux, Mac OS X, and Windows)
- real-time and offline data analysis tools
- a more complete STDP implementation which includes dopaminergic neuromodulation
- an automated parameter tuning interface that utilizes evolutionary algorithms to construct functional SNNs
- a test suite for functional code verification
- an exhaustive User Guide and Tutorials


## Installation

Detailed installation instructions for Mac OS X / Linux / Windows can be found in
our [User Guide](http://www.socsci.uci.edu/~jkrichma/CARLsim/doc/ch1_getting_started.html).

In brief (OS X/Linux):

1. Download the latest CARLsim release:
```
$ git clone https://github.com/UCI-CARL/CARLsim3
```

2. Create a configuration file from the sample:
```
$ cd CARLsim3
$ cp user.mk.sample user.mk
```

3. Edit the configuration file.
  - If you do not have an NVIDIA GPU and want to run CARLsim in `CPU_MODE` only,
    set `CPU_ONLY=1`. Skip to Step 4.
  - Else, look up the [compute capability](https://en.wikipedia.org/wiki/CUDA#GPUs_supported)
    of your NVIDIA GPU (`$ nvidia-smi`), and update `CUDA_MAJOR_NUM` and 
    `CUDA_MINOR_NUM` accordingly.
    For example, Maxwell devices have `CUDA_MAJOR_NUM`=5 and `CUDA_MINOR_NUM`
    either 0, 1, or 2.
  - Look up the version of your CUDA toolkit (`$ nvcc --version`), and update
    `CARLSIM_CUDAVER` accordingly.

4. Make and install:
```
$ make -j4
$ sudo make install
```

5. Make sure installation was successful by running the "Hello World" example:
```
$ cd projects/hello_world
$ make
$ ./hello_world
```

On Windows 7: Simply download the code and open/run the "Hello World" project file
`projects\hello_world\hello_world.vcxproj`.
