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

#include <motion_energy.h>

#include <vector>
#include <assert.h>

#include <stdio.h>



int main(int argc, const char* argv[]) {
	// ---------------- CONFIG STATE -------------------
	bool onGPU = true;
	int frameDurMs = 50;
	int numDir = 8;
	double speed = 1.5;

	CARLsim sim("me", onGPU?GPU_MODE:CPU_MODE, USER);

	// Input stimulus created from an image using the MATLAB script
	// "createStimFromImage.m":
	VisualStimulus stim("input/grating.dat");
	stim.print();

	MotionEnergy me(stim.getWidth(), stim.getHeight(), stim.getChannels());
	unsigned char* frame;

	Grid3D gridV1(stim.getWidth(), stim.getHeight(), numDir);
	Grid3D gridMT(stim.getWidth()/2, stim.getHeight()/2, numDir);
	int gV1=sim.createSpikeGeneratorGroup("V1", gridV1, EXCITATORY_NEURON);

	int gMT=sim.createGroup("MT", gridMT, EXCITATORY_NEURON);
	sim.setNeuronParameters(gMT, 0.02f, 0.2f, -65.0f, 8.0f);
	sim.connect(gV1, gMT, "gaussian", RangeWeight(0.02), 1.0f, RangeDelay(1), RadiusRF(7,7,1));

	PoissonRate rateV1(gridV1.N, onGPU);

	// Use CUBA mode
	sim.setConductances(true);


	// ---------------- SETUP STATE -------------------
	sim.setupNetwork();

	sim.setSpikeMonitor(gV1, "DEFAULT");
	sim.setSpikeMonitor(gMT, "DEFAULT");


	// ---------------- RUN STATE -------------------
	for (int i=0; i<stim.getLength(); i++) {
		// get stim frame
		frame = stim.readFrameChar();

		// calculate V1 complex cell response
		me.calcV1complex(frame, onGPU?rateV1.getRatePtrGPU():rateV1.getRatePtrCPU(),
			speed, onGPU);

		sim.setSpikeRate(gV1, &rateV1);

		sim.runNetwork(frameDurMs/1000, frameDurMs%1000);
	}

	return 0;
}
