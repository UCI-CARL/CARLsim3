#include "gtest/gtest.h"
#include "carlsim_tests.h"

#include <carlsim.h>

#if defined(WIN32) || defined(WIN64)
#include <periodic_spikegen.h>
#endif

/// ****************************************************************************
/// compartmental model
/// ****************************************************************************



TEST(COMPARTMENTS, spikeTimesCPUvsData) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	float numModes;

#ifdef __CPU_ONLY__
	numModes = 0;
#else
	numModes = 1;
#endif

	// expected spike times for soma
	int expectSpikeTimeSO[5][7] = {	{148, 188, 243, 339, 514, 690, 866},
						 			{148, 189, 242, 328, 508, 681, 855},
									{148, 188, 240, 328, 506, 679, 853},
									{148, 187, 240, 326, 504, 677, 851},
									{148, 187, 240, 326, 505, 677, 851}};

	int expectSpikeTimeSP[5][7] = {	{148, 188, 243, 339, 515, 691, 866},
	 					 			{148, 189, 243, 329, 509, 681, 856},
	 								{148, 188, 241, 328, 507, 679, 853},
	 								{148, 188, 240, 327, 505, 678, 852},
	 								{148, 188, 240, 326, 506, 677, 852} };

	// \FIXME: GPU side not implemented yet
	numModes = 0;

	for (int inver_timestep = 10; inver_timestep <= 50; inver_timestep += 10) {

		for (int isGPUmode = 0; isGPUmode <= numModes; isGPUmode++) {
			CARLsim* sim = new CARLsim("CUBA.firingRateVsData", isGPUmode ? GPU_MODE : CPU_MODE, SILENT, 0, 42);
			sim->setIntegrationMethod(RUNGE_KUTTA4, inver_timestep);

			int N = 5;

			int s = sim->createGroup("SP soma", N, EXCITATORY_NEURON);
			int d1 = sim->createGroup("SR", N, EXCITATORY_NEURON);
			int d2 = sim->createGroup("SLM", N, EXCITATORY_NEURON);
			int d3 = sim->createGroup("SO", N, EXCITATORY_NEURON);

			sim->setNeuronParameters(s, 550.0f, 2.3330991f, -59.101414f, -50.428886f, 0.0021014998f, -0.41361538f, 24.98698f, -53.223213f, 109.0f);//9 parameter setNeuronParametersCall (RS NEURON) (soma)
			sim->setNeuronParameters(d1, 367.0f, 1.1705916f, -59.101414f, -44.298294f, 0.2477681f, 3.3198094f, 20.274296f, -46.076824f, 24.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
			sim->setNeuronParameters(d2, 425.0f, 2.2577047f, -59.101414f, -25.137894f, 0.32122386f, 0.14995363f, 13.203414f, -38.54892f, 69.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
			sim->setNeuronParameters(d3, 225.0f, 1.109572f, -59.101414f, -36.55802f, 0.29814243f, -4.385603f, 21.473854f, -40.343994f, 21.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)

			sim->setCompartmentParameters(d1, 28.396f, 5.526f);//SR 28 and 5
			sim->setCompartmentParameters(d2, 50.474f, 0.0f);//SLM 50 and 0
			sim->setCompartmentParameters(d3, 0.0f, 49.14f);//SO 0 and 49
			sim->setCompartmentParameters(s, 116.861f, 4.60f);// SP (somatic) 116 and 4

			int gin = sim->createSpikeGeneratorGroup("input", N, EXCITATORY_NEURON);
			sim->connect(gin, s, "one-to-one", RangeWeight(0.0f), 1.0f, RangeDelay(1), RadiusRF(-1));

			sim->setConductances(false);//This forces use of CUBA model.

			sim->compConnect(d2, d1);
			sim->compConnect(d1, s);
			sim->compConnect(s, d3);

			sim->setESTDP(ALL, false);
			sim->setISTDP(ALL, false);

			sim->setupNetwork();

			SpikeMonitor* spikeSP = sim->setSpikeMonitor(s, "DEFAULT"); // put spike times into file
			SpikeMonitor* spikeSR = sim->setSpikeMonitor(d1, "DEFAULT"); // put spike times into file
			SpikeMonitor* spikeSLM = sim->setSpikeMonitor(d2, "DEFAULT"); // put spike times into file
			SpikeMonitor* spikeSO = sim->setSpikeMonitor(d3, "DEFAULT"); // put spike times into file

			PoissonRate in(N);

			in.setRates(0.0f);
			sim->setSpikeRate(gin, &in);//Inactive input group

			spikeSP->startRecording();
			spikeSR->startRecording();
			spikeSLM->startRecording();
			spikeSO->startRecording();
			sim->setExternalCurrent(s, 0);
			sim->runNetwork(0, 100);
			sim->setExternalCurrent(s, 592);
                        sim->runNetwork(0, 400);
			sim->setExternalCurrent(s, 592);
                        sim->runNetwork(0, 400);
			sim->setExternalCurrent(s, 0);
                        sim->runNetwork(0, 100);

			spikeSP->stopRecording();
			spikeSR->stopRecording();
			spikeSLM->stopRecording();
			spikeSO->stopRecording();

			// SP (somatic): expect 8 spikes at specific times
			EXPECT_EQ(spikeSP->getPopNumSpikes(), 7*N);
			if (spikeSP->getPopNumSpikes() == 7*N) {
				// only execute if #spikes matches, otherwise we'll segfault
				std::vector<std::vector<int> > spikeTimeSP = spikeSP->getSpikeVector2D();
				for (int neurId=0; neurId<spikeTimeSP.size(); neurId++) {
					for (int spkT=0; spkT<spikeTimeSP[neurId].size(); spkT++) {
						EXPECT_EQ(spikeTimeSP[neurId][spkT], expectSpikeTimeSP[inver_timestep/10-1][spkT]);
					}
				}
			}

			// SR: expect silent
			EXPECT_EQ(spikeSR->getPopNumSpikes(), 0);

			// SLM: expect silent
			EXPECT_EQ(spikeSLM->getPopNumSpikes(), 0);

			// SO (d3 dendritic): expect 8 spikes at specific times
			EXPECT_EQ(spikeSO->getPopNumSpikes(), 7*N);
			if (spikeSO->getPopNumSpikes() == 7*N) {
				// only execute if #spikes matches, otherwise we'll segfault
				std::vector<std::vector<int> > spikeTimeSO = spikeSO->getSpikeVector2D();
				for (int neurId=0; neurId<spikeTimeSO.size(); neurId++) {
					for (int spkT=0; spkT<spikeTimeSO[neurId].size(); spkT++) {
						// spike times such precede SP by 1 ms
						EXPECT_EQ(spikeTimeSO[neurId][spkT], expectSpikeTimeSO[inver_timestep/10-1][spkT]);
					}
				}
			}

			delete sim;
		}
	}
}



/*!
* \brief testing CARLsim compartments + CUBA CPU vs GPU
*
* This test makes sure that firing rate of CUBA compartmental groups are similar accross CPU and GPU.
*/
TEST(COMPARTMENTS, firingRateCPUvsGPU_CUBA) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	for (int inver_timestep = 10; inver_timestep < 50; inver_timestep += 10) {
		float cpu_firingRate, cpu_firingRate_1, cpu_firingRate_2, cpu_firingRate_3;

		for (int isGPUmode = 0; isGPUmode <= 1; isGPUmode++) {
			CARLsim* sim = new CARLsim("CUBA.firingRateVsData", isGPUmode ? GPU_MODE : CPU_MODE, SILENT, 0, 42);
			sim->setIntegrationMethod(RUNGE_KUTTA4, inver_timestep);

			int N = 5;

			int s = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d1 = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d2 = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d3 = sim->createGroup("excit", N, EXCITATORY_NEURON);

			sim->setNeuronParameters(s, 550.0f, 2.3330991f, -59.101414f, -50.428886f, 0.0021014998f, -0.41361538f, 24.98698f, -53.223213f, 109.0f);//9 parameter setNeuronParametersCall (RS NEURON) (soma)
			sim->setNeuronParameters(d1, 367.0f, 1.1705916f, -59.101414f, -44.298294f, 0.2477681f, 3.3198094f, 20.274296f, -46.076824f, 24.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
			sim->setNeuronParameters(d2, 425.0f, 2.2577047f, -59.101414f, -25.137894f, 0.32122386f, 0.14995363f, 13.203414f, -38.54892f, 69.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
			sim->setNeuronParameters(d3, 225.0f, 1.109572f, -59.101414f, -36.55802f, 0.29814243f, -4.385603f, 21.473854f, -40.343994f, 21.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)

			sim->setCompartmentParameters(d1, 28.396f, 5.526f);//SR 28 and 5
			sim->setCompartmentParameters(d2, 50.474f, 0.0f);//SLM 50 and 0
			sim->setCompartmentParameters(d3, 0.0f, 49.14f);//SO 0 and 49
			sim->setCompartmentParameters(s, 116.861f, 4.60f);// SP (somatic) 116 and 4

			int gin = sim->createSpikeGeneratorGroup("input", N, EXCITATORY_NEURON);
			sim->connect(gin, s, "one-to-one", RangeWeight(0.0f), 1.0f, RangeDelay(1), RadiusRF(-1));

			sim->setConductances(false);//This forces use of CUBA model.

			sim->compConnect(d2, d1);
			sim->compConnect(d1, s);
			sim->compConnect(s, d3);

			sim->setSTDP(d1, false);
			sim->setSTDP(d2, false);
			sim->setSTDP(d3, false);
			sim->setSTDP(s, false);

			sim->setupNetwork();

			SpikeMonitor* SM = sim->setSpikeMonitor(s, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_1 = sim->setSpikeMonitor(d1, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_2 = sim->setSpikeMonitor(d2, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_3 = sim->setSpikeMonitor(d3, "DEFAULT"); // put spike times into file

			PoissonRate in(N);

			in.setRates(0.0f);
			sim->setSpikeRate(gin, &in);//Inactive input group

			SM->startRecording();
			SM_1->startRecording();
			SM_2->startRecording();
			SM_3->startRecording();
			sim->setExternalCurrent(s, 600);
			sim->runNetwork(1, 0);
			SM->stopRecording();
			SM_1->stopRecording();
			SM_2->stopRecording();
			SM_3->stopRecording();

			if (isGPUmode == 0) {
				cpu_firingRate = SM->getPopMeanFiringRate();
				cpu_firingRate_1 = SM_1->getPopMeanFiringRate();
				cpu_firingRate_2 = SM_2->getPopMeanFiringRate();
				cpu_firingRate_3 = SM_3->getPopMeanFiringRate();
			} else {
				EXPECT_FLOAT_EQ(SM->getPopMeanFiringRate(), cpu_firingRate);
				EXPECT_FLOAT_EQ(SM_1->getPopMeanFiringRate(), cpu_firingRate_1);
				EXPECT_FLOAT_EQ(SM_2->getPopMeanFiringRate(), cpu_firingRate_2);
				EXPECT_FLOAT_EQ(SM_3->getPopMeanFiringRate(), cpu_firingRate_3);
			}

			delete sim;
		}
	}
}


/*!
* \brief testing CARLsim compartments + COBA CPU vs GPU
*
* This test makes sure that firing rate of COBA compartmental groups are similar accross CPU and GPU.
*/
TEST(COMPARTMENTS, firingRateCPUvsGPU_COBA) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	for (int inver_timestep = 10; inver_timestep < 50; inver_timestep += 10) {
		float cpu_firingRate, cpu_firingRate_1, cpu_firingRate_2, cpu_firingRate_3;

		for (int isGPUmode = 0; isGPUmode <= 1; isGPUmode++) {
			CARLsim* sim = new CARLsim("CUBA.firingRateVsData", isGPUmode ? GPU_MODE : CPU_MODE, SILENT, 0, 42);
			sim->setIntegrationMethod(RUNGE_KUTTA4, inver_timestep);

			int N = 5;

			int s = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d1 = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d2 = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d3 = sim->createGroup("excit", N, EXCITATORY_NEURON);

			sim->setNeuronParameters(s, 550.0f, 2.3330991f, -59.101414f, -50.428886f, 0.0021014998f, -0.41361538f, 24.98698f, -53.223213f, 109.0f);//9 parameter setNeuronParametersCall (RS NEURON) (soma)
			sim->setNeuronParameters(d1, 367.0f, 1.1705916f, -59.101414f, -44.298294f, 0.2477681f, 3.3198094f, 20.274296f, -46.076824f, 24.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
			sim->setNeuronParameters(d2, 425.0f, 2.2577047f, -59.101414f, -25.137894f, 0.32122386f, 0.14995363f, 13.203414f, -38.54892f, 69.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
			sim->setNeuronParameters(d3, 225.0f, 1.109572f, -59.101414f, -36.55802f, 0.29814243f, -4.385603f, 21.473854f, -40.343994f, 21.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)

			sim->setCompartmentParameters(d1, 28.396f, 5.526f);//SR 28 and 5
			sim->setCompartmentParameters(d2, 50.474f, 0.0f);//SLM 50 and 0
			sim->setCompartmentParameters(d3, 0.0f, 49.14f);//SO 0 and 49
			sim->setCompartmentParameters(s, 116.861f, 4.60f);// SP (somatic) 116 and 4

			int gin = sim->createSpikeGeneratorGroup("input", N, EXCITATORY_NEURON);
			sim->connect(gin, s, "one-to-one", RangeWeight(0.0f), 1.0f, RangeDelay(1), RadiusRF(-1));

			sim->setConductances(true);//This forces use of COBA model.

			sim->compConnect(d2, d1);
			sim->compConnect(d1, s);
			sim->compConnect(s, d3);

			sim->setSTDP(d1, false);
			sim->setSTDP(d2, false);
			sim->setSTDP(d3, false);
			sim->setSTDP(s, false);

			sim->setupNetwork();

			SpikeMonitor* SM = sim->setSpikeMonitor(s, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_1 = sim->setSpikeMonitor(d1, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_2 = sim->setSpikeMonitor(d2, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_3 = sim->setSpikeMonitor(d3, "DEFAULT"); // put spike times into file

			PoissonRate in(N);

			in.setRates(0.0f);
			sim->setSpikeRate(gin, &in);//Inactive input group

			SM->startRecording();
			SM_1->startRecording();
			SM_2->startRecording();
			SM_3->startRecording();
			sim->setExternalCurrent(s, 600);
			sim->runNetwork(1, 0);
			SM->stopRecording();
			SM_1->stopRecording();
			SM_2->stopRecording();
			SM_3->stopRecording();

			if (isGPUmode == 0) {
				cpu_firingRate = SM->getPopMeanFiringRate();
				cpu_firingRate_1 = SM_1->getPopMeanFiringRate();
				cpu_firingRate_2 = SM_2->getPopMeanFiringRate();
				cpu_firingRate_3 = SM_3->getPopMeanFiringRate();
			} else {
				EXPECT_FLOAT_EQ(SM->getPopMeanFiringRate(), cpu_firingRate);
				EXPECT_FLOAT_EQ(SM_1->getPopMeanFiringRate(), cpu_firingRate_1);
				EXPECT_FLOAT_EQ(SM_2->getPopMeanFiringRate(), cpu_firingRate_2);
				EXPECT_FLOAT_EQ(SM_3->getPopMeanFiringRate(), cpu_firingRate_3);
			}
			delete sim;
		}
	}
}

/*!
* \brief testing CARLsim compartments via voltage recording
*
* This test makes sure that firing rate of COBA compartmental groups are similar accross CPU and GPU.
* It also makes sure that soma compartments on CPU and GPU both have similar voltage values every timestep.
*/
/*
TEST(COMPARTMENTS, firingRateCPUvsGPU_COBA_Volt_Rec) {
	::testing::FLAGS_gtest_death_test_style = "threadsafe";

	for (int inver_timestep = 10; inver_timestep < 50; inver_timestep += 10)
	{
		int N = 5;
		int simTime = 100; //1000ms or 1s

		int voltRecordBufferSize = N * inver_timestep * simTime;

		float* CPU_Soma_Volt_Rec;
		float* GPU_Soma_Volt_Rec;

		CPU_Soma_Volt_Rec = new float[voltRecordBufferSize];
		GPU_Soma_Volt_Rec = new float[voltRecordBufferSize];

		float cpu_firingRate, cpu_firingRate_1, cpu_firingRate_2, cpu_firingRate_3;

		for (int isGPUmode = 0; isGPUmode <= 1; isGPUmode++) {
			CARLsim* sim = new CARLsim("CUBA.firingRateVsData", isGPUmode ? GPU_MODE : CPU_MODE, SILENT, 0, 42);
			sim->setTimestep(inver_timestep);

			int s = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d1 = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d2 = sim->createGroup("excit", N, EXCITATORY_NEURON);
			int d3 = sim->createGroup("excit", N, EXCITATORY_NEURON);

			sim->recordVoltage(s, 0);

			sim->setNeuronParameters(s, 550.0f, 2.3330991f, -59.101414f, -50.428886f, 0.0021014998f, -0.41361538f, 24.98698f, -53.223213f, 109.0f);//9 parameter setNeuronParametersCall (RS NEURON) (soma)
			sim->setNeuronParameters(d1, 367.0f, 1.1705916f, -59.101414f, -44.298294f, 0.2477681f, 3.3198094f, 20.274296f, -46.076824f, 24.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
			sim->setNeuronParameters(d2, 425.0f, 2.2577047f, -59.101414f, -25.137894f, 0.32122386f, 0.14995363f, 13.203414f, -38.54892f, 69.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
			sim->setNeuronParameters(d3, 225.0f, 1.109572f, -59.101414f, -36.55802f, 0.29814243f, -4.385603f, 21.473854f, -40.343994f, 21.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)

			sim->setCompartmentParameters(d1, 28.396f, 5.526f);//SR 28 and 5
			sim->setCompartmentParameters(d2, 50.474f, 0.0f);//SLM 50 and 0
			sim->setCompartmentParameters(d3, 0.0f, 49.14f);//SO 0 and 49
			sim->setCompartmentParameters(s, 116.861f, 4.60f);// SP (somatic) 116 and 4

			int gin = sim->createSpikeGeneratorGroup("input", N, EXCITATORY_NEURON);
			sim->connect(gin, s, "one-to-one", RangeWeight(0.0f), 1.0f, RangeDelay(1), RadiusRF(-1));

			sim->setConductances(true);//This forces use of COBA model.

			sim->compConnect(d2, d1);
			sim->compConnect(d1, s);
			sim->compConnect(s, d3);

			sim->setSTDP(d1, false);
			sim->setSTDP(d2, false);
			sim->setSTDP(d3, false);
			sim->setSTDP(s, false);

			sim->setupNetwork();

			SpikeMonitor* SM = sim->setSpikeMonitor(s, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_1 = sim->setSpikeMonitor(d1, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_2 = sim->setSpikeMonitor(d2, "DEFAULT"); // put spike times into file
			SpikeMonitor* SM_3 = sim->setSpikeMonitor(d3, "DEFAULT"); // put spike times into file

			PoissonRate in(N);

			in.setRates(0.0f);
			sim->setSpikeRate(gin, &in);//Inactive input group

			SM->startRecording();
			SM_1->startRecording();
			SM_2->startRecording();
			SM_3->startRecording();
			sim->setExternalCurrent(s, 600);
			sim->runNetwork(0, 100);
			SM->stopRecording();
			SM_1->stopRecording();
			SM_2->stopRecording();
			SM_3->stopRecording();

			if (isGPUmode == 0)
			{
				float* CPU_Soma_VREC = sim->retrieveGroupVoltages(0);
				for (int j = 0; j < voltRecordBufferSize; j++)
				{
					CPU_Soma_Volt_Rec[j] = CPU_Soma_VREC[j];
				}
				cpu_firingRate = SM->getPopMeanFiringRate();
				cpu_firingRate_1 = SM_1->getPopMeanFiringRate();
				cpu_firingRate_2 = SM_2->getPopMeanFiringRate();
				cpu_firingRate_3 = SM_3->getPopMeanFiringRate();
			}
			else
			{
				float* GPU_Soma_VREC = sim->retrieveGroupVoltages(0);
				for (int j = 0; j < voltRecordBufferSize; j++)
				{
					GPU_Soma_Volt_Rec[j] = GPU_Soma_VREC[j];
				}
				EXPECT_FLOAT_EQ(SM->getPopMeanFiringRate(), cpu_firingRate);
				EXPECT_FLOAT_EQ(SM_1->getPopMeanFiringRate(), cpu_firingRate_1);
				EXPECT_FLOAT_EQ(SM_2->getPopMeanFiringRate(), cpu_firingRate_2);
				EXPECT_FLOAT_EQ(SM_3->getPopMeanFiringRate(), cpu_firingRate_3);
			}
			delete sim;
		}

		for (int j = 0; j < voltRecordBufferSize; j++)
		{
			EXPECT_LE(CPU_Soma_Volt_Rec[j], GPU_Soma_Volt_Rec[j] + 0.5f);
			EXPECT_GE(CPU_Soma_Volt_Rec[j], GPU_Soma_Volt_Rec[j] - 0.5f);
		}
	}
}*/
