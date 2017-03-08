/*
 * Copyright (c) 2016 Regents of the University of California. All rights reserved.
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
 */

// include CARLsim user interface
#include <carlsim.h>

#if defined(WIN32) || defined(WIN64)
#include <stopwatch.h>
#endif


int main() {

	// keep track of execution time
	Stopwatch watch;
	

	// ---------------- CONFIG STATE -------------------

	// Create a network on the CPU.
	// In order to run a network on the GPU, change CPU_MODE to GPU_MODE. However, please note that this
	// won't work if you compiled CARLsim with flag NO_CUDA=1.
	// USER mode will print status and error messages to console. Suppress these by changing mode to SILENT.
	int ithGPU = 0;
	int randSeed = 42;
	CARLsim sim("hello_world", CPU_MODE, USER, ithGPU, randSeed);

	// Configure the network.
	// Organize neurons on a 2D grid: A SpikeGenerator group `gin` and a regular-spiking group `gout`
	Grid3D gridIn(13,9,1); // pre is on a 13x9 grid
	Grid3D gridOut(3,3,1); // post is on a 3x3 grid
	int gin=sim.createSpikeGeneratorGroup("input", gridIn, EXCITATORY_NEURON);
	int gout=sim.createGroup("output", gridOut, EXCITATORY_NEURON);
	sim.setNeuronParameters(gout, 0.02f, 0.2f, -65.0f, 8.0f);

	// Connect with Gaussian connectivity with a 3x3 Gaussian kernel. The max weight (peak of the Gaussian)
	// is 0.05, and is connected with probability 1. Both connection strength and probability drop off with
	// a Gaussian based on spatial arrangement. All synaptic delays are 1 ms.
	sim.connect(gin, gout, "gaussian", RangeWeight(0.05), 1.0f, RangeDelay(1), RadiusRF(3,3,1));

	// Make synapses conductance based. Set to false for current based synapses.
	sim.setConductances(true);

	// Use the forward Euler integration method with 2 integration steps per 1 ms time step.
	sim.setIntegrationMethod(FORWARD_EULER, 2);


	// ---------------- SETUP STATE -------------------
	// build the network
	watch.lap("setupNetwork");
	sim.setupNetwork();
	
	// Set spike monitors on both groups. Also monitor the weights between `gin` and `gout`.
	sim.setSpikeMonitor(gin,"DEFAULT");
	sim.setSpikeMonitor(gout,"DEFAULT");
	sim.setConnectionMonitor(gin,gout,"DEFAULT");

	// Setup some baseline input: Every neuron in `gin` will spike according to a Poisson process with
	// 30 Hz mean firing rate.
	PoissonRate in(gridIn.N);
	in.setRates(30.0f);
	sim.setSpikeRate(gin,&in);


	// ---------------- RUN STATE -------------------
	watch.lap("runNetwork");

	// Run for a total of 10 seconds.
	// At the end of each `runNetwork` call, SpikeMonitor stats will be printed (thus, every second).
	for (int i=0; i<10; i++) {
		sim.runNetwork(1,0);
	}

	// Print stopwatch summary
	watch.stop();
	
	return 0;
}
