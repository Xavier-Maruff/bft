#ifndef BFT_COLOURS
#define BFT_COLOURS

#ifndef _WIN32
#define ANSI_RESET "\033[0m"
#define ANSI_CYAN "\033[36m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#else
#define ANSI_RESET ""
#define ANSI_CYAN ""
#define ANSI_RED ""
#define ANSI_GREEN ""
#define ANSI_YELLOW ""
#endif

#endif