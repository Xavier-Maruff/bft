#ifndef BFT_RUST_CODEGEN
#define BFT_RUST_CODEGEN

#include "codegen.hpp"

class codegen_rust: public codegen {
    static std::pair<std::string, std::string> skeleton;
    
    public:
    codegen_rust(std::deque<size_t> loop_stack_) noexcept;
    virtual ~codegen_rust() noexcept;

    void dump_code(std::ostream* code_output) const noexcept override;
    std::string dump_code_string() const noexcept override;
    void generate(asc_node* node_) override;    
};

#endif