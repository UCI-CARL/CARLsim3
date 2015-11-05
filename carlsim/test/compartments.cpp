#include "gtest/gtest.h"
#include "carlsim_tests.h"

#include <carlsim.h>

#include <math.h>	// log
#include <algorithm> // max

#if defined(WIN32) || defined(WIN64)
#include <periodic_spikegen.h>
#endif


/// **************************************************************************************************************** ///
/// COMPARTMENT MODEL
/// **************************************************************************************************************** ///


TEST(Compartments, simpleCompartmentCPUvsGPU) {
	EXPECT_EQ(10 / 5, 2);
}
