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
    
    size_t indent_level;
    inline std::ostream& code_stream_fmt() noexcept;

    public:
    codegen(std::deque<size_t> loop_stack_) noexcept;
    virtual ~codegen() noexcept = 0;

    virtual void dump_code(std::ostream* code_output) const noexcept = 0;
    virtual std::string dump_code_string() const noexcept = 0;
    virtual void generate(asc_node* node_) = 0;

};

inline std::ostream& codegen::code_stream_fmt() noexcept {
	return code_stream << std::string(indent_level, '\t');
}

#endif