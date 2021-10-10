#ifndef BFT_CLI
#define BFT_CLI

#include <string>

enum compile_target {
    target_js,
    target_c,
    target_rust,
    target_ada,
    target_fortran
};

typedef struct cli_args {
    bool verbose;
    uint8_t optimization_level;
    compile_target target;
    std::string source_filename;
    std::string output_filename;

    cli_args();
    ~cli_args();
} cli_args;


cli_args parse_args(int argc, char* argv[]);


#endif