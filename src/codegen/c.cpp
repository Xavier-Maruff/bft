#include "./c.hpp"
#include "../log.hpp"
#include "../err_codes.h"

#include <map>
#include <functional>


#define BF_ARRAY_SIZE 30000
#define ERR_PREFIX "C backend -> "

const std::string bf_array_size_str = std::to_string(BF_ARRAY_SIZE);

std::pair<std::string, std::string> codegen_c::skeleton = {
    R"(#include <stdio.h>
#include <string.h>

int *scan_left(const int* scan_space, int search_term, size_t scan_length){
    int* s = (int*)scan_space;
	while(scan_length--){
		if(*s == search_term) scan_space-scan_length;
		else s--;
	}
}

int main(int argc, char* argv[]){
	int bf_array[)"+bf_array_size_str+R"(] = {0};
	int* bf_ptr = bf_array;
	)",
	R"(
	return 0;
}
    )"
};

codegen_c::codegen_c(std::deque<size_t> loop_stack_) noexcept:
codegen(loop_stack_) {
    indent_level = 1;
}

codegen_c::~codegen_c() noexcept{
    //
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
		if(open_loop_stack.size() >= open_loop_stack.max_size()){
			stdlog.err() << "Deque stack passed max size (" << open_loop_stack.max_size() << ")" << std::endl;
			throw MEM_ERR;
		}
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

		case zero_assign:
		code_stream_fmt() << "*bf_ptr = 0;\n";
		break;

		default:
		break;
    }
}

