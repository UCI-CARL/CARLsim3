# CARLsim-ECJ Parameter Tuning Interface (PTI)

The CARLsim-ECJ PTI is a small library that allows you to use the ECJ evolutionary computation 
toolkit to tune the parameter of SNN models built with CARLsim3.

This is an optionally auxiliary library.  It is not installed by default with CARLsim3, but you 
must have CARLsim3 install in order to use the PTI.


## Installation

Detailed instructions for installing the CARLsim-ECJ PTI on can be found 
in our [User Guide](https://uci-carl.github.io/CARLsim3/ch10_ecj.html).  
Mac OS X and Linux are the only supported platforms at this time.

In brief:

1. Make sure you have a C++ compiler, a recent Java SDK (7 or higher), and Ant 1.8 or higher installed.

2. Install CARLsim3.  Instructions are found in the CARLsim3 README file, or 
the [User's Guide](https://uci-carl.github.io/CARLsim3/ch1_getting_started.html).
   - If you installed %CARLsim into a non-default location, make sure your <tt>CARLSIM3_INSTALL_DIR</tt> 
environment variable points to the root directory of the %CARLsim installation.

3. Ensure that the Jar file for ECJ version 23 or higher is installed.
    - You can download the latest stable release of ECJ from https://cs.gmu.edu/~eclab/projects/ecj/
    - By default, CARLsim-ECJ assumes that ECJ will be installed to <tt>/opt/ecj/jar/ecj.23.jar</tt>. 
If the path to your ECJ installation is different, you'll want to customize the <tt>ECJ_DIR</tt> 
environment variable of your CARLsim3 <tt>configure.mk</tt> file (this can be found in your 
CARLsim3 installation's include directory).

4. Consider your options: By default, the CARLsim-ECJ PTI library lives in 
<tt>/opt/CARL/carlsim_ecj_pti/</tt>.  You can override this by customizing the <tt>ECJ_PTI_DIR</tt> 
variable in your <tt>configure.mk</tt>.

5. Make and install:
```bash
 $ make
 $ sudo make install
```

6. If you have already installed `gtest` in the CARLsim3 source tree, you can run the unit tests for 
CARLsim-ECJ PTI's C++ components like so:
```bash
 $ cd test
 $ make
 $ ./pti_test
```

## Using the PTI

A discussion of how to use CARLsim3 with ECJ can be found in the 
[PTI tutorial](https://uci-carl.github.io/CARLsim3/tut7_pti.html), and a complete 
description of the features we've added to ECJ is found in the 
[User's Guide](https://uci-carl.github.io/CARLsim3/ch10_ecj.html).

For any other questions on buildind and using evolutionary algorithms with ECJ,
we refer you https://cs.gmu.edu/~eclab/projects/ecj/, and especially to the 
excellent [ECJ Manual](https://cs.gmu.edu/~eclab/projects/ecj/docs/manual/manual.pdf).