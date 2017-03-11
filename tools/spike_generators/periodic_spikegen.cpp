#include <periodic_spikegen.h>

#include <carlsim.h>

#include <user_errors.h>	// fancy error messages
#include <algorithm>		// std::find
#include <vector>			// std::vector
#include <cassert>			// assert



class PeriodicSpikeGenerator::Impl {
public:
	Impl(bool spikeAtZero) {
		_spikeAtZero = spikeAtZero;

		_needToUpdateFromVector = false;
		_needToUpdateFromFloat = false;
		_tmpRate = 0.0f;
		_tmpRates.clear();
	}

	~Impl() {}

	void setRates(float rateHz) {
		UserErrors::assertTrue(rateHz>=0, UserErrors::CANNOT_BE_NEGATIVE,
			"PeriodicSpikeGenerator", "Firing rate");
		_tmpRate = rateHz;
		_needToUpdateFromFloat = true;
		_needToUpdateFromVector = false;
	}

	void setRates(std::vector<float> ratesHz) {
		for (int i=0; i<ratesHz.size(); i++) {
			UserErrors::assertTrue(ratesHz[i] >= 0, UserErrors::MUST_BE_POSITIVE,
				"PeriodicSpikeGenerator", "Firing rate");
		}
		_tmpRates = ratesHz;
		_needToUpdateFromVector = true;
		_needToUpdateFromFloat = false;
	}

	unsigned int nextSpikeTime(CARLsim* sim, int grpId, int nid,
		unsigned int currentTime, 
		unsigned int lastScheduledSpikeTime,
		unsigned int endOfTimeSlice)
	{
		if (needToUpdateISI()) {
			updateISI(sim->getGroupNumNeurons(grpId), endOfTimeSlice);
		}

		if (_spikeAtZero) {
			// insert spike at t=0 for each neuron (keep track of neuron IDs to avoid getting stuck in infinite loop)
			if (std::find(_nIdFiredAtZero.begin(), _nIdFiredAtZero.end(), nid)==_nIdFiredAtZero.end()) {
				// spike at t=0 has not been scheduled yet for this neuron
				_nIdFiredAtZero.push_back(nid);
				return 0;
			}
		}

		// periodic spiking according to ISI
		return lastScheduledSpikeTime + _isis[nid];
	}

private:
	bool needToUpdateISI() {
		return _needToUpdateFromVector | _needToUpdateFromFloat;
	}

	void updateISI(int numNeur, unsigned int endOfTimeSlice) {
		if (_needToUpdateFromVector) {
			UserErrors::assertTrue(_tmpRates.size() == numNeur, UserErrors::MUST_BE_IDENTICAL,
				"PeriodicSpikeGenerator", "Vector size", "the number of neurons");

			// Set all ISIs as 1/f, but avoid division by zero.
			// If no spikes should be scheduled, set ISI to `endOfTimeSlice`, so it for sure falls outside
			// the current scheduling time slice.
			_isis.resize(numNeur, (int)endOfTimeSlice);
			for (int i=0; i<numNeur; i++) {
				if (_tmpRates[i] > 0.0f) {
					_isis[i] = (int) (1000.0f / _tmpRates[i]);
				}
			}

			_needToUpdateFromVector = false;
		} else if (_needToUpdateFromFloat) {
			UserErrors::assertTrue(_tmpRate>=0, UserErrors::CANNOT_BE_NEGATIVE,
				"PeriodicSpikeGenerator", "Firing rate");

			// Set all ISIs to 1/rate, but avoid division by zero.
			// If no spikes should be scheduled, set ISI to `endOfTimeSlice`, so it for sure falls outside
			// the current scheduling time slice.
			int isi = (int)endOfTimeSlice;
			if (_tmpRate > 0.0f) {
				isi = (int) (1000.0f / _tmpRate);
			}
			_isis.resize(numNeur, isi);

			_needToUpdateFromFloat = false;
		}
	}


	bool _needToUpdateFromVector;
	bool _needToUpdateFromFloat;

	float _tmpRate;					//!< spike rate (Hz)
	std::vector<float> _tmpRates;	//!< vector of spike rates (Hz)

	std::vector<int> _isis;			//!< vector of inter-spike intervals

	std::vector<int> _nIdFiredAtZero; //!< keep track of all neuron IDs for which a spike at t=0 has been scheduled
	bool _spikeAtZero; //!< whether to emit a spike at t=0
};


// ****************************************************************************************************************** //
// API IMPLEMENTATION
// ****************************************************************************************************************** //

// create and destroy a pImpl instance

PeriodicSpikeGenerator::PeriodicSpikeGenerator(bool spikeAtZero) : _impl( new Impl(spikeAtZero) ) {}
PeriodicSpikeGenerator::~PeriodicSpikeGenerator() { delete _impl; }

void PeriodicSpikeGenerator::setRates(float rateHz) { _impl->setRates(rateHz); }
void PeriodicSpikeGenerator::setRates(std::vector<float> ratesHz) { _impl->setRates(ratesHz); }

unsigned int PeriodicSpikeGenerator::nextSpikeTime(CARLsim* sim, int grpId, int nid, unsigned int currentTime, 
		unsigned int lastScheduledSpikeTime, unsigned int endOfTimeSlice) {
	return _impl->nextSpikeTime(sim, grpId, nid, currentTime, lastScheduledSpikeTime, endOfTimeSlice);
}