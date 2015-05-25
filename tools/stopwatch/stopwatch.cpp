#include "stopwatch.h"

#include <stdio.h>

// ****************************************************************************************************************** //
// STOPWATCH UTILITY PRIVATE IMPLEMENTATION
// ****************************************************************************************************************** //

/*!
 * \brief Private implementation of the Stopwatch Utility
 *
 * This class provides a timer with milliseconds resolution.
 * \see http://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c/1861337#1861337
 */
class Stopwatch::Impl {
public:
	// +++++ PUBLIC METHODS: CONSTRUCTOR / DESTRUCTOR +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

	Impl(bool startTimer) {
		clear();

		if (startTimer) {
			start("");
		}
	}

	~Impl() {
	}

	void clear() {
		_isTimerOn = false;
		_startTimeMs.clear();
		_stopTimeMs.clear();
		_accumTimeMs.clear();
		_tags.clear();
	}

	void start(std::string tag) {
		if (_isTimerOn) {
			// error
		}

		// start/continue timer
		_isTimerOn = true;
		if (tag.length() > 60) {
			tag = tag.substr(0,60);
		}
		_tags.push_back(tag);
		_startTimeMs.push_back( getCurrentTime() );
	}

	void stop(bool printMessage = true) {
		if (!_isTimerOn) {
			// error
		}

		// pause/stop timer and update exe time
		_isTimerOn = false;
		_stopTimeMs.push_back( getCurrentTime() );
		_accumTimeMs.push_back( _stopTimeMs.back() - _startTimeMs.back() );

		if (printMessage) {
			uint64_t totalMs = 0;
			for(std::vector<uint64_t>::iterator j=_accumTimeMs.begin(); j!=_accumTimeMs.end(); ++j) {
				totalMs += *j;
			}

			printf("--------------------------------------------------------------\n");
			printf("| Stopwatch                                                  |\n");
			printf("|------------------------------------------------------------|\n");

			printf("| Start\t\tStop\t\tLap\t\tTotal        |\n");
			uint64_t totalMsSoFar = 0;
			for (int i=0; i<_accumTimeMs.size(); i++) {
				totalMsSoFar += _accumTimeMs[i];
				uint64_t startMs = _startTimeMs[i] - _startTimeMs[0];
				uint64_t stopMs = _stopTimeMs[i] - _startTimeMs[0];
				uint64_t accumMs = _accumTimeMs[i];

				printf("| %02lu:%02lu:%02lu.%03lu\t%02lu:%02lu:%02lu.%03lu\t%02lu:%02lu:%02lu.%03lu\t%02lu:%02lu:%02lu.%03lu |\n", 
					startMs/3600000, (startMs/1000/60)%60, (startMs/1000)%60, startMs%1000,
					stopMs/3600000, (stopMs/1000/60)%60, (stopMs/1000)%60, stopMs%1000,
					accumMs/3600000, (accumMs/1000/60)%60, (accumMs/1000)%60, accumMs%1000,
					totalMsSoFar/3600000, (totalMsSoFar/1000/60)%60, (totalMsSoFar/1000)%60, totalMsSoFar%1000);
			}
			printf("--------------------------------------------------------------\n");
		}
	}

	void split(std::string tag) {
		stop(false);
		start(tag);
	}

	uint64_t elapsedTime() const {
		if (!_isTimerOn) {
			// error
		}
		return 0;
//		return (getCurrentTime() - _startTimeMs);
	}

private:
	// +++++ PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

	uint64_t getCurrentTime() const {
	#ifdef _WIN32
		/* Windows */
		FILETIME ft;
		LARGE_INTEGER li;

		/* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
		 * to a LARGE_INTEGER structure. */
		GetSystemTimeAsFileTime(&ft);
		li.LowPart = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;

		uint64 ret = li.QuadPart;
		ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
		ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

		return ret;
	#else
		/* Linux */
		struct timeval tv;

		gettimeofday(&tv, NULL);

		uint64_t ret = tv.tv_usec;
		/* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
		ret /= 1000;

		/* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
		ret += (tv.tv_sec * 1000);

		return ret;
	#endif
	}


	// +++++ PRIVATE STATIC PROPERTIES ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

	// +++++ PRIVATE PROPERTIES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
	std::vector<uint64_t> _startTimeMs;
	std::vector<uint64_t> _stopTimeMs;
	std::vector<uint64_t> _accumTimeMs;
	std::vector<std::string> _tags;
	bool _isTimerOn;

};


// ****************************************************************************************************************** //
// STOPWATCH API IMPLEMENTATION
// ****************************************************************************************************************** //

// create and destroy a pImpl instance
Stopwatch::Stopwatch(bool startTimer)
  : _impl( new Impl(startTimer) ) {}
Stopwatch::~Stopwatch() { delete _impl; }


void Stopwatch::start(std::string tag) { _impl->start(tag); }
void Stopwatch::stop(bool printMessage) { _impl->stop(printMessage); }
void Stopwatch::split(std::string tag) { _impl->split(tag); }
uint64_t Stopwatch::elapsedTime() const { return _impl->elapsedTime(); }