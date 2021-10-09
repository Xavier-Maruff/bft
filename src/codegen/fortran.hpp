#ifndef BFT_FORTRAN_CODEGEN
#define BFT_FORTRAN_CODEGEN

#include <vector>
#include "codegen.hpp"

class codegen_fortran: public codegen {
    static std::vector<std::string> skeleton;
    std::string output_filename;
    
    public:
    codegen_fortran(std::deque<size_t> loop_stack_, std::string output_filename_) noexcept;
    virtual ~codegen_fortran() noexcept;

    void dump_code(std::ostream* code_output) const noexcept override;
    std::string dump_code_string() const noexcept override;
    void generate(asc_node* node_) override;    
};

#endif