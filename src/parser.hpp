#ifndef BFT_PARSER
#define BFT_PARSER

#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <deque>

#include "asc.hpp"
#include "cli.hpp"

class parser{
    std::unique_ptr<asc_node> root_node;
    std::stringstream bf_asm;
    std::string output_filename;

    std::deque<size_t> loop_stack;

    public:
    parser(std::string output_filename_) noexcept;
    virtual ~parser() noexcept;

    void tokenize(std::istream* input_stream);
    void optimize_asc();
    void generate_code(compile_target code_lang, std::ostream* output_stream);
    
    friend std::ostream& operator<<(std::ostream& os, parser& p);
};


#endif