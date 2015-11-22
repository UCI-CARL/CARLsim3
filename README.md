README for CARLsim SNN simulator
-------------------------------------------------------------------------------

CARLsim is available from: http://socsci.uci.edu/~jkrichma/CARLsim/.

For a list of all people who have contributed to this project, please refer to 
AUTHORS.

For more detailed installation instructions, please refer to INSTALL.

For a log of source code changes, refer to CHANGELOG.

For a description of added features please refer to the RELEASE_NOTES.

### REQUIREMENTS

For full support of all CARLsim features, the following hardware and software
requirements should be met.

**Operating System**: Windows 7, Mac OS X or Linux (for a complete list of
supported OS’s see the INSTALL file)

**Software**: NVIDIA CUDA drivers/toolkits/SDKs version 5.0 and up should be
installed and configured correctly.
However, CARLsim 3.1 also ships with a CPU-only mode.
On Windows, Microsoft Visual Studio 2012 or newer is required.

**Hardware**: An NVIDIA CUDA-capable GPU device with a compute capability of 2.0
or higher is required.
However, CARLsim 3.1 also ships with a CPU-only mode.

### QUICKSTART INSTALLATION
**For Linux**:
Set the required environment variables in your ~/.bashrc file.
They are described below:

`CARLSIM_LIB_DIR`: desired installation location of the CARLsim library
`CUDA_INSTALL_PATH`:  location of CUDA installation
`CARLSIM_CUDAVER`: CUDA driver/toolkit version number (major only e.g. 6.5 is 6.0)
`CUDA_MAJOR_NUM`: Major number of CUDA GPU device
`CUDA_MINOR_NUM`: Minor number of CUDA GPU device
`CPU_ONLY`: Whether to use CPU-only mode (off by default)

You can copy and paste the below code into your .bashrc file replacing their
values with values appropriate for your system.

```
# set CARLsim variables
export CARLSIM_LIB_DIR=/opt/CARL/CARLsim
export CUDA_INSTALL_PATH=/usr/local/cuda
export CARLSIM_CUDAVER=6
export CUDA_MAJOR_NUM=3
export CUDA_MINOR_NUM=5
export CPU_ONLY=0
```

Enter the unzipped CARLsim directory (same directory in which this README is
located) and run the following:

```
make && sudo make install
```

This will compile and install the CARLsim library.

Note: ‘sudo’ is only necessary when installing CARLsim in a location other than
your home directory.

**For Windows**:

On the Windows platform, CARLsim 3 includes a solution file and project files for
example and tutorial networks as well as the regression suite.
For more information, please see \ref ch11_regression_suite.

The solution file was created by Visual Studio (VS) 2012 with CUDA 5.5.
If you happen to use the same version, <tt>Build Solution</tt> in VS 2012 will
generate all executables (.exe) and static library (.lib) for you.
Before buiding the solution or projects, please make sure <tt>Configuration</tt>
is set to <tt>x64</tt> and <tt>Release</tt> for example executables and <tt>Debug</tt> for
regression suite. For higher version of VS, <tt>CARLsim.sln</tt> and <tt>.vcxproj </tt> will be
automatically upgraded by VS. For higher version of CUDA toolkit, please replace <tt>5.5</tt>
with <tt>YOUR_CUDA_VERSION</tt> in every <tt>.vcxproj</tt> file and add them back to
<tt>CARLsim.sln</tt>.


### SOURCE CODE DIRECTORY DESCRIPTION
Below is the directory layout of the CARLsim source code.

<pre>
├── carlsim                       # CARLsim source code folder
│   ├── connection_monitor          # Utility to record synaptic data
│   ├── group_monitor               # Utility to record neuron group data
│   ├── interface                   # CARLsim interface (public user interface)
│   ├── kernel                      # CARLsim core functionality
│   ├── server                      # Utility to implement real-time server functionality
│   ├── spike_monitor               # Utility to record neuron spike data
│   └── test                        # Google test regression suite tests
├── doc                           # CARLsim documentation generation folder
│   ├── html                        # Generated documentation in html
│   └── source                      # Documentation source code
├── projects                      # User projects directory
│   └── hello_world                 # Project template for new users
└── tools                         # CARLsim tools that are not built-in
    ├── ecj_pti                     # Automated parameter-tuning interface using ECJ
    ├── eo_pti                      # Automated parameter-tuning interface using EO (deprecated)
    ├── offline_analysis_toolbox    # Collection of MATLAB scripts for data analysis
    ├── simple_weight_tuner         # Simple weight parameter-tuning tool
    ├── spike_generators            # Collection of input spike generation tools
    ├── stopwatch                   # A stopwatch for measuring execution (wall-clock) time
    └── visual_stimulus             # Collection of MATLAB/CARLsim tools for visual stimuli
</pre>


* Main directory - contains the Makefile, documentation files, and other
directories.

* carlsim - contains the source code of CARLsim.

* doc - location of user-guide, tutorial, and source code for documentation.

* projects - meant to contain user’s CARLsim projects, all it is not
necessary to place project code here as long as the user.mk is configured
correctly and the CARLsim library is installed.

* tools - contains additional tools to be used with CARLsim.


### Documentation
A Doxygen-based User Guide and Tutorial have already been generated and can
be found in doc/html as well as at http://www.socsci.uci.edu/~jkrichma/CARLsim/doc.
The local documentation can be accessed by opening doc/html/index.html in a Web
browser.
In Linux, the documentation can be generated by entering the ‘doc’ directory and
typing `./makedoc`.


### Using the CARLsim testing framework
CARLsim uses the googletest framework v1.7 for testing. For more information
on googltest please visit the website at https://code.google.com/p/googletest/.
For a quick primer, visit: https://code.google.com/p/googletest/wiki/Primer.

After google tests has been installed:

1) From the carlsim/test directory of the CARLsim installation, type:

```
make carlsim_tests
```

which compiles the carlsim tests

2) To run the tests, run ‘carlsim_tests’:

```
./carlsim_tests
```

This runs the CARLsim tests which should all pass. If they do not pass,
you may have inadvertantly modified the source code and should probably
redownload CARLsim.

3) To delete all testing output files type:

...
make distclean
...

### FAQ
1. **Why do I need to have CUDA installed for CARLsim’s CPU mode?**

You don't! Starting with CARLsim 3.1, it is no longer necessary to have
the CUDA framework installed. However, CARLsim will continue to focus
on the GPU implementation.


2. **When will you guys introduce multi-GPU support?**

You can already launch separate CARLsim instances simultaneously on
multiple GPUs.
However, full multi-GPU support will be introduced in CARLsim 4.


3. **When are you going to support feature X / fix bug Y?**

We continuously strive to make CARLsim better. Over the years our
development team has grown significantly, and we do not plan to change
that in the near future. If you sign up for our 
[mailing list](https://department-lists.uci.edu/mailman/listinfo/ics-carlsim), 
you will be the first to hear about future releases.


4. **I’m having trouble installing CUDA in Linux. Any tips?**

As of the writing of this README (2/2015) the CUDA driver/toolkit/SDK are all
installed with the same installation script/package.  Once ‘./deviceQuery’
returns valid information about your runtime and driver CUDA installation, you
are in business. There are often problems with NVIDIA driver conflicts and
other video drivers in a Linux install. Often times people purge all NVIDIA
packages before attempting to install CUDA.

Usually you have to add a few key environment variables to your .bashrc file.
Below I set the CUDA_HOME variable to where CUDA is installed. I then set my
LD_LIBRARY_PATH variable to the CUDA_HOME/lib64 directory. Finally, I point to
the location of the CUDA tools/compiler (nvcc) by setting the PATH variable to
the CUDA_HOME/bin directory.

```
# set CUDA variables
export CUDA_HOME=/usr/local/cuda
export LD_LIBRARY_PATH=${CUDA_HOME}/lib64
export PATH=$PATH:${CUDA_HOME}/bin
```


5. **How do I install Google Tests?**

In general you should:

1)  Download google tests and unzip them to a location of your choice.

2)  Edit the environment variables GTEST_DIR and GTEST_LIB_DIR
    found in test/gtest.mk. GTEST_DIR should point to the location of the
    unzipped google test source code that you  downloaded. GTEST_LIB_DIR
    should point to the location you want the compiled google test libraries
    to which you will be linking should be installed.

