#ifndef BFT_CODEGEN
#define BFT_CODEGEN

#include <string>
#include <sstream>
#include <vector>
#include <deque>

#include "../asc.hpp"

class codegen {
    protected:
    std::stringstream code_stream;
    std::deque<size_t> loop_stack;
    std::deque<size_t> open_loop_stack;

    public:
    codegen(std::deque<size_t> loop_stack_) noexcept;
    virtual ~codegen() noexcept = 0;

    virtual void dump_code(std::ostream* code_output) const noexcept = 0;
    virtual std::string dump_code_string() const noexcept = 0;
    virtual void generate(asc_node* node_) = 0;

};

#endif