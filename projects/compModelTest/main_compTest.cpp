/*
* Copyright (c) 2013 Regents of the University of California. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* 3. The names of its contributors may not be used to endorse or promote
*    products derived from this software without specific prior written
*    permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* *********************************************************************************************** *
* CARLsim
* created by: 		(MDR) Micah Richert, (JN) Jayram M. Nageswaran
* maintained by:	(MA) Mike Avery <averym@uci.edu>, (MB) Michael Beyeler <mbeyeler@uci.edu>,
*					(KDC) Kristofor Carlson <kdcarlso@uci.edu>
*
* CARLsim available from http://socsci.uci.edu/~jkrichma/CARL/CARLsim/
* Ver 11/6/14
*/



#include <carlsim.h>	//Necessary in order to use CarlSim

#if defined(WIN32) || defined(WIN64)
#define _CRT_SECURE_NO_WARNINGS
#endif

//4 compartments tutorial
int main() {

	//Goal of this tutorial is to introduce you to the basics of creating, setting up, simulating, and tuning multi-compartment neurons within CARLSim network. 
	//There are 3 main stages when using CARLSim. CONFIG, SETUP, and RUN. 
	//CONFIG is the initial stage. This is where we initialize CARLsim object in either GPU or CPU mode.
	//We then proceed to configure the simulation, by creating groups, setting their parameters, and connecting them.
	//We then call setupNetwork() method onto CARLSim object, thus, entering the SETUP stage.
	//In this stage we can tune Izhikevich/Coupling parameters per neuron basis. We can also setup various monitors.
	//Finally, we enter RUN stage by calling runNetwork() method onto CARLSim object.

	//Added as of 5/27/2015. Voltages can now be recorded per group.

	// create a network on GPU
	int ithGPU = 0;//The ith GPU to be used, where i is the device Id. Default value is 0.
	int randSeed = 42;// A random seed.

	//GPU Mode:
	CARLsim sim("random", GPU_MODE, USER, ithGPU, 42);//For large networks CARLSim performs best in GPU mode.

	//CPU Mode:
	//CARLsim sim("hello_world");//For small networks CARLsim performs best in CPU mode. 

	// simulation details
	int N = 5; //number of neurons

	//Each of these groups represents a layer. Together these 4 groups represent N 4-compartment neurons. Ex: If N is 5, there are 5 4-compartment neurons in this simulation.
	int s = sim.createGroup("soma", N, EXCITATORY_NEURON);
	int d1 = sim.createGroup("d1", N, EXCITATORY_NEURON);
	int d2 = sim.createGroup("d2", N, EXCITATORY_NEURON);
	int d3 = sim.createGroup("d3", N, EXCITATORY_NEURON);

	//Izhikevich Neuron parameters are initially set per layer (group). However, izhikevich parameters of individual neurons within a layer can be later modified. More on that later.
	sim.setNeuronParameters(s, 550.0f, 2.3330991f, -59.101414f, -50.428886f, 0.0021014998f, -0.41361538f, 24.98698f, -53.223213f, 109.0f);//9 parameter setNeuronParametersCall (RS NEURON) (soma)
	sim.setNeuronParameters(d1, 367.0f, 1.1705916f, -59.101414f, -44.298294f, 0.2477681f, 3.3198094f, 20.274296f, -46.076824f, 24.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
	sim.setNeuronParameters(d2, 425.0f, 2.2577047f, -59.101414f, -25.137894f, 0.32122386f, 0.14995363f, 13.203414f, -38.54892f, 69.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)
	sim.setNeuronParameters(d3, 225.0f, 1.109572f, -59.101414f, -36.55802f, 0.29814243f, -4.385603f, 21.473854f, -40.343994f, 21.0f);//9 parameter setNeuronParametersCall (RS NEURON) (dendr)

	//Couping Constants are also initally set per layer (group). However, coupling constants of individual neurons within a layer can be later modified. More on that later as well.
	sim.setCompartmentParameters(d1, 28.396f, 5.526f);//SR 28 and 5
	sim.setCompartmentParameters(d2, 50.474f, 0.0f);//SLM 50 and 0
	sim.setCompartmentParameters(d3, 0.0f, 49.14f);//SO 0 and 49
	sim.setCompartmentParameters(s, 116.861f, 4.60f);// SP (somatic) 116 and 4
	
	//CarlSim requires having at least one input group. This might change as externalCurrent has been recently introduced as another option to introduce current into a network.
	int gin = sim.createSpikeGeneratorGroup("input", N, EXCITATORY_NEURON);

	//Setting conductances to false forces CUBA mode. 
	//sim.setConductances(true, 5, 150, 6, 150);
	sim.setConductances(false);

	// 100% probability of connection (set RangeWeight to 0)
	// dummy connection so that we can run the network
	sim.connect(gin, s, "one-to-one", RangeWeight(0.0f), 1.0f, RangeDelay(1), RadiusRF(-1));

	//Establish compartmental connections in order to form the following configuration:
	//	d3
	//	|
	//	s
	//	|
	//	d1
	//	|
	//	d2

	sim.compConnect(d2, d1);//Connects layer (group) d2 to layer (group) d1. D2 is below d1.
	sim.compConnect(d1, s);
	sim.compConnect(s, d3);

	sim.setIntegrationMethod(RUNGE_KUTTA4, 10);

	sim.setupNetwork();

	SpikeMonitor* SM = sim.setSpikeMonitor(s, "DEFAULT"); // put spike times into file
	SpikeMonitor* SM_1 = sim.setSpikeMonitor(d1, "DEFAULT"); // put spike times into file
	SpikeMonitor* SM_2 = sim.setSpikeMonitor(d2, "DEFAULT"); // put spike times into file
	SpikeMonitor* SM_3 = sim.setSpikeMonitor(d3, "DEFAULT"); // put spike times into file
	
	//setup some baseline input
	PoissonRate in(N);

	in.setRates(0.0f);//Set the poissonRate to be inactive (rate 0)
	sim.setSpikeRate(gin, &in);//Make gin an inactive (silent) input group.

	//Testing Segment
	SM->startRecording();//Record spikes within layer (group) s
	SM_1->startRecording();//Record spikes within layer (group) d1
	SM_2->startRecording();//Record spikes within layer (group) d2
	SM_3->startRecording();//Record spikes within layer (group) d3

	sim.setExternalCurrent(s, 0);//Set external current of 0 into layer (group) s 
	sim.runNetwork(0, 100);//Run network for 100ms
	sim.setExternalCurrent(s, 592);//Set external current of 592 into layer (group) s 
	sim.runNetwork(0, 400);//Run network for 400ms

	sim.setExternalCurrent(s, 592);//Set external current of 592 into layer (group) s 
	sim.runNetwork(0, 400);//Run network for 400ms
	sim.setExternalCurrent(s, 0);//Set external current of 0 into layer (group) s 
	sim.runNetwork(0, 100);//Run network for 100ms

	SM->stopRecording();//Stop recording layer (group) s
	SM_1->stopRecording();//Stop recording layer (group) d1
	SM_2->stopRecording();//Stop recording layer (group) d2
	SM_3->stopRecording();//Record spikes within layer (group) d3

	SM_3->print();//Print spike times for layer (group) d3
	SM->print();//Print spike times for layer (group) s
	SM_1->print();//Print spike times for layer (group) d1
	SM_2->print();//Print spike times for layer (group) d2


	return 0;
}
