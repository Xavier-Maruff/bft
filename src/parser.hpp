#ifndef BFT_PARSER
#define BFT_PARSER

#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <deque>

#include "asc.hpp"
#include "cli.hpp"

//main parser
class parser{
    //root asc node
    std::unique_ptr<asc_node> root_node;
    std::string output_filename;
    uint8_t optimization_level;

    //keeps track of loop level
    std::deque<size_t> loop_stack;

    //various optimizations
    void contract_repeating_nodes();
    void cancel_opposing_ops();
    void zero_loop();


    public:
    parser(std::string output_filename_, uint8_t optimization_level_ = 3) noexcept;
    virtual ~parser() noexcept;

    //tokenize an input character stream
    void tokenize(std::istream* input_stream);
    //run the optimizations
    void optimize_asc();
    //pipe the asc through a codegen backend
    void generate_code(compile_target code_lang, std::ostream* output_stream);
    //dump the IR to an output stream
    void dump_ir(std::ostream* output_stream);
};


#endif