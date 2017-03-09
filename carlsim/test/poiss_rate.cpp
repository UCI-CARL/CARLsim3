#include "gtest/gtest.h"
#include "carlsim_tests.h"

#include <carlsim.h>

// trigger all UserErrors
TEST(PoissRate, constructDeath) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	EXPECT_DEATH({PoissonRate poiss(0);},""); // nNeur==0
	EXPECT_DEATH({PoissonRate poiss(-1);},""); // nNeur<0

#ifdef __NO_CUDA__
	// can't allocate on GPU in CPU-only mode
	EXPECT_DEATH({PoissonRate poiss(10, true);},"");
#endif
}

// testing getRate(neurId)
TEST(PoissRate, getRateNeurId) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	int nNeur = 100;
#ifdef __NO_CUDA__
	for (int onGPU=0; onGPU<=0; onGPU++) {
#else
	for (int onGPU=0; onGPU<=1; onGPU++) {
#endif
		PoissonRate rate(nNeur,onGPU==true);

		for (int i=0; i<nNeur; i++) {
			EXPECT_FLOAT_EQ(rate.getRate(i), 0.0f);
		}
	}	
}

TEST(PoissRate, getNumNeurons) {
	PoissonRate* rate = NULL;

	for (int numNeur=1; numNeur<=30; numNeur+=5) {
		rate = new PoissonRate(numNeur, false);
		EXPECT_EQ(rate->getNumNeurons(), numNeur);
		delete rate;
	}
}

// testing getRates vector
TEST(PoissRate, getRates) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	int nNeur = 100;
#ifdef __NO_CUDA__
	for (int onGPU=0; onGPU<=0; onGPU++) {
#else
	for (int onGPU=0; onGPU<=1; onGPU++) {
#endif
		PoissonRate rate(nNeur,true==onGPU);
		std::vector<float> ratesVec = rate.getRates();
		EXPECT_TRUE(rate.isOnGPU() == onGPU);

		bool isSizeCorrect = ratesVec.size() == nNeur;
		EXPECT_TRUE(isSizeCorrect);

		if (isSizeCorrect) {
			for (int i=0; i<nNeur; i++) {
				EXPECT_FLOAT_EQ(ratesVec[i], 0.0f);
			}
		}

		if (onGPU) {
			// in GPU mode, can't access CPU pointers
			EXPECT_DEATH({rate.getRatePtrCPU();},"");
		} else {
			// in CPU mode, can't access GPU pointers
			EXPECT_DEATH({rate.getRatePtrGPU();},"");
		}
	}
}

// setting rates with vector
TEST(PoissRate, setRatesVector) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	int nNeur = 100;
#ifdef __NO_CUDA__
	for (int onGPU=0; onGPU<=0; onGPU++) {
#else
	for (int onGPU=0; onGPU<=1; onGPU++) {
#endif
		PoissonRate rate(nNeur,true==onGPU);
		std::vector<float> ratesVecIn;

		for (int i=0; i<nNeur; i++)
			ratesVecIn.push_back(i);

		rate.setRates(ratesVecIn);

		std::vector<float> ratesVecOut = rate.getRates();
		bool isSizeCorrect = ratesVecOut.size() == nNeur;
		EXPECT_TRUE(isSizeCorrect);
		if (isSizeCorrect) {
			for (int i=0; i<nNeur; i++) {
				EXPECT_FLOAT_EQ(ratesVecOut[i], i);
			}
		}
	}
}

// setting all rates to same float
TEST(PoissRate, setRatesFloat) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	int nNeur = 100;
#ifdef __NO_CUDA__
	for (int onGPU=0; onGPU<=0; onGPU++) {
#else
	for (int onGPU=0; onGPU<=1; onGPU++) {
#endif
		PoissonRate rate(nNeur,true==onGPU);
		rate.setRates(42.0f);

		std::vector<float> ratesVec = rate.getRates();
		bool isSizeCorrect = ratesVec.size() == nNeur;
		EXPECT_TRUE(isSizeCorrect);
		if (isSizeCorrect) {
			for (int i=0; i<nNeur; i++) {
				EXPECT_FLOAT_EQ(ratesVec[i], 42.0f);
			}
		}

		// smoke test
		rate.setRate(ALL, 42.0f);
	}
}

// setting single neuron ID to float
TEST(PoissRate, setRateNeurId) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	int nNeur = 100;
	int neurId = 42;
	float neurIdRate = 10.25f;
#ifdef __NO_CUDA__
	for (int onGPU=0; onGPU<=0; onGPU++) {
#else
	for (int onGPU=0; onGPU<=1; onGPU++) {
#endif
		PoissonRate rate(nNeur,true==onGPU);
		rate.setRate(neurId,neurIdRate);

		std::vector<float> ratesVec = rate.getRates();
		bool isSizeCorrect = ratesVec.size() == nNeur;
		EXPECT_TRUE(isSizeCorrect);

		if (isSizeCorrect) {
			for (int i=0; i<nNeur; i++) {
				if (i!=neurId) {
					EXPECT_FLOAT_EQ(ratesVec[i], 0.0f);
				} else {
					EXPECT_FLOAT_EQ(ratesVec[neurId],neurIdRate);
				}
			}
		}
	}
}

//! \NOTE: There is no good way to further test PoissonRate and in a CARLsim environment. Running the network twice
//! will not reproduce the same spike train (because of the random seed). Comparing CPU mode to GPU mode will not work
//! because CPU and GPU use different random seeds. Comparing lambda in PoissonRate.setRate(lambda) to the one from
//! SpikeMonitor.getNeuronMeanFiringRate() will not work because the Poisson process has standard deviation == lambda.
TEST(PoissRate, runSim) {
	// \TODO test CARLsim integration
	// \TODO use cuRAND
}