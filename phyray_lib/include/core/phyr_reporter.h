#ifndef PHYRAY_CORE_REPORTER_H
#define PHYRAY_CORE_REPORTER_H

// PhyRay imports
#include <core/debug.h>
#include <core/phyr_mem.h>

// Standard library imports
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

// Timer declarations
class Timer {
  public:
    Timer() {}

    // Interface

    /**
     * Starts and seeds the timer. Further calls to this function
     * simply resets the timer.
     */
    void startTimer() {
        startTime = std::chrono::system_clock::now();
    }

    /**
     * Returns the elapsed time since the last call to {startTimer}
     */
    uint64_t getElapsedTime() const {
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;
        return static_cast<uint64_t>(elapsed.count());
    }

    /**
     * Returns the formatted time in string from the given
     * {duration} value. {duration} is expected to be in seconds.
     */
    std::string getFormattedTime(uint64_t duration) const {
        int hh, mm, ss;

        // Extract time components from given duration
        hh = duration / 3600; duration %= 3600;
        mm = duration / 60; duration %= 60;
        ss = duration;

        std::string formattedTime = formatString("%02d:%02d:%02d", hh, mm, ss);
        return formattedTime;
    }

  private:
    std::chrono::system_clock::time_point startTime;
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

            timer.startTimer();
            // Show an initial progress
            showProgress(0);

            return token;
        } else {
            throw ProgressReporterException();
        }
    }

    /**
     * Report progress to the this reporter. Must be called with the
     * access token returned on calling {startReport()}
     */
    void updateReport(const std::string& p_token) {
        if (state == ProgressReporterStates::ACQUIRED && token == p_token) {
            currentStepNumber = std::min(currentStepNumber + 1, nSteps);
            showProgress(float(currentStepNumber) / nSteps);
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
            state = ProgressReporterStates::AVAILABLE;
            token = std::string();
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

    Timer timer;
    struct winsize term;
    int termWidth;

    static ProgressReporter* pr_obj;
    static constexpr int TokenLength = 16;

    ProgressReporter() :
        state(ProgressReporterStates::AVAILABLE),
        nSteps(0), currentStepNumber(0), token(std::string()) {
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

    void showProgress(float perc) {
        updateTerminalSpecs();

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
        std::cout << "]\x1b[0m";
        std::cout << " " << static_cast<int>(perc * 100) << "% ";

        uint64_t sec = timer.getElapsedTime();
        std::cout << " ET: \x1b[31;1m" << timer.getFormattedTime(sec);
        std::cout << "\x1b[0m" << std::endl;
    }
};

}  // namespace phyr

#endif
