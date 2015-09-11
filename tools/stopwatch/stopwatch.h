#ifndef STOPWATCH_H
#define STOPWATCH_H

#if defined(WIN32) || defined(WIN64)
	#include <Windows.h>
#else
	#include <sys/time.h>
	#include <ctime>
#endif

#include <stdint.h>


#include <vector>	// std::vector
#include <string>	// std::string

/*!
 * \brief Stopwatch utility to measure script execution time in milliseconds
 *
 * This class provides a means to measure script execution time in milliseconds.
 */
class Stopwatch {
public:
	/*!
	 * \brief Stopwatch for timing script execution (ms resolution)
	 *
	 * For timing script execution. If startTimer is set to true, stopwatch will start
	 * the timer upon object creation.
	 */
	Stopwatch(bool startTimer = true);

	/*!
	 * \brief CARLsim destructor
	 *
	 * Add details.
	 */
	~Stopwatch();

	/*!
	 * \brief starts/restarts timer
	 *
	 * Add details
	 */
	void start(std::string tag = "");

	/*!
	 * \brief stops timer
	 *
	 * Add details
	 */
	void stop(bool printMessage = true);

	void lap(std::string tag = "");

	uint64_t getLapTime(const std::string& tag) const;

	uint64_t getLapTime(int index) const;

private:
	class Impl;

	/*!
	 * \brief Private implementation.
	 *
	 * This class provides a pImpl for the CARLsim User API.
	 * \see https://marcmutz.wordpress.com/translated-articles/pimp-my-pimpl/
	 */
	Impl* _impl;
};

#endif // STOPWATCH_H