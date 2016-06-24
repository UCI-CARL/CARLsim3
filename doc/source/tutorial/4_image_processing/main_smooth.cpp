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
 * CARLsim available from http://socsci.uci.edu/~jkrichma/CARL/CARLsim
 */
#include <carlsim.h>

#include <vector>
#include <assert.h>

#include <stdio.h>



int main(int argc, const char* argv[]) {
	// ---------------- CONFIG STATE -------------------
	CARLsim sim("image_proc", GPU_MODE, USER);

	VisualStimulus stim("input/carl.dat");
	stim.print();

	Grid3D imgDim(stim.getWidth(), stim.getHeight(), stim.getChannels());
	Grid3D imgSmallDim(imgDim.width/2, imgDim.height/2, imgDim.channels);

	int gIn = sim.createSpikeGeneratorGroup("input", imgDim, EXCITATORY_NEURON);
	int gSmooth = sim.createGroup("smooth", imgSmallDim, EXCITATORY_NEURON);
	sim.setNeuronParameters(gSmooth, 0.02f, 0.2f, -65.0f, 8.0f);

	int gSum = sim.createGroup("sum", imgSmallDim, EXCITATORY_NEURON);
	sim.setNeuronParameters(gSum, 0.02f, 0.2f, -65.0f, 8.0f);

	sim.connect(gIn, gSmooth, "gaussian", RangeWeight(2.0f), 1.0f, RangeDelay(1), RadiusRF(5,5,0));
	sim.connect(gSmooth, gSum, "gaussian", RangeWeight(20.0f), 1.0f, RangeDelay(1), RadiusRF(0.5,0.5,-1));

	sim.setConductances(false);


	// ---------------- SETUP STATE -------------------
	sim.setupNetwork();

	sim.setSpikeMonitor(gIn, "DEFAULT");
	sim.setSpikeMonitor(gSmooth, "DEFAULT");
	sim.setSpikeMonitor(gSum, "DEFAULT");


	// ---------------- RUN STATE -------------------
	for (int i=0; i<stim.getLength(); i++) {
		PoissonRate* rates = stim.readFrame(50.0f, 0.0f);
		sim.setSpikeRate(gIn, rates);
 		sim.runNetwork(1,0); // run the network
 	}


	return 0;
}
