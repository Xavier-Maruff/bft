#ifndef BFT_ADA_CODEGEN
#define BFT_ADA_CODEGEN

#include <vector>
#include "codegen.hpp"

class codegen_ada: public codegen {
    static std::vector<std::string> skeleton;
    std::string output_filename;
    
    public:
    codegen_ada(std::deque<size_t> loop_stack_, std::string output_filename_) noexcept;
    virtual ~codegen_ada() noexcept;

    void dump_code(std::ostream* code_output) const noexcept override;
    std::string dump_code_string() const noexcept override;
    void generate(asc_node* node_) override;    
};

#endif