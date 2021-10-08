#include "./c.hpp"
#include "../log.hpp"

#include <map>
#include <functional>

#define BF_ARRAY_SIZE 30000
#define ERR_PREFIX "C backend -> "

std::pair<std::string, std::string> codegen_c::skeleton = {
    R"(#include <stdio.h>

int main(int argc, char* argv[]){
	int bf_array[)"+std::to_string(BF_ARRAY_SIZE)+R"(] = {0};
	int* bf_ptr = bf_array;
	)",
	R"(
	return 0;
}
    )"
};

codegen_c::codegen_c(std::deque<size_t> loop_stack_) noexcept:
codegen(loop_stack_), indent_level(1) {
    //
}

codegen_c::~codegen_c() noexcept{
    //
}

inline std::ostream& codegen_c::code_stream_fmt() noexcept {
	return code_stream << std::string(indent_level, '\t');
}

void codegen_c::dump_code(std::ostream* code_output) const noexcept {
    *code_output << skeleton.first;
    *code_output << code_stream.rdbuf();
    *code_output << skeleton.second;
}

std::string codegen_c::dump_code_string() const noexcept {
    std::stringstream code_output;
    code_output << skeleton.first;
    code_output << code_stream.rdbuf();
    code_output << skeleton.second;
    return code_output.str();
}

void codegen_c::generate(asc_node* node_) {
    switch(node_->node_type){
        case inc_ptr:
        code_stream_fmt() << "bf_ptr += " << node_->iterations << ";\n";
        break;

		case dec_ptr:
		code_stream_fmt() << "bf_ptr -= " << node_->iterations << ";\n";
		break;

		case inc_val:
		code_stream_fmt() << "*bf_ptr += " << node_->iterations << ";\n";
		break;

		case dec_val:
		code_stream_fmt() << "*bf_ptr-= " << node_->iterations << ";\n";
		break;

		case loop_start:
		open_loop_stack.push_back(loop_stack.back());
		loop_stack.pop_back();
		code_stream_fmt() << "while(*bf_ptr){\n";
		indent_level++;
		break;

		case loop_end:
		indent_level--;
		code_stream_fmt() << "}\n";
		open_loop_stack.pop_back();
		break;

		case put_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "putchar(*bf_ptr);\n";
		break;

		case get_char:
		for(size_t i = 0; i < node_->iterations; i++)
			code_stream_fmt() << "*bf_ptr = getchar();\n";
		break;

		default:
		break;
    }
}

