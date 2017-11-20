#ifndef PHYRAY_CORE_REPORTER_H
#define PHYRAY_CORE_REPORTER_H

// PhyRay imports
#include <core/debug.h>
#include <core/phyr_mem.h>

// Standard library imports
#include <mutex>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <iostream>
#include <stdexcept>

#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>

namespace phyr {

struct NoOpFunctor { void operator()(...) {} };

// Timer declarations
class Timer {
  public:
    /**
     * Treat this timer as a task scheduler if {asScheduler} is true.
     * The parameters for the scheduler will be then taken from a call
     * to {startTimer()}
     */
    explicit Timer(bool asScheduler = false) : asScheduler(asScheduler) {}

    ~Timer() {
        // Stop the internal scheduler
        if (asScheduler)
            setTimerState(TimerState::IDLE);
    }

    // Interface

    /**
     * Starts and seeds the timer. Further calls to this function
     * simply resets the timer. The parameters {delay},
     * {f} and {args} are ignored if {asScheduler} is false.
     *
     * @param delay         The amount of duration in milliseconds after which
     *                      the functor {f()} is called repeatedly.
     * @param f             The functor specifying the operation to be executed
     *                      on every timer tick as defined by the {delay} amount.
     * @param args          Variadic arguments used for calling {f()}
     */
    template <class Functor = NoOpFunctor, typename ... Args>
    void startTimer(int delay = 1000,
                    std::unique_ptr<Functor> f = std::unique_ptr<Functor>(new NoOpFunctor),
                    Args... args) {
        startTime = std::chrono::system_clock::now();
        setTimerState(TimerState::ACTIVE);

        // Initiate scheduler if {asScheduler} is true
        if (asScheduler) {
            std::thread thrd(TimerLoop<Functor, Args...>(this), std::move(f), delay, args...);

            // Move thread control from {thrd} to {schedulerThread} to extend
            // its scope to that of the parent {Timer} object.
            schedulerThread = std::move(thrd);
        }
    }

    /**
     * Returns the elapsed time since the last call to {startTimer} in milliseconds
     */
    uint64_t getElapsedTime() const {
        auto currentTime = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
        return static_cast<uint64_t>(elapsed.count());
    }

    /**
     * Resets timer state to idle. Stops the internal scheduler if
     * this timer was set to be a scheduler.
     */
    void resetTimer() {
        setTimerState(TimerState::IDLE);
        // Wait for {schedulerThread} to actually stop
        schedulerThread.join();
    }

    /**
     * Returns the formatted time in string from the given
     * {duration} value. {duration} is expected to be in seconds.
     */
    std::string getFormattedTime(uint64_t duration) const {
        int hh, mm, ss;

        // Extract time components from given duration

        // Convert to seconds
        duration /= 1000;
        hh = duration / 3600; duration %= 3600;
        mm = duration / 60; duration %= 60;
        ss = duration;

        std::string formattedTime = formatString("%02d:%02d:%02d", hh, mm, ss);
        return formattedTime;
    }

    inline bool isActive() {
        std::lock_guard<std::mutex> lock(mutex);
        return state == TimerState::ACTIVE;
    }

  private:
    enum TimerState { IDLE, ACTIVE };

    std::chrono::system_clock::time_point startTime;
    TimerState state = TimerState::IDLE;
    bool asScheduler;

    std::mutex mutex;
    std::thread schedulerThread;

    inline void setTimerState(TimerState _state) {
        std::lock_guard<std::mutex> lock(mutex);
        state = _state;
    }

    friend class TimerLoop;

    // TimerLoop declarations
    template <class Functor, typename ... Args>
    class TimerLoop {
      public:
        TimerLoop(Timer* timer) : timer(timer) {}

        void operator()(std::unique_ptr<Functor> f, int delay, Args... args) {
            uint64_t elapsed, delta;
            uint64_t last_call_tstamp = timer->getElapsedTime();

            while (timer->isActive()) {
                elapsed = timer->getElapsedTime();
                delta = elapsed - last_call_tstamp;

                // Check if elapsed time is more than provided delay
                if (delta >= delay) {
                    // Reset {last_call_tstamp}
                    last_call_tstamp = elapsed;
                    // Call the provided functor
                    (*f)(args...);
                }

                // Sleep to allow for context switches
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }

      private:
        Timer* timer;
    };
};

// ProgressReporterException declarations
class ProgressReporterException : public std::exception {
  public:
    ProgressReporterException(const char* err = DEF_ERR) : err(err) {}

    // Interface
    const char* what() const throw() override { return err; }

  private:
    const char* err;
    static const char* DEF_ERR;
};

enum ProgressReporterStates { AVAILABLE, ACQUIRED };

// ProgressReporter declarations
class ProgressReporter {
  public:
    ~ProgressReporter() {
        if (pr_obj) delete pr_obj;
        pr_obj = nullptr;
    }

    // Interface
    static ProgressReporter* getInstance() {
        if (!pr_obj)
            pr_obj = new ProgressReporter();
        return pr_obj;
    }

    /**
     * Gives the calling method access to the {ProgressReporter} object
     * only if the reporter is in the {AVAILABLE} state or else, it throws
     * a {ProgressReporterException}.
     */
    std::string startReport(int steps) {
        if (state == ProgressReporterStates::AVAILABLE) {
            token = generateToken();
            state = ProgressReporterStates::ACQUIRED;
            nSteps = steps; currentStepNumber = 0;

            std::unique_ptr<RefreshFunctor> f =
                    std::unique_ptr<RefreshFunctor>(new RefreshFunctor(this));
            // Start the scheduler to ping the
            // {RefreshFunctor} every 250 milliseconds
            timer->startTimer(250, std::move(f));
            return token;
        } else {
            throw ProgressReporterException();
        }
    }

    /**
     * Report progress to the this reporter. Must be called with the
     * access token returned on calling {startReport()}
     */
    void updateProgress(const std::string& p_token) {
        // Acquire lock on mutex
        std::lock_guard<std::mutex> lock(mutex);

        if (state == ProgressReporterStates::ACQUIRED && token == p_token) {
            currentStepNumber = std::min(currentStepNumber + 1, nSteps);
        } else {
            if (state != ProgressReporterStates::ACQUIRED)
                throw ProgressReporterException("Invalid ProgressReporter state");
            else
                throw ProgressReporterException("Invalid access token");
        }
    }

    /**
     * Must be called once update calls are no longer required, to reset this reporter.
     */
    void endReport(const std::string& p_token) {
        if (state == ProgressReporterStates::ACQUIRED && token == p_token) {
            timer->resetTimer();
            state = ProgressReporterStates::AVAILABLE;
            token = std::string();

            // Last call to {showProgress()} to make sure
            // progress has been shown to 100%
            showProgress();
        } else {
            if (state != ProgressReporterStates::ACQUIRED)
                throw ProgressReporterException("Invalid ProgressReporter state");
            else
                throw ProgressReporterException("Invalid access token");
        }
    }

  private:
    ProgressReporterStates state;
    // The total number of steps to evaluate till completion
    int nSteps, currentStepNumber;
    std::string token;

    // Lock mutexes
    std::mutex mutex;

    std::unique_ptr<Timer> timer;
    struct winsize term;
    int termWidth;

    static ProgressReporter* pr_obj;
    static constexpr int TokenLength = 16;

    ProgressReporter() :
        state(ProgressReporterStates::AVAILABLE),
        nSteps(0), currentStepNumber(0), token(std::string()) {
        // Create the timer
        timer = std::unique_ptr<Timer>(new Timer(true));
        updateTerminalSpecs();
    }

    inline void updateTerminalSpecs() {
        // Get terminal size
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &term);
        termWidth = term.ws_col - 30;
    }

    std::string generateToken() const {
        // Define the character set
        static const char chars[] = "0123456789"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "abcdefghijklmnopqrstuvwxyz";
        static const int charSetLen = sizeof(chars);

        // Generate string from random choices from the character set
        thread_local static std::mt19937 rng(std::random_device{}());
        thread_local static std::uniform_int_distribution<int> distrib(0, charSetLen - 1);

        char* token_chars = STACK_ALLOC(char, TokenLength + 1);
        for (int i = 0; i < TokenLength; i++)
            token_chars[i] = distrib(rng);
        token_chars[TokenLength] = '\0';

        return token_chars;
    }

    void showProgress() {
        std::lock_guard<std::mutex> lock(mutex);
        updateTerminalSpecs();

        // Compute progress
        float perc = float(currentStepNumber) / nSteps;

        // Clear line
        std::cout << "\033[A\33[2K\r";
        // Set terminal color (yellow)
        std::cout << "\x1b[33;1m";
        int i, prc = static_cast<int>(perc * termWidth);

        std::cout << "[";
        // Print progress arrow
        for (i = 0; i < prc; i++) { std::cout << "="; }
        if (prc < termWidth) { std::cout << ">"; i++; }

        // Print filler spaces
        for (; i < termWidth; i++) { std::cout << " "; }

        // Print terminating bracket and reset color
        std::cout << "] \x1b[37;1m" << static_cast<int>(perc * 100) << "%\x1b[0m ";

        uint64_t msec = timer->getElapsedTime();
        std::cout << " ET: \x1b[31;1m" << timer->getFormattedTime(msec);
        std::cout << "\x1b[0m" << std::endl;
    }

    friend class RefreshFunctor;

    struct RefreshFunctor {
      public:
        RefreshFunctor(ProgressReporter* reporter) : reporter(reporter) {}

        void operator()(...) { reporter->showProgress(); }

      private:
        ProgressReporter* reporter;
    };
};

}  // namespace phyr

#endif
