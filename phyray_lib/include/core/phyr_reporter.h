#include <iostream>
#include <string>
#include <stdlib>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ctime>

class ProgressReporter {
    public:
        static ProgressReporter *instance() {
            if (!pr_obj)
                pr_obj = new ProgressReporter;
            return pr_obj;
        }
        
        std::string startReport () {
            if (!is_busy) {
                token = generateToken();
                progress = 0;
                is_busy = true;
                return token;
            } else {
                return "ProgressReporter is busy!!";
            }
        }
        
        std::string generateToken () {
            std::string _token;
            static const char alphanum[] =
            "0123456789"
            "!@#$%^&*"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            
            int stringLength = sizeof(alphanum) - 1;
            
            srand(time(0));
            for(unsigned int i = 0; i < 16; ++i) {
                _token += alphanum[rand() % stringLength];
            }
            return _token;
        }
        
        
        void showProgress (int per) {
            /* Get terminal size */
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            term_width = w.ws_col - 30;
            
            std::cout << "\033[A\33[2K\r";
            int j, i = per*term_width/100;
            
            std::cout << "[ ";
            for (j = 0; j <= i; j++) {
                std::cout << "-";
            }
            
            // std::cout << ">";
            
            for (j = i+1; j < term_width; j++) {
                std::cout << " ";
            }
            
            std::cout << " ]";
            std::cout << " " << per << "%";
            std::cout << std::endl;
        }
        
        void updateReport (std::string &p_token, float progress) {
            if (token == p_token) {
                showProgress(int(progress*100));
            } else {
                // Throw exception
                std::cout << "Invalid token";
            }
        }
        
        void endReport (const std::string &p_token) {
            if (token == p_token) {
                is_busy = false;
                token = "";
                progress = 0;
            } else {
                // Write exception here
                std::cout << "Token is not valid";
            }
        }
        
    private:
        bool is_busy;
        std::string token;
        float progress;
        
        struct winsize w;
        int term_width;
        
        static ProgressReporter *pr_obj;
        
        ProgressReporter() {
            is_busy = false;
            progress = 0;
            token = "";
            /* Get terminal size */
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            term_width = w.ws_col - 30;
        }
};

// Create object like this
// ProgressReporter *ProgressReporter::pr_obj = 0;
