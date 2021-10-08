#ifndef BFT_LOG
#define BFT_LOG

#ifdef DEBUG
#define DEBUG_LOG(MSG) std::cout << "DEBUG: " << (MSG) << std::endl
#else
#define DEBUG_LOG(MSG) (void)0
#endif


#include <iostream>
#include <sstream>

typedef struct logger {
    bool is_verbose = false;
    bool suppress_warnings = false;

    std::ostream& operator()(bool override_verbose = false);
    std::ostream& err();
    std::ostream& warn();

    logger();
    ~logger();

    private:
    std::stringstream dummy;
} logger;

extern logger stdlog;

#endif