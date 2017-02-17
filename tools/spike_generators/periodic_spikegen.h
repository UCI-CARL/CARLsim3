#ifndef _PERIODIC_SPIKEGEN_H_
#define _PERIODIC_SPIKEGEN_H_

#include <callback.h>
#include <vector>

class CARLsim;


/*!
 * \brief a periodic SpikeGenerator (constant ISI) creating spikes at a certain rate
 *
 * This class implements a period SpikeGenerator that schedules spikes with a constant inter-spike-interval.
 * The firing rate can be set per neuron or per group.
 * For example, a PeriodicSpikeGenerator with all rates=10Hz will schedule spikes for each neuron in the
 * group at t=100, t=200, t=300 ms, etc.
 * If spikeAtZero is set to true, then the first spike will be scheduled at t=0.
 *
 * Otherwise, if rates are given in a vector, one rate per neuron in the group is assumed.
 */
class PeriodicSpikeGenerator : public SpikeGenerator {
public:
	/*!
	 * \brief PeriodicSpikeGenerator constructor
	 *
	 * \param[in] spikeAtZero a boolean flag to indicate whether to insert the first spike at t=0
	 */
	PeriodicSpikeGenerator(bool spikeAtZero=true);

	//! default destructor
	~PeriodicSpikeGenerator();

	/*!
	 * \brief Sets the spike rate of all neurons in the group
	 *
	 * The spike rate of every neuron in the group will be set to `rateHz`.
	 *
	 * \param[in] rateHz the firing rate (Hz) to be applied to all neurons in the group
	 */
	void setRates(float rateHz);

	/*!
	 * \brief Sets the spike rate on a neuron-by-neuron basis
	 *
	 * This sets the spike rate of every neuron in the group to a value specified in the vector.
	 * One entry per neuron is assumed.
	 *
	 * \param[in] ratesHz A vector specifying the firing rate (Hz) for every neuron in the group
	 */
	void setRates(std::vector<float> ratesHz);

	/*!
	 * \brief schedules the next spike time
	 *
	 * This function schedules the next spike time, given the currentTime and the lastScheduledSpikeTime. It implements
	 * the virtual function of the base class.
	 * \param[in] sim pointer to a CARLsim object
	 * \param[in] grpId current group ID for which to schedule spikes
	 * \param[in] nid current neuron ID for which to schedule spikes
	 * \param[in] currentTime current time (ms) at which spike scheduler is called
	 * \param[in] lastScheduledSpikeTime the last time (ms) at which a spike was scheduled for this nid, grpId
	 * \param[in] endOfTimeSlice the end of the time slice (ms) for which to schedule spikes
	 * \returns the next spike time (ms)
	 */
	unsigned int nextSpikeTime(CARLsim* sim, int grpId, int nid, unsigned int currentTime, 
		unsigned int lastScheduledSpikeTime, unsigned int endOfTimeSlice);

private:
	// This class provides a pImpl.
	// \see https://marcmutz.wordpress.com/translated-articles/pimp-my-pimpl/
	class Impl;
	Impl* _impl;
};

#endif