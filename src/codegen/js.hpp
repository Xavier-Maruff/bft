#include "codegen.hpp"

class codegen_js: public codegen {
    static std::pair<std::string, std::string> skeleton;
    size_t indent_level;
    inline std::ostream& code_stream_fmt() noexcept;
    
    public:
    codegen_js(std::deque<size_t> loop_stack_) noexcept;
    virtual ~codegen_js() noexcept;

    void dump_code(std::ostream* code_output) const noexcept override;
    std::string dump_code_string() const noexcept override;
    void generate(asc_node* node_) override;    
};