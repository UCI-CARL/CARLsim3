##----------------------------------------------------------------------------##
##
##   CARLsim3 Tests
##   --------------
##
##   Authors:   Michael Beyeler <mbeyeler@uci.edu>
##              Kristofor Carlson <kdcarlso@uci.edu>
##
##   Institute: Cognitive Anteater Robotics Lab (CARL)
##              Department of Cognitive Sciences
##              University of California, Irvine
##              Irvine, CA, 92697-5100, USA
##
##   Version:   03/04/2017
##
##----------------------------------------------------------------------------##


#------------------------------------------------------------------------------
# CARLsim3 Test Files
#------------------------------------------------------------------------------

GTEST_DIR := external/googletest
GTEST_INC := -I$(GTEST_DIR)/include
GTEST_LD := -L$(GTEST_DIR)/build -lgtest 

test_dir := carlsim/test
test_inc_files := $(wildcard $(test_dir)/*.h)
test_cpp_files := $(wildcard $(test_dir)/*.cpp)
test_target := $(test_dir)/carlsim_tests
targets += $(test_target)


#------------------------------------------------------------------------------
# CARLsim3 Targets and Rules
#------------------------------------------------------------------------------

.PHONY: test $(test_target)

test: $(test_target)

$(test_target): $(test_cpp_files) $(test_inc_files)
	$(NVCC) $(CARLSIM3_INC) $(GTEST_INC) $(CARLSIM3_LD) $(GTEST_LD) $(NVCCFL) $(test_cpp_files) -o $@
